// Copyright © John Stratton 2024. All rights reserved. Valor & Blade is a trademark of John Stratton. This game was developed using Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. This software product contains Unreal® Engine code, including but not limited to computer code, audio-visual materials, and related documentation ("Unreal® Engine Code"). © 2024, Epic Games, Inc. All rights reserved. Any unauthorized copying, alteration, distribution, transmission, performance, display, or other use of this game or any part thereof is prohibited without the express written consent of John Stratton. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved.


#include "ValorAbilitySystem/Public/GAS/ValorAttributeSet.h"
#include "GameplayEffectExtension.h"

UValorAttributeSet::UValorAttributeSet()
{
    // Initialize default values for attributes
    Health.SetBaseValue(100.0f);
    Health.SetCurrentValue(100.0f);
    MaxHealth.SetBaseValue(100.0f);
    MaxHealth.SetCurrentValue(100.0f);

    Stamina.SetBaseValue(100.0f);
    Stamina.SetCurrentValue(100.0f);
    MaxStamina.SetBaseValue(100.0f);
    MaxStamina.SetCurrentValue(100.0f);

    Mana.SetBaseValue(100.0f);
    Mana.SetCurrentValue(100.0f);
    MaxMana.SetBaseValue(100.0f);
    MaxMana.SetCurrentValue(100.0f);

    Strength.SetBaseValue(10.0f);
    Strength.SetCurrentValue(10.0f);
    MaxStrength.SetBaseValue(10.0f);
    MaxStrength.SetCurrentValue(10.0f);

    Dexterity.SetBaseValue(10.0f);
    Dexterity.SetCurrentValue(10.0f);
    MaxDexterity.SetBaseValue(10.0f);
    MaxDexterity.SetCurrentValue(10.0f);

    Intelligence.SetBaseValue(10.0f);
    Intelligence.SetCurrentValue(10.0f);
    MaxIntelligence.SetBaseValue(10.0f);
    MaxIntelligence.SetCurrentValue(10.0f);

    ExperiencePoints.SetBaseValue(0.0f);
    ExperiencePoints.SetCurrentValue(0.0f);
    MaxExperiencePoints.SetBaseValue(100.0f);
    MaxExperiencePoints.SetCurrentValue(100.0f);
}

void UValorAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);

    // Ensure that the new values for MaxHealth, MaxStamina, etc., are always valid
    if (Attribute == GetMaxHealthAttribute())
    {
        Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.0f, NewValue));
    }
    else if (Attribute == GetMaxStaminaAttribute())
    {
        Stamina.SetCurrentValue(FMath::Clamp(Stamina.GetCurrentValue(), 0.0f, NewValue));
    }
    else if (Attribute == GetMaxManaAttribute())
    {
        Mana.SetCurrentValue(FMath::Clamp(Mana.GetCurrentValue(), 0.0f, NewValue));
    }
    else if (Attribute == GetMaxStrengthAttribute())
    {
        Strength.SetCurrentValue(FMath::Clamp(Strength.GetCurrentValue(), 0.0f, NewValue));
    }
    else if (Attribute == GetMaxDexterityAttribute())
    {
        Dexterity.SetCurrentValue(FMath::Clamp(Dexterity.GetCurrentValue(), 0.0f, NewValue));
    }
    else if (Attribute == GetMaxIntelligenceAttribute())
    {
        Intelligence.SetCurrentValue(FMath::Clamp(Intelligence.GetCurrentValue(), 0.0f, NewValue));
    }
    else if (Attribute == GetMaxExperiencePointsAttribute())
    {
        ExperiencePoints.SetCurrentValue(FMath::Clamp(ExperiencePoints.GetCurrentValue(), 0.0f, NewValue));
    }
}

void UValorAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    // Handle changes in attributes here, for example:
    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.0f, MaxHealth.GetCurrentValue()));
    }
    else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
    {
        Stamina.SetCurrentValue(FMath::Clamp(Stamina.GetCurrentValue(), 0.0f, MaxStamina.GetCurrentValue()));
    }
    else if (Data.EvaluatedData.Attribute == GetManaAttribute())
    {
        Mana.SetCurrentValue(FMath::Clamp(Mana.GetCurrentValue(), 0.0f, MaxMana.GetCurrentValue()));
    }
	else if (Data.EvaluatedData.Attribute == GetStrengthAttribute())
	{
		Strength.SetCurrentValue(FMath::Clamp(Strength.GetCurrentValue(), 0.0f, MaxStrength.GetCurrentValue()));
	}
	else if (Data.EvaluatedData.Attribute == GetDexterityAttribute())
	{
		Dexterity.SetCurrentValue(FMath::Clamp(Dexterity.GetCurrentValue(), 0.0f, MaxDexterity.GetCurrentValue()));
	}
	else if (Data.EvaluatedData.Attribute == GetIntelligenceAttribute())
	{
		Intelligence.SetCurrentValue(FMath::Clamp(Intelligence.GetCurrentValue(), 0.0f, MaxIntelligence.GetCurrentValue()));
	}
	else if (Data.EvaluatedData.Attribute == GetExperiencePointsAttribute())
	{
		ExperiencePoints.SetCurrentValue(FMath::Clamp(ExperiencePoints.GetCurrentValue(), 0.0f, MaxExperiencePoints.GetCurrentValue()));
	}
}

