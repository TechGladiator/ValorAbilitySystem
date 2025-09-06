// Copyright © John Stratton 2024. All rights reserved. Valor & Blade is a trademark of John Stratton. This game was developed using Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. This software product contains Unreal® Engine code, including but not limited to computer code, audio-visual materials, and related documentation ("Unreal® Engine Code"). © 2024, Epic Games, Inc. All rights reserved. Any unauthorized copying, alteration, distribution, transmission, performance, display, or other use of this game or any part thereof is prohibited without the express written consent of John Stratton. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ValorAbilitySystem/Public/ValorCombatAbility.h"
#include "ValorCombatAbilities.generated.h"

/**
 * UValorLightAttackAbility
 *
 * Basic light attack ability that deals moderate damage with low stamina cost.
 */
UCLASS(BlueprintType, Blueprintable)
class VALORABILITYSYSTEM_API UValorLightAttackAbility : public UValorCombatAbility
{
	GENERATED_BODY()

public:
	UValorLightAttackAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Attack", meta = (AllowPrivateAccess = "true"))
	float DamageMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Attack", meta = (AllowPrivateAccess = "true"))
	float Range = 200.0f;
};

/**
 * UValorHeavyAttackAbility
 *
 * Heavy attack ability that deals high damage but costs more stamina and has longer cooldown.
 */
UCLASS(BlueprintType, Blueprintable)
class VALORABILITYSYSTEM_API UValorHeavyAttackAbility : public UValorCombatAbility
{
	GENERATED_BODY()

public:
	UValorHeavyAttackAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heavy Attack", meta = (AllowPrivateAccess = "true"))
	float DamageMultiplier = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heavy Attack", meta = (AllowPrivateAccess = "true"))
	float Range = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heavy Attack", meta = (AllowPrivateAccess = "true"))
	bool bCanKnockback = true;
};

/**
 * UValorBlockAbility
 *
 * Block ability that reduces incoming damage while active.
 */
UCLASS(BlueprintType, Blueprintable)
class VALORABILITYSYSTEM_API UValorBlockAbility : public UValorCombatAbility
{
	GENERATED_BODY()

public:
	UValorBlockAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block", meta = (AllowPrivateAccess = "true"))
	float BlockReduction = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block", meta = (AllowPrivateAccess = "true"))
	float StaminaDrainPerSecond = 2.0f;

	FTimerHandle BlockStaminaDrainTimer;
};

/**
 * UValorDodgeAbility
 *
 * Dodge ability that provides brief invincibility and movement.
 */
UCLASS(BlueprintType, Blueprintable)
class VALORABILITYSYSTEM_API UValorDodgeAbility : public UValorCombatAbility
{
	GENERATED_BODY()

public:
	UValorDodgeAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge", meta = (AllowPrivateAccess = "true"))
	float DodgeDistance = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge", meta = (AllowPrivateAccess = "true"))
	float DodgeDuration = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge", meta = (AllowPrivateAccess = "true"))
	bool bInvincibleDuringDodge = true;
};