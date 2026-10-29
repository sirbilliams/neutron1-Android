#!/bin/sh
# Flatpak launcher: runs the game from the app's data folder with the settings in neutron1.conf.
DATA="$XDG_DATA_HOME/game"
CONF="$XDG_CONFIG_HOME/neutron1.conf"
if [ ! -d "$DATA/omt" ]; then
    echo "No game files: copy your Jimmy Neutron: Boy Genius game folder (omt/, gam/, Neutron.exe ...) to" >&2
    echo "  ~/.var/app/io.github.sirbilliams.Neutron1/data/game" >&2
    exit 1
fi
if [ ! -f "$CONF" ]; then
    mkdir -p "$XDG_CONFIG_HOME"
    cat > "$CONF" <<'CFG'
# Jimmy Neutron: Boy Genius (desktop) settings. Edit and restart the game.
OMT_FULLSCREEN=1        # 1 = fullscreen, 0 = window (OMT_WINDOW_SIZE=1920x1080 sets its size)
NEG_WIDESCREEN=1        # 1 = fill a widescreen display, 0 = original 4:3
NEG_UI=bills            # bills = Bill's UI, vanilla = the original HUD
NEG_UI_ENERGY=a         # Bill's UI energy meter: a tube, b orbit, c dial, bar = the original bar
NEG_UI_HOLO=a           # gadget hologram: a, b, c or off
NEG_SEAMLESS=0  # 1 = seamless Neighborhood <-> School (beta)
NEG_LOD=2               # draw distance 1..4
CFG
fi
# each NAME=value line, unless NAME is already set (flatpak run --env=... wins)
while IFS= read -r line; do
    line=${line%%#*}
    case "$line" in *=*) ;; *) continue ;; esac
    name=$(printf '%s' "${line%%=*}" | tr -d ' ')
    value=$(printf '%s' "${line#*=}" | sed 's/[[:space:]]*$//')
    eval "[ -n \"\${$name+x}\" ]" || export "$name=$value"
done < "$CONF"
export OMT_DATA="$DATA"
cd "$DATA" || exit 1
exec /app/bin/negatron "$@"
