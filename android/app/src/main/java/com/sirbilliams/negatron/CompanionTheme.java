package com.sirbilliams.negatron;

import android.graphics.Color;
import android.graphics.Typeface;
import java.util.Locale;

/** Paint tokens in .theme-mock.html, in 1240 x 1080 layout pixels. */
final class CompanionTheme {
    static final CompanionTheme RETROVILLE = new CompanionTheme(false, new String[] {
        "137c79", "094d52", "fff4d9", "e3d6b8", "234a4b", "637c75", "1fb5ad",
        "ff7a1a", "d94b35", "15383b", "295358", "fff9e9", "6c8583",
        "ffffff", "b0c1bf", "f8ffff", "768c8d", "d6d5bd", "7d8980" });
    static final CompanionTheme YOKIAN = new CompanionTheme(true, new String[] {
        "1a0b2e", "09051b", "322047", "201230", "f4e8ff", "c4a7db", "76ff03",
        "e040fb", "ff698e", "100c24", "30203f", "faffec", "8651ad",
        "9b65c0", "38234e", "79459e", "211332", "221332", "735c8c" });
    final boolean alien;
    final int bg, bg2, panel, panel2, ink, muted, accent, hot, danger, screen, grid,
            white, edge, slot, off, iconWell;
    final int[] metal;
    final float glow, radius, tvRadius, pill, slotRadius;
    final float detailRadius = 2, dialStroke = 9, stroke = 2, heavy = 5;
    final float small = 28, body = 30, title = 34, number = 40;
    final Typeface font;

    private CompanionTheme(boolean alien, String[] hex) {
        this.alien = alien;
        int[] c = new int[hex.length];
        for (int i = 0; i < c.length; i++) c[i] = Color.parseColor("#" + hex[i]);
        bg=c[0]; bg2=c[1]; panel=c[2]; panel2=c[3]; ink=c[4]; muted=c[5];
        accent=c[6]; hot=c[7]; danger=c[8]; screen=c[9]; grid=c[10]; white=c[11];
        edge=c[12]; metal=new int[] {c[13],c[14],c[15],c[16]}; slot=c[17]; off=c[18];
        // Deliberately lighter than the approved slots so dark game icons remain readable.
        iconWell = alien ? Color.rgb(113, 86, 133) : Color.rgb(230, 226, 202);
        glow=alien?12:0; radius=alien?48:24; tvRadius=alien?110:76;
        pill=alien?60:40; slotRadius=alien?28:12;
        // Android has neither of the desktop mockup's proprietary font families.
        font=Typeface.create(alien ? "sans-serif" : "sans-serif-rounded", Typeface.BOLD);
    }

    static CompanionTheme resolve(String choice, String level) {
        if ("retroville".equals(choice)) return RETROVILLE;
        if ("yokian".equals(choice)) return YOKIAN;
        String name = level.replace('\\', '/');
        name = name.substring(name.lastIndexOf('/') + 1).toLowerCase(Locale.ROOT);
        if (name.endsWith(".gam")) name = name.substring(0, name.length() - 4);
        switch (name) {
            case "level1d": case "level5": case "level5a": case "level5b":
            case "level6": case "level6a": case "level7": return YOKIAN;
            default: return RETROVILLE;
        }
    }
}
