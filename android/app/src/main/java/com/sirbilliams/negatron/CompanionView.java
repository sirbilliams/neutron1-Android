package com.sirbilliams.negatron;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.LinearGradient;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.RadialGradient;
import android.graphics.RectF;
import android.graphics.Shader;
import android.os.SystemClock;
import android.view.View;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;
import android.animation.ValueAnimator;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.HashMap;

/** Procedural companion panel, scaled from the Thor's 1240 x 1080 screen.
 * Snapshot parsing and the native map/icon source are shared by both themes.
 * Static theme chrome is cached until the size or theme changes.
 */
final class CompanionView extends View {
    /** Where the map picture and icons come from (the JNI on Android, PNG files on the desktop). */
    interface Source {
        /** [gen, w, h, ARGB...] or null. */
        int[] map();
        /** [w, h, ARGB...] or null. */
        int[] icon(int id);
    }

    private CompanionTheme theme = CompanionTheme.RETROVILLE;
    private String mThemeChoice = "auto", mLevelFile = "";
    private final java.util.ArrayList<RectF> mLabels = new java.util.ArrayList<>();
    private final RectF mLabelBounds = new RectF();

    void setThemeChoice(String choice) {
        mThemeChoice = choice;
        updateTheme();
    }

    private void updateTheme() {
        CompanionTheme next = CompanionTheme.resolve(mThemeChoice, mLevelFile);
        if (next == theme) return;
        theme = next;
        setBackgroundColor(theme.bg);
        if (mChrome != null) { mChrome.recycle(); mChrome = null; }
        if (mMapChrome != null) { mMapChrome.recycle(); mMapChrome = null; }
        // Refresh the raster tint even if the snapshot has not changed.
        mMapGen = -1;
        if (mHave) fetchMapFromLast();
        invalidate();
    }

    private void fetchMapFromLast() {
        try {
            JSONObject map = new JSONObject(mLast).optJSONObject("map");
            fetchMap(map != null ? map.optInt("gen", 0) : 0);
        } catch (Exception ignored) { }
    }

    // map zoom: world units per layout pixel
    private static final float ZOOM_NEAR = 5f, ZOOM_FAR = 12f;

    // layout (Thor pixels)
    private static final float LW = 1240, LH = 1080;
    private static final RectF R_HEAD = new RectF(20, 20, 424, 104);
    private static final RectF R_GAUGE = new RectF(20, 116, 424, 326);
    private static final RectF R_LEVEL = new RectF(20, 338, 424, 570);
    private static final RectF R_SHEET = new RectF(20, 582, 424, 1060);
    private final RectF R_MAP = new RectF(440, 20, 1220, 1060);
    private static final float BEZEL = 22, CHROME_PAD = 16;     // pad: room for the plate's glow
    private static final float R_MAP_W_EXPANDED = 1200;        // R_MAP when the dashboard has slid away

    private final Paint mFill = new Paint(Paint.ANTI_ALIAS_FLAG);
    private final Paint mStroke = new Paint(Paint.ANTI_ALIAS_FLAG);
    private final Paint mText = new Paint(Paint.ANTI_ALIAS_FLAG);
    private final Paint mBmp = new Paint(Paint.ANTI_ALIAS_FLAG | Paint.FILTER_BITMAP_FLAG);
    private final Path mPath = new Path();
    private final RectF mRect = new RectF();
    private final RectF mRect2 = new RectF();

    private Source mSource;
    private Bitmap mMapChrome;
    private int mMapChromeWidth;
    private Bitmap mCase;                   // full brushed case behind the moving panels
    private Bitmap mChrome;                 // the static plates, drawn once per size
    private int mChromeW, mChromeH;
    private Bitmap mMap;                    // the level map in its original colors
    private int mMapGen, mWantedMapGen;
    private final HashMap<Integer, Bitmap> mIcons = new HashMap<>();
    private boolean mFar;
    private boolean mExplore;
    private float mExpansion, mPanX, mPanY, mTouchX, mTouchY, mDownX, mDownY;
    private float mExploreZoom = 1;
    private boolean mMoved;
    private final boolean[] mFilters = {true, true, true, true};
    private final String[] mFilterNames = {"ITEMS", "ENEMIES", "CHECKPOINTS", "EXITS"};
    private final ScaleGestureDetector mPinch;
    private ValueAnimator mSlide;

    private void explore(boolean on) {
        mExplore = on;
        mPanX = mPanY = 0;
        mExploreZoom = 1;
        if (on) fitMap();
        if (mSlide != null) mSlide.cancel();
        mSlide = ValueAnimator.ofFloat(mExpansion, on ? 1 : 0);
        mSlide.setDuration(250);
        mSlide.addUpdateListener(a -> {
            mExpansion = (Float) a.getAnimatedValue();
            invalidate();
        });
        mSlide.start();
    }

    private float fitZoom() {
        if (mMapW <= 0 || mMapH <= 0 || mMapS <= 0) return 1;
        // the expanded window (R_MAP at its widest) less a margin, and the part below the toolbar
        float pixelsPerMapPixel = Math.min((R_MAP_W_EXPANDED - 2 * BEZEL - 48) / mMapW, (R_MAP.height() - 172 - TOOL_H - 40) / mMapH);
        return Math.max(0.0001f, Math.min(4f, pixelsPerMapPixel * (mFar ? ZOOM_FAR : ZOOM_NEAR) / mMapS));
    }

    private void fitMap() {
        mExploreZoom = fitZoom();
        float k = mMapS / (mFar ? ZOOM_FAR : ZOOM_NEAR) * mExploreZoom;
        mPanX = (mJx - mMapW / 2f) * k;
        // Leave room above the map for the exploration toolbar.
        mPanY = (mJy - mMapH / 2f) * k + TOOL_H / 2;
        invalidate();
    }

    private float layoutScale() { return Math.min(getWidth() / LW, getHeight() / LH); }
    private float layoutX(float x) { float u = layoutScale(); return (x - (getWidth() - LW * u) / 2) / u; }
    private float layoutY(float y) { float u = layoutScale(); return (y - (getHeight() - LH * u) / 2) / u; }

    @Override public boolean onTouchEvent(MotionEvent e) {
        if (layoutScale() <= 0) return false;
        mPinch.onTouchEvent(e);
        float x = layoutX(e.getX()), y = layoutY(e.getY());
        switch (e.getActionMasked()) {
            case MotionEvent.ACTION_DOWN:
                mDownX = mTouchX = x; mDownY = mTouchY = y; mMoved = false;
                return true;
            case MotionEvent.ACTION_POINTER_DOWN:
                mMoved = true;
                return true;
            case MotionEvent.ACTION_MOVE:
                if (Math.hypot(x - mDownX, y - mDownY) > 12) mMoved = true;
                if (mExplore && mPickName.length == 0 && e.getPointerCount() == 1
                        && !mPinch.isInProgress() && mDownY > mapRect().top + TOOL_H) {
                    mPanX += x - mTouchX; mPanY += y - mTouchY;
                    invalidate();
                }
                mTouchX = x; mTouchY = y;
                return true;
            case MotionEvent.ACTION_POINTER_UP:
                mMoved = true;
                int remaining = e.getActionIndex() == 0 ? 1 : 0;
                mTouchX = layoutX(e.getX(remaining)); mTouchY = layoutY(e.getY(remaining));
                return true;
            case MotionEvent.ACTION_UP:
                if (!mMoved && mPickName.length == 0) {
                    if (hit(B_TOGGLE, x, y)) explore(!mExplore);
                    else if (mExplore && hit(B_RECENTER, x, y)) {
                        mPanX = mPanY = 0; mExploreZoom = 1; invalidate();
                    } else if (mExplore && hit(B_FIT, x, y)) {
                        fitMap();
                    } else if (mExplore) {
                        for (int i = 0; i < mFilters.length; i++)
                            if (hit(B_FILTER + i, x, y)) { mFilters[i] = !mFilters[i]; invalidate(); }
                    }
                    performClick();
                }
                return true;
            case MotionEvent.ACTION_CANCEL: mMoved = true; return true;
            default: return true;
        }
    }

    @Override protected void onDetachedFromWindow() {
        if (mSlide != null) mSlide.cancel();
        super.onDetachedFromWindow();
    }

    @Override public boolean performClick() { super.performClick(); return true; }

    private boolean showMarker(char kind) {
        if (!mExplore) return true;
        if (kind == 'p' || kind == 'n' || kind == 'r') return mFilters[0];
        if (kind == 'e') return mFilters[1];
        if (kind == 'c') return mFilters[2];
        if (kind == 'd' || kind == 'x' || kind == 'X') return mFilters[3];
        return true;
    }

    // Toolbar geometry, in layout px relative to the map window. Collapsed: only the toggle, at the
    // window's top right; expanded: toggle, RECENTER, FIT LEVEL on one row, the four filters under it.
    private static final int B_TOGGLE = 0, B_RECENTER = 1, B_FIT = 2, B_FILTER = 3;
    private static final float TOOL_H = 150;         // toolbar depth below the window's top edge
    private final RectF mBtn = new RectF();

    private RectF btn(int id) {
        RectF crt = mapRect();
        float x0 = crt.left + 12;
        if (id == B_TOGGLE) {
            float x = x0 + (crt.right - 12 - 280 - x0) * (1 - mExpansion);
            mBtn.set(x, crt.top + 14, x + 280, crt.top + 74);
        } else if (id == B_RECENTER) mBtn.set(x0 + 300, crt.top + 14, x0 + 560, crt.top + 74);
        else if (id == B_FIT) mBtn.set(x0 + 580, crt.top + 14, x0 + 840, crt.top + 74);
        else mBtn.set(x0 + (id - B_FILTER) * 285, crt.top + 84, x0 + (id - B_FILTER) * 285 + 275, crt.top + 138);
        return mBtn;
    }

    /** North marker: top centre on the dashboard, top right (clear of both toolbar rows) when expanded. */
    private float compassX() {
        RectF crt = mapRect();
        return crt.centerX() + (crt.right - 70 - crt.centerX()) * mExpansion;
    }

    private boolean hit(int id, float x, float y) { return btn(id).contains(x, y); }

    private void toolButton(Canvas c, int id, String label, int colour) {
        RectF r = btn(id);
        float x = r.left, y = r.top, w = r.width(), h = r.height();
        screenBox(c, x, y, w, h);
        mText.setTypeface(theme.font); mText.setTextSize(theme.small); mText.setColor(colour);
        mText.setTextAlign(Paint.Align.CENTER);
        c.drawText(label, x + w / 2, y + h / 2 + theme.small * 0.35f, mText);
        mText.setTextAlign(Paint.Align.LEFT);
    }

    private void mapControls(Canvas c) {
        if (mPickName.length > 0) return;
        toolButton(c, B_TOGGLE, mExplore ? "< DASHBOARD" : "EXPAND MAP >", theme.accent);
        if (!mExplore) return;
        toolButton(c, B_RECENTER, "RECENTER", theme.accent);
        toolButton(c, B_FIT, "FIT LEVEL", theme.accent);
        for (int i = 0; i < mFilters.length; i++) {
            RectF r = btn(B_FILTER + i);
            float x = r.left, y = r.top, h = r.height();
            screenBox(c, x, y, r.width(), h);
            led(c, x + 32, y + h / 2, 9, mFilters[i] ? 2 : 0, theme.accent);
            // a touch under the usual label size so CHECKPOINTS fits its pill
            mText.setTypeface(theme.font); mText.setTextSize(theme.small - 4); mText.setColor(mFilters[i] ? theme.white : theme.muted);
            c.drawText(mFilterNames[i], x + 52, y + h / 2 + (theme.small - 4) * 0.35f, mText);
        }
    }

    private String mLast;

    // --- the parsed snapshot (setSnapshot), read by onDraw
    private boolean mHave;
    private String mLevel = "", mMode = "", mScore = "0", mEnergyText = "0", mTime = "";
    private boolean mCut;
    private float mEnergy = -1;
    private boolean mBoss;
    private float mBossHp;
    private String mBossName = "";
    // the level's collectables: neutrons, checkpoints, then the pickups by kind (icon, got, all)
    private int[] mColIcon = new int[0], mColGot = new int[0], mColAll = new int[0];
    private String[] mColText = new String[0];
    private int mNeutronIcon = -1, mEnergyIcon = -1;
    private int mGadgetIcon = -1;
    private float mGadgetCharge = -1;
    // the inventory sheet: icon, count, owned, grid column, grid row
    private int[] mShIcon = new int[0], mShCount = new int[0], mShCol = new int[0], mShRow = new int[0];
    private boolean[] mShOwned = new boolean[0];
    private String[] mShText = new String[0];
    private int mShCols = 1, mShRows = 1, mShOwnedCount;
    private String mShSummary = "";
    private final HashMap<Integer, Bitmap> mIconsDim = new HashMap<>();
    private String[] mCaption = new String[0];
    private int mMapW, mMapH;
    private float mMapS = 1;
    private boolean mJimmy, mGoddard;
    private float mJx, mJy, mHx, mHy;
    private char[] mPtKind = new char[0];
    private float[] mPtX = new float[0], mPtY = new float[0];
    private int[] mPtIcon = new int[0];
    private String[] mPtLabel = new String[0];
    // the Vox world picker (port/VoxPicker.h): cards left to right, the lit one
    private String[] mPickName = new String[0];
    private int[] mPickWire = new int[0], mPickColour = new int[0];
    private int mPickSel = -1;

    CompanionView(Context c) {
        super(c);
        setBackgroundColor(theme.bg);
        setLayerType(View.LAYER_TYPE_SOFTWARE, null);
        mStroke.setStyle(Paint.Style.STROKE);
        setContentDescription("Companion map. Expand map for touch pan, zoom and marker filters.");
        mPinch = new ScaleGestureDetector(c, new ScaleGestureDetector.SimpleOnScaleGestureListener() {
            @Override public boolean onScale(ScaleGestureDetector detector) {
                if (!mExplore || mPickName.length > 0) return false;
                mMoved = true;
                float old = mExploreZoom;
                mExploreZoom = Math.max(Math.min(0.35f, fitZoom()), Math.min(4f, old * detector.getScaleFactor()));
                float ratio = mExploreZoom / old;
                mPanX *= ratio; mPanY *= ratio;
                invalidate();
                return true;
            }
        });
    }

    void setSource(Source s) { mSource = s; }

    void setZoomFar(boolean far) {
        mFar = far;
        invalidate();
    }

    /** Called with every poll (10 a second): the sweep moves even when nothing else does. */
    void tick() { invalidate(); }

    /** New snapshot (JSON) from the poll; parsed only when it changed. */
    void setSnapshot(String json) {
        if (json == null || json.isEmpty()) return;
        if (json.equals(mLast)) {
            if (mWantedMapGen != mMapGen) fetchMap(mWantedMapGen);
            return;
        }
        try {
            parse(new JSONObject(json));
            mLast = json;
        } catch (Exception e) {
            return;
        }
        invalidate();
    }

    // ------------------------------------------------------------------ parsing

    private void parse(JSONObject d) {
        mHave = true;
        String level = d.optString("level", "");
        if (!level.equals(mLevelFile)) { mPanX = mPanY = 0; mExploreZoom = 1; }   // a new level: back to the default view
        mLevelFile = level;
        updateTheme();
        int dot = level.lastIndexOf('.');
        // the game's own level name (its Load/Save page's), else the file name
        String name = d.optString("name", "");
        mLevel = !name.isEmpty() ? name : (dot > 0 ? level.substring(0, dot) : level).toUpperCase();
        int mode = d.optInt("mode", -1);
        mMode = modeName(mode);
        mCut = d.optInt("cut", 0) == 1;
        mEnergy = (float) (d.optDouble("energy", -1) / d.optDouble("energy_max", 100));
        mEnergyText = String.valueOf(Math.round(d.optDouble("energy", 0)));
        mEnergyIcon = d.optInt("energy_icon", -1);
        mScore = String.valueOf(d.optInt("score", 0));
        int secs = (int) d.optDouble("time", 0);
        mTime = (secs / 60 < 10 ? "0" : "") + (secs / 60) + ":" + (secs % 60 < 10 ? "0" : "") + (secs % 60);

        JSONObject boss = d.optJSONObject("boss");
        mBoss = boss != null && (boss.optInt("bar", 0) == 1 || boss.optDouble("hp", 1) < 1);
        if (boss != null) {
            mBossHp = (float) boss.optDouble("hp", 0);
            mBossName = boss.optString("name", "Boss").toUpperCase();
        }

        // collectables of this level: neutrons, checkpoints, then each kind of pickup
        JSONArray ne = d.optJSONArray("neutrons"), ck = d.optJSONArray("checkpoints"), col = d.optJSONArray("collect");
        mNeutronIcon = ne != null ? ne.optInt(2, -1) : -1;
        int n = 0;
        if (ne != null && ne.optInt(1, 0) > 0) n++;
        if (ck != null && ck.optInt(1, 0) > 0) n++;
        n += col != null ? col.length() : 0;
        mColIcon = new int[n];
        mColGot = new int[n];
        mColAll = new int[n];
        mColText = new String[n];
        int k = 0;
        if (ne != null && ne.optInt(1, 0) > 0) {
            mColIcon[k] = mEnergyIcon >= 0 ? mEnergyIcon : mNeutronIcon;
            mColGot[k] = ne.optInt(0, 0);
            mColAll[k++] = ne.optInt(1, 0);
        }
        if (ck != null && ck.optInt(1, 0) > 0) {
            mColIcon[k] = ck.optInt(2, -1) >= 0 ? ck.optInt(2, -1) : -2;      // -2: a drawn flag
            mColGot[k] = ck.optInt(0, 0);
            mColAll[k++] = ck.optInt(1, 0);
        }
        for (int i = 0; col != null && i < col.length(); i++) {
            JSONArray e = col.optJSONArray(i);
            if (e == null) continue;
            mColIcon[k] = e.optInt(3, -1);
            mColGot[k] = e.optInt(1, 0);
            mColAll[k++] = e.optInt(2, 0);
        }
        for (int i = 0; i < n; i++) {
            mColText[i] = mColGot[i] + "/" + mColAll[i];
            if (mColIcon[i] >= 0) fetchIcon(mColIcon[i]);
        }

        // the current gadget and its HUD bar
        mGadgetIcon = d.optInt("gadget_icon", -1);
        mGadgetCharge = (float) d.optDouble("gadget_charge", -1);
        if (mGadgetIcon >= 0) fetchIcon(mGadgetIcon);

        // the inventory sheet, laid out on the game's own grid (its 640 x 480 page positions)
        JSONArray sh = d.optJSONArray("sheet");
        n = sh != null ? sh.length() : 0;
        if (mShIcon.length != n) {
            mShIcon = new int[n];
            mShCount = new int[n];
            mShCol = new int[n];
            mShRow = new int[n];
            mShOwned = new boolean[n];
            mShText = new String[n];
        }
        // the game's own order on its page (rows of 40 px, left to right), in a compact grid
        long[] order = new long[n];
        mShOwnedCount = 0;
        for (int i = 0; i < n; i++) {
            JSONArray e = sh.optJSONArray(i);
            if (e == null) continue;
            mShIcon[i] = e.optInt(2, -1);
            mShCount[i] = e.optInt(1, 0);
            mShOwned[i] = e.optInt(3, 0) != 0;
            if (mShOwned[i]) mShOwnedCount++;
            mShText[i] = mShCount[i] > 1 ? String.valueOf(mShCount[i]) : null;
            order[i] = ((long) (e.optInt(5, 0) / 40) << 32) | ((long) e.optInt(4, 0) << 8) | i;
            if (mShIcon[i] >= 0) fetchIcon(mShIcon[i]);
        }
        java.util.Arrays.sort(order);
        mShCols = 6;
        mShRows = Math.max(1, (n + mShCols - 1) / mShCols);
        for (int k2 = 0; k2 < n; k2++) {
            int i = (int) (order[k2] & 0xff);
            mShCol[i] = k2 % mShCols;
            mShRow[i] = k2 / mShCols;
        }
        mShSummary = mShOwnedCount + "/" + n;

        String cap = d.optString("caption", "");
        mCaption = cap.isEmpty() ? new String[0] : cap.split("\n");

        JSONObject map = d.optJSONObject("map");
        int previousGen = mWantedMapGen;
        int gen = map != null ? map.optInt("gen", 0) : 0;
        mMapW = map != null ? map.optInt("w", 0) : 0;
        mMapH = map != null ? map.optInt("h", 0) : 0;
        mMapS = map != null ? (float) map.optDouble("s", 1) : 1;
        mWantedMapGen = gen;
        if (gen != mMapGen) fetchMap(gen);

        float previousX = mJx, previousY = mJy;
        boolean hadJimmy = mJimmy;
        JSONArray j = d.optJSONArray("jimmy");
        mJimmy = j != null && j.length() >= 4;
        if (mJimmy) {
            mJx = (float) j.optDouble(0, 0);
            mJy = (float) j.optDouble(1, 0);
            mHx = (float) j.optDouble(2, 0);
            mHy = (float) j.optDouble(3, -1);
            if (mExplore && hadJimmy && previousGen == gen) {
                float pixelsPerMapUnit = mMapS / (mFar ? ZOOM_FAR : ZOOM_NEAR) * mExploreZoom;
                mPanX += (mJx - previousX) * pixelsPerMapUnit;
                mPanY += (mJy - previousY) * pixelsPerMapUnit;
            } else if (previousGen != gen) {
                mPanX = mPanY = 0;
            }
        }

        JSONArray p = d.optJSONArray("pts");
        n = p != null ? p.length() : 0;
        mPtKind = new char[n];
        mPtX = new float[n];
        mPtY = new float[n];
        mPtIcon = new int[n];
        mPtLabel = new String[n];
        for (int i = 0; i < n; i++) {
            JSONArray e = p.optJSONArray(i);
            if (e == null) continue;
            String kind = e.optString(0, "?");
            mPtKind[i] = kind.isEmpty() ? '?' : kind.charAt(0);
            mPtX[i] = (float) e.optDouble(1, 0);
            mPtY[i] = (float) e.optDouble(2, 0);
            Object extra = e.opt(3);
            mPtIcon[i] = extra instanceof Number ? ((Number) extra).intValue() : -1;
            if (extra instanceof String) {
                String s = (String) extra;
                int dt = s.lastIndexOf('.');
                mPtLabel[i] = dt > 0 ? s.substring(0, dt).toUpperCase() : s;   // a level name as it is, a file name in caps
            }
            if (mPtIcon[i] >= 0) fetchIcon(mPtIcon[i]);
        }
        mGoddard = false;
        for (int i = 0; i < n; i++) if (mPtKind[i] == 'g') mGoddard = true;
        JSONObject pk = d.optJSONObject("picker");
        JSONArray cards = pk != null ? pk.optJSONArray("cards") : null;
        n = cards != null ? cards.length() : 0;
        if (mPickName.length != n) {
            mPickName = new String[n];
            mPickWire = new int[n];
            mPickColour = new int[n];
        }
        mPickSel = pk != null ? pk.optInt("sel", -1) : -1;
        for (int i = 0; i < n; i++) {
            JSONArray e = cards.optJSONArray(i);
            mPickName[i] = e != null ? e.optString(0, "") : "";
            mPickWire[i] = e != null ? e.optInt(1, -1) : -1;
            mPickColour[i] = e != null ? e.optInt(2, -1) : -1;
            if (mPickWire[i] >= 0) fetchIcon(mPickWire[i]);
            if (mPickColour[i] >= 0) fetchIcon(mPickColour[i]);
        }
        if (mNeutronIcon >= 0) fetchIcon(mNeutronIcon);
        if (mEnergyIcon >= 0) fetchIcon(mEnergyIcon);
    }

    private void fetchMap(int gen) {
        if (mSource == null) return;
        int[] a = gen > 0 ? mSource.map() : null;
        if (a == null || a.length < 3 || a[0] != gen) {
            if (gen == 0) { mMap = null; mMapGen = 0; }
            return;                                         // (asked again with the next snapshot)
        }
        int w = a[1], h = a[2];
        if (w <= 0 || h <= 0 || a.length < 3 + w * h) return;
        int[] px = new int[w * h];
        // The approved themes preserve map colors; the screen supplies the backdrop.
        System.arraycopy(a, 3, px, 0, px.length);
        if (mMap != null) mMap.recycle();
        mMap = Bitmap.createBitmap(px, w, h, Bitmap.Config.ARGB_8888);
        mMapGen = gen;
    }

    private void fetchIcon(int id) {
        if (mSource == null || mIcons.containsKey(id)) return;
        int[] a = mSource.icon(id);
        if (a == null || a.length < 2 || a.length < 2 + a[0] * a[1]) return;
        int[] px = new int[a[0] * a[1]];
        System.arraycopy(a, 2, px, 0, px.length);
        mIcons.put(id, Bitmap.createBitmap(px, a[0], a[1], Bitmap.Config.ARGB_8888));
        // and a dark silhouette for the sheet's items Jimmy doesn't have
        for (int i = 0; i < px.length; i++) {
            int c = px[i], al = c >>> 24;
            int l = (((c >> 16) & 0xff) * 30 + ((c >> 8) & 0xff) * 59 + (c & 0xff) * 11) / 100;
            px[i] = (al * 34 / 100 << 24) | (l << 16) | (l << 8) | l;
        }
        mIconsDim.put(id, Bitmap.createBitmap(px, a[0], a[1], Bitmap.Config.ARGB_8888));
    }

    // ------------------------------------------------------------------ drawing

    @Override
    protected void onDraw(Canvas c) {
        int w = getWidth(), h = getHeight();
        if (w <= 0 || h <= 0) return;
        float u = Math.min(w / LW, h / LH);
        float ox = (w - LW * u) / 2, oy = (h - LH * u) / 2;
        R_MAP.left = 440;
        if (mChrome == null || mChromeW != w || mChromeH != h) buildChrome(w, h, u, ox, oy);
        c.drawBitmap(mCase, 0, 0, null);
        c.save();
        c.clipRect(ox, oy, ox + (440 - 440 * mExpansion) * u, oy + LH * u);
        c.drawBitmap(mChrome, -440 * mExpansion * u, 0, null);
        c.restore();
        c.save();
        c.translate(ox, oy);
        c.scale(u, u);
        c.save();
        c.clipRect(0, 0, 440 - 440 * mExpansion, LH);
        c.translate(-440 * mExpansion, 0);
        drawBrand(c);
        if (!mHave) {
            lcdText(c, "WAITING FOR THE GAME", 500, 550, theme.small, theme.accent);
        } else {
            drawGauges(c);
            drawLevel(c);
            drawSheet(c);
        }
        c.restore();
        R_MAP.left = 440 - 420 * mExpansion;
        // Cache at the current width; redraw only while resizing, then reuse without stretching.
        int mapWidth = Math.round(R_MAP.width());
        if (mMapChrome == null || mMapChromeWidth != mapWidth) {
            if (mMapChrome != null) mMapChrome.recycle();
            mMapChromeWidth = mapWidth;
            mMapChrome = Bitmap.createBitmap(mapWidth + 2 * (int) CHROME_PAD, 1040 + 2 * (int) CHROME_PAD, Bitmap.Config.ARGB_8888);
            Canvas chrome = new Canvas(mMapChrome);
            chrome.translate(CHROME_PAD - R_MAP.left, CHROME_PAD - R_MAP.top);
            plate(chrome, R_MAP);
            drawMapRecess(chrome);
        }
        mRect.set(R_MAP.left - CHROME_PAD, R_MAP.top - CHROME_PAD, R_MAP.right + CHROME_PAD, R_MAP.bottom + CHROME_PAD);
        c.drawBitmap(mMapChrome, null, mRect, mBmp);
        drawMapTitle(c);
        if (mPickName.length > 0) drawPicker(c);
        else drawMap(c);
        mapControls(c);
        c.restore();
    }

    /** The brand plate on the dashboard side. */
    private void drawBrand(Canvas c) {
        lcdText(c, "JIMMY NEUTRON", 102, 56, theme.body, theme.ink);
        lcdText(c, theme.alien ? "YOKIAN INTERFACE" : "RETROVILLE LABS", 102, 88, theme.small, theme.muted);
    }

    /** The map plate's title row and footer; they follow the plate as it slides over the dashboard. */
    private void drawMapTitle(Canvas c) {
        float l = R_MAP.left, r = R_MAP.right;
        fittedText(c, mLevel, l + 36, 69, R_MAP.width() - 220, theme.title, theme.ink);
        mText.setTextAlign(Paint.Align.RIGHT);
        mText.setTypeface(theme.font); mText.setTextSize(theme.body); mText.setColor(theme.muted);
        c.drawText(mTime, r - 39, 69, mText); mText.setTextAlign(Paint.Align.LEFT);
        led(c, l + 46, 1005, 7, 2, mCut ? theme.hot : theme.accent);
        lcdText(c, mCut ? "CUT SCENE" : mMode, l + 66, 1016, theme.small, theme.ink);
        mText.setTextAlign(Paint.Align.RIGHT); mText.setColor(theme.muted);
        c.drawText(theme.alien ? "YOKIAN TELEMETRY" : "ATOMIC FIELD MONITOR", r - 39, 1050, mText);
        mText.setTextAlign(Paint.Align.LEFT);
    }

    private void drawGauges(Canvas c) {
        float cx = 128, cy = 211;
        dial(c, cx, cy, 78, mEnergy);
        mText.setTypeface(theme.font); mText.setTextSize(theme.number);
        mText.setColor(theme.alien ? theme.accent : theme.ink); mText.setTextAlign(Paint.Align.CENTER);
        c.drawText(mEnergyText,cx,cy+51,mText); mText.setTextAlign(Paint.Align.LEFT);
        engraved(c, "ENERGY", cx, 309, theme.small, true);
        Bitmap hud = mIcons.get(mEnergyIcon);
        if (theme.alien && hud != null) drawIcon(c, hud, cx, cy - 20, 39);
        engraved(c, "SCORE", 238, 152, theme.small, false);
        String score = mScore;
        while (score.length() < 6) score = "0" + score;
        fittedText(c, score, 238, 193, 166, theme.number, theme.ink);
        fittedText(c, mBoss ? mBossName : "GADGET", 238, 231, 166, theme.small, theme.muted);
        if (mBoss) {
            lcd(c,238,253,160,34);
            mFill.setColor(theme.danger);
            mRect.set(238,253,238+160*Math.max(0,Math.min(1,mBossHp)),287);
            c.drawRoundRect(mRect,theme.pill,theme.pill,mFill);
            return;
        }
        button(c,266,270,30,2);
        Bitmap gadget = mIcons.get(mGadgetIcon);
        if (gadget != null) drawIcon(c,gadget,266,270,49);
        else lcdText(c,"—",254,282,theme.body,theme.muted);
        if (mGadgetCharge >= 0) {
            lcdText(c,Math.round(mGadgetCharge)+"%",312,269,theme.small,theme.accent);
            lcd(c,312,285,88,10);
            mFill.setColor(mGadgetCharge > 0 ? theme.accent : theme.danger);
            mRect.set(312,285,312+88*Math.max(0.02f,Math.min(1,mGadgetCharge/100)),295);
            c.drawRoundRect(mRect,theme.pill,theme.pill,mFill);
        } else lcdText(c,"NONE",313,280,theme.small,theme.muted);
    }

    /** This level's collectables: the game's picture, collected / all, lit when all are in. */
    private void drawLevel(Canvas c) {
        engraved(c, "THIS LEVEL", R_LEVEL.left + 26, R_LEVEL.top + 37, 18, false);
        float x0 = R_LEVEL.left + 22, y0 = R_LEVEL.top + 51;
        float tw = (R_LEVEL.width() - 44) / 3, th = 49;
        int max = 9;
        if (mColIcon.length == 0) lcdText(c, "NOTHING TO COLLECT", x0 + 6, y0 + 36, 20, theme.muted);
        for (int i = 0; i < mColIcon.length && i < max; i++) {
            float tx = x0 + (i % 3) * tw, ty = y0 + (i / 3) * (th + 7);
            boolean all = mColAll[i] > 0 && mColGot[i] >= mColAll[i];
            lcd(c, tx, ty, tw - 8, th);
            float ix = tx + 19, iy = ty + th / 2;
            Bitmap b = mColIcon[i] >= 0 ? mIcons.get(mColIcon[i]) : null;
            if (b != null) drawIcon(c, b, ix - 2, iy, 33);
            else if (mColIcon[i] == -2) flag(c, ix, iy, 14, theme.accent);
            else star(c, ix, iy, 16, theme.hot);
            // Counts must remain complete, even when the theme font is wider than the mock's.
            // Reserve padding inside the chip and fit the full fraction, never an ellipsis.
            mText.setTypeface(theme.font);
            mText.setTextAlign(Paint.Align.LEFT);
            mText.setTextSize(theme.small);
            float countWidth = tw - 49;
            float measured = mText.measureText(mColText[i]);
            if (measured > countWidth) mText.setTextSize(theme.small * countWidth / measured);
            mText.setColor(all ? theme.hot : theme.ink);
            c.drawText(mColText[i], tx + 35, ty + 34, mText);
            if (all) led(c, tx + tw - 18, ty + 10, 4, 2, theme.accent);
        }
    }

    /** The inventory sheet (the pickups page Jimmy opens): the game's own pictures on its own
     *  grid; lit when Jimmy has the item (with how many), a dark silhouette when not. */
    private void drawSheet(Canvas c) {
        engraved(c, "INVENTORY", R_SHEET.left + 26, R_SHEET.top + 42, 18, false);
        engraved(c, mShSummary, R_SHEET.right - 30, R_SHEET.top + 42, 18, false, true);
        float x0 = R_SHEET.left + 20, y0 = R_SHEET.top + 62;
        float cw = (R_SHEET.width() - 40) / mShCols, ch = (R_SHEET.bottom - 58 - y0) / mShRows;
        float size = Math.min(cw, ch) - 6;
        for (int i = 0; i < mShIcon.length; i++) {
            float cx = x0 + (mShCol[i] + 0.5f) * cw, cy = y0 + (mShRow[i] + 0.5f) * ch;
            mRect.set(cx - cw / 2 + 2, cy - ch / 2 + 2, cx + cw / 2 - 2, cy + ch / 2 - 2);
            mFill.setShader(null);
            mFill.setColor(theme.iconWell);
            c.drawRoundRect(mRect, theme.slotRadius, theme.slotRadius, mFill);
            if (mShOwned[i]) {
                mStroke.setShader(null);
                mStroke.setStrokeWidth(theme.stroke);
                mStroke.setColor(theme.accent);
                c.drawRoundRect(mRect, theme.slotRadius, theme.slotRadius, mStroke);
            }
            Bitmap b = mShIcon[i] >= 0 ? (mShOwned[i] ? mIcons.get(mShIcon[i]) : mIconsDim.get(mShIcon[i])) : null;
            if (b != null) drawIcon(c, b, cx, cy, size);
            if (mShOwned[i] && mShText[i] != null) {
                mFill.setColor(alpha(theme.screen, 200));
                c.drawCircle(cx + cw / 2 - 17, cy + ch / 2 - 17, 18, mFill);
                mText.setTypeface(theme.font);
                mText.setTextAlign(Paint.Align.CENTER);
                mText.setTextSize(theme.small);
                mText.setColor(theme.accent);
                c.drawText(mShText[i], cx + cw / 2 - 17, cy + ch / 2 - 7, mText);
                mText.setTextAlign(Paint.Align.LEFT);
            }
        }
        led(c,49,1031,5,mGoddard?2:0,theme.accent);
        lcdText(c,"GODDARD",65,1041,theme.small,theme.muted);
        lcdText(c,mGoddard?"ONLINE":"—",280,1041,theme.small,theme.muted);
    }

    // --- the map window

    private void drawMap(Canvas c) {
        RectF crt = mRect2;
        crt.set(mapRect());
        float cx = crt.centerX(), cy = crt.centerY();
        c.save();
        mPath.reset();
        mPath.addRoundRect(crt, theme.tvRadius - 5, theme.tvRadius - 5, Path.Direction.CW);
        c.clipPath(mPath);
        mStroke.setColor(theme.grid); mStroke.setStrokeWidth(theme.stroke);
        for (float x=crt.left; x<crt.right; x+=64) c.drawLine(x,crt.top,x,crt.bottom,mStroke);
        for (float y=crt.top; y<crt.bottom; y+=64) c.drawLine(crt.left,y,crt.right,y,mStroke);
        mLabels.clear();
        mLabelBounds.set(crt.left+24, crt.top+90, crt.right-24, crt.bottom-70);
        float jx = cx + (mExplore ? mPanX : 0), jy = cy + (mExplore ? mPanY : 0);
        mLabels.add(new RectF(crt.left, crt.top, crt.left+280, crt.top+90));
        // the toolbar rows (expanded) or the EXPAND button, and the compass: no map label under them
        if (mExplore && mPickName.length == 0) mLabels.add(new RectF(crt.left, crt.top, crt.right, crt.top + TOOL_H));
        else if (mPickName.length == 0) mLabels.add(new RectF(btn(B_TOGGLE)));
        mLabels.add(new RectF(compassX() - 30, crt.top + 14, compassX() + 30, crt.top + 62));
        if (mCaption.length > 0) mLabels.add(new RectF(crt.left, crt.bottom-190, crt.right, crt.bottom));
        if (mMap == null || !mJimmy) {
            lcdText(c, mHave ? "NO SIGNAL" : "", cx - 95, cy, 30, theme.muted);
        } else {
            float k = mMapS / (mFar ? ZOOM_FAR : ZOOM_NEAR) * (mExplore ? mExploreZoom : 1);     // layout px per map px
            float left = cx - mJx * k + (mExplore ? mPanX : 0), top = cy - mJy * k + (mExplore ? mPanY : 0);
            if (mMapW == mMap.getWidth() && mMapH == mMap.getHeight()) {
                mRect.set(left, top, left + mMapW * k, top + mMapH * k);
                mBmp.setAlpha(theme.alien ? 196 : 222);
                c.drawBitmap(mMap, null, mRect, mBmp);
                mBmp.setAlpha(255);
            }
            float iconScale = mFar ? 0.7f : 1f;
            reserveJimmy(jx, jy);
            // exits leading to the same place, close together on screen, become one marker at their centre
            int np = mPtKind.length;
            boolean[] merged = new boolean[np];
            float[] mx = new float[np], my = new float[np];
            for (int i = 0; i < np; i++) { mx[i] = left + mPtX[i] * k; my[i] = top + mPtY[i] * k; }
            for (int i = 0; i < np; i++) {
                if ((mPtKind[i] != 'x' && mPtKind[i] != 'X') || merged[i] || mPtLabel[i] == null || !showMarker(mPtKind[i])) continue;
                float sx = mx[i], sy = my[i];
                int cnt = 1;
                for (int j = i + 1; j < np; j++) {
                    if (merged[j] || mPtKind[j] != mPtKind[i] || !mPtLabel[i].equals(mPtLabel[j])) continue;
                    if (Math.hypot(mx[j] - mx[i], my[j] - my[i]) > 90) continue;
                    merged[j] = true; sx += mx[j]; sy += my[j]; cnt++;
                }
                mx[i] = sx / cnt; my[i] = sy / cnt;
            }
            // pickup, neutron and checkpoint pictures are drawn over the exit labels: keep labels off them
            for (int i = 0; i < np; i++) {
                char kd = mPtKind[i];
                if (kd != 'p' && kd != 'n' && kd != 'c' && kd != 'r' || !showMarker(kd)) continue;
                mLabels.add(new RectF(mx[i] - 30, my[i] - 30, mx[i] + 30, my[i] + 30));
            }
            // points, then Jimmy on top
            for (int priority = 0; priority < 3; priority++) {
            for (int i = 0; i < mPtKind.length; i++) {
                int rank = mPtKind[i] == 'x' ? 0 : mPtKind[i] == 'o' ? 1 : 2;
                if (rank != priority) continue;
                float px = mx[i], py = my[i];
                char kind = mPtKind[i];
                if (merged[i] || !showMarker(kind)) continue;
                float margin = (kind == 'o' || kind == 'x' || kind == 'X') ? -12 : 40;   // marked on the edge when out
                boolean in = px > crt.left - margin && px < crt.right + margin && py > crt.top - margin && py < crt.bottom + margin;
                if (!in) {
                    if (kind == 'o' || kind == 'x' || kind == 'X') edgeMarker(c, crt, cx, cy, px, py, kind, mPtLabel[i]);
                    continue;
                }
                drawPoint(c, kind, px, py, mPtIcon[i], mPtLabel[i], iconScale);
            }
            }
            jimmy(c, cx + (mExplore ? mPanX : 0), cy + (mExplore ? mPanY : 0));
        }
        // scanlines and the sweep
        mStroke.setShader(null);
        mStroke.setStrokeWidth(theme.stroke);
        mStroke.setColor(alpha(theme.screen, 28));
        for (float y = crt.top; y < crt.bottom; y += 4) c.drawLine(crt.left, y, crt.right, y, mStroke);
        float a = (SystemClock.uptimeMillis() % 6000L) / 6000f * 360f;
        float rr = (float) Math.hypot(crt.width(), crt.height()) / 2;
        mRect.set(cx - rr, cy - rr, cx + rr, cy + rr);
        mFill.setShader(null);
        for (int i = 0; i < 10; i++) {                                 // fading trail
            mFill.setColor(alpha(theme.accent, 22 - i * 2));
            c.drawArc(mRect, a - 90 - (i + 1) * 4, 4, true, mFill);
        }
        mStroke.setColor(alpha(theme.accent, 150));
        mStroke.setStrokeWidth(theme.stroke);
        double ar = Math.toRadians(a - 90);
        c.drawLine(cx, cy, cx + (float) Math.cos(ar) * rr, cy + (float) Math.sin(ar) * rr, mStroke);
        c.restore();

        mStroke.setColor(theme.accent); mStroke.setStrokeWidth(theme.stroke);
        for (int i=0; i<16; i++) {
            float yy=crt.top+120+i*40;
            c.drawLine(crt.left+12,yy,crt.left+(i%4==0?29:20),yy,mStroke);
        }
        // compass, zoom and scale bar on the glass
        float nx = compassX();
        lcdText(c, "N", nx - 9, crt.top + 51, 30, theme.accent);
        mFill.setColor(theme.accent);
        mPath.reset();
        mPath.moveTo(nx, crt.top + 19);
        mPath.lineTo(nx - 8, crt.top + 27);
        mPath.lineTo(nx + 8, crt.top + 27);
        mPath.close();
        c.drawPath(mPath, mFill);
        lcdText(c, mFar ? "ZOOM FAR" : "ZOOM NEAR", crt.left + 46, crt.bottom - 31, 28, theme.accent);
        float bar = 1000f / (mFar ? ZOOM_FAR : ZOOM_NEAR) * (mExplore ? mExploreZoom : 1);             // 1000 world units
        mStroke.setColor(theme.accent);
        mStroke.setStrokeWidth(theme.stroke);
        float bx = crt.right - 24 - bar, by = crt.bottom - 36;
        c.drawLine(bx, by, bx + bar, by, mStroke);
        c.drawLine(bx, by - 8, bx, by + 8, mStroke);
        c.drawLine(bx + bar, by - 8, bx + bar, by + 8, mStroke);
        lcdText(c, "1000", Math.min(bx + bar / 2 - 24, crt.right - 100), by - 12, 18, theme.accent);

        // the nearest open exit: an arrow towards it and the level it leads to (top left)
        int ex = -1;
        float best = Float.MAX_VALUE;
        for (int i = 0; mJimmy && i < mPtKind.length; i++) {
            if (mPtKind[i] != 'x') continue;
            float dx = mPtX[i] - mJx, dy = mPtY[i] - mJy, d2 = dx * dx + dy * dy;
            if (d2 < best) { best = d2; ex = i; }
        }
        if (ex >= 0) {
            float lx = crt.left + 16, ly = crt.top + 27 + 123 * mExpansion;   // below the toolbar when expanded
            screenBox(c, lx, ly, 270, 76);
            float ax = lx + 28, ay = ly + 26;
            float dx = mPtX[ex] - mJx, dy = mPtY[ex] - mJy, len = (float) Math.sqrt(best);
            if (len > 1e-3f) { dx /= len; dy /= len; } else { dx = 0; dy = -1; }
            mFill.setShader(null);
            mFill.setColor(theme.accent);
            mPath.reset();
            mPath.moveTo(ax + dx * 18, ay + dy * 18);
            mPath.lineTo(ax - dx * 12 - dy * 12, ay - dy * 12 + dx * 12);
            mPath.lineTo(ax - dx * 5, ay - dy * 5);
            mPath.lineTo(ax - dx * 12 + dy * 12, ay - dy * 12 - dx * 12);
            mPath.close();
            c.drawPath(mPath, mFill);
            lcdText(c, "EXIT", lx + 56, ly + 27, 28, theme.muted);
            fittedText(c, mPtLabel[ex] != null ? mPtLabel[ex] : "", lx + 56, ly + 60, 190, 28, theme.accent);
        }

        // the caption being typed (the game's own text), along the bottom of the glass
        if (mCaption.length > 0) {
            int lines = Math.min(3, mCaption.length);
            float bh = 16 + lines * 34;
            float lx = crt.left + 16, ly = crt.bottom - 52 - bh;
            screenBox(c, lx, ly, crt.width() - 32, bh);
            for (int i = 0; i < lines; i++)
                fittedText(c, mCaption[mCaption.length - lines + i], lx + 12, ly + 32 + i * 34, crt.width() - 56, theme.small, theme.white);
        }
    }

    /** (Negatron only: the native picker is disabled in Boy Genius, so this never shows.)
     *  The Vox world picker, in the map's window: one steel card per world, left to right as on
     *  the game's screen, the chosen one raised and lit (its colour picture, LED on). */
    private void drawPicker(Canvas c) {
        RectF crt = mRect2;
        crt.set(mapRect());
        mFill.setShader(null);
        mFill.setColor(theme.screen);
        c.save(); mPath.reset();
        mPath.addRoundRect(crt,theme.tvRadius-5,theme.tvRadius-5,Path.Direction.CW); c.clipPath(mPath);
        c.drawRect(crt.left, crt.top, crt.right, crt.bottom, mFill);
        lcdText(c, "VOX", crt.left + 30, crt.top + 70, 44, theme.accent);
        lcdText(c, "CHOOSE A WORLD", crt.left + 150, crt.top + 70, 34, theme.muted);
        int n = mPickName.length;
        float gap = 18, cw = (crt.width() - 40 - gap * (n - 1)) / n, ch = Math.min(560, cw * 2.4f);
        float top = crt.centerY() - ch / 2 + 10;
        for (int i = 0; i < n; i++) {
            boolean lit = i == mPickSel;
            float l = crt.left + 20 + i * (cw + gap), t = top - (lit ? 10 : 0);
            // glow, body, edges
            if (lit) {
                mFill.setColor(alpha(theme.accent, 60));
                mRect.set(l - 12, t - 12, l + cw + 12, t + ch + 12);
                c.drawRoundRect(mRect, theme.radius, theme.radius, mFill);
            }
            mFill.setColor(alpha(theme.screen, 110));
            mRect.set(l + 4, t + 7, l + cw + 4, t + ch + 7);
            c.drawRoundRect(mRect, theme.radius, theme.radius, mFill);
            mFill.setColor(lit ? theme.panel : theme.panel2);
            mRect.set(l, t, l + cw, t + ch);
            c.drawRoundRect(mRect, theme.radius, theme.radius, mFill);
            mStroke.setShader(null);
            mStroke.setStrokeWidth(lit ? theme.heavy : theme.stroke);
            mStroke.setColor(lit ? theme.accent : theme.edge);
            mStroke.setStyle(Paint.Style.STROKE);
            c.drawRoundRect(mRect, theme.radius, theme.radius, mStroke);
            // the picture in a CRT inset
            float ps = cw - 36, px = l + 18, py = t + 22;
            mFill.setColor(theme.white);
            mRect.set(px - 4, py - 4, px + ps + 4, py + ps + 4);
            c.drawRoundRect(mRect, theme.slotRadius, theme.slotRadius, mFill);
            mFill.setColor(theme.screen);
            c.drawRect(px, py, px + ps, py + ps, mFill);
            Bitmap b = mIcons.get(lit ? mPickColour[i] : mPickWire[i]);
            if (b == null) b = mIcons.get(mPickWire[i]);
            if (b != null) drawIcon(c, b, px + ps / 2, py + ps / 2, ps);
            if (!lit) {
                mFill.setColor(alpha(theme.accent, 90));
                c.drawRect(px, py, px + ps, py + ps, mFill);
            }
            // the name, engraved, sized to the card
            String name = mPickName[i];
            float size = Math.min(34, (cw - 24) / (0.66f * Math.max(4, name.length())));
            fittedText(c, name, l + 12, py + ps + 26 + Math.max(theme.small, size), cw - 24, size, theme.ink);
            // the LED and the button hint
            float ly = t + ch - 46;
            led(c, l + cw / 2, ly, 14, lit ? 2 : 1, theme.accent);
            if (lit) engraved(c, "A  GO", l + cw / 2, ly + 40, 20, true);
        }
        lcdText(c, "< >  CHOOSE    A  GO    B  BACK", crt.left + 30, crt.bottom - 34, 28, theme.muted);
        c.restore();
    }

    private void drawPoint(Canvas c, char kind, float px, float py, int iconId, String label, float sc) {
        Bitmap icon = iconId >= 0 ? mIcons.get(iconId) : null;
        int glowColor = kind == 'e' || kind == 'X' || kind == 'r' ? theme.danger : kind == 'o' ? theme.hot : theme.accent;
        mFill.setShadowLayer(theme.glow, 0, 0, glowColor);
        mStroke.setShadowLayer(theme.glow, 0, 0, glowColor);
        switch (kind) {
            case 'p':
                if (icon != null) {
                    mFill.setShader(null);
                    mFill.setColor(alpha(theme.screen, 170));
                    c.drawCircle(px, py, 26 * sc, mFill);
                    drawIcon(c, icon, px, py, 46 * sc);
                } else star(c, px, py, 12 * sc, theme.hot);
                break;
            case 'n':
                if (icon != null) {
                    mFill.setShader(null);
                    mFill.setColor(alpha(theme.screen, 150));
                    c.drawCircle(px, py, 15 * sc, mFill);
                    drawIcon(c, icon, px, py, 38 * sc);
                }
                else atom(c, px, py, 10 * sc, theme.accent);
                break;
            case 'c':
                if (icon != null) {
                    mFill.setShader(null);
                    mFill.setColor(alpha(theme.screen, 150));
                    c.drawCircle(px, py, 20 * sc, mFill);
                    drawIcon(c, icon, px, py, 40 * sc);
                }
                else flag(c, px, py, 12 * sc, theme.accent);
                break;
            case 'o': {
                float pulse = (SystemClock.uptimeMillis() % 1200L) / 1200f;
                mStroke.setShader(null);
                mStroke.setStrokeWidth(theme.stroke);
                mStroke.setColor(alpha(theme.hot, (int)(255 * (1 - pulse))));
                c.drawCircle(px, py, 14 + 26 * pulse, mStroke);
                star(c, px, py, 14, theme.hot);
                break;
            }
            case 'x':
            case 'X': {
                boolean open = kind == 'x';
                int col = open ? theme.accent : theme.danger;
                mStroke.setShader(null);
                mStroke.setStrokeWidth(theme.stroke);
                mStroke.setColor(col);
                mRect.set(px - 12, py - 18, px + 12, py + 18);
                c.drawRoundRect(mRect, theme.slotRadius, theme.slotRadius, mStroke);
                mFill.setShader(null);
                mFill.setColor(col);
                c.drawCircle(px + 5, py + 2, 3, mFill);
                if (label != null) mapLabel(c, label, px + 20, py + 8, col);
                break;
            }
            case 'd':
                mStroke.setShader(null);
                mStroke.setStrokeWidth(theme.stroke);
                mStroke.setColor(theme.white);
                c.drawLine(px - 10, py, px + 10, py, mStroke);
                break;
            case 'f':
                atom(c, px, py, 11 * sc, theme.white);
                break;
            case 'r':                                           // a red neutron (its menu picture)
                if (icon != null) {
                    mFill.setShader(null);
                    mFill.setColor(alpha(theme.screen, 150));
                    c.drawCircle(px, py, 18 * sc, mFill);
                    drawIcon(c, icon, px, py, 38 * sc);
                } else atom(c, px, py, 11 * sc, theme.danger);
                break;
            case 'g': {                                         // Goddard: a bone-white paw ring
                mStroke.setShader(null);
                mStroke.setStrokeWidth(theme.stroke);
                mStroke.setColor(theme.white);
                c.drawCircle(px, py, 11, mStroke);
                mFill.setShader(null);
                mFill.setColor(theme.white);
                c.drawCircle(px, py, 4, mFill);
                break;
            }
            case 'e':
                mFill.setShader(null);
                mFill.setColor(theme.danger);
                mPath.reset();
                mPath.moveTo(px, py - 10);
                mPath.lineTo(px + 9, py);
                mPath.lineTo(px, py + 10);
                mPath.lineTo(px - 9, py);
                mPath.close();
                c.drawPath(mPath, mFill);
                break;
            default:
                break;
        }
        mFill.clearShadowLayer();
        mStroke.clearShadowLayer();
    }

    /** An objective or exit off the screen: a chevron on the CRT's edge pointing at it. */
    private void edgeMarker(Canvas c, RectF crt, float cx, float cy, float px, float py, char kind, String label) {
        float dx = px - cx, dy = py - cy;
        float hw = crt.width() / 2 - 30, hh = crt.height() / 2 - 30;
        float t = Math.min(Math.abs(dx) > 0 ? hw / Math.abs(dx) : 1e9f, Math.abs(dy) > 0 ? hh / Math.abs(dy) : 1e9f);
        float ex = cx + dx * t, ey = cy + dy * t;
        float len = (float) Math.hypot(dx, dy);
        float ux = dx / len, uy = dy / len;
        int col = kind == 'o' ? theme.hot : kind == 'x' ? theme.accent : theme.danger;
        mFill.setShader(null);
        mFill.setColor(col);
        mPath.reset();
        mPath.moveTo(ex + ux * 16, ey + uy * 16);
        mPath.lineTo(ex - ux * 10 - uy * 12, ey - uy * 10 + ux * 12);
        mPath.lineTo(ex - ux * 10 + uy * 12, ey - uy * 10 - ux * 12);
        mPath.close();
        c.drawPath(mPath, mFill);
        if (label != null) {
            mapLabel(c, label, ex - ux * 24, ey - uy * 24 + 7, col);
        }
    }

    private void fittedText(Canvas c, String text, float x, float y, float width, float size, int color) {
        mText.setTypeface(theme.font); mText.setTextSize(Math.max(theme.small, size));
        String out = text;
        float fittedSize = Math.max(theme.small, size);
        if (mText.measureText(out) > width) {
            fittedSize = Math.max(theme.small, fittedSize * width / mText.measureText(out));
            mText.setTextSize(fittedSize);
        }
        if (mText.measureText(out) > width) {
            while (out.length() > 0 && mText.measureText(out + "…") > width)
                out = out.substring(0, out.length()-1);
            out += "…";
        }
        lcdText(c, out, x, y, fittedSize, color);
    }

    /** Labels keep clear of Jimmy's marker (it is drawn last, over them). */
    private void reserveJimmy(float x, float y) { mLabels.add(new RectF(x - 48, y - 48, x + 48, y + 48)); }

    private void mapLabel(Canvas c, String label, float x, float y, int color) {
        mText.setTypeface(theme.font); mText.setTextSize(theme.small);
        float width = mText.measureText(label);
        if (width > mLabelBounds.width()) return;
        x = Math.max(mLabelBounds.left, Math.min(mLabelBounds.right-width, x));
        for (int attempt=0; attempt<7; attempt++) {
            float yy = y + (attempt == 0 ? 0 : ((attempt+1)/2)*34*(attempt%2==1?1:-1));
            RectF box = new RectF(x-4, yy + mText.ascent()-4, x+width+4, yy+mText.descent()+4);
            if (box.top < mLabelBounds.top || box.bottom > mLabelBounds.bottom) continue;
            boolean hit = false;
            for (RectF used : mLabels) if (RectF.intersects(used, box)) { hit=true; break; }
            if (hit) continue;
            mLabels.add(box); lcdText(c,label,x,yy,theme.small,color); return;
        }
    }

    /** Jimmy: a white arrow along his heading over a small electron-orbit ring. */
    private void jimmy(Canvas c, float x, float y) {
        mFill.setShader(null);
        mFill.setColor(alpha(theme.accent, 80));
        c.drawCircle(x, y, 36, mFill);
        mStroke.setShader(null);
        mStroke.setStrokeWidth(theme.stroke);
        mStroke.setColor(alpha(theme.accent, 200));
        mRect.set(x - 36, y - 12, x + 36, y + 12);
        c.save();
        for (int i = 0; i < 3; i++) {
            c.drawOval(mRect, mStroke);
            c.rotate(60, x, y);
        }
        c.restore();
        float s = 30;
        mPath.reset();
        mPath.moveTo(x + mHx * s, y + mHy * s);
        mPath.lineTo(x - mHx * s * 0.7f - mHy * s * 0.6f, y - mHy * s * 0.7f + mHx * s * 0.6f);
        mPath.lineTo(x - mHx * s * 0.35f, y - mHy * s * 0.35f);
        mPath.lineTo(x - mHx * s * 0.7f + mHy * s * 0.6f, y - mHy * s * 0.7f - mHx * s * 0.6f);
        mPath.close();
        mStroke.setColor(theme.white);
        mStroke.setStrokeWidth(theme.stroke);
        c.drawPath(mPath, mStroke);
        mFill.setColor(theme.white);
        c.drawPath(mPath, mFill);
    }

    // ------------------------------------------------------------------ the static chrome

    private void buildChrome(int w, int h, float u, float ox, float oy) {
        if (mChrome != null) mChrome.recycle();
        mChrome = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
        mChromeW = w; mChromeH = h;
        // the case: the themed backdrop, full screen, under both the dashboard and the sliding map
        if (mCase != null) mCase.recycle();
        mCase = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
        Canvas c = new Canvas(mCase);
        mFill.setAlpha(255);
        mFill.setShader(new LinearGradient(0, 0, w, h, theme.bg, theme.bg2, Shader.TileMode.CLAMP));
        c.drawRect(0, 0, w, h, mFill);
        mFill.setShader(null);
        c.save(); c.translate(ox, oy); c.scale(u, u);
        for (int i = 0; i < 9; i++) {
            if (theme.alien) {
                mPath.reset(); mPath.moveTo(0, 80 + i * 125);
                mPath.cubicTo(500, -90 + i * 125, 600, 300 + i * 125, 1240, 60 + i * 125);
                mStroke.setColor(alpha(theme.hot, 38)); mStroke.setStrokeWidth(theme.stroke);
                c.drawPath(mPath, mStroke);
            } else star(c, 430, 90 + i * 120, 20, alpha(theme.white, 38));
        }
        c.restore();
        // the dashboard plates: transparent, drawn over the case and slid away with the dashboard
        c = new Canvas(mChrome);
        c.save(); c.translate(ox, oy); c.scale(u, u);
        for (RectF r : new RectF[] {R_HEAD, R_GAUGE, R_LEVEL, R_SHEET}) plate(c, r);
        atom(c, 61, 61, 27, theme.accent);
        star(c, 391, 363, 17, theme.hot);
        c.restore();
    }

    /** The map window's recess (bezel and screen), drawn into the map plate's cached bitmap. */
    private void drawMapRecess(Canvas c) {
        RectF crt = mapRect();
        mRect.set(crt.left - 5, crt.top - 5, crt.right + 5, crt.bottom + 5);
        mFill.setAlpha(255);
        mFill.setShader(new LinearGradient(crt.left, crt.top, crt.right, crt.bottom, theme.metal, null, Shader.TileMode.CLAMP));
        c.drawRoundRect(mRect, theme.tvRadius, theme.tvRadius, mFill);
        mFill.setShader(new RadialGradient(crt.centerX(), crt.centerY(), crt.height(),
                theme.screen, theme.bg2, Shader.TileMode.CLAMP));
        c.drawRoundRect(crt, theme.tvRadius - 5, theme.tvRadius - 5, mFill);
        mFill.setShader(null);
        mStroke.setColor(theme.accent); mStroke.setStrokeWidth(theme.stroke);
        c.drawRoundRect(crt, theme.tvRadius - 5, theme.tvRadius - 5, mStroke);
    }

    private RectF mapRect() {
        return new RectF(R_MAP.left + BEZEL, R_MAP.top + 71, R_MAP.right - BEZEL, R_MAP.bottom - 101);
    }

    private void plate(Canvas c, RectF r) {
        // Shader colors retain their own alpha; Paint alpha additionally modulates them.
        // Decorative stars and radar trails share this paint and leave it translucent.
        mFill.setAlpha(255);
        mFill.setShadowLayer(theme.alien ? 10 : 8, 0, 0, theme.alien ? theme.hot : theme.bg2);
        mFill.setShader(new LinearGradient(r.left, r.top, r.left, r.bottom, theme.metal, null, Shader.TileMode.CLAMP));
        c.drawRoundRect(r, theme.radius, theme.radius, mFill);
        mFill.clearShadowLayer();
        mRect.set(r.left + 5, r.top + 5, r.right - 5, r.bottom - 5);
        mFill.setShader(new LinearGradient(r.left, r.top, r.right, r.bottom, theme.panel, theme.panel2, Shader.TileMode.CLAMP));
        c.drawRoundRect(mRect, theme.radius - 3, theme.radius - 3, mFill);
        mFill.setShader(null);
        mStroke.setColor(theme.alien ? theme.edge : theme.white);
        mStroke.setStrokeWidth(theme.stroke);
        c.drawRoundRect(mRect, theme.radius - 3, theme.radius - 3, mStroke);
        if (theme.alien) {
            mStroke.setColor(theme.hot);
            c.drawLine(r.left + 32, r.bottom - 12, r.right - 64, r.bottom - 12, mStroke);
            mFill.setColor(theme.accent); c.drawCircle(r.right - 40, r.bottom - 12, 4, mFill);
        }
    }

    private void dial(Canvas c, float cx, float cy, float r, float f) {
        f = Math.max(0, Math.min(1, f));
        mStroke.setShader(null);
        if (theme.alien) {
            mRect.set(cx - 69, cy - 79, cx + 69, cy + 79);
            mStroke.setStrokeWidth(theme.dialStroke);
            for (int i = 0; i < 20; i++) {
                mStroke.setColor(i < 20 * f ? theme.accent : theme.edge);
                mStroke.setShadowLayer(i < 20 * f ? theme.glow : 0, 0, 0, theme.accent);
                c.drawArc(mRect, 150 + i * 12, 8, false, mStroke);
            }
            mStroke.clearShadowLayer();
            return;
        }
        mFill.setAlpha(255);
        mFill.setShader(new LinearGradient(cx, cy-r, cx, cy+r, theme.metal, null, Shader.TileMode.CLAMP));
        c.drawCircle(cx, cy, r, mFill); mFill.setShader(null);
        mFill.setColor(theme.panel); c.drawCircle(cx, cy, r-8, mFill);
        for (int i = 0; i <= 20; i++) {
            double a = Math.toRadians(150 + i * 12);
            float nx = (float)Math.cos(a), ny = (float)Math.sin(a);
            mStroke.setStrokeWidth(i % 5 == 0 ? theme.heavy : theme.stroke);
            mStroke.setColor(i < 6 ? theme.danger : i < 14 ? theme.hot : theme.accent);
            c.drawLine(cx + nx*57, cy + ny*57, cx + nx*(i%5==0?68:63), cy + ny*(i%5==0?68:63), mStroke);
        }
        double a = Math.toRadians(150 + 240*f);
        float nx=(float)Math.cos(a), ny=(float)Math.sin(a);
        mStroke.setColor(theme.hot); mStroke.setStrokeWidth(theme.heavy);
        c.drawLine(cx-nx*8, cy-ny*8, cx+nx*51, cy+ny*51, mStroke);
        mFill.setColor(theme.ink); c.drawCircle(cx, cy, 8, mFill);
    }

    private int energyColour(float e) { return e >= 70 ? theme.accent : e >= 30 ? theme.hot : theme.danger; }

    private void lcd(Canvas c, float x, float y, float w, float h) {
        mFill.setShader(null); mFill.setColor(theme.slot);
        mRect.set(x,y,x+w,y+h);
        c.drawRoundRect(mRect, theme.slotRadius, theme.slotRadius, mFill);
    }

    private void screenBox(Canvas c, float x, float y, float w, float h) {
        mFill.setShader(null); mFill.setColor(theme.screen);
        mRect.set(x,y,x+w,y+h);
        c.drawRoundRect(mRect,theme.pill,theme.pill,mFill);
        mStroke.setColor(theme.edge); mStroke.setStrokeWidth(theme.stroke);
        c.drawRoundRect(mRect,theme.pill,theme.pill,mStroke);
    }

    private void led(Canvas c, float x, float y, float r, int state, int colour) {
        mFill.setShader(null); mFill.setColor(state == 2 ? colour : state == 1 ? theme.muted : theme.off);
        mFill.setShadowLayer(state == 2 ? theme.glow : 0, 0, 0, colour);
        c.drawCircle(x,y,r,mFill); mFill.clearShadowLayer();
    }

    private void button(Canvas c, float x, float y, float r, int state) {
        mFill.setShader(null); mFill.setColor(theme.iconWell);
        mRect.set(x-r,y-r,x+r,y+r);
        c.drawRoundRect(mRect, theme.pill, theme.pill, mFill);
    }

    private static int alpha(int color, int opacity) { return (color & 0x00ffffff) | (opacity << 24); }

    private void drawIcon(Canvas c, Bitmap b, float x, float y, float size) {
        float sc = size / Math.max(b.getWidth(), b.getHeight());
        float w = b.getWidth() * sc, h = b.getHeight() * sc;
        mRect.set(x - w / 2, y - h / 2, x + w / 2, y + h / 2);
        c.drawBitmap(b, null, mRect, mBmp);
    }

    /** Text uses the theme font and never falls below its minimum size. */
    private void lcdText(Canvas c, String s, float x, float y, float size, int colour) {
        mText.setTypeface(theme.font);
        mText.setTextAlign(Paint.Align.LEFT);
        mText.setTextSize(Math.max(theme.small, size));
        mText.setColor(colour);
        c.drawText(s, x, y, mText);
    }

    /** A label on the panel surface. */
    private void engraved(Canvas c, String s, float x, float y, float size, boolean centre) {
        engraved(c, s, x, y, size, centre, false);
    }

    private void engraved(Canvas c, String s, float x, float y, float size, boolean centre, boolean right) {
        mText.setTypeface(theme.font);
        mText.setTextAlign(right ? Paint.Align.RIGHT : centre ? Paint.Align.CENTER : Paint.Align.LEFT);
        mText.setTextSize(Math.max(theme.small, size));
        mText.setColor(theme.ink);
        c.drawText(s, x, y, mText);
        mText.setTextAlign(Paint.Align.LEFT);
    }

    /** Electron orbits: three ellipses round a nucleus. */
    private void atom(Canvas c, float x, float y, float r, int colour) {
        mStroke.setShader(null);
        mStroke.setStrokeWidth(theme.stroke);
        mStroke.setColor(colour);
        mStroke.setShadowLayer(theme.glow, 0, 0, colour);
        mRect.set(x - r, y - r * 0.38f, x + r, y + r * 0.38f);
        c.save();
        for (int i = 0; i < 3; i++) {
            c.drawOval(mRect, mStroke);
            c.rotate(60, x, y);
        }
        c.restore();
        mFill.setShader(null);
        mFill.setColor(colour);
        c.drawCircle(x, y, r * 0.16f, mFill);
        mStroke.clearShadowLayer();
    }

    /** A four-point starburst. */
    private void star(Canvas c, float x, float y, float r, int colour) {
        mFill.setShader(null);
        mFill.setColor(colour);
        mPath.reset();
        for (int i = 0; i < 8; i++) {
            double a = Math.PI / 4 * i - Math.PI / 2;
            float rr = (i % 2 == 0) ? r : r * 0.24f;
            float px = x + (float) Math.cos(a) * rr, py = y + (float) Math.sin(a) * rr;
            if (i == 0) mPath.moveTo(px, py);
            else mPath.lineTo(px, py);
        }
        mPath.close();
        c.drawPath(mPath, mFill);
    }

    /** A little pennant on a pole (checkpoints when the game's sprite is not there). */
    private void flag(Canvas c, float x, float y, float r, int colour) {
        mStroke.setShader(null);
        mStroke.setStrokeWidth(theme.stroke);
        mStroke.setColor(colour);
        c.drawLine(x - r * 0.5f, y + r, x - r * 0.5f, y - r, mStroke);
        mFill.setShader(null);
        mFill.setColor(colour);
        mPath.reset();
        mPath.moveTo(x - r * 0.5f, y - r);
        mPath.lineTo(x + r, y - r * 0.5f);
        mPath.lineTo(x - r * 0.5f, y);
        mPath.close();
        c.drawPath(mPath, mFill);
    }

    private static String modeName(int m) {
        switch (m) {
            case 0: return "START";
            case 1: return "LOADING";
            case 2: return "SAVING";
            case 3: return "CUT SCENE";
            case 4: return "MENU";
            case 5: return "PLAYING";
            case 6: return "PAUSED";
            default: return "";
        }
    }
}
