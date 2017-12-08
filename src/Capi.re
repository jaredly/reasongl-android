
/* type jniEnv;
type glView; */
type window;
type bitmap;
 /* = Window(jniEnv, glView); */

external logAndroid : string => unit = "logAndroid";
external getWindowHeight : window => int = "getWindowHeight";
external getWindowWidth : window => int = "getWindowWidth";

external texImage2DWithBitmap : (~context: window, ~target: int, ~level: int, ~bitmap: bitmap, ~border: int) => unit = "texImage2DWithBitmap";
