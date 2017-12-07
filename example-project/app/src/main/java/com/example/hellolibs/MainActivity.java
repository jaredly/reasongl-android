package com.example.hellolibs;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainActivity extends Activity {
    Renderer renderer;
    GLSurfaceView glView;

    private static class Renderer implements GLSurfaceView.Renderer {
        OCamlBindings bindings = new OCamlBindings();
        GLSurfaceView view;

        public Renderer(GLSurfaceView view) {
            super();

            this.view = view;
            view.setRenderer(this);
        }

        public void onDrawFrame(GL10 gl) {
            bindings.reasonglUpdate(1.0);
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
