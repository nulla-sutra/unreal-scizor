# Scizor

Scizor provides combo-window queries and input routing for Unreal Engine StateTree and GAS. It uses a separate, official `UStateTreeComponent`; the combo component is a regular `UActorComponent`.

## Setup

1. Add `ScizorComboComponent` and a stock `StateTreeComponent` to the actor.
2. Use the official **StateTree Component** schema for the tree.
3. Add **Ability System (Gengar)** and **Combo Component (Scizor)** as global evaluators. Bind each evaluator's `Actor` input to the schema's `Actor` context.
4. Bind Ability tasks' ASC inputs to the GAS evaluator's `AbilitySystemComponent` output. Bind combo conditions/property functions to the combo evaluator's `ComboComponent` output.
5. Set the combo component's `StateTreeComponentReference` to the desired StateTree component. An actor with exactly one StateTree can use the automatic lookup.
6. Initialize GAS ActorInfo before calling the official component's `StartLogic`. Send input through `SendComboInputEvent`.

The GAS evaluator exposes `bReady`, ASC, AbilityOwner, Avatar, Pawn, MeshComponent, Controller, and PlayerController. Mesh and Controller are optional. It resolves ASC through GAS, including a Pawn's PlayerState, and refreshes on StateTree updates. It does not replicate another copy of GAS context.

The evaluators provide data. Gameplay Abilities remain responsible for ability execution and their network policies. Sending a StateTree event queues a local event; it is not an RPC.

## Combo windows

`GetComboInfoSummary` reports NoCombo, BeforeComboWindow, InsideComboWindow, or AfterComboWindow, using the active montage section and `AnimNotifyState_ScizorComboWindow` timestamps. Animation delegates are rebound when the avatar mesh or AnimInstance changes and removed on EndPlay.

`SendComboInputEvent` accepts an optional input payload and forwards it to the selected stock StateTree component. It does not start the tree automatically.

## Dependencies

- Gengar
- StateTree
- GameplayStateTree
- GameplayAbilities
- EnhancedInput

## Migrating Treecko assets

Use the official StateTree Component schema and rebind legacy ASC/Avatar/Mesh/Controller inputs to the GAS evaluator's outputs. Rebind combo inputs to the combo evaluator. Replace unbound Treecko delay tasks with delayed OnTick transitions.

Move each actor's old `StateTreeRef` and parameters to its new stock component. In combo-input abilities, route inherited StateTree/Brain calls through `ScizorComboComponent.GetStateTreeComponent()`. Keep combo-window queries on the combo component.

Compile and save migrated assets before removing Treecko. The plugin no longer defines a custom StateTree component or schema.

## License

[Mozilla Public License 2.0](LICENSE).
