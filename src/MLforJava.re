
let setMain = (fn: (Capi.window) => unit) => Callback.register("reasonglMain", fn);
let setUpdate = (fn: (float) => unit) => Callback.register("reasonglUpdate", fn);
let setResize = (fn: (int, int) => unit) => Callback.register("reasonglResize", fn);
/** TODO change type */
let setTouchPress = (fn: list((float, float, float)) => unit) => Callback.register("reasonglTouchPress", fn);
let setTouchRelease = (fn: list((float, float, float)) => unit) => Callback.register("reasonglTouchRelease", fn);
let setTouchDrag = (fn: list((float, float, float)) => unit) => Callback.register("reasonglTouchDrag", fn);
let setBackPressed = (fn: unit => bool) => Callback.register("reasonglBackPressed", fn);

let setHotReloaded = (fn: (string) => unit) => Callback.register("reasonglHotReloaded", fn);
