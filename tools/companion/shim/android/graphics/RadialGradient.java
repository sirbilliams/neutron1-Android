package android.graphics;
/** Desktop shim (tools/companion). */
public class RadialGradient extends Shader {
    public RadialGradient(float cx, float cy, float radius, int centerColor, int edgeColor, TileMode mode) {
        this(cx, cy, radius, new int[] {centerColor, edgeColor}, null, mode);
    }
    public RadialGradient(float cx, float cy, float radius, int[] colors, float[] positions, TileMode mode) {
        awt = new java.awt.RadialGradientPaint(cx, cy, radius, stops(colors, positions), colors(colors));
    }
}
