package com.example.hellolibs;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.SystemClock;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainActivity extends Activity {
    Renderer renderer;
    GLSurfaceView glView;

    private static class Renderer implements GLSurfaceView.Renderer {
        OCamlBindings bindings = new OCamlBindings();
        long lastDrawTime = SystemClock.elapsedRealtimeNanos();
        GLSurfaceView view;

        public Renderer(GLSurfaceView view) {
            super();

            this.view = view;
            view.setRenderer(this);
            //https://stackoverflow.com/questions/8141209/android-setheight-of-glsurfaceview
        }

        public void onDrawFrame(GL10 gl) {
            SystemClock.elapsedRealtime();
            long now = SystemClock.elapsedRealtimeNanos();
            long elapsed = now - lastDrawTime;
            bindings.reasonglUpdate(elapsed / 1000.0);
            lastDrawTime = now;
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) {
            // TODO
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            bindings.reasonglMain(view);
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        glView = new GLSurfaceView(this);
        renderer = new Renderer(glView);
        setContentView(glView);
    }
}
