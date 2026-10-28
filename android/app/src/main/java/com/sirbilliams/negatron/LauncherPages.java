package com.sirbilliams.negatron;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;

/**
 * port: the launcher's pages (SetupActivity builds them, LauncherView draws them) and the save-slot
 * listing. Plain Java, so tools/companion/render-menu.sh can draw the same pages on the desktop.
 */
final class LauncherPages {
    private LauncherPages() {}

    /** The game takes NEG_START=continue / load:<slot> (game/src/port/StartMode.h). */
    static final boolean GAME_LOADS = true;
    static final int SLOTS = 10;

    /** A saved game (Boy Genius: JimmyGame1..10.tsk in the game folder, written from the PDA's
     *  load / save page; no autosave): its slot, the level it was saved in, the score, the story
     *  message ("objective") number and when. */
    static final class Slot {
        int number;                 // 1..10 (index 0 of slots() is never present)
        String levelName = "", gam = "";
        long score = -1;
        int story = -1;
        long time;                  // ms
        boolean present;

        /** The level as the player knows it: the game's own name, else the .gam file. */
        String level() {
            if (levelName.length() > 0) return levelName;
            return gam.toLowerCase(Locale.ROOT).endsWith(".gam") ? gam.substring(0, gam.length() - 4) : gam;
        }

        String[] detail() {
            String title = "GAME " + number;
            if (!present) return new String[] { title, "EMPTY" };
            ArrayList<String> d = new ArrayList<>();
            d.add(title);
            d.add("LEVEL  " + level().toUpperCase(Locale.ROOT));
            if (score >= 0) d.add("POINTS  " + score);
            if (story > 0) d.add("OBJECTIVE  " + story);
            if (time > 0) d.add("SAVED  " + new SimpleDateFormat("yyyy-MM-dd HH:mm", Locale.ROOT).format(new Date(time)));
            return d.toArray(new String[0]);
        }
    }

    /** Slots 1..10 (index 0 unused), from slots.txt (written by the game: TAB-separated slot, level
     *  id, level name, gam, score, mtime, story; game/src/port/StartMode.h), else from the .tsk
     *  headers. saveDir: the game folder. */
    static Slot[] slots(File saveDir) {
        Slot[] out = new Slot[SLOTS + 1];
        for (int n = 0; n <= SLOTS; n++) {
            out[n] = new Slot();
            out[n].number = n;
        }
        File list = new File(saveDir, "slots.txt");
        if (list.isFile()) {
            try (BufferedReader r = new BufferedReader(new InputStreamReader(new FileInputStream(list), "ISO-8859-1"))) {
                String line;
                while ((line = r.readLine()) != null) {
                    if (line.startsWith("#")) continue;
                    String[] f = line.split("\t", -1);
                    if (f.length < 6) continue;
                    int n = parseInt(f[0], -1);
                    if (n < 1 || n > SLOTS) continue;
                    Slot s = out[n];
                    s.levelName = f[2].trim();
                    s.gam = f[3].trim();
                    s.score = parseInt(f[4], -1);
                    s.time = parseLong(f[5]) * 1000L;
                    if (f.length > 6) s.story = parseInt(f[6], -1);
                    s.present = true;
                }
            } catch (IOException e) {
                // as if empty
            }
            return out;
        }
        for (int n = 1; n <= SLOTS; n++) readTsk(saveDir, out[n]);
        return out;
    }

    /** The newest save (Continue), or null. */
    static Slot newest(Slot[] slots) {
        Slot best = null;
        for (Slot s : slots) if (s.present && (best == null || s.time > best.time)) best = s;
        return best;
    }

    private static long parseLong(String s) {
        try {
            return Long.parseLong(s.trim());
        } catch (NumberFormatException e) {
            return 0;
        }
    }

    private static int parseInt(String s, int def) {
        try {
            return Integer.parseInt(s.trim());
        } catch (NumberFormatException e) {
            return def;
        }
    }

