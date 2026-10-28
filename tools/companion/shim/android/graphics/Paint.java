package android.graphics;
/** Desktop shim (tools/companion). */
public class Paint {
    public static final int ANTI_ALIAS_FLAG = 1, FILTER_BITMAP_FLAG = 2;
    public enum Style { FILL, STROKE }
    public enum Align { LEFT, CENTER, RIGHT }
    public enum Cap { BUTT, ROUND }
    int color = 0xff000000; Style style = Style.FILL; float strokeWidth = 1; Cap cap = Cap.BUTT;
    float textSize = 12; Typeface typeface = Typeface.SANS_SERIF; Align align = Align.LEFT; Shader shader;
    public Paint() {}
    public Paint(int flags) {}
    int alpha = 255;
    public void setColor(int c) { color = c; alpha = c >>> 24; }
    public void setAlpha(int a) { alpha = a; color = (color & 0xffffff) | (a << 24); }
    public void setStyle(Style s) { style = s; }
    public void setStrokeWidth(float w) { strokeWidth = w; }
    public void setStrokeCap(Cap c) { cap = c; }
    public void setTextSize(float s) { textSize = s; }
    public void setTypeface(Typeface t) { typeface = t; }
    public void setTextAlign(Align a) { align = a; }
    public Shader setShader(Shader s) { shader = s; return s; }
    float shadowRadius, shadowDx, shadowDy;
    int shadowColor;
    public void setShadowLayer(float radius, float dx, float dy, int shadowColor) {
        shadowRadius = radius; shadowDx = dx; shadowDy = dy; this.shadowColor = shadowColor;
    }
    public void clearShadowLayer() { setShadowLayer(0, 0, 0, 0); }
    private static final java.awt.font.FontRenderContext METRICS =
            new java.awt.font.FontRenderContext(null, true, true);
    public float measureText(String text) { return (float) font().getStringBounds(text, METRICS).getWidth(); }
    public float ascent() { return -font().getLineMetrics("Hg", METRICS).getAscent(); }
    public float descent() { return font().getLineMetrics("Hg", METRICS).getDescent(); }
    java.awt.Font font() { return new java.awt.Font(typeface.family, typeface.style == 1 ? java.awt.Font.BOLD : java.awt.Font.PLAIN, 1).deriveFont(textSize); }
}
