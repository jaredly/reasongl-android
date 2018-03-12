package com.jaredforsyth.reasongl;

import static android.os.Looper.getMainLooper;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Handler;
import android.os.SystemClock;
import android.util.DisplayMetrics;
import android.view.MotionEvent;
import android.view.View;

import java.util.List;
import java.util.ArrayList;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by jared on 12/9/17.
 */

public class ReasonGLView extends GLSurfaceView {
    private final Context mContext;
    private final Renderer mRenderer;
    private boolean firstReload = true;

    private static class Renderer implements GLSurfaceView.Renderer {
        OCamlBindings bindings = new OCamlBindings();
        long lastDrawTime = SystemClock.elapsedRealtimeNanos();
        MyAssetManager mMyAssetManager;
        GLSurfaceView view;
        Context context;

        public Renderer(ReasonGLView view, Context context, float pixelDensity) {
            super();

            mMyAssetManager = new MyAssetManager(context, pixelDensity, view);
            this.view = view;
            this.context = context;
            view.setEGLContextClientVersion(2);
            view.setRenderer(this);
        }

        public void onDrawFrame(GL10 gl) {
            long now = SystemClock.elapsedRealtimeNanos();
            long elapsed = now - lastDrawTime;
            bindings.reasonglUpdate(elapsed / 1000.0);
            lastDrawTime = now;
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) {
            bindings.reasonglResize(width, height);
            // TODO
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            bindings.reasonglMain(view, mMyAssetManager);
        }
    }

    private static double[] getPointerList(final MotionEvent event, final float density) {
        double[] pointers = new double[event.getPointerCount() * 3];
        for (int i=0; i<event.getPointerCount(); i++) {
          pointers[i * 3] = (double)event.getPointerId(i);
          pointers[i * 3 + 1] = (double)event.getX(i) / density;
          pointers[i * 3 + 2] = (double)event.getY(i) / density;
        }
        return pointers;
    }

    private static double[] getActionPointerList(final MotionEvent event, final float density) {
        double[] pointers = new double[3];
        int index = event.getActionIndex();
        pointers[0] = (double)event.getPointerId(index);
        pointers[1] = (double)event.getX(index) / density;
        pointers[2] = (double)event.getY(index) / density;
        return pointers;
    }

    public ReasonGLView(Activity context) {
        super(context);
        mContext = context;

        DisplayMetrics metrics = new DisplayMetrics();
        context.getWindowManager().getDefaultDisplay().getMetrics(metrics);
        final float density = metrics.density;

        mRenderer = new Renderer(this, context, density);

        final ReasonGLView self = this;
        final OCamlBindings bindings = mRenderer.bindings;
        this.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, final MotionEvent event) {
                if (event != null) {
                    if (event.getActionMasked() == MotionEvent.ACTION_DOWN) {
                        final double[] pointers = getPointerList(event, density);
                        self.queueEvent(new Runnable() {
                            @Override
                            public void run() {
                                bindings.reasonglTouchPress(pointers);
                                // bindings.reasonglTouchPress(event.getX() / density, event.getY()/ density);
                            }
                        });
                    } else if (event.getActionMasked() == MotionEvent.ACTION_POINTER_DOWN) {
                        final double[] pointers = getActionPointerList(event, density);
                        self.queueEvent(new Runnable() {
                            @Override
                            public void run() {
                                bindings.reasonglTouchPress(pointers);
                            }
                        });
                    } else if (event.getActionMasked() == MotionEvent.ACTION_MOVE) {
                        final double[] pointers = getPointerList(event, density);
                        self.queueEvent(new Runnable() {
                            @Override
                            public void run() {
                                bindings.reasonglTouchDrag(pointers);
                                // bindings.reasonglTouchDrag(event.getX() / density, event.getY() / density);
                            }
                        });
                    } else if (event.getActionMasked() == MotionEvent.ACTION_UP) {
                        final double[] pointers = getPointerList(event, density);
                        self.queueEvent(new Runnable() {
                            @Override
                            public void run() {
                                bindings.reasonglTouchRelease(pointers);
                                // bindings.reasonglTouchRelease(event.getX() / density, event.getY() / density);
                            }
                        });
                    } else if (event.getActionMasked() == MotionEvent.ACTION_POINTER_UP) {
                        final double[] pointers = getActionPointerList(event, density);
                        self.queueEvent(new Runnable() {
                            @Override
                            public void run() {
                                bindings.reasonglTouchRelease(pointers);
                                // bindings.reasonglTouchRelease(event.getX() / density, event.getY() / density);
                            }
                        });
                    }
                    return true;
                } else {
                    return false;
                }
            }
        });
    }

    public void handleBackPressed(final Runnable superBackPressed) {
        this.queueEvent(new Runnable() {
            @Override
            public void run() {
                if (!mRenderer.bindings.reasonglBackPressed()) {
                    new Handler(getMainLooper()).post(superBackPressed);
                }
            }
        });
    }

    public void onHotReload(final String filePath) {
        final ReasonGLView view = this;
        final boolean firstReload = this.firstReload;
        this.firstReload = false;
        // I probably want to rotate through several file names so I don't step on the previous one?
        this.queueEvent(new Runnable() {
            @Override
            public void run() {
                mRenderer.bindings.reasonglHotReloaded(filePath);
                if (firstReload) {
                    mRenderer.bindings.reasonglMain(view, mRenderer.mMyAssetManager);
                }
            }
        });
    }
}
