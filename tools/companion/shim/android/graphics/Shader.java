package android.graphics;
/** Desktop shim (tools/companion). */
public class Shader {
    public enum TileMode { CLAMP }
    java.awt.Paint awt;
    static float[] stops(int[] colors, float[] positions) {
        if (colors.length < 2) throw new IllegalArgumentException("needs >= 2 colors");
        if (positions != null) {
            if (positions.length != colors.length) throw new IllegalArgumentException("color/position length mismatch");
            return positions.clone();
        }
        float[] result = new float[colors.length];
        for (int i = 0; i < result.length; i++) result[i] = (float) i / (result.length - 1);
        return result;
    }
    static java.awt.Color[] colors(int[] colors) {
        java.awt.Color[] result = new java.awt.Color[colors.length];
        for (int i = 0; i < result.length; i++) result[i] = new java.awt.Color(colors[i], true);
        return result;
    }
}
