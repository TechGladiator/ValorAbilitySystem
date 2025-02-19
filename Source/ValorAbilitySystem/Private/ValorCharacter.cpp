// Copyright © John Stratton 2024. All rights reserved. Valor & Blade is a trademark of John Stratton. This game was developed using Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. This software product contains Unreal® Engine code, including but not limited to computer code, audio-visual materials, and related documentation ("Unreal® Engine Code"). © 2024, Epic Games, Inc. All rights reserved. Any unauthorized copying, alteration, distribution, transmission, performance, display, or other use of this game or any part thereof is prohibited without the express written consent of John Stratton. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved.


#include "ValorAbilitySystem/Public/ValorCharacter.h"
#include "ValorAbilitySystem/Public/GAS/ValorAttributeSet.h"

// Sets default values
AValorCharacter::AValorCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize the Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	// Initialize the Attribute Set
	AttributeSet = CreateDefaultSubobject<UValorAttributeSet>(TEXT("AttributeSet"));

}

UAbilitySystemComponent* AValorCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void AValorCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Ensure the Ability System Component is valid
	if (AbilitySystemComponent)
	{
		// Initialize default abilities and attributes here, if needed
		// Example: Grant an initial ability
		// AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(YourAbilityClass, 1, 0));

		// Initialize default attribute values if necessary
		// Example: Set an initial value for health if it hasn't been set
		if (AttributeSet)
		{
			// Ensure that the initial attribute values are set
			AttributeSet->Health.SetBaseValue(100.0f);
			AttributeSet->Health.SetCurrentValue(100.0f);

			AttributeSet->Stamina.SetBaseValue(100.0f);
			AttributeSet->Stamina.SetCurrentValue(100.0f);

			AttributeSet->Mana.SetBaseValue(100.0f);
			AttributeSet->Mana.SetCurrentValue(100.0f);

			AttributeSet->Strength.SetBaseValue(10.0f);
			AttributeSet->Strength.SetCurrentValue(10.0f);

			AttributeSet->Dexterity.SetBaseValue(10.0f);
			AttributeSet->Dexterity.SetCurrentValue(10.0f);

			AttributeSet->Intelligence.SetBaseValue(10.0f);
			AttributeSet->Intelligence.SetCurrentValue(10.0f);

		}
	}
	
}

// Called every frame
void AValorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AValorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

