# Scizor

![UPlugin](https://img.shields.io/badge/.uplugin-1.0-blue)
![License](https://img.shields.io/badge/license-MPL--2.0-blue)

A powerful combo system plugin for Unreal Engine 5, built on StateTree architecture and integrated with the Gameplay Ability System (GAS) and Enhanced Input.

## Features

- **StateTree-Based Combo System**: Leverages Unreal Engine's StateTree for flexible and efficient combo management
- **ScizorComboComponent**: Core component for managing combo states and logic
- **Combo Window Management**: Sophisticated state management system with four distinct phases:
  - NoCombo
  - BeforeComboWindow
  - InsideComboWindow
  - AfterComboWindow
- **Animation Montage Integration**: Seamless integration with Unreal's animation system for combo execution
- **Enhanced Input Support**: Full integration with Unreal Engine's Enhanced Input system for responsive combo inputs
- **StateTree Nodes**: Custom conditions and property functions for combo logic
  - Combo window check conditions
  - Combo input check conditions
  - Property functions for combo state queries
- **Custom Schema Support**: Extensible schema system for custom combo configurations
- **GAS Integration**: Full compatibility with the Gameplay Ability System

## Dependencies

Scizor requires the following Unreal Engine plugins to be enabled:

- **[unreal-treecko](https://github.com/nulla-sutra/unreal-treecko)** (Required)
- **StateTree** (Required)
- **GameplayStateTree** (Required)
- **GameplayAbilities** (Required)
- **EnhancedInput** (Required)

## Installation

1. Clone or download this repository
2. Copy the `Scizor` folder to your project's `Plugins` directory
3. If the `Plugins` directory doesn't exist, create it in your project root
4. Open your Unreal Engine project
5. When prompted, allow the engine to rebuild the plugin
6. Enable the plugin in Edit → Plugins → Search for "Scizor"
7. Restart the editor

## Usage

### Basic Setup

1. **Add the ScizorComboComponent** to your character or actor:
   ```cpp
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combo")
   TObjectPtr<UScizorComboComponent> ComboComponent;
   ```

2. **Configure the StateTree** for your combo system using the Scizor schema

3. **Send combo input events** when the player performs an action:
   ```cpp
   ComboComponent->SendComboInputEvent(ComboInputTag, Payload);
   ```

4. **Query combo state** to check the current combo window:
   ```cpp
   FScizorComboInfoSummary ComboInfo = ComboComponent->GetComboInfoSummary();
   ```

### Key Concepts

- **Combo Windows**: The system tracks animation phases and combo timing through window states
- **Event-Driven**: Combos are triggered through gameplay events sent to the StateTree
- **Input Actions**: Enhanced Input actions are used to identify specific combo inputs

## License

This project is licensed under the Mozilla Public License Version 2.0 (MPL-2.0). See the [LICENSE](LICENSE) file for details.

## Author

Created by [tarnishablec](https://github.com/tarnishablec)

## Contributing

This plugin is currently in beta. Contributions, issues, and feature requests are welcome!
