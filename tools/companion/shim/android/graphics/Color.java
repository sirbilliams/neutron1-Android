package android.graphics;
/** Desktop shim (tools/companion). */
public final class Color {
    public static final int WHITE = 0xffffffff, BLACK = 0xff000000;
    public static int parseColor(String value) {
        if (value.startsWith("#")) {
            long color = Long.parseLong(value.substring(1), 16);
            if (value.length() == 7) color |= 0xff000000L;
            else if (value.length() != 9) throw new IllegalArgumentException("Unknown color");
            return (int) color;
        }
        switch (value.toLowerCase(java.util.Locale.ROOT)) {
            case "black": return BLACK;
            case "darkgray": case "darkgrey": return 0xff444444;
            case "gray": case "grey": return 0xff888888;
            case "lightgray": case "lightgrey": return 0xffcccccc;
            case "white": return WHITE;
            case "red": return 0xffff0000;
            case "green": case "lime": return 0xff00ff00;
            case "blue": return 0xff0000ff;
            case "yellow": return 0xffffff00;
            case "cyan": case "aqua": return 0xff00ffff;
            case "magenta": case "fuchsia": return 0xffff00ff;
            case "maroon": return 0xff800000;
            case "navy": return 0xff000080;
            case "olive": return 0xff808000;
            case "purple": return 0xff800080;
            case "silver": return 0xffc0c0c0;
            case "teal": return 0xff008080;
            case "transparent": return 0;
            default: throw new IllegalArgumentException("Unknown color");
        }
    }
    public static int rgb(int r, int g, int b) { return argb(255, r, g, b); }
    public static int argb(int a, int r, int g, int b) { return (a << 24) | (r << 16) | (g << 8) | b; }
    public static int alpha(int c) { return c >>> 24; }
    public static int red(int c) { return (c >> 16) & 0xff; }
    public static int green(int c) { return (c >> 8) & 0xff; }
    public static int blue(int c) { return c & 0xff; }
}
