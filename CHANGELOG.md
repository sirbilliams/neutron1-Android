# Changelog

## v1.0-rc5

First step towards a seamless Retroville.

- New launcher setting **Seamless transitions (beta)**, off by default: walking between the Neighborhood and the School no longer shows a fade or loading picture. The School loads in the background while you walk up the road and the view dissolves into it in a third of a second (about 0.1 s of real loading on a PC instead of 1.1 s). Story gates, roadblocks and saves are unchanged.

## v1.0-rc4

The bottom-screen map from Negatron 1.0.3-1.0.5, in both panel themes.

- Companion map: tap EXPAND MAP for a full-screen, whole-level map with touch pan, pinch zoom, RECENTER, FIT LEVEL and item / enemy / checkpoint / exit filters; tap DASHBOARD to slide back.
- Exit labels use friendly level names, one label per exit, kept clear of Jimmy, the toolbar and the compass.
- Score screen: the A button skips the count-up again.
- Desktop: a Flatpak build (packaging/flatpak).

## v1.0-rc3

Fixes from the second Thor play-test.

- Background music plays again in every level (the music volume started at 0: the original sets it in a start-up initialiser the port never ran).
- The A button only uses Jimmy's gadget; it no longer also opens the gadgets page. X opens the gadgets page, Y the inventory page.
- Widescreen: the HUD (energy, lives, gadget pictures, timers, pickup counters) sits at the real screen edges instead of a 4:3 box.
- Bill's UI now works with Boy Genius: tube, orbit, dial and bar energy meters, the gadget hologram, and Negatron 2's score counter showing Boy Genius's own score.

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
