// Copyright © John Stratton 2024. All rights reserved. Valor & Blade is a trademark of John Stratton. This game was developed using Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. This software product contains Unreal® Engine code, including but not limited to computer code, audio-visual materials, and related documentation ("Unreal® Engine Code"). © 2024, Epic Games, Inc. All rights reserved. Any unauthorized copying, alteration, distribution, transmission, performance, display, or other use of this game or any part thereof is prohibited without the express written consent of John Stratton. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved.

#include "ValorCombatComponent.h"
#include "ValorAbilitySystem/Public/ValorCharacter.h"
#include "ValorAbilitySystem/Public/GAS/ValorAttributeSet.h"
#include "GameFramework/Character.h"
#include "GameplayTagsModule.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"

UValorCombatComponent::UValorCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	// Initialize damage type tags
	PhysicalDamageTag = FGameplayTag::RequestGameplayTag(FName("Damage.Physical"));
	MagicalDamageTag = FGameplayTag::RequestGameplayTag(FName("Damage.Magical"));
	TrueDamageTag = FGameplayTag::RequestGameplayTag(FName("Damage.True"));
}

void UValorCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeReferences();
	UpdateHealthDisplay();
}

void UValorCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// Update combat state
	if (bIsAlive)
	{
		CheckDeath();
	}
}

void UValorCombatComponent::InitializeReferences()
{
	// Get references to the owner's ability system components
	AActor* Owner = GetOwner();
	if (AValorCharacter* ValorChar = Cast<AValorCharacter>(Owner))
	{
		AbilitySystemComponent = ValorChar->GetAbilitySystemComponent();
		if (AbilitySystemComponent)
		{
			AttributeSet = AbilitySystemComponent->GetSet<UValorAttributeSet>();
		}
	}
}

void UValorCombatComponent::UpdateHealthDisplay()
{
	if (AttributeSet)
	{
		float CurrentHealth = GetCurrentHealth();
		float MaxHealth = GetMaxHealth();
		float Percentage = MaxHealth > 0.0f ? CurrentHealth / MaxHealth : 0.0f;
		
		OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, Percentage);
	}
}

void UValorCombatComponent::CheckDeath()
{
	if (AttributeSet && bIsAlive)
	{
		float CurrentHealth = GetCurrentHealth();
		if (CurrentHealth <= 0.0f)
		{
			Die();
		}
	}
}

float UValorCombatComponent::CalculateDamage(float BaseDamage, FGameplayTag DamageType) const
{
	if (!AttributeSet)
	{
		return BaseDamage;
	}

	float FinalDamage = BaseDamage;
	
	// Apply strength modifier for physical damage
	if (DamageType == PhysicalDamageTag)
	{
		float Strength = AttributeSet->GetStrength();
		FinalDamage += Strength * 0.1f; // 10% damage per strength point
	}
	// Apply intelligence modifier for magical damage
	else if (DamageType == MagicalDamageTag)
	{
		float Intelligence = AttributeSet->GetIntelligence();
		FinalDamage += Intelligence * 0.1f; // 10% damage per intelligence point
	}
	
	return FMath::Max(0.0f, FinalDamage);
}

float UValorCombatComponent::CalculateDefense(FGameplayTag DamageType) const
{
	if (!AttributeSet)
	{
		return 0.0f;
	}

	float Defense = 0.0f;
	
	// Apply dexterity modifier for physical defense
	if (DamageType == PhysicalDamageTag)
	{
		float Dexterity = AttributeSet->GetDexterity();
		Defense += Dexterity * 0.05f; // 5% damage reduction per dexterity point
	}
	// Apply intelligence modifier for magical defense
	else if (DamageType == MagicalDamageTag)
	{
		float Intelligence = AttributeSet->GetIntelligence();
		Defense += Intelligence * 0.05f; // 5% damage reduction per intelligence point
	}
	
	return FMath::Clamp(Defense, 0.0f, 0.95f); // Cap at 95% damage reduction
}

bool UValorCombatComponent::IsEnemy(AActor* Target) const
{
	if (!Target || Target == GetOwner())
	{
		return false;
	}

	// Simple enemy detection - can be expanded with faction system
	// For now, check if the target has a different controller
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	APawn* TargetPawn = Cast<APawn>(Target);
	
	if (OwnerPawn && TargetPawn)
	{
		return OwnerPawn->GetController() != TargetPawn->GetController();
	}
	
	return false;
}

// Combat State Functions
bool UValorCombatComponent::CanAttack() const
{
	return bIsAlive && !bIsAttacking && !bIsDodging && 
		   (GetWorld()->GetTimeSeconds() - LastAttackTime) >= AttackCooldown;
}

bool UValorCombatComponent::CanBlock() const
{
	return bIsAlive && !bIsAttacking && !bIsDodging && !bIsBlocking;
}

bool UValorCombatComponent::CanDodge() const
{
	return bIsAlive && !bIsAttacking && !bIsBlocking && !bIsDodging;
}

void UValorCombatComponent::StartAttack()
{
	if (CanAttack())
	{
		bIsAttacking = true;
		LastAttackTime = GetWorld()->GetTimeSeconds();
		
		// Set a timer to stop attacking after a short duration
		GetWorld()->GetTimerManager().SetTimer(AttackCooldownTimer, this, &UValorCombatComponent::StopAttack, 0.5f, false);
	}
}

void UValorCombatComponent::StopAttack()
{
	bIsAttacking = false;
}

void UValorCombatComponent::StartBlock()
{
	if (CanBlock())
	{
		bIsBlocking = true;
	}
}

void UValorCombatComponent::StopBlock()
{
	bIsBlocking = false;
}

void UValorCombatComponent::StartDodge()
{
	if (CanDodge())
	{
		bIsDodging = true;
		
		// Set a timer to stop dodging after a short duration
		GetWorld()->GetTimerManager().SetTimer(DodgeTimer, this, &UValorCombatComponent::StopDodge, 0.3f, false);
	}
}

void UValorCombatComponent::StopDodge()
{
	bIsDodging = false;
}

// Damage Functions
void UValorCombatComponent::DealDamage(AActor* Target, float Damage, FGameplayTag DamageType)
{
	if (!Target || !bIsAlive || !IsEnemy(Target))
	{
		return;
	}

	// Calculate final damage
	float FinalDamage = CalculateDamage(Damage, DamageType);
	
	// Apply damage to target
	if (UValorCombatComponent* TargetCombat = Target->FindComponentByClass<UValorCombatComponent>())
	{
		TargetCombat->TakeDamage(GetOwner(), FinalDamage, DamageType);
	}
	
	// Broadcast damage dealt event
	OnDamageDealt.Broadcast(Target, FinalDamage, DamageType);
}

void UValorCombatComponent::TakeDamage(AActor* Source, float Damage, FGameplayTag DamageType)
{
	if (!bCanTakeDamage || !bIsAlive || !AttributeSet)
	{
		return;
	}

	// Apply block damage reduction
	if (bIsBlocking)
	{
		Damage *= (1.0f - BlockDamageReduction);
	}
	
	// Apply defense
	float Defense = CalculateDefense(DamageType);
	Damage *= (1.0f - Defense);
	
	// Apply damage to health
	float CurrentHealth = GetCurrentHealth();
	float NewHealth = FMath::Max(0.0f, CurrentHealth - Damage);
	
	// Use gameplay effect to modify health attribute
	if (AbilitySystemComponent)
	{
		FGameplayEffectSpecHandle DamageEffectSpec = AbilitySystemComponent->MakeOutgoingSpec(
			UValorAttributeSet::GetHealthAttribute().GetUProperty(), 1.0f, AbilitySystemComponent->MakeEffectContext());
		
		if (DamageEffectSpec.IsValid())
		{
			FGameplayEffectSpec* Spec = DamageEffectSpec.Data.Get();
			Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), -Damage);
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec);
		}
	}
	
	// Broadcast damage received event
	OnDamageReceived.Broadcast(Source, Damage, DamageType);
	
	// Update health display
	UpdateHealthDisplay();
}

void UValorCombatComponent::Heal(float HealAmount)
{
	if (!bIsAlive || !AttributeSet || HealAmount <= 0.0f)
	{
		return;
	}

	float CurrentHealth = GetCurrentHealth();
	float MaxHealth = GetMaxHealth();
	float NewHealth = FMath::Min(MaxHealth, CurrentHealth + HealAmount);
	
	// Use gameplay effect to modify health attribute
	if (AbilitySystemComponent)
	{
		FGameplayEffectSpecHandle HealEffectSpec = AbilitySystemComponent->MakeOutgoingSpec(
			UValorAttributeSet::GetHealthAttribute().GetUProperty(), 1.0f, AbilitySystemComponent->MakeEffectContext());
		
		if (HealEffectSpec.IsValid())
		{
			FGameplayEffectSpec* Spec = HealEffectSpec.Data.Get();
			Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Healing")), HealAmount);
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec);
		}
	}
	
	UpdateHealthDisplay();
}

void UValorCombatComponent::Die(AActor* Killer)
{
	if (!bIsAlive)
	{
		return;
	}

	bIsAlive = false;
	bIsAttacking = false;
	bIsBlocking = false;
	bIsDodging = false;
	
	// Broadcast death event
	OnCharacterDied.Broadcast(GetOwner(), Killer);
}

void UValorCombatComponent::Revive(float HealthPercentage)
{
	if (bIsAlive || !AttributeSet)
	{
		return;
	}

	bIsAlive = true;
	
	float MaxHealth = GetMaxHealth();
	float ReviveHealth = MaxHealth * FMath::Clamp(HealthPercentage, 0.0f, 1.0f);
	
	// Set health to revive amount
	if (AbilitySystemComponent)
	{
		FGameplayEffectSpecHandle ReviveEffectSpec = AbilitySystemComponent->MakeOutgoingSpec(
			UValorAttributeSet::GetHealthAttribute().GetUProperty(), 1.0f, AbilitySystemComponent->MakeEffectContext());
		
		if (ReviveEffectSpec.IsValid())
		{
			FGameplayEffectSpec* Spec = ReviveEffectSpec.Data.Get();
			Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Revive")), ReviveHealth);
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec);
		}
	}
	
	UpdateHealthDisplay();
}

// Health Management
float UValorCombatComponent::GetCurrentHealth() const
{
	return AttributeSet ? AttributeSet->GetHealth() : 0.0f;
}

float UValorCombatComponent::GetMaxHealth() const
{
	return AttributeSet ? AttributeSet->GetMaxHealth() : 0.0f;
}

float UValorCombatComponent::GetHealthPercentage() const
{
	float MaxHealth = GetMaxHealth();
	return MaxHealth > 0.0f ? GetCurrentHealth() / MaxHealth : 0.0f;
}

bool UValorCombatComponent::IsAlive() const
{
	return bIsAlive && GetCurrentHealth() > 0.0f;
}

// Combat Stats
float UValorCombatComponent::GetAttackDamage() const
{
	if (!AttributeSet)
	{
		return BaseDamage;
	}

	float Strength = AttributeSet->GetStrength();
	return BaseDamage + (Strength * 0.1f);
}

float UValorCombatComponent::GetAttackSpeed() const
{
	if (!AttributeSet)
	{
		return 1.0f / AttackCooldown;
	}

	float Dexterity = AttributeSet->GetDexterity();
	float SpeedMultiplier = 1.0f + (Dexterity * 0.02f); // 2% speed increase per dexterity point
	return (1.0f / AttackCooldown) * SpeedMultiplier;
}

float UValorCombatComponent::GetDefense() const
{
	if (!AttributeSet)
	{
		return 0.0f;
	}

	float Dexterity = AttributeSet->GetDexterity();
	return Dexterity * 0.05f; // 5% damage reduction per dexterity point
}

// Utility Functions
bool UValorCombatComponent::IsInRange(AActor* Target) const
{
	if (!Target || !GetOwner())
	{
		return false;
	}

	float Distance = FVector::Dist(GetOwner()->GetActorLocation(), Target->GetActorLocation());
	return Distance <= AttackRange;
}

AActor* UValorCombatComponent::FindNearestEnemy(float SearchRadius) const
{
	TArray<AActor*> Enemies = FindEnemiesInRange(SearchRadius);
	
	if (Enemies.Num() == 0)
	{
		return nullptr;
	}

	AActor* NearestEnemy = nullptr;
	float NearestDistance = FLT_MAX;
	
	for (AActor* Enemy : Enemies)
	{
		float Distance = FVector::Dist(GetOwner()->GetActorLocation(), Enemy->GetActorLocation());
		if (Distance < NearestDistance)
		{
			NearestDistance = Distance;
			NearestEnemy = Enemy;
		}
	}
	
	return NearestEnemy;
}

TArray<AActor*> UValorCombatComponent::FindEnemiesInRange(float SearchRadius) const
{
	TArray<AActor*> Enemies;
	
	if (!GetOwner())
	{
		return Enemies;
	}

	// Find all actors in range
	TArray<AActor*> ActorsInRange;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), ActorsInRange);
	
	for (AActor* Actor : ActorsInRange)
	{
		if (IsEnemy(Actor))
		{
			float Distance = FVector::Dist(GetOwner()->GetActorLocation(), Actor->GetActorLocation());
			if (Distance <= SearchRadius)
			{
				Enemies.Add(Actor);
			}
		}
	}
	
	return Enemies;
}