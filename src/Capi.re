
type jniEnv;
type glView;
type window = Window(jniEnv, glView);

external logAndroid : string => unit = "logAndroid";
external getWindowHeight : window => int = "getWindowHeight";
external getWindowWidth : window => int = "getWindowWidth";

