// Copyright © John Stratton 2024. All rights reserved. Valor & Blade is a trademark of John Stratton. This game was developed using Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. This software product contains Unreal® Engine code, including but not limited to computer code, audio-visual materials, and related documentation ("Unreal® Engine Code"). © 2024, Epic Games, Inc. All rights reserved. Any unauthorized copying, alteration, distribution, transmission, performance, display, or other use of this game or any part thereof is prohibited without the express written consent of John Stratton. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ValorCombatData.generated.h"

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	Idle,
	Attacking,
	Blocking,
	Dodging,
	Stunned,
	Dead
};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Physical,
	Magical,
	True,
	Fire,
	Ice,
	Lightning,
	Poison,
	Holy,
	Dark
};

UENUM(BlueprintType)
enum class ECombatAction : uint8
{
	None,
	LightAttack,
	HeavyAttack,
	Block,
	Dodge,
	Special,
	Ultimate
};

USTRUCT(BlueprintType)
struct VALORABILITYSYSTEM_API FCombatStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Stats")
	float BaseDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Stats")
	float AttackSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Stats")
	float AttackRange = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Stats")
	float CriticalChance = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Stats")
	float CriticalMultiplier = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Stats")
	float Defense = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Stats")
	float BlockReduction = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Stats")
	float DodgeChance = 0.1f;

	FCombatStats()
	{
		BaseDamage = 10.0f;
		AttackSpeed = 1.0f;
		AttackRange = 200.0f;
		CriticalChance = 0.05f;
		CriticalMultiplier = 2.0f;
		Defense = 0.0f;
		BlockReduction = 0.5f;
		DodgeChance = 0.1f;
	}
};

USTRUCT(BlueprintType)
struct VALORABILITYSYSTEM_API FDamageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Info")
	float Damage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Info")
	EDamageType DamageType = EDamageType::Physical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Info")
	FGameplayTag DamageTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Info")
	bool bCanCrit = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Info")
	bool bIgnoresDefense = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Info")
	bool bIgnoresBlock = false;

	FDamageInfo()
	{
		Damage = 0.0f;
		DamageType = EDamageType::Physical;
		DamageTag = FGameplayTag::EmptyTag;
		bCanCrit = true;
		bIgnoresDefense = false;
		bIgnoresBlock = false;
	}
};

USTRUCT(BlueprintType)
struct VALORABILITYSYSTEM_API FCombatActionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Data")
	ECombatAction ActionType = ECombatAction::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Data")
	FDamageInfo DamageInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Data")
	float Duration = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Data")
	float Cooldown = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Data")
	float StaminaCost = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Data")
	float ManaCost = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Data")
	FGameplayTag RequiredTag;

	FCombatActionData()
	{
		ActionType = ECombatAction::None;
		Duration = 0.0f;
		Cooldown = 0.0f;
		StaminaCost = 0.0f;
		ManaCost = 0.0f;
		RequiredTag = FGameplayTag::EmptyTag;
	}
};

/**
 * UValorCombatData
 *
 * Data asset containing combat configuration and stats for characters.
 * Can be used to define different combat profiles for different character types.
 */
UCLASS(BlueprintType)
class VALORABILITYSYSTEM_API UValorCombatData : public UDataAsset
{
	GENERATED_BODY()

public:
	UValorCombatData();

	// Combat Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Configuration")
	FCombatStats CombatStats;

	// Available Combat Actions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Actions")
	TArray<FCombatActionData> AvailableActions;

	// Damage Type Tags
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Types")
	TMap<EDamageType, FGameplayTag> DamageTypeTags;

	// Combat State Tags
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat States")
	TMap<ECombatState, FGameplayTag> CombatStateTags;

	// Utility Functions
	UFUNCTION(BlueprintCallable, Category = "Combat Data")
	FGameplayTag GetDamageTypeTag(EDamageType DamageType) const;

	UFUNCTION(BlueprintCallable, Category = "Combat Data")
	FGameplayTag GetCombatStateTag(ECombatState CombatState) const;

	UFUNCTION(BlueprintCallable, Category = "Combat Data")
	FCombatActionData GetActionData(ECombatAction ActionType) const;

	UFUNCTION(BlueprintCallable, Category = "Combat Data")
	bool HasAction(ECombatAction ActionType) const;
};