    /** The game has not written slots.txt yet: the .tsk header (big-endian: int level 4cc; float
     *  health; short lives; int score; int story message). */
    private static void readTsk(File saveDir, Slot s) {
        File f = new File(saveDir, fileName(s.number));
        if (!f.isFile()) return;
        try (InputStream in = new FileInputStream(f)) {
            readTsk(in, s);
            if (s.present) s.time = f.lastModified();
        } catch (IOException e) {
            s.present = false;
        }
    }

    static void readTsk(InputStream raw, Slot s) {
        try {
            DataInputStream in = new DataInputStream(raw);
            int id = in.readInt();
            s.gam = levelFile(id);
            if (s.gam == null) return;                  // not a Boy Genius save
            s.levelName = levelName(id);
            in.readFloat();
            in.readShort();
            s.score = in.readInt();
            s.story = in.readInt();
            s.present = true;
        } catch (IOException e) {
            s.present = false;
        }
    }

    /** The level file of a level id (the game's TaskGameLoad 0x4603f0: 'LEV1' level1.gam, 'LV1B'
     *  level1b.gam, 'VR01' VR01.gam), or null. */
    static String levelFile(int id) {
        char a = (char) ((id >> 24) & 0xff), b = (char) ((id >> 16) & 0xff), c = (char) ((id >> 8) & 0xff), d = (char) (id & 0xff);
        if (a == 'V' && b == 'R' && Character.isDigit(c) && Character.isDigit(d)) return "VR" + c + d + ".gam";
        if (a == 'L' && b == 'E' && c == 'V' && Character.isDigit(d)) return "level" + d + ".gam";
        if (a == 'L' && b == 'V' && Character.isDigit(c) && Character.isLetter(d)) return "level" + c + Character.toLowerCase(d) + ".gam";
        return null;
    }

    /** The game's own name of a level (the phone booth keypad), else "" (game/src/port/Companion.cpp). */
    static String levelName(int id) {
        switch (id) {
            case 0x4c563142: return "Lab";              // LV1B
            case 0x4c455631: return "Neighborhood";     // LEV1
            case 0x4c455632: return "School";           // LEV2
            case 0x4c455634: return "Downtown";         // LEV4
            case 0x4c455633: return "Retroland";        // LEV3
            case 0x4c563443: return "Powerplant";       // LV4C
            case 0x4c455635: return "Area 51";          // LEV5
            case 0x4c563541: return "Rocket Maze";      // LV5A
            case 0x4c455636: return "Yokian Ship";      // LEV6
            default: return "";
        }
    }

    /** The save file of slot n (1..10). */
    static String fileName(int n) {
        return "JimmyGame" + n + ".tsk";
    }

    // ------------------------------------------------------------------ export / import

    /** The game folder's files that travel: the saves JimmyGame1..10.tsk (the game's own .tsk
     *  format, as the PC game keeps them in its folder). */
    static boolean exported(String name) {
        return slotOf(name) >= 1;
    }

    /** The slot a save file's name stands for: 1..10 JimmyGame<n>.tsk, -1 not a save file. */
    static int slotOf(String name) {
        String n = name.toLowerCase(Locale.ROOT);
        if (n.matches("jimmygame([1-9]|10)\\.tsk")) return Integer.parseInt(n.substring(9, n.length() - 4));
        return -1;
    }

    /** A save file found in an import: its name in the save folder, its bytes and what it holds. */
    static final class Incoming {
        String name;
        byte[] data;
        Slot info = new Slot();
        boolean replaces;           // a save of that name is already there
    }

    static LauncherModel.Page saves() {
        LauncherModel.Page p = new LauncherModel.Page("saves", "Saves");
        p.pic = "saves";
        p.add("export_all", "Export saves", "Write all the game's saves (games 1-10) to one "
                + ".zip: Downloads, Drive... They are the original game's own files (JimmyGame1..10.tsk), "
                + "usable in the PC game's folder too. One slot: Load Game, then Y.");
        p.add("import", "Import saves", "Bring saves back from a .zip (or one .tsk file). You see what is in it "
                + "and confirm before any save is replaced.");
        p.add("back", "Back", "Back to the main menu.");
        return p;
    }

