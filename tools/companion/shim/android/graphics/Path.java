package android.graphics;
/** Desktop shim (tools/companion). */
public class Path {
    final java.awt.geom.Path2D.Float p = new java.awt.geom.Path2D.Float();
    public enum Direction { CW, CCW }
    public void cubicTo(float x1, float y1, float x2, float y2, float x3, float y3) {
        if (p.getCurrentPoint() == null) p.moveTo(0, 0);
        p.curveTo(x1, y1, x2, y2, x3, y3);
    }
    public void addRoundRect(RectF r, float rx, float ry, Direction dir) {
        if (r.width() <= 0 || r.height() <= 0) return;
        rx = Math.min(Math.max(0, rx), r.width() / 2);
        ry = Math.min(Math.max(0, ry), r.height() / 2);
        java.awt.geom.Path2D.Float round = new java.awt.geom.Path2D.Float();
        float l = r.left, t = r.top, b = r.bottom, right = r.right, k = 0.55228475f;
        round.moveTo(l + rx, t);
        round.lineTo(right - rx, t);
        round.curveTo(right-rx+k*rx,t,right,t+ry-k*ry,right,t+ry);
        round.lineTo(right,b-ry);
        round.curveTo(right,b-ry+k*ry,right-rx+k*rx,b,right-rx,b);
        round.lineTo(l+rx,b);
        round.curveTo(l+rx-k*rx,b,l,b-ry+k*ry,l,b-ry);
        round.lineTo(l,t+ry);
        round.curveTo(l,t+ry-k*ry,l+rx-k*rx,t,l+rx,t);
        round.closePath();
        // Reflection reverses winding while preserving the rounded rectangle.
        if (dir == Direction.CCW) round.transform(new java.awt.geom.AffineTransform(-1,0,0,1,l+right,0));
        p.append(round, false);
    }
    public void reset() { p.reset(); }
    public void moveTo(float x, float y) { p.moveTo(x, y); }
    public void lineTo(float x, float y) { p.lineTo(x, y); }
    public void close() { p.closePath(); }
}
