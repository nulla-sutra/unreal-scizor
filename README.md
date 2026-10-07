# Scizor

Scizor provides combo-window queries and typed input events for Unreal Engine StateTree and GAS. `UScizorComboComponent` inherits the official `UStateTreeComponent`, using its schema, execution lifecycle, and tick scheduling.

## Setup

1. Add `ScizorComboComponent` to the actor.
2. Use the official **StateTree Component** schema for the tree and assign it to the combo component's inherited **State Tree** property.
3. Add **Combo Component (Scizor)** as a global evaluator. Bind its `Actor` input to the schema's `Actor` context.
4. Bind combo conditions/property functions to the combo evaluator's `ComboComponent` output.
5. Initialize GAS ActorInfo before starting the tree. Disable **Start Logic Automatically** when initialization occurs after BeginPlay, then call `StartLogic` on the combo component when ready.
6. Send input through `SendComboInputEvent`. Call inherited StateTree/Brain APIs directly on the combo component.

Scizor resolves ASC through Unreal's GameplayAbilities API, including Pawn/Controller PlayerState fallback, and reads the avatar mesh from initialized GAS ActorInfo. For StateTrees that also run GAS tasks, bind their inputs through a GAS provider chosen by the project. Gengar's **Ability System** global evaluator is an optional provider.

The combo evaluator provides data. Gameplay Abilities remain responsible for ability execution and their network policies. Sending a StateTree event queues a local event; it is not an RPC.

## Combo windows

`GetComboInfoSummary` reports NoCombo, BeforeComboWindow, InsideComboWindow, or AfterComboWindow, using the active montage section and `AnimNotifyState_ScizorComboWindow` timestamps. Animation delegates are rebound when the avatar mesh or AnimInstance changes and removed on EndPlay.

`SendComboInputEvent` packages the optional typed input payload and calls the inherited `SendStateTreeEvent` on this component. Tree startup follows the official component's **Start Logic Automatically** setting or an explicit `StartLogic` call.

## Dependencies

- StateTree
- GameplayStateTree
- GameplayAbilities
- EnhancedInput

## Migrating Treecko assets

Use the official StateTree Component schema and rebind legacy ASC/Avatar/Mesh/Controller inputs to the project's GAS provider. Rebind combo inputs to the combo evaluator. Replace unbound Treecko delay tasks with delayed OnTick transitions.

Keep each actor's `StateTreeRef`, parameters, and linked overrides on `ScizorComboComponent`. Call StateTree/Brain APIs directly on it. For assets created with a separate combo-tree component, copy that component's tree reference and startup settings to Scizor, then remove the separate component and replace `GetStateTreeComponent()` connections with the combo component itself.

Compile and save migrated assets before removing Treecko. Scizor extends the official StateTree component without defining a custom schema.

## License

[Mozilla Public License 2.0](LICENSE).
