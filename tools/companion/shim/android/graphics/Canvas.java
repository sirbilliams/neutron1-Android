package android.graphics;
import java.awt.BasicStroke;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.geom.Arc2D;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Line2D;
import java.awt.geom.Rectangle2D;
import java.awt.geom.RoundRectangle2D;
import java.util.ArrayDeque;
/** Desktop shim (tools/companion): the calls CompanionView makes, on Java2D. */
public class Canvas {
    private Graphics2D g;
    private final ArrayDeque<Graphics2D> stack = new ArrayDeque<>();
    public Canvas(Bitmap b) {
        g = b.img.createGraphics();
        g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        g.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
        g.setRenderingHint(RenderingHints.KEY_FRACTIONALMETRICS, RenderingHints.VALUE_FRACTIONALMETRICS_ON);
        g.setRenderingHint(RenderingHints.KEY_INTERPOLATION, RenderingHints.VALUE_INTERPOLATION_BILINEAR);
    }
    public int save() { stack.push(g); g = (Graphics2D) g.create(); return stack.size(); }
    public void restore() { g.dispose(); g = stack.pop(); }
    public void translate(float x, float y) { g.translate(x, y); }
    public void scale(float x, float y) { g.scale(x, y); }
    public void rotate(float d) { g.rotate(Math.toRadians(d)); }
    public void rotate(float d, float x, float y) { g.rotate(Math.toRadians(d), x, y); }
    public boolean clipRect(RectF r) { g.clip(new Rectangle2D.Float(r.left, r.top, r.width(), r.height())); return !g.getClip().getBounds2D().isEmpty(); }
    public boolean clipPath(Path path) {
        g.clip(path.p);
        return !g.getClip().getBounds2D().isEmpty();
    }
    private void use(Paint p) {
        g.setComposite(java.awt.AlphaComposite.getInstance(java.awt.AlphaComposite.SRC_OVER,
                p.shader == null ? 1f : p.alpha / 255f));
        g.setPaint(p.shader != null ? p.shader.awt : new java.awt.Color(p.color, true));
        g.setStroke(new BasicStroke(p.strokeWidth, p.cap == Paint.Cap.ROUND ? BasicStroke.CAP_ROUND : BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND));
    }
    private void shadow(java.awt.Shape shape, Paint p) {
        if (p.shadowRadius <= 0 || (p.shadowColor >>> 24) == 0) return;
        java.awt.Shape mask = p.style == Paint.Style.STROKE
                ? new BasicStroke(p.strokeWidth, p.cap == Paint.Cap.ROUND ? BasicStroke.CAP_ROUND : BasicStroke.CAP_BUTT,
                        BasicStroke.JOIN_ROUND).createStrokedShape(shape) : shape;
        // Blur in local coordinates; the canvas transform and clip apply to the resulting shadow.
        float sigma = p.shadowRadius * 0.57735f + 0.5f;
        int spread = (int) Math.ceil(3 * sigma);
        java.awt.geom.Rectangle2D bounds = mask.getBounds2D();
        int x = (int) Math.floor(bounds.getX()) - spread - 1;
        int y = (int) Math.floor(bounds.getY()) - spread - 1;
        int w = (int) Math.ceil(bounds.getMaxX()) - x + spread + 1;
        int h = (int) Math.ceil(bounds.getMaxY()) - y + spread + 1;
        if (w <= 0 || h <= 0) return;
        java.awt.image.BufferedImage image = new java.awt.image.BufferedImage(w,h,java.awt.image.BufferedImage.TYPE_INT_ARGB);
        Graphics2D sg = image.createGraphics();
        sg.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        sg.translate(-x,-y);
        // Android uses an opaque shadow color with the paint alpha, otherwise the shadow color alpha.
        int color = p.shadowColor;
        if ((color >>> 24) == 255) color = (color & 0xffffff) | (p.alpha << 24);
        sg.setColor(new java.awt.Color(color,true)); sg.fill(mask); sg.dispose();
        float[] kernel = new float[2*spread+1];
        float sum = 0;
        for (int i = -spread; i <= spread; i++) {
            kernel[i+spread] = (float) Math.exp(-i*i/(2*sigma*sigma)); sum += kernel[i+spread];
        }
        for (int i = 0; i < kernel.length; i++) kernel[i] /= sum;
        java.awt.image.ConvolveOp horizontal = new java.awt.image.ConvolveOp(
                new java.awt.image.Kernel(kernel.length,1,kernel), java.awt.image.ConvolveOp.EDGE_ZERO_FILL,null);
        java.awt.image.ConvolveOp vertical = new java.awt.image.ConvolveOp(
                new java.awt.image.Kernel(1,kernel.length,kernel), java.awt.image.ConvolveOp.EDGE_ZERO_FILL,null);
        image = vertical.filter(horizontal.filter(image,null),null);
        g.setComposite(java.awt.AlphaComposite.SrcOver);
        g.drawImage(image, java.awt.geom.AffineTransform.getTranslateInstance(x+p.shadowDx,y+p.shadowDy),null);
    }
    private void shape(java.awt.Shape s, Paint p) {
        shadow(s,p); use(p);
        if (p.style == Paint.Style.STROKE) g.draw(s); else g.fill(s);
    }
    public void drawRect(float l, float t, float r, float b, Paint p) { shape(new Rectangle2D.Float(l, t, r - l, b - t), p); }
    public void drawRoundRect(RectF r, float rx, float ry, Paint p) { shape(new RoundRectangle2D.Float(r.left, r.top, r.width(), r.height(), rx * 2, ry * 2), p); }
    public void drawCircle(float x, float y, float r, Paint p) { shape(new Ellipse2D.Float(x - r, y - r, 2 * r, 2 * r), p); }
    public void drawOval(RectF r, Paint p) { shape(new Ellipse2D.Float(r.left, r.top, r.width(), r.height()), p); }
    public void drawArc(RectF r, float start, float sweep, boolean centre, Paint p) {
        shape(new Arc2D.Float(r.left, r.top, r.width(), r.height(), -start, -sweep, centre ? Arc2D.PIE : Arc2D.OPEN), p);
    }
    public void drawLine(float x0, float y0, float x1, float y1, Paint p) { Paint.Style style = p.style;
        p.style = Paint.Style.STROKE;
        shadow(new Line2D.Float(x0,y0,x1,y1), p);
        p.style = style;
        use(p); g.draw(new Line2D.Float(x0, y0, x1, y1)); }
    public void drawPath(Path path, Paint p) { shape(path.p, p); }
    public void drawText(String s, float x, float y, Paint p) {
        use(p);
        g.setFont(p.font());
        float w = p.measureText(s);
        if (p.align == Paint.Align.CENTER) x -= w / 2; else if (p.align == Paint.Align.RIGHT) x -= w;
        shadow(p.font().createGlyphVector(g.getFontRenderContext(),s).getOutline(x,y),p);
        use(p);
        g.drawString(s, x, y);
    }
    public void drawBitmap(Bitmap b, float x, float y, Paint p) {
        java.awt.Composite old = g.getComposite();
        g.setComposite(java.awt.AlphaComposite.getInstance(java.awt.AlphaComposite.SRC_OVER, p == null ? 1f : p.alpha / 255f));
        g.drawImage(b.img, java.awt.geom.AffineTransform.getTranslateInstance(x,y), null);
        g.setComposite(old);
    }
    public void drawBitmap(Bitmap b, Rect src, RectF dst, Paint p) {
        java.awt.geom.AffineTransform t = g.getTransform();
        g.translate(dst.left, dst.top);
        g.scale(dst.width() / b.getWidth(), dst.height() / b.getHeight());
        java.awt.Composite old = g.getComposite();
        g.setComposite(java.awt.AlphaComposite.getInstance(java.awt.AlphaComposite.SRC_OVER, p == null ? 1f : p.alpha / 255f));
        g.drawImage(b.img, 0, 0, null);
        g.setComposite(old);
        g.setTransform(t);
    }
}