    /** What an import holds: Cancel first (the default), Import, then one row per file. */
    static LauncherModel.Page importPage(List<Incoming> files) {
        LauncherModel.Page p = new LauncherModel.Page("import", "Import saves");
        p.pic = "saves";
        int replaced = 0;
        StringBuilder which = new StringBuilder();
        for (Incoming f : files) {
            if (!f.replaces) continue;
            replaced++;
            if (which.length() > 0) which.append(", ");
            which.append(f.info.number > 0 ? "game " + f.info.number : f.name);
        }
        p.warning = replaced == 0 ? "Nothing you have is replaced."
                : "Replaces " + replaced + " save" + (replaced == 1 ? "" : "s") + " you have: " + which + ".";
        p.add("back", "Cancel", "Keep your saves as they are.");
        p.add("import_yes", replaced > 0 ? "Import and replace" : "Import",
                "Copy the " + files.size() + " file" + (files.size() == 1 ? "" : "s") + " into the game's folder.");
        for (Incoming f : files) {
            int n = f.info.number;
            LauncherModel.Item i = p.add("file", f.name, f.replaces ? "Replaces the save you have." : "New.");
            i.value = f.replaces ? "REPLACES" : "NEW";
            i.detail = n >= 1 ? f.info.detail() : new String[] { f.name.toUpperCase(Locale.ROOT) };
        }
        p.sel = 0;
        return p;
    }

    /** A single .tsk whose name is no slot's: which slot it goes into. */
    static LauncherModel.Page importTarget(Slot[] have) {
        LauncherModel.Page p = new LauncherModel.Page("import_to", "Import into slot");
        p.pic = "saves";
        for (int n = 1; n <= SLOTS; n++) {
            Slot s = have[n];
            LauncherModel.Item i = p.add("to:" + n, n + "  " + (s.present ? s.level() : "---"),
                    s.present ? "Put the save here (you confirm before this one is replaced)." : "Put the save here.");
            i.detail = s.detail();
        }
        p.add("back", "Cancel", "Keep your saves as they are.");
        return p;
    }

    /** The main page. newest: the newest save (Continue), or null. */
    static LauncherModel.Page main(Slot newest, boolean test) {
        LauncherModel.Page p = new LauncherModel.Page("main", "Main menu");
        boolean canContinue = GAME_LOADS && newest != null;
        LauncherModel.Item i = p.add("continue", "Continue", canContinue
                ? "Carry on from your newest save (game " + newest.number + ")."
                : "No saved game yet: start a new game. (In the game: the PDA's Load/Save page.)");
        i.enabled = canContinue;
        if (canContinue) i.detail = newest.detail();
        p.add("new", "New game", "Start the story from the beginning: the opening, then the neighborhood.");
        i = p.add("load", "Load game", "Pick one of the game's ten saves.");
        i.enabled = GAME_LOADS;
        p.add("saves", "Saves", "Export the game's saves to a file, or import them.");
        p.add("assets", "Asset viewer", "Fly freely through any level, or look at the game's models.");
        p.add("settings", "Settings", "Widescreen, the bottom screen, box art and background.");
        if (test) p.add("test", "Test", "Jump to a level, the model viewer, share the log.");
        p.add("quit", "Quit", "Close the game.");
        for (LauncherModel.Item it : p.items) it.pic = it.id;     // each option its own picture
        return p;
    }

    static LauncherModel.Page newGame() {
        LauncherModel.Page p = new LauncherModel.Page("new", "New game");
        p.pic = "new";
        p.warning = "Your saves are kept: save in the game (PDA, Load/Save) to keep the new one too.";
        p.add("new_yes", "Start a new game", "The opening, then the neighborhood.");
        p.add("back", "Cancel", "Back to the main menu.");
        p.sel = 1;                                  // the safe choice first
        return p;
    }

