// Copyright © John Stratton 2024. All rights reserved. Valor & Blade is a trademark of John Stratton. This game was developed using Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. This software product contains Unreal® Engine code, including but not limited to computer code, audio-visual materials, and related documentation ("Unreal® Engine Code"). © 2024, Epic Games, Inc. All rights reserved. Any unauthorized copying, alteration, distribution, transmission, performance, display, or other use of this game or any part thereof is prohibited without the express written consent of John Stratton. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved.

#include "ValorCombatAbilities.h"
#include "ValorAbilitySystem/Public/ValorCombatComponent.h"
#include "ValorAbilitySystem/Public/ValorCharacter.h"
#include "ValorAbilitySystem/Public/GAS/ValorAttributeSet.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"

// Light Attack Ability
UValorLightAttackAbility::UValorLightAttackAbility()
{
	// Set up action data
	ActionData.ActionType = ECombatAction::LightAttack;
	ActionData.DamageInfo.Damage = 10.0f;
	ActionData.DamageInfo.DamageType = EDamageType::Physical;
	ActionData.Duration = 0.5f;
	ActionData.Cooldown = 1.0f;
	ActionData.StaminaCost = 5.0f;
}

void UValorLightAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Get combat component
	UValorCombatComponent* CombatComp = GetCombatComponent();
	if (!CombatComp)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Start attack animation/state
	CombatComp->StartAttack();

	// Find target
	AActor* Target = GetPrimaryTarget();
	if (Target && CombatComp->IsInRange(Target))
	{
		// Calculate damage
		float BaseDamage = CombatComp->GetAttackDamage() * DamageMultiplier;
		
		// Create damage info
		FDamageInfo DamageInfo;
		DamageInfo.Damage = BaseDamage;
		DamageInfo.DamageType = EDamageType::Physical;
		DamageInfo.bCanCrit = true;

		// Apply damage
		ApplyDamage(Target, DamageInfo);
	}

	// End ability after duration
	FTimerHandle EndTimer;
	GetWorld()->GetTimerManager().SetTimer(EndTimer, [this, Handle, ActorInfo, ActivationInfo]()
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}, ActionData.Duration, false);
}

// Heavy Attack Ability
UValorHeavyAttackAbility::UValorHeavyAttackAbility()
{
	// Set up action data
	ActionData.ActionType = ECombatAction::HeavyAttack;
	ActionData.DamageInfo.Damage = 20.0f;
	ActionData.DamageInfo.DamageType = EDamageType::Physical;
	ActionData.Duration = 1.0f;
	ActionData.Cooldown = 2.0f;
	ActionData.StaminaCost = 15.0f;
}

void UValorHeavyAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Get combat component
	UValorCombatComponent* CombatComp = GetCombatComponent();
	if (!CombatComp)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Start attack animation/state
	CombatComp->StartAttack();

	// Find target
	AActor* Target = GetPrimaryTarget();
	if (Target && CombatComp->IsInRange(Target))
	{
		// Calculate damage
		float BaseDamage = CombatComp->GetAttackDamage() * DamageMultiplier;
		
		// Create damage info
		FDamageInfo DamageInfo;
		DamageInfo.Damage = BaseDamage;
		DamageInfo.DamageType = EDamageType::Physical;
		DamageInfo.bCanCrit = true;

		// Apply damage
		ApplyDamage(Target, DamageInfo);

		// Apply knockback if enabled
		if (bCanKnockback)
		{
			// This would typically be handled by a gameplay effect or physics
			// For now, we'll just log it
			UE_LOG(LogTemp, Log, TEXT("Heavy Attack: Knockback applied to %s"), *Target->GetName());
		}
	}

	// End ability after duration
	FTimerHandle EndTimer;
	GetWorld()->GetTimerManager().SetTimer(EndTimer, [this, Handle, ActorInfo, ActivationInfo]()
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}, ActionData.Duration, false);
}

// Block Ability
UValorBlockAbility::UValorBlockAbility()
{
	// Set up action data
	ActionData.ActionType = ECombatAction::Block;
	ActionData.Duration = 0.0f; // Can be held
	ActionData.Cooldown = 0.0f;
	ActionData.StaminaCost = 2.0f; // Per second
}

void UValorBlockAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Get combat component
	UValorCombatComponent* CombatComp = GetCombatComponent();
	if (!CombatComp)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Start blocking
	CombatComp->StartBlock();

	// Set up stamina drain timer
	GetWorld()->GetTimerManager().SetTimer(BlockStaminaDrainTimer, [this, Handle, ActorInfo, ActivationInfo]()
	{
		// Check if we still have enough stamina
		if (!HasEnoughStamina())
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
			return;
		}

		// Consume stamina
		ConsumeStamina();

		// Continue draining stamina
		GetWorld()->GetTimerManager().SetTimer(BlockStaminaDrainTimer, [this, Handle, ActorInfo, ActivationInfo]()
		{
			// This will be called again in 1 second
		}, 1.0f, false);
	}, 1.0f, false);
}

void UValorBlockAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Get combat component
	UValorCombatComponent* CombatComp = GetCombatComponent();
	if (CombatComp)
	{
		CombatComp->StopBlock();
	}

	// Clear stamina drain timer
	GetWorld()->GetTimerManager().ClearTimer(BlockStaminaDrainTimer);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

// Dodge Ability
UValorDodgeAbility::UValorDodgeAbility()
{
	// Set up action data
	ActionData.ActionType = ECombatAction::Dodge;
	ActionData.Duration = 0.3f;
	ActionData.Cooldown = 1.5f;
	ActionData.StaminaCost = 10.0f;
}

void UValorDodgeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Get combat component
	UValorCombatComponent* CombatComp = GetCombatComponent();
	if (!CombatComp)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Start dodging
	CombatComp->StartDodge();

	// Get character for movement
	AActor* Owner = GetAvatarActorFromActorInfo();
	if (ACharacter* Character = Cast<ACharacter>(Owner))
	{
		// Calculate dodge direction (backward from current facing direction)
		FVector DodgeDirection = -Character->GetActorForwardVector();
		FVector DodgeLocation = Character->GetActorLocation() + (DodgeDirection * DodgeDistance);

		// Move character
		Character->SetActorLocation(DodgeLocation);

		// Set invincibility if enabled
		if (bInvincibleDuringDodge)
		{
			// This would typically be handled by a gameplay effect
			// For now, we'll just log it
			UE_LOG(LogTemp, Log, TEXT("Dodge: Invincibility active for %f seconds"), DodgeDuration);
		}
	}

	// End ability after duration
	FTimerHandle EndTimer;
	GetWorld()->GetTimerManager().SetTimer(EndTimer, [this, Handle, ActorInfo, ActivationInfo]()
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}, DodgeDuration, false);
}