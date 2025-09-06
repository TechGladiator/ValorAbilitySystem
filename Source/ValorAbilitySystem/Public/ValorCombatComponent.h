// Copyright © John Stratton 2024. All rights reserved. Valor & Blade is a trademark of John Stratton. This game was developed using Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. This software product contains Unreal® Engine code, including but not limited to computer code, audio-visual materials, and related documentation ("Unreal® Engine Code"). © 2024, Epic Games, Inc. All rights reserved. Any unauthorized copying, alteration, distribution, transmission, performance, display, or other use of this game or any part thereof is prohibited without the express written consent of John Stratton. Unreal® Engine, Copyright 1998 - Epic Games, Inc. All rights reserved. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "ValorAbilitySystem/Public/GAS/ValorAttributeSet.h"
#include "ValorCombatComponent.generated.h"

class UAbilitySystemComponent;
class UValorAttributeSet;

// Forward declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChanged, float, CurrentHealth, float, MaxHealth, float, Percentage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterDied, AActor*, Character, AActor*, Killer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDamageDealt, AActor*, Target, float, Damage, FGameplayTag, DamageType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDamageReceived, AActor*, Source, float, Damage, FGameplayTag, DamageType);

/**
 * UValorCombatComponent
 *
 * Combat system component that handles damage dealing, health management, and combat events.
 * Integrates with the Gameplay Ability System for robust combat mechanics.
 */
UCLASS(Blueprintable, ClassGroup=(ValorAbilitySystem), meta=(BlueprintSpawnableComponent))
class VALORABILITYSYSTEM_API UValorCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UValorCombatComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Combat Events
	UPROPERTY(BlueprintAssignable, Category = "Combat Events")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Combat Events")
	FOnCharacterDied OnCharacterDied;

	UPROPERTY(BlueprintAssignable, Category = "Combat Events")
	FOnDamageDealt OnDamageDealt;

	UPROPERTY(BlueprintAssignable, Category = "Combat Events")
	FOnDamageReceived OnDamageReceived;

	// Combat Configuration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Settings")
	bool bCanTakeDamage = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Settings")
	bool bIsAlive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Settings")
	float BaseDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Settings")
	float AttackRange = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Settings")
	float AttackCooldown = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Settings")
	float BlockDamageReduction = 0.5f;

	// Combat State
	UPROPERTY(BlueprintReadOnly, Category = "Combat State")
	bool bIsAttacking = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat State")
	bool bIsBlocking = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat State")
	bool bIsDodging = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat State")
	float LastAttackTime = 0.0f;

	// Damage Types
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Types")
	FGameplayTag PhysicalDamageTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Types")
	FGameplayTag MagicalDamageTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Types")
	FGameplayTag TrueDamageTag;

	// Combat Functions
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool CanAttack() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool CanBlock() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool CanDodge() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartAttack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StopAttack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartBlock();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StopBlock();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartDodge();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StopDodge();

	// Damage Functions
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void DealDamage(AActor* Target, float Damage, FGameplayTag DamageType);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TakeDamage(AActor* Source, float Damage, FGameplayTag DamageType);

	// Overloaded versions with default damage type
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void DealDamage(AActor* Target, float Damage);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TakeDamage(AActor* Source, float Damage);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Heal(float HealAmount);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Die(AActor* Killer = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Revive(float HealthPercentage = 1.0f);

	// Health Management
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
	float GetHealthPercentage() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
	bool IsAlive() const;

	// Combat Stats
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
	float GetAttackDamage() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
	float GetAttackSpeed() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
	float GetDefense() const;

	// Utility Functions
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsInRange(AActor* Target) const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	AActor* FindNearestEnemy(float SearchRadius = 1000.0f) const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	TArray<AActor*> FindEnemiesInRange(float SearchRadius) const;

private:
	// Internal References
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UValorAttributeSet* AttributeSet;

	// Internal Functions
	void InitializeReferences();
	void UpdateHealthDisplay();
	void CheckDeath();
	float CalculateDamage(float BaseDamage, FGameplayTag DamageType) const;
	float CalculateDefense(FGameplayTag DamageType) const;
	bool IsEnemy(AActor* Target) const;

	// Combat Timers
	FTimerHandle AttackCooldownTimer;
	FTimerHandle BlockTimer;
	FTimerHandle DodgeTimer;
};