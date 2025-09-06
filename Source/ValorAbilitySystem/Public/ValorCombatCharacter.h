// Copyright © John Stratton 2024. All rights reserved. Valor & Blade is a trademark of John Stratton. This game was developed using Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. This software product contains Unreal® Engine code, including but not limited to computer code, audio-visual materials, and related documentation ("Unreal® Engine Code"). © 2024, Epic Games, Inc. All rights reserved. Any unauthorized copying, alteration, distribution, transmission, performance, display, or other use of this game or any part thereof is prohibited without the express written consent of John Stratton. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 - 2024, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ValorAbilitySystem/Public/ValorCharacter.h"
#include "ValorAbilitySystem/Public/ValorCombatData.h"
#include "ValorCombatCharacter.generated.h"

class UValorCombatData;

/**
 * AValorCombatCharacter
 *
 * Extended character class that demonstrates the full integration of the combat system.
 * This class shows how to use the combat component with input handling and ability management.
 */
UCLASS(Blueprintable, ClassGroup=(ValorAbilitySystem), meta=(BlueprintSpawnableComponent))
class VALORABILITYSYSTEM_API AValorCombatCharacter : public AValorCharacter
{
	GENERATED_BODY()

public:
	AValorCombatCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Combat Data Asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Configuration")
	UValorCombatData* CombatData;

	// Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* LightAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* HeavyAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* BlockAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* DodgeAction;

	// Input Handlers
	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnLightAttackPressed();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnHeavyAttackPressed();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnBlockPressed();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnBlockReleased();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnDodgePressed();

	// Combat Event Handlers
	UFUNCTION(BlueprintImplementableEvent, Category = "Combat Events")
	void OnHealthChangedEvent(float CurrentHealth, float MaxHealth, float Percentage);

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat Events")
	void OnCharacterDiedEvent(AActor* Character, AActor* Killer);

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat Events")
	void OnDamageDealtEvent(AActor* Target, float Damage, FGameplayTag DamageType);

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat Events")
	void OnDamageReceivedEvent(AActor* Source, float Damage, FGameplayTag DamageType);

private:
	// Internal functions
	void BindCombatEvents();
	void UnbindCombatEvents();
	void GrantCombatAbilities();

	// Combat ability references
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> CombatAbilityHandles;
};