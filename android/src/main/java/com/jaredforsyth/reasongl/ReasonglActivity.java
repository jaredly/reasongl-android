package com.jaredforsyth.reasongl;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;

/**
 * Created by jared on 12/12/17.
 */

public class ReasonglActivity extends Activity {
    ReasonGLView glView;
    @Override
    public void onBackPressed() {
        glView.handleBackPressed(new Runnable() {
            @Override
            public void run() {
                ReasonglActivity.super.onBackPressed();
            }
        });
    }

    private void ensureFullScreen() {
        getWindow().getDecorView().setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_FULLSCREEN |
                        View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION |
                        View.SYSTEM_UI_FLAG_LAYOUT_STABLE
        );

        // Keep the status bar hidden
        View decorView = getWindow().getDecorView();
        decorView.setOnSystemUiVisibilityChangeListener
                (new View.OnSystemUiVisibilityChangeListener() {
                    @Override
                    public void onSystemUiVisibilityChange(int visibility) {
                        // Note that system bars will only be "visible" if none of the
                        // LOW_PROFILE, HIDE_NAVIGATION, or FULLSCREEN flags are set.
                        if ((visibility & View.SYSTEM_UI_FLAG_FULLSCREEN) == 0) {
                            final Handler handler = new Handler();
                            handler.postDelayed(new Runnable() {
                                @Override
                                public void run() {
                                    getWindow().getDecorView().setSystemUiVisibility(
                                            View.SYSTEM_UI_FLAG_FULLSCREEN |
                                                    View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION |
                                                    View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                                    );
                                }
                            }, 1000);
                            // TODO: The system bars are visible. Make any desired
                            // adjustments to your UI, such as showing the action bar or
                            // other navigational controls.
                        } else {
                            // TODO: The system bars are NOT visible. Make any desired
                            // adjustments to your UI, such as hiding the action bar or
                            // other navigational controls.
                        }
                    }
                });

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        ensureFullScreen();

        glView = new ReasonGLView(this);
        setContentView(glView);
    }
}
