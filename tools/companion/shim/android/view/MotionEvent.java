package android.view;
/** Single-pointer desktop stand-in so render tools can feed taps and drags to onTouchEvent. */
public class MotionEvent {
    public static final int ACTION_DOWN=0,ACTION_UP=1,ACTION_MOVE=2,ACTION_CANCEL=3,ACTION_POINTER_DOWN=5,ACTION_POINTER_UP=6;
    private final int action; private final float x, y;
    public MotionEvent(int action, float x, float y) { this.action = action; this.x = x; this.y = y; }
    public int getActionMasked(){return action;} public int getActionIndex(){return 0;}
    public int getPointerCount(){return 1;} public float getX(){return x;} public float getY(){return y;}
    public float getX(int i){return x;} public float getY(int i){return y;}
}
