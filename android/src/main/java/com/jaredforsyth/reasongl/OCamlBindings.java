package com.jaredforsyth.reasongl;

import com.jaredforsyth.reasongl.MyAssetManager;
import java.util.List;

import android.opengl.GLSurfaceView;

public class OCamlBindings {
    public native void reasonglMain(GLSurfaceView view, MyAssetManager assetManager);
    public native void reasonglResize(int width, int height);
    public native void reasonglUpdate(double timeSinceLastDraw);
    public native boolean reasonglBackPressed();
    public native void reasonglTouchPress(double[] points);
    public native void reasonglTouchDrag(double[] points);
    public native void reasonglTouchRelease(double[] points);

    public native void reasonglHotReloaded(String filePath);

    static {
        System.loadLibrary("reasongl");
    }
}
