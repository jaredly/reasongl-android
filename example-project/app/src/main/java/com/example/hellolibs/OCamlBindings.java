package com.example.hellolibs;

public class OCamlBindings {
    public native String stringFromC();
    static {
        System.loadLibrary("reasongl");
    }
}
