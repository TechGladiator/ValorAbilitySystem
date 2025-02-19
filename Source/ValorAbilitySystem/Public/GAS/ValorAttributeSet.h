// Copyright © John Stratton 2024. All rights reserved. Valor & Blade is a trademark of John Stratton. This game was developed using Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. This software product contains Unreal® Engine code, including but not limited to computer code, audio-visual materials, and related documentation ("Unreal® Engine Code"). © 2024, Epic Games, Inc. All rights reserved. Any unauthorized copying, alteration, distribution, transmission, performance, display, or other use of this game or any part thereof is prohibited without the express written consent of John Stratton. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ValorAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * UValorAttributeSet
 *
 * Attribute set class containing character attributes such as health, stamina, mana, and more.
 * Used in conjunction with the Ability System Component for robust gameplay mechanics.
 */
UCLASS(Blueprintable, ClassGroup=(ValorAbilitySystem), meta=(BlueprintSpawnableComponent))
class VALORABILITYSYSTEM_API UValorAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
    UValorAttributeSet();
	
	// This function is called before an attribute value is changed. 
	// It allows you to modify the new value before the attribute is actually changed.
	// For example, you can clamp the new value within certain limits or apply additional logic.
	void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue);

	// This function is called after a gameplay effect has been executed and attributes have been modified.
	// It allows you to handle any special logic or events that should occur as a result of the attribute changes.
	// For example, you can clamp attributes to their maximum values or trigger other gameplay events.
	void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data);

    // Health
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UValorAttributeSet, Health)

    // Max Health
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UValorAttributeSet, MaxHealth)

    // Stamina
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FGameplayAttributeData Stamina;
    ATTRIBUTE_ACCESSORS(UValorAttributeSet, Stamina)

    // Max Stamina
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FGameplayAttributeData MaxStamina;
    ATTRIBUTE_ACCESSORS(UValorAttributeSet, MaxStamina)

    // Mana
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FGameplayAttributeData Mana;
    ATTRIBUTE_ACCESSORS(UValorAttributeSet, Mana)

    // Max Mana
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FGameplayAttributeData MaxMana;
    ATTRIBUTE_ACCESSORS(UValorAttributeSet, MaxMana)

    // Strength
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FGameplayAttributeData Strength;
    ATTRIBUTE_ACCESSORS(UValorAttributeSet, Strength)

    // Max Strength
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FGameplayAttributeData MaxStrength;
    ATTRIBUTE_ACCESSORS(UValorAttributeSet, MaxStrength)

    // Dexterity
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FGameplayAttributeData Dexterity;
    ATTRIBUTE_ACCESSORS(UValorAttributeSet, Dexterity)

    // Max Dexterity
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FGameplayAttributeData MaxDexterity;
    ATTRIBUTE_ACCESSORS(UValorAttributeSet, MaxDexterity)

    // Intelligence
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FGameplayAttributeData Intelligence;
    ATTRIBUTE_ACCESSORS(UValorAttributeSet, Intelligence)

    // Max Intelligence
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FGameplayAttributeData MaxIntelligence;
    ATTRIBUTE_ACCESSORS(UValorAttributeSet, MaxIntelligence)

    // Experience Points
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FGameplayAttributeData ExperiencePoints;
    ATTRIBUTE_ACCESSORS(UValorAttributeSet, ExperiencePoints)

    // Max Experience Points
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FGameplayAttributeData MaxExperiencePoints;
    ATTRIBUTE_ACCESSORS(UValorAttributeSet, MaxExperiencePoints)
};
