package android.graphics;
/** Desktop shim (tools/companion). */
public class RectF {
    public float left, top, right, bottom;
    public RectF() {}
    public RectF(float l, float t, float r, float b) { set(l, t, r, b); }
    public void set(float l, float t, float r, float b) { left = l; top = t; right = r; bottom = b; }
    public void set(RectF src) { set(src.left, src.top, src.right, src.bottom); }
    public static boolean intersects(RectF a, RectF b) {
        return a.left < b.right && b.left < a.right && a.top < b.bottom && b.top < a.bottom;
    }
    public boolean contains(float x, float y) { return x >= left && x < right && y >= top && y < bottom; }
    public float width() { return right - left; }
    public float height() { return bottom - top; }
    public float centerX() { return (left + right) / 2; }
    public float centerY() { return (top + bottom) / 2; }
}