    static LauncherModel.Page load(Slot[] slots) {
        LauncherModel.Page p = new LauncherModel.Page("load", "Load game");
        p.pic = "load";
        for (int n = 1; n < slots.length; n++) {
            Slot s = slots[n];
            LauncherModel.Item i = p.add("slot:" + n, "Game " + n + "  " + (s.present ? s.level() : "---"),
                    s.present ? "Load this game. Y (or hold): export this slot to a file." : "Empty slot.");
            i.value = s.present && s.score >= 0 ? String.valueOf(s.score) : null;
            i.enabled = s.present && GAME_LOADS;
            i.detail = s.detail();
        }
        p.add("back", "Back", "Back to the main menu.");
        p.footer = "A  LOAD     Y  EXPORT THIS SLOT     B  BACK";
        return p;
    }

    static LauncherModel.Page settings(boolean wide, boolean panel, boolean secondScreen,
            boolean boxArt, boolean background) {
        return settings(wide, true, "a", "a", panel, secondScreen, boxArt, background, "2");
    }

    /** port: Bill's UI's gadget hologram (NEG_UI_HOLO a / b / c / off, game/src/port/BillsUi.h). */
    static final String[] HOLO_IDS = { "a", "b", "c", "off" };
    static final String[] HOLO_NAMES = { "BAY", "CONSOLE", "WHEEL", "OFF" };

    static String holoName(String id) {
        for (int i = 0; i < HOLO_IDS.length; i++) if (HOLO_IDS[i].equals(id)) return HOLO_NAMES[i];
        return HOLO_NAMES[0];
    }

    /** The next (dir 1) or previous (dir -1) gadget hologram after `id`. */
    static String holoStep(String id, int dir) {
        int n = HOLO_IDS.length, i = 0;
        for (int k = 0; k < n; k++) if (HOLO_IDS[k].equals(id)) i = k;
        return HOLO_IDS[((i + dir) % n + n) % n];
    }

    /** The game's number for a hologram id: 0 off, 1 BAY, 2 CONSOLE, 3 WHEEL. */
    static int holoNumber(String id) {
        if ("off".equals(id)) return 0;
        if ("b".equals(id)) return 2;
        if ("c".equals(id)) return 3;
        return 1;
    }

    /** port: the energy meter designs of Bill's UI (NEG_UI_ENERGY a / b / c, game/src/port/BillsUi.h). */
    static final String[] ENERGY_IDS = { "a", "b", "c" };
    static final String[] ENERGY_NAMES = { "TUBE", "ORBIT", "DIAL" };

    static String energyName(String id) {
        for (int i = 0; i < ENERGY_IDS.length; i++) if (ENERGY_IDS[i].equals(id)) return ENERGY_NAMES[i];
        return ENERGY_NAMES[0];
    }

    /** The next (dir 1) or previous (dir -1) energy meter after `id`. */
    /** port: the draw distance setting, "1", "2" or "3" (NEG_LOD). */
    static String lodStep(String id, int dir) {
        int v = "1".equals(id) ? 1 : "3".equals(id) ? 3 : 2;
        return String.valueOf(((v - 1 + dir) % 3 + 3) % 3 + 1);
    }

    static String energyStep(String id, int dir) {
        int n = ENERGY_IDS.length, i = 0;
        for (int k = 0; k < n; k++) if (ENERGY_IDS[k].equals(id)) i = k;
        return ENERGY_IDS[((i + dir) % n + n) % n];
    }

    static LauncherModel.Page settings(boolean wide, boolean billsUi, String energy, String holo, boolean panel,
            boolean secondScreen, boolean boxArt, boolean background, String lod) {
        return settings(wide, billsUi, energy, holo, panel, secondScreen, boxArt, background, lod, "auto");
    }

