# Changelog

## v1.0-rc2

Smoother gameplay on high-refresh screens (first Thor play-test: speed and movement felt unstable).

- The game is paced at a steady 60 fps, like the original on its 60 Hz-era monitors; on 90/120/144 Hz screens the app asks for the 60 Hz display mode so frames land evenly.
- A long frame (loading, switching apps) no longer throws Jimmy or objects through floors and walls: the physics step is capped at 0.1 s.
- The device log (Test > Share log) now reports frame timing every 5 seconds.

## v1.0-rc1

Release candidate: every part of the game's own code is reconstructed and checked against `Neutron.exe`. Install over earlier builds is possible from this version on (one uninstall of a development build first: they were signed with a throwaway key).

- Boy Genius story levels load, including the late-story exit sequence and ending.
- Corrected gadget keys, shrink ray behaviour, player camera, animation timing and level fog.
- Fixed HUD placement, pickup counts, gadget picture scaling and energy display.
- Adapted the companion panel and gadget UI to Boy Genius; added Retroville, Yokian and Auto themes.
- Added .ase models to the Asset Viewer, BG HD texture pack support and adjustable draw distance.
- Loading pictures now come from the player's own game; improved transparent-object ordering.
- Stability: every level and VR01-08 run clean under AddressSanitizer / UBSan; unaligned reads in the engine's file and image readers fixed (they can crash on some ARM devices).
- Every story exit checked to load the right level in order; Yokian turret missiles hit and drain energy as in the original.
- Signed with a permanent key: later updates install in place and keep your saves.
