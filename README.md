# SC_Radio

A small UE5 plugin for simulating radio communication between actors, built for drone simulation. Transmitters broadcast messages; receivers within reach get them. *How* signals propagate is pluggable via a swappable environment.

## Concepts

- **Transmitter** (`USC_TransmitterComponent`) - call `SendMessage(Message)` to broadcast from the component's location.
- **Receiver** (`USC_ReceiverComponent`) - bind to `OnMessageReceived(Message, DeliveryInfo)` to react. Registers itself with the world automatically.
- **Message** (`USC_RadioMessage`) - the payload. Subclass it (e.g. `USC_RadioMessage_String`) for your own data.
- **Environment** (`ISC_RadioEnvironment`) - the propagation model. Decides who receives what. One active per world, owned by `USC_RadioWorldSubsystem`.

## Environments

| Class | Models | Delivery |
|-------|--------|----------|
| `InstantBroadcast` | Radius only | Immediate |
| `LineOfSight` | Path loss (RSSI) + obstruction | Immediate |
| `SharedChannel` | LineOfSight + channels, interference, collisions (SINR) | End of frame |

Subclass any of these in Blueprint to expose tuned presets (urban, open field, etc.).

## Quick start

1. Add a `USC_TransmitterComponent` to the sending actor and a `USC_ReceiverComponent` to each listener.
2. On the receiver, bind `OnMessageReceived`.
3. Create a message (e.g. `USC_RadioMessage_String`), set its data, and call `SendMessage` on the transmitter.
4. Choose the propagation model in **Project Settings → Game → SC Radio Settings → Radio Environment Class**. Empty defaults to `InstantBroadcast`.

## Notes

- `DeliveryInfo` reports `RssiDbm`, `Distance`, `Channel`, `bLineOfSightBlocked`, and (for `SharedChannel`) `SinrDb`.
- `SendMessage` returns whether the message was *accepted*. With `SharedChannel`, delivery is decided next tick and surfaces through `OnMessageReceived`.
- The environment is selected once at world startup; changing the setting applies on the next map load / PIE session.
- `LineOfSight` traces on `ECC_Visibility` by default - geometry meant to block signal must respond to that channel.

## Extending

Add a propagation model by implementing `ISC_RadioEnvironment`; add a payload type by subclassing `USC_RadioMessage`. New classes appear automatically in the settings dropdown and the editable-inline message picker.