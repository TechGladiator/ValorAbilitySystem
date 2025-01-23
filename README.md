![Icon](Resources/Icon128.png)

# Valor Ability System Plugin

**Plugin for Unreal Engine 5: Valor Ability System**

This plugin provides a robust framework for managing gameplay abilities and attributes in Unreal Engine 5. It includes a base character class with an integrated Ability System Component and a comprehensive set of attributes, making it easier to create complex gameplay mechanics primarily using Blueprints. Ideal for RPGs, action games, and any project utilizing the Gameplay Ability System.

## Features

- **Base Character Class**: Includes `AValorCharacter`, a versatile base character class.
- **Ability System Component**: Integrated `UAbilitySystemComponent` for managing abilities.
- **Comprehensive Attributes**: Predefined attributes such as health, stamina, mana, strength, dexterity, intelligence, and experience points.
- **Blueprint Integration**: Easily used in Blueprint-only projects with optional C++ coding.
- **Modular Design**: Flexible structure suitable for a variety of gameplay mechanics.

## Installation

1. **Clone or Download**: Clone the repository or download the plugin as a ZIP file.
2. **Extract**: If downloaded as a ZIP file, extract it to your Unreal Engine project's `Plugins` directory.
3. **Build in your IDE.**
4. **Enable Plugin**: In Unreal Engine, navigate to `Edit > Plugins`, and enable the `Valor Ability System` plugin.
5. **Restart Editor**: Restart Unreal Engine to complete the plugin installation.

## Getting Started

### Blueprint-Only Instructions

1. **Create a Character**: Create a new Blueprint class based on `AValorCharacter`.
2. **Assign Abilities**: In the Blueprint editor, select the Ability System Component and assign abilities to your character.
3. **Manage Attributes**: Use Blueprints to initialize and manage character attributes.
4. **Set Up Input**: Define input actions in Project Settings and bind them to abilities in your character Blueprints.

### Example Usage in Blueprints

#### Initializing Attributes

1. **Open Your Character Blueprint**: Based on `AValorCharacter`.
2. **Access the Ability System Component**: Drag the Ability System Component to the Blueprint graph.
3. **Set Initial Attribute Values**: Use nodes to set initial values for attributes like health, stamina, etc.

```blueprint
// Pseudo-Blueprint instructions
Event BeginPlay
    Set Health (Value: 100)
    Set Stamina (Value: 100)
    Set Mana (Value: 100)
    // Add other attributes as needed
```

#### Assigning Abilities
1. **Open the Character Blueprint.**
2. **Find the Ability System Component.**
3. **Use the `Give Ability` node** to assign abilities to the character.

```blueprint
// Pseudo-Blueprint instructions
Event BeginPlay
    Give Ability (Ability: YourAbilityBlueprint)
```

### Optional C++ Instructions
For developers who prefer or need to use C++, the plugin also supports C++ enhancements:
1. **Create a Derived Class:** Derive a new class from `AValorCharacter` and add custom logic.
2. **Initialize Attributes and Abilities:** In the derived class, use the `BeginPlay` method to initialize attributes and abilities.

```cpp
void ACustomCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (AbilitySystemComponent)
    {
        if (AttributeSet)
        {
            AttributeSet->Health.SetBaseValue(100.0f);
            AttributeSet->Health.SetCurrentValue(100.0f);
            AttributeSet->Stamina.SetBaseValue(100.0f);
            AttributeSet->Stamina.SetCurrentValue(100.0f);
            // Add other attributes as needed
        }
    }
}
```

### Contributing
If you'd like to contribute to this plugin, feel free to submit pull requests or report issues.

### License
This plugin is released under the MIT License. See the [LICENSE](https://github.com/TechGladiator/ValorAbilitySystem/blob/main/LICENSE) file for details.

### Acknowledgments
Special thanks to the Unreal Engine community for their invaluable resources and support.

##

Enjoy developing with the Valor Ability System plugin and bring your gameplay mechanics to life!
