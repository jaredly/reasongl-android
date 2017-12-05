package com.jaredforsyth.reasonglexample;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

    // Example of a call to a native method
    TextView tv = (TextView) findViewById(R.id.sample_text);
    tv.setText(stringFromJNI() + " " + stringFromJNI2());
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native String stringFromJNI2();

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("reasongl");
        System.loadLibrary("native-lib");
        System.loadLibrary("reasongl-bindings");
    }
}