    static String panelThemeStep(String id, int dir) {
        String[] choices = { "retroville", "yokian", "auto" };
        int at = 2;
        for (int i = 0; i < choices.length; i++) if (choices[i].equals(id)) at = i;
        return choices[(at + dir + choices.length) % choices.length];
    }

    static LauncherModel.Page settings(boolean wide, boolean billsUi, String energy, String holo, boolean panel,
            boolean secondScreen, boolean boxArt, boolean background, String lod, String panelTheme) {
        LauncherModel.Page p = new LauncherModel.Page("settings", "Settings");
        p.pic = "settings";
        LauncherModel.Item i = p.add("wide", "Widescreen 16:9", "Fill a 16:9 screen; the HUD moves to the edges. Off: the original 4:3.");
        i.value = wide ? "ON" : "OFF";
        i = p.add("ui", "In-game UI", "Bill's UI: the port's polish (the pause screen's scope, the score "
                + "counting up, framed help pictures, the sonar readout centred). Vanilla: the original look. "
                + "Also in game: hold Back.");
        i.value = billsUi ? "BILL'S UI" : "VANILLA";
        if (billsUi) {
            i = p.add("energy", "Energy meter", "Bill's UI's energy meter: TUBE (a steel LED tube), ORBIT (an "
                    + "atom with an orbit gauge) or DIAL (the bottom screen's dial). Also in game: hold Back.");
            i.value = energyName(energy);
            i = p.add("holo", "Gadget hologram", "Bill's UI's picture of the current gadget or vehicle: BAY (a "
                    + "framed bay), CONSOLE (a wrist console with your gadgets), WHEEL (a gadget wheel) or OFF "
                    + "(the game's own). Also in game: hold Back.");
            i.value = holoName(holo);
        }
        i = p.add("lod", "Draw distance", "How far the level is drawn before it fades out (trees and flat "
                + "pictures pop in less). 1X is the original game, 2X (default) and 3X reach farther.");
        i.value = lod + "X";
        i = p.add("panel", "Dual screen", (secondScreen ? "" : "No second screen found. ")
                + "The bottom screen shows the level map, the tasks and the gadgets while you play "
                + "(on by default when there is a second screen). Also in game: hold Back.");
        i.value = panel ? "ON" : "OFF";
        i = p.add("panel_theme", "Panel theme", "Retroville or Yokian. Auto follows the current level: "
                + "Yokian for alien levels, Retroville elsewhere.");
        i.value = "retroville".equals(panelTheme) ? "Retroville" : "yokian".equals(panelTheme) ? "Yokian" : "Auto";
        i = p.add("boxart", "Box art: choose image...", "A picture of the game's box (your own photo or "
                + "scan) for the bottom screen while the main menu is up. It is copied into the app."
                + (boxArt ? " Now: your picture." : " Now: a picture from the game."));
        i = p.add("boxart_clear", "Box art: clear", "Remove the chosen box art (the bottom screen then shows "
                + "a picture from the game).");
        i.enabled = boxArt;
        i = p.add("bg", "Menu background: choose image...", "A picture of your own behind this menu "
                + "(16:9 fits best). It is copied into the app."
                + (background ? " Now: your picture." : " Now: brushed steel."));
        i = p.add("bg_clear", "Menu background: clear", "Back to the plain brushed steel.");
        i.enabled = background;
        i = p.add("hd", "HD texture pack: install (.zip)", "Sharper pictures: a zip of upscaled copies of "
                + "the game's own textures (omt/ and png/ folders, each picture 2x, 4x... its size). "
                + "It is unpacked into the game's hd/ folder.");
        i = p.add("hd_remove", "HD texture pack: remove", "Delete the installed HD pack (the game goes back to "
                + "its own pictures).");
        p.add("back", "Back", "Back to the main menu.");
        return p;
    }

