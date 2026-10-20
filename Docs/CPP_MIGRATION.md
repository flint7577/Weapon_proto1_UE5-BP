# Dubok C++ Migration

The project identity and runtime module are now `Dubok`. The existing Blueprint assets remain in `Content` so their meshes, materials, animations, sounds, widgets, maps, and current level work can be reused while gameplay logic moves into C++.

The outdated GDD was deliberately not used as the migration specification. This pass mirrors the gameplay that was present in the project at migration time.

## Native gameplay classes

- `ADubokCharacter`: first-person movement, look, jump, crouch, sprint with stamina, weapon input, sonar pulse, health, and death.
- `ADubokWeapon`: two-click prime/fire loop, projectile spawning, ammunition, and refilling.
- `ADubokProjectile`: fast physical projectile, unlimited enemy penetration, unlimited wood penetration, and one hard-material penetration.
- `ADubokPenetrableWall`: reusable wood/kevlar wall actor with a sonar target component.
- `ADubokConcreteWall`: intact mesh plus dormant Geometry Collection, randomized shot requirement, Chaos strain, and impact impulse when broken.
- `UDubokSonarTargetComponent`: reusable temporary overlay-material reveal for any actor with mesh components.
- `ADubokAmmoPickup`: overlap pickup that refills the equipped weapon.
- `ADubokEnemy`: simple chase, contact attack, health, death, and sonar visibility.
- `ADubokPlayerController`: crosshair, pause/death/options widget hosting, respawn, and quit functions.
- `ADubokGameMode`: selects the native player character and controller.

## Asset hookup

The native classes deliberately expose assets and tuning values to the Unreal Editor. Create Blueprint children only as data/visual wrappers when needed; gameplay rules stay in C++.

1. Create a Blueprint child of `DubokCharacter` and assign the desired arms/model and animation Blueprint.
2. Create a Blueprint child of `DubokWeapon`, assign the weapon mesh, and position its `Muzzle` component.
3. Set that weapon child as the `Weapon` Child Actor Class on the character child.
4. Create wood and kevlar children of `DubokPenetrableWall`; assign their meshes, surface type, normal material, and sonar reveal material.
5. Create a child of `DubokConcreteWall`; assign matching intact mesh and fractured Geometry Collection. The collection remains hidden and non-simulating until the required shot count is reached.
6. Create children of `DubokAmmoPickup` and `DubokEnemy` to assign their final meshes, animations, audio, and sonar materials.
7. Existing widgets can call the public functions on `DubokPlayerController` for resume, options, respawn, and quit.

The global default GameMode now points to `/Script/Dubok.DubokGameMode`. If an existing map has its own World Settings GameMode Override, clear it or replace it with a Blueprint child of `DubokGameMode` before testing the native pawn.

## Verification

`DubokEditor Win64 Development` builds successfully with Unreal Engine 5.6. This confirms the module, reflection data, and native code compile. PIE behavior and asset assignments still require an editor pass because the source assets are binary Unreal packages.
