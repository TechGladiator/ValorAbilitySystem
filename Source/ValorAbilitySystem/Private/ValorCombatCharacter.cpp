// Copyright © John Stratton 2024. All rights reserved. Valor & Blade is a trademark of John Stratton. This game was developed using Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. This software product contains Unreal® Engine code, including but not limited to computer code, audio-visual materials, and related documentation ("Unreal® Engine Code"). © 2024, Epic Games, Inc. All rights reserved. Any unauthorized copying, alteration, distribution, transmission, performance, display, or other use of this game or any part thereof is prohibited without the express written consent of John Stratton. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved.

#include "ValorCombatCharacter.h"
#include "ValorAbilitySystem/Public/ValorCombatAbilities.h"
#include "ValorAbilitySystem/Public/ValorCombatComponent.h"
#include "ValorAbilitySystem/Public/ValorCombatData.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagsModule.h"

AValorCombatCharacter::AValorCombatCharacter()
{
	// Set default values
	PrimaryActorTick.bCanEverTick = true;
}

void AValorCombatCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Bind combat events
	BindCombatEvents();

	// Grant combat abilities
	GrantCombatAbilities();
}

void AValorCombatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up input bindings for combat actions
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (LightAttackAction)
		{
			EnhancedInputComponent->BindAction(LightAttackAction, ETriggerEvent::Triggered, this, &AValorCombatCharacter::OnLightAttackPressed);
		}

		if (HeavyAttackAction)
		{
			EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Triggered, this, &AValorCombatCharacter::OnHeavyAttackPressed);
		}

		if (BlockAction)
		{
			EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Triggered, this, &AValorCombatCharacter::OnBlockPressed);
			EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &AValorCombatCharacter::OnBlockReleased);
		}

		if (DodgeAction)
		{
			EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AValorCombatCharacter::OnDodgePressed);
		}
	}
}

void AValorCombatCharacter::OnLightAttackPressed()
{
	if (!CombatComponent || !AbilitySystemComponent)
	{
		return;
	}

	// Try to activate light attack ability
	FGameplayAbilitySpecHandle LightAttackHandle = AbilitySystemComponent->FindAbilitySpecFromClass(UValorLightAttackAbility::StaticClass());
	if (LightAttackHandle.IsValid())
	{
		AbilitySystemComponent->TryActivateAbility(LightAttackHandle);
	}
}

void AValorCombatCharacter::OnHeavyAttackPressed()
{
	if (!CombatComponent || !AbilitySystemComponent)
	{
		return;
	}

	// Try to activate heavy attack ability
	FGameplayAbilitySpecHandle HeavyAttackHandle = AbilitySystemComponent->FindAbilitySpecFromClass(UValorHeavyAttackAbility::StaticClass());
	if (HeavyAttackHandle.IsValid())
	{
		AbilitySystemComponent->TryActivateAbility(HeavyAttackHandle);
	}
}

void AValorCombatCharacter::OnBlockPressed()
{
	if (!CombatComponent || !AbilitySystemComponent)
	{
		return;
	}

	// Try to activate block ability
	FGameplayAbilitySpecHandle BlockHandle = AbilitySystemComponent->FindAbilitySpecFromClass(UValorBlockAbility::StaticClass());
	if (BlockHandle.IsValid())
	{
		AbilitySystemComponent->TryActivateAbility(BlockHandle);
	}
}

void AValorCombatCharacter::OnBlockReleased()
{
	if (!CombatComponent || !AbilitySystemComponent)
	{
		return;
	}

	// End block ability
	FGameplayAbilitySpecHandle BlockHandle = AbilitySystemComponent->FindAbilitySpecFromClass(UValorBlockAbility::StaticClass());
	if (BlockHandle.IsValid())
	{
		AbilitySystemComponent->CancelAbility(BlockHandle);
	}
}

void AValorCombatCharacter::OnDodgePressed()
{
	if (!CombatComponent || !AbilitySystemComponent)
	{
		return;
	}

	// Try to activate dodge ability
	FGameplayAbilitySpecHandle DodgeHandle = AbilitySystemComponent->FindAbilitySpecFromClass(UValorDodgeAbility::StaticClass());
	if (DodgeHandle.IsValid())
	{
		AbilitySystemComponent->TryActivateAbility(DodgeHandle);
	}
}

void AValorCombatCharacter::BindCombatEvents()
{
	if (!CombatComponent)
	{
		return;
	}

	// Bind to combat component events
	CombatComponent->OnHealthChanged.AddDynamic(this, &AValorCombatCharacter::OnHealthChangedEvent);
	CombatComponent->OnCharacterDied.AddDynamic(this, &AValorCombatCharacter::OnCharacterDiedEvent);
	CombatComponent->OnDamageDealt.AddDynamic(this, &AValorCombatCharacter::OnDamageDealtEvent);
	CombatComponent->OnDamageReceived.AddDynamic(this, &AValorCombatCharacter::OnDamageReceivedEvent);
}

void AValorCombatCharacter::UnbindCombatEvents()
{
	if (!CombatComponent)
	{
		return;
	}

	// Unbind from combat component events
	CombatComponent->OnHealthChanged.RemoveDynamic(this, &AValorCombatCharacter::OnHealthChangedEvent);
	CombatComponent->OnCharacterDied.RemoveDynamic(this, &AValorCombatCharacter::OnCharacterDiedEvent);
	CombatComponent->OnDamageDealt.RemoveDynamic(this, &AValorCombatCharacter::OnDamageDealtEvent);
	CombatComponent->OnDamageReceived.RemoveDynamic(this, &AValorCombatCharacter::OnDamageReceivedEvent);
}

void AValorCombatCharacter::GrantCombatAbilities()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	// Grant combat abilities
	FGameplayAbilitySpec LightAttackSpec(UValorLightAttackAbility::StaticClass(), 1, 0);
	FGameplayAbilitySpecHandle LightAttackHandle = AbilitySystemComponent->GiveAbility(LightAttackSpec);
	CombatAbilityHandles.Add(LightAttackHandle);

	FGameplayAbilitySpec HeavyAttackSpec(UValorHeavyAttackAbility::StaticClass(), 1, 0);
	FGameplayAbilitySpecHandle HeavyAttackHandle = AbilitySystemComponent->GiveAbility(HeavyAttackSpec);
	CombatAbilityHandles.Add(HeavyAttackHandle);

	FGameplayAbilitySpec BlockSpec(UValorBlockAbility::StaticClass(), 1, 0);
	FGameplayAbilitySpecHandle BlockHandle = AbilitySystemComponent->GiveAbility(BlockSpec);
	CombatAbilityHandles.Add(BlockHandle);

	FGameplayAbilitySpec DodgeSpec(UValorDodgeAbility::StaticClass(), 1, 0);
	FGameplayAbilitySpecHandle DodgeHandle = AbilitySystemComponent->GiveAbility(DodgeSpec);
	CombatAbilityHandles.Add(DodgeHandle);
}