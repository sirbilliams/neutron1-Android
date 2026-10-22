# Continuation prompt (paste into a fresh session)

Continue the Jimmy Neutron: Boy Genius Android port (github sirbilliams/neutron1-android; the finished
Negatron port is sirbilliams/neutron2-android; game files + decomp inputs in the private repo
sirbilliams/negatron-gamefiles). Read docs/HANDOFF.md first: "Status ... READ THIS FIRST", then
"Game plan for the next sessions", and docs/agent-brief.md for agents.

Rules (Bill's): never commit game files or exe/DLL bytes; no leaked source; faithful to Neutron.exe,
deviations marked `// port:`; commits as `git -c user.name=sirbilliams -c user.email=bravehooves@gmail.com`;
push to main only after build + smoke test (+ an ASan desktop run before sending an APK); every push
to main builds the "latest" APK; send Bill an APK and pictures when there's something new; keep status
notes short.

Usage plan (to stretch Bill's limits):
- The lead (Opus) plans, merges and checks; agents run on Sonnet (`model: "sonnet"`) for porting
  classes, levels and tests; Opus agents only for hard core/debugging work.
- At most 2-3 agents at once (2-CPU machine, `make -j2`); one job per agent ("port C3DGoddard",
  "the School level"), fresh agent per job, short brief; agents report in <= 12 lines.
- The lead doesn't poll: wait for agent and CI notifications.
- End every session by updating docs/HANDOFF.md and this file.

First steps: set up per HANDOFF "Setting up" (restore /home/claude/decomp1 from
decomp/bg-decomp-inputs.tar.gz, game from the iso release), ask Bill for his Thor notes on the latest
APK, then step 1 of the game plan.