    static LauncherModel.Page test() {
        LauncherModel.Page p = new LauncherModel.Page("test", "Test");
        p.pic = "test";
        p.add("levels", "Jump to a level", "Play one level straight away, without its opening (NEG_TEST_PLAY).");
        p.add("viewer", "Level viewer (OMT)", "The engine's own viewer: fly through the level .omt files.");
        p.add("log", "Share log", "Send the last run's log (omt-log.txt) as text.");
        p.add("back", "Back", "Back to the main menu.");
        return p;
    }

    // ------------------------------------------------------------------ port: the Asset Viewer

    static LauncherModel.Page assets() {
        LauncherModel.Page p = new LauncherModel.Page("assets", "Asset viewer");
        p.pic = "assets";
        p.add("explore", "Explore a level", "Fly freely through any level with a noclip camera: "
                + "nothing moves, no story, no enemies, nothing is saved.");
        LauncherModel.Item m = p.add("models", "Model viewer", null);
        m.detail = new String[] { "Models: .ase files, .omt shapes", "Up/down model  L1/R1 jump 10  Sticks turn",
            "L2/R2 zoom  A auto-turn  B leave" };
        p.add("back", "Back", "Back to the main menu.");
        return p;
    }

    /** The explorer's controls (NEG_ASSET_VIEW, game/src/port/AssetView.h). */
    static final String[] EXPLORE_HELP = {
        "L stick move  R stick look  L2/R2 down/up",
        "A speed  Y fog  X Jimmy  Start reset  B leave" };

    /** Explore a level: the installed gam files, named as the bottom-screen panel names them (levels.txt,
     *  written by the game at every start; before that, the file names). */
    static LauncherModel.Page explore(File saveDir, List<String> gams, String select) {
        java.util.HashMap<String, String> names = new java.util.HashMap<>();
        File list = new File(saveDir, "levels.txt");
        if (list.isFile()) {
            try (BufferedReader r = new BufferedReader(new InputStreamReader(new FileInputStream(list), "ISO-8859-1"))) {
                String line;
                while ((line = r.readLine()) != null) {
                    if (line.startsWith("#")) continue;
                    String[] f = line.split("\t", -1);
                    if (f.length >= 2 && f[1].trim().length() > 0) names.put(f[0].toLowerCase(Locale.ROOT), f[1].trim());
                }
            } catch (IOException e) {
                // the file names then
            }
        }
        java.util.HashMap<String, Integer> uses = new java.util.HashMap<>();
        for (String g : gams) {
            String n = names.get(g.toLowerCase(Locale.ROOT));
            if (n != null) uses.put(n, uses.containsKey(n) ? uses.get(n) + 1 : 1);
        }
        LauncherModel.Page p = new LauncherModel.Page("explore", "Explore a level");
        p.pic = "assets";
        for (String g : gams) {
            String base = g.toLowerCase(Locale.ROOT).endsWith(".gam") ? g.substring(0, g.length() - 4) : g;
            String n = names.get(g.toLowerCase(Locale.ROOT));
            String label = n == null ? base : uses.get(n) > 1 ? n + " (" + base + ")" : n;
            LauncherModel.Item i = p.add("explore:" + g, label, null);
            i.detail = new String[] { label + "   " + g, EXPLORE_HELP[0], EXPLORE_HELP[1] };
            if (g.equalsIgnoreCase(select != null ? select : "Level1.gam")) p.sel = p.items.size() - 1;
        }
        p.add("back", "Back", "Back to the Asset viewer.");
        return p;
    }

    static LauncherModel.Page levels(List<String> gams) {
        LauncherModel.Page p = new LauncherModel.Page("levels", "Jump to a level");
        p.pic = "test";
        for (String g : gams) {
            p.add("level:" + g, g, "Play " + g + " from its start.");
            if (g.equalsIgnoreCase("Level1.gam")) p.sel = p.items.size() - 1;
        }
        p.add("back", "Back", "Back to the test page.");
        return p;
    }
}
