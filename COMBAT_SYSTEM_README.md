# Valor Combat System

This document explains how to use the combat system component that has been added to the Valor Ability System plugin.

## Overview

The combat system consists of several key components:

1. **ValorCombatComponent** - Core combat functionality
2. **ValorCombatData** - Data assets for combat configuration
3. **ValorCombatAbility** - Base class for combat abilities
4. **ValorCombatCharacter** - Example character with full combat integration

## Components

### ValorCombatComponent

The main combat component that handles:
- Health management
- Damage dealing and receiving
- Combat state management (attacking, blocking, dodging)
- Combat events and callbacks
- Integration with the Gameplay Ability System

#### Key Features:
- **Health System**: Automatic health management with events
- **Damage System**: Configurable damage types and calculations
- **Combat States**: Track attacking, blocking, dodging states
- **Resource Management**: Stamina and mana consumption
- **Event System**: Blueprint-friendly events for UI and gameplay

### ValorCombatData

Data asset containing:
- Combat statistics (damage, defense, critical chance, etc.)
- Available combat actions
- Damage type tags
- Combat state tags

### Combat Abilities

Pre-built combat abilities:
- **Light Attack**: Quick, low-cost attack
- **Heavy Attack**: Powerful, high-cost attack with knockback
- **Block**: Damage reduction while active
- **Dodge**: Brief invincibility and movement

## Usage

### Basic Setup

1. **Add to Character**: The `ValorCharacter` class already includes the combat component
2. **Configure Attributes**: Set up health, stamina, and mana in the attribute set
3. **Grant Abilities**: Add combat abilities to the character's ability system

### Example Character Setup

```cpp
// In your character's BeginPlay()
void AMyCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // Get the combat component
    UValorCombatComponent* CombatComp = GetCombatComponent();
    
    // Bind to combat events
    CombatComp->OnHealthChanged.AddDynamic(this, &AMyCharacter::OnHealthChanged);
    CombatComp->OnCharacterDied.AddDynamic(this, &AMyCharacter::OnCharacterDied);
    
    // Grant combat abilities
    if (AbilitySystemComponent)
    {
        FGameplayAbilitySpec LightAttackSpec(UValorLightAttackAbility::StaticClass(), 1, 0);
        AbilitySystemComponent->GiveAbility(LightAttackSpec);
    }
}
```

### Input Handling

```cpp
// In SetupPlayerInputComponent()
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    // Bind input actions to combat abilities
    PlayerInputComponent->BindAction("LightAttack", IE_Pressed, this, &AMyCharacter::OnLightAttack);
    PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AMyCharacter::OnBlockPressed);
    PlayerInputComponent->BindAction("Block", IE_Released, this, &AMyCharacter::OnBlockReleased);
}

void AMyCharacter::OnLightAttack()
{
    if (AbilitySystemComponent)
    {
        FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->FindAbilitySpecFromClass(UValorLightAttackAbility::StaticClass());
        AbilitySystemComponent->TryActivateAbility(Handle);
    }
}
```

### Blueprint Integration

The combat system is fully Blueprint-accessible:

1. **Events**: All combat events can be bound in Blueprint
2. **Functions**: All combat functions are Blueprint-callable
3. **Properties**: All combat properties are Blueprint-editable

### Custom Combat Abilities

To create custom combat abilities:

1. Inherit from `UValorCombatAbility`
2. Override `ActivateAbility` to implement your logic
3. Use the provided helper functions for damage, targeting, etc.

```cpp
UCLASS()
class MYGAME_API UMyCustomAbility : public UValorCombatAbility
{
    GENERATED_BODY()

public:
    UMyCustomAbility();

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override
    {
        Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
        
        // Your custom combat logic here
        AActor* Target = GetPrimaryTarget();
        if (Target)
        {
            FDamageInfo DamageInfo;
            DamageInfo.Damage = 50.0f;
            DamageInfo.DamageType = EDamageType::Magical;
            ApplyDamage(Target, DamageInfo);
        }
    }
};
```

## Configuration

### Combat Data Asset

Create a `ValorCombatData` asset to configure:
- Base combat stats
- Available actions
- Damage type mappings
- Combat state tags

### Attribute Setup

Ensure your character has the required attributes:
- Health / MaxHealth
- Stamina / MaxStamina
- Mana / MaxMana
- Strength, Dexterity, Intelligence

## Events

The combat system provides several events for integration:

- `OnHealthChanged`: When health changes
- `OnCharacterDied`: When character dies
- `OnDamageDealt`: When damage is dealt
- `OnDamageReceived`: When damage is received

## Best Practices

1. **Use Data Assets**: Configure combat stats through data assets for easy balancing
2. **Event-Driven**: Use combat events for UI updates and gameplay responses
3. **Resource Management**: Always check stamina/mana before activating abilities
4. **State Management**: Use combat states to prevent invalid actions
5. **Blueprint Integration**: Expose key functions to Blueprint for designers

## Troubleshooting

### Common Issues

1. **Abilities Not Activating**: Check if character has enough stamina/mana
2. **Damage Not Applying**: Ensure target has a combat component
3. **Events Not Firing**: Verify event bindings are set up correctly
4. **Attributes Not Updating**: Check if attribute set is properly initialized

### Debug Tips

- Use `GetCombatComponent()` to access combat functionality
- Check `IsAlive()` before performing combat actions
- Use Blueprint debugger to trace ability activation
- Monitor attribute values in the attribute set

## Future Enhancements

Potential additions to the combat system:
- Faction system for enemy detection
- Status effects and buffs/debuffs
- Combo system for chaining attacks
- Area-of-effect abilities
- Projectile system integration
- Animation integration helpers