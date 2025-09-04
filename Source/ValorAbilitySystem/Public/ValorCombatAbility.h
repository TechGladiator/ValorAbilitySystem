// Copyright © John Stratton 2024. All rights reserved. Valor & Blade is a trademark of John Stratton. This game was developed using Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. This software product contains Unreal® Engine code, including but not limited to computer code, audio-visual materials, and related documentation ("Unreal® Engine Code"). © 2024, Epic Games, Inc. All rights reserved. Any unauthorized copying, alteration, distribution, transmission, performance, display, or other use of this game or any part thereof is prohibited without the express written consent of John Stratton. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ValorAbilitySystem/Public/ValorCombatData.h"
#include "ValorCombatAbility.generated.h"

class UValorCombatComponent;

/**
 * UValorCombatAbility
 *
 * Base class for combat abilities that integrate with the Valor Combat System.
 * Provides common functionality for all combat-related abilities.
 */
UCLASS(BlueprintType, Blueprintable)
class VALORABILITYSYSTEM_API UValorCombatAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UValorCombatAbility();

	// Combat Action Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Ability")
	FCombatActionData ActionData;

	// Override to check combat-specific conditions
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	// Override to handle combat-specific activation
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// Override to handle combat-specific ending
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	// Get the combat component from the actor
	UFUNCTION(BlueprintCallable, Category = "Combat Ability")
	UValorCombatComponent* GetCombatComponent() const;

	// Check if the actor has enough stamina for this ability
	UFUNCTION(BlueprintCallable, Category = "Combat Ability")
	bool HasEnoughStamina() const;

	// Check if the actor has enough mana for this ability
	UFUNCTION(BlueprintCallable, Category = "Combat Ability")
	bool HasEnoughMana() const;

	// Consume stamina for this ability
	UFUNCTION(BlueprintCallable, Category = "Combat Ability")
	void ConsumeStamina();

	// Consume mana for this ability
	UFUNCTION(BlueprintCallable, Category = "Combat Ability")
	void ConsumeMana();

	// Apply damage to a target
	UFUNCTION(BlueprintCallable, Category = "Combat Ability")
	void ApplyDamage(AActor* Target, const FDamageInfo& DamageInfo);

	// Find targets in range
	UFUNCTION(BlueprintCallable, Category = "Combat Ability")
	TArray<AActor*> FindTargetsInRange(float Range) const;

	// Get the primary target (for single-target abilities)
	UFUNCTION(BlueprintCallable, Category = "Combat Ability")
	AActor* GetPrimaryTarget() const;

	// Combat ability events
	UFUNCTION(BlueprintImplementableEvent, Category = "Combat Ability")
	void OnCombatAbilityActivated();

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat Ability")
	void OnCombatAbilityEnded();

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat Ability")
	void OnCombatAbilityCancelled();

private:
	// Internal helper functions
	bool CheckCombatConditions() const;
	void ApplyResourceCosts();
};