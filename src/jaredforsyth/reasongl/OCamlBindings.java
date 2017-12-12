package com.jaredforsyth.reasongl;

import com.jaredforsyth.reasongl.MyAssetManager;

import android.opengl.GLSurfaceView;

public class OCamlBindings {
    public native void reasonglMain(GLSurfaceView view, MyAssetManager assetManager);
    public native void reasonglResize(int width, int height);
    public native void reasonglUpdate(double timeSinceLastDraw);
    public native boolean reasonglBackPressed();
    public native void reasonglTouchPress(double x, double y);
    public native void reasonglTouchDrag(double x, double y);
    public native void reasonglTouchRelease(double x, double y);

    static {
        System.loadLibrary("reasongl");
    }
}
