// Copyright © John Stratton 2024. All rights reserved. Valor & Blade is a trademark of John Stratton. This game was developed using Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. This software product contains Unreal® Engine code, including but not limited to computer code, audio-visual materials, and related documentation ("Unreal® Engine Code"). © 2024, Epic Games, Inc. All rights reserved. Any unauthorized copying, alteration, distribution, transmission, performance, display, or other use of this game or any part thereof is prohibited without the express written consent of John Stratton. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved.

#include "ValorCombatAbility.h"
#include "ValorAbilitySystem/Public/ValorCombatComponent.h"
#include "ValorAbilitySystem/Public/ValorCharacter.h"
#include "ValorAbilitySystem/Public/GAS/ValorAttributeSet.h"
#include "GameplayTagsModule.h"
#include "Kismet/GameplayStatics.h"

UValorCombatAbility::UValorCombatAbility()
{
	// Set default ability settings
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("CombatState.Dead")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("CombatState.Stunned")));
}

bool UValorCombatAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return CheckCombatConditions();
}

void UValorCombatAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Apply resource costs
	ApplyResourceCosts();

	// Call blueprint event
	OnCombatAbilityActivated();

	// If this is an instant ability, end it immediately
	if (ActionData.Duration <= 0.0f)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UValorCombatAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Call blueprint event
	if (bWasCancelled)
	{
		OnCombatAbilityCancelled();
	}
	else
	{
		OnCombatAbilityEnded();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

UValorCombatComponent* UValorCombatAbility::GetCombatComponent() const
{
	AActor* Owner = GetAvatarActorFromActorInfo();
	if (AValorCharacter* ValorChar = Cast<AValorCharacter>(Owner))
	{
		return ValorChar->GetCombatComponent();
	}
	return nullptr;
}

bool UValorCombatAbility::HasEnoughStamina() const
{
	UValorCombatComponent* CombatComp = GetCombatComponent();
	if (!CombatComp)
	{
		return false;
	}

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC)
	{
		return false;
	}

	UValorAttributeSet* AttributeSet = ASC->GetSet<UValorAttributeSet>();
	if (!AttributeSet)
	{
		return false;
	}

	float CurrentStamina = AttributeSet->GetStamina();
	return CurrentStamina >= ActionData.StaminaCost;
}

bool UValorCombatAbility::HasEnoughMana() const
{
	UValorCombatComponent* CombatComp = GetCombatComponent();
	if (!CombatComp)
	{
		return false;
	}

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC)
	{
		return false;
	}

	UValorAttributeSet* AttributeSet = ASC->GetSet<UValorAttributeSet>();
	if (!AttributeSet)
	{
		return false;
	}

	float CurrentMana = AttributeSet->GetMana();
	return CurrentMana >= ActionData.ManaCost;
}

void UValorCombatAbility::ConsumeStamina()
{
	if (ActionData.StaminaCost <= 0.0f)
	{
		return;
	}

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC)
	{
		return;
	}

	UValorAttributeSet* AttributeSet = ASC->GetSet<UValorAttributeSet>();
	if (!AttributeSet)
	{
		return;
	}

	// Apply stamina cost using gameplay effect
	FGameplayEffectSpecHandle StaminaEffectSpec = ASC->MakeOutgoingSpec(
		UValorAttributeSet::GetStaminaAttribute().GetUProperty(), 1.0f, ASC->MakeEffectContext());
	
	if (StaminaEffectSpec.IsValid())
	{
		FGameplayEffectSpec* Spec = StaminaEffectSpec.Data.Get();
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.StaminaCost")), -ActionData.StaminaCost);
		ASC->ApplyGameplayEffectSpecToSelf(*Spec);
	}
}

void UValorCombatAbility::ConsumeMana()
{
	if (ActionData.ManaCost <= 0.0f)
	{
		return;
	}

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC)
	{
		return;
	}

	UValorAttributeSet* AttributeSet = ASC->GetSet<UValorAttributeSet>();
	if (!AttributeSet)
	{
		return;
	}

	// Apply mana cost using gameplay effect
	FGameplayEffectSpecHandle ManaEffectSpec = ASC->MakeOutgoingSpec(
		UValorAttributeSet::GetManaAttribute().GetUProperty(), 1.0f, ASC->MakeEffectContext());
	
	if (ManaEffectSpec.IsValid())
	{
		FGameplayEffectSpec* Spec = ManaEffectSpec.Data.Get();
		Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.ManaCost")), -ActionData.ManaCost);
		ASC->ApplyGameplayEffectSpecToSelf(*Spec);
	}
}

void UValorCombatAbility::ApplyDamage(AActor* Target, const FDamageInfo& DamageInfo)
{
	UValorCombatComponent* CombatComp = GetCombatComponent();
	if (!CombatComp || !Target)
	{
		return;
	}

	// Get the damage tag for the damage type
	FGameplayTag DamageTag = FGameplayTag::EmptyTag;
	if (DamageInfo.DamageTag.IsValid())
	{
		DamageTag = DamageInfo.DamageTag;
	}
	else
	{
		// Use default damage tags based on type
		switch (DamageInfo.DamageType)
		{
		case EDamageType::Physical:
			DamageTag = FGameplayTag::RequestGameplayTag(FName("Damage.Physical"));
			break;
		case EDamageType::Magical:
			DamageTag = FGameplayTag::RequestGameplayTag(FName("Damage.Magical"));
			break;
		case EDamageType::True:
			DamageTag = FGameplayTag::RequestGameplayTag(FName("Damage.True"));
			break;
		default:
			DamageTag = FGameplayTag::RequestGameplayTag(FName("Damage.Physical"));
			break;
		}
	}

	// Apply damage through combat component
	CombatComp->DealDamage(Target, DamageInfo.Damage, DamageTag);
}

TArray<AActor*> UValorCombatAbility::FindTargetsInRange(float Range) const
{
	UValorCombatComponent* CombatComp = GetCombatComponent();
	if (!CombatComp)
	{
		return TArray<AActor*>();
	}

	return CombatComp->FindEnemiesInRange(Range);
}

AActor* UValorCombatAbility::GetPrimaryTarget() const
{
	UValorCombatComponent* CombatComp = GetCombatComponent();
	if (!CombatComp)
	{
		return nullptr;
	}

	return CombatComp->FindNearestEnemy(CombatComp->AttackRange);
}

bool UValorCombatAbility::CheckCombatConditions() const
{
	// Check if we have enough stamina
	if (!HasEnoughStamina())
	{
		return false;
	}

	// Check if we have enough mana
	if (!HasEnoughMana())
	{
		return false;
	}

	// Check if combat component is valid and character is alive
	UValorCombatComponent* CombatComp = GetCombatComponent();
	if (!CombatComp || !CombatComp->IsAlive())
	{
		return false;
	}

	return true;
}

void UValorCombatAbility::ApplyResourceCosts()
{
	ConsumeStamina();
	ConsumeMana();
}