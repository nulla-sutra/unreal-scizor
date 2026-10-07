# Scizor

Scizor provides combo-window queries and input routing for Unreal Engine StateTree and GAS. It uses a separate, official `UStateTreeComponent`; the combo component is a regular `UActorComponent`.

## Setup

1. Add `ScizorComboComponent` and a stock `StateTreeComponent` to the actor.
2. Use the official **StateTree Component** schema for the tree.
3. Add **Combo Component (Scizor)** as a global evaluator. Bind its `Actor` input to the schema's `Actor` context.
4. Bind combo conditions/property functions to the combo evaluator's `ComboComponent` output.
5. Set the combo component's `StateTreeComponentReference` to the desired StateTree component. An actor with exactly one StateTree can use the automatic lookup.
6. Initialize GAS ActorInfo before calling the official component's `StartLogic`. Send input through `SendComboInputEvent`.

Scizor resolves ASC through Unreal's GameplayAbilities API, including Pawn/Controller PlayerState fallback, and reads the avatar mesh from initialized GAS ActorInfo. For StateTrees that also run GAS tasks, bind their inputs through a GAS provider chosen by the project. Gengar's **Ability System** global evaluator is an optional provider.

The combo evaluator provides data. Gameplay Abilities remain responsible for ability execution and their network policies. Sending a StateTree event queues a local event; it is not an RPC.

## Combo windows

`GetComboInfoSummary` reports NoCombo, BeforeComboWindow, InsideComboWindow, or AfterComboWindow, using the active montage section and `AnimNotifyState_ScizorComboWindow` timestamps. Animation delegates are rebound when the avatar mesh or AnimInstance changes and removed on EndPlay.

`SendComboInputEvent` accepts an optional input payload and forwards it to the selected stock StateTree component. It does not start the tree automatically.

## Dependencies

- StateTree
- GameplayStateTree
- GameplayAbilities
- EnhancedInput

## Migrating Treecko assets

Use the official StateTree Component schema and rebind legacy ASC/Avatar/Mesh/Controller inputs to the project's GAS provider. Rebind combo inputs to the combo evaluator. Replace unbound Treecko delay tasks with delayed OnTick transitions.

Move each actor's old `StateTreeRef` and parameters to its new stock component. In combo-input abilities, route inherited StateTree/Brain calls through `ScizorComboComponent.GetStateTreeComponent()`. Keep combo-window queries on the combo component.

Compile and save migrated assets before removing Treecko. The plugin no longer defines a custom StateTree component or schema.

## License

[Mozilla Public License 2.0](LICENSE).
