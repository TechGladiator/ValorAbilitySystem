// Copyright © John Stratton 2024. All rights reserved. Valor & Blade is a trademark of John Stratton. This game was developed using Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. This software product contains Unreal® Engine code, including but not limited to computer code, audio-visual materials, and related documentation ("Unreal® Engine Code"). © 2024, Epic Games, Inc. All rights reserved. Any unauthorized copying, alteration, distribution, transmission, performance, display, or other use of this game or any part thereof is prohibited without the express written consent of John Stratton. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved.

#include "ValorCombatData.h"
#include "GameplayTagsModule.h"

UValorCombatData::UValorCombatData()
{
	// Initialize default damage type tags
	DamageTypeTags.Add(EDamageType::Physical, FGameplayTag::RequestGameplayTag(FName("Damage.Physical")));
	DamageTypeTags.Add(EDamageType::Magical, FGameplayTag::RequestGameplayTag(FName("Damage.Magical")));
	DamageTypeTags.Add(EDamageType::TrueDamage, FGameplayTag::RequestGameplayTag(FName("Damage.True")));
	DamageTypeTags.Add(EDamageType::Fire, FGameplayTag::RequestGameplayTag(FName("Damage.Fire")));
	DamageTypeTags.Add(EDamageType::Ice, FGameplayTag::RequestGameplayTag(FName("Damage.Ice")));
	DamageTypeTags.Add(EDamageType::Lightning, FGameplayTag::RequestGameplayTag(FName("Damage.Lightning")));
	DamageTypeTags.Add(EDamageType::Poison, FGameplayTag::RequestGameplayTag(FName("Damage.Poison")));
	DamageTypeTags.Add(EDamageType::Holy, FGameplayTag::RequestGameplayTag(FName("Damage.Holy")));
	DamageTypeTags.Add(EDamageType::Dark, FGameplayTag::RequestGameplayTag(FName("Damage.Dark")));

	// Initialize default combat state tags
	CombatStateTags.Add(ECombatState::Idle, FGameplayTag::RequestGameplayTag(FName("CombatState.Idle")));
	CombatStateTags.Add(ECombatState::Attacking, FGameplayTag::RequestGameplayTag(FName("CombatState.Attacking")));
	CombatStateTags.Add(ECombatState::Blocking, FGameplayTag::RequestGameplayTag(FName("CombatState.Blocking")));
	CombatStateTags.Add(ECombatState::Dodging, FGameplayTag::RequestGameplayTag(FName("CombatState.Dodging")));
	CombatStateTags.Add(ECombatState::Stunned, FGameplayTag::RequestGameplayTag(FName("CombatState.Stunned")));
	CombatStateTags.Add(ECombatState::Dead, FGameplayTag::RequestGameplayTag(FName("CombatState.Dead")));

	// Initialize default combat actions
	FCombatActionData LightAttack;
	LightAttack.ActionType = ECombatAction::LightAttack;
	LightAttack.DamageInfo.Damage = 10.0f;
	LightAttack.DamageInfo.DamageType = EDamageType::Physical;
	LightAttack.Duration = 0.5f;
	LightAttack.Cooldown = 1.0f;
	LightAttack.StaminaCost = 5.0f;
	AvailableActions.Add(LightAttack);

	FCombatActionData HeavyAttack;
	HeavyAttack.ActionType = ECombatAction::HeavyAttack;
	HeavyAttack.DamageInfo.Damage = 20.0f;
	HeavyAttack.DamageInfo.DamageType = EDamageType::Physical;
	HeavyAttack.Duration = 1.0f;
	HeavyAttack.Cooldown = 2.0f;
	HeavyAttack.StaminaCost = 15.0f;
	AvailableActions.Add(HeavyAttack);

	FCombatActionData Block;
	Block.ActionType = ECombatAction::Block;
	Block.Duration = 0.0f; // Can be held
	Block.Cooldown = 0.0f;
	Block.StaminaCost = 2.0f; // Per second
	AvailableActions.Add(Block);

	FCombatActionData Dodge;
	Dodge.ActionType = ECombatAction::Dodge;
	Dodge.Duration = 0.3f;
	Dodge.Cooldown = 1.5f;
	Dodge.StaminaCost = 10.0f;
	AvailableActions.Add(Dodge);
}

FGameplayTag UValorCombatData::GetDamageTypeTag(EDamageType DamageType) const
{
	if (const FGameplayTag* FoundTag = DamageTypeTags.Find(DamageType))
	{
		return *FoundTag;
	}
	return FGameplayTag::EmptyTag;
}

FGameplayTag UValorCombatData::GetCombatStateTag(ECombatState CombatState) const
{
	if (const FGameplayTag* FoundTag = CombatStateTags.Find(CombatState))
	{
		return *FoundTag;
	}
	return FGameplayTag::EmptyTag;
}

FCombatActionData UValorCombatData::GetActionData(ECombatAction ActionType) const
{
	for (const FCombatActionData& Action : AvailableActions)
	{
		if (Action.ActionType == ActionType)
		{
			return Action;
		}
	}
	return FCombatActionData();
}

bool UValorCombatData::HasAction(ECombatAction ActionType) const
{
	for (const FCombatActionData& Action : AvailableActions)
	{
		if (Action.ActionType == ActionType)
		{
			return true;
		}
	}
	return false;
}