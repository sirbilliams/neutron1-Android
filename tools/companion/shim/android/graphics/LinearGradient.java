package android.graphics;
/** Desktop shim (tools/companion). */
public class LinearGradient extends Shader {
    public LinearGradient(float x0, float y0, float x1, float y1, int c0, int c1, TileMode m) {
        this(x0, y0, x1, y1, new int[] {c0, c1}, null, m);
    }
    public LinearGradient(float x0, float y0, float x1, float y1, int[] colors, float[] positions, TileMode m) {
        awt = new java.awt.LinearGradientPaint(x0, y0, x1, y1, stops(colors, positions), colors(colors));
    }
}
