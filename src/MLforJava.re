
let setMain = (fn: (Capi.window) => unit) => Callback.register("reasonglMain", fn);
let setUpdate = (fn: (float) => unit) => Callback.register("reasonglUpdate", fn);
let setTouchPress = (fn: (float, float) => unit) => Callback.register("reasonglTouchPress", fn);
let setTouchRelease = (fn: (float, float) => unit) => Callback.register("reasonglTouchRelease", fn);
let setTouchDrag = (fn: (float, float) => unit) => Callback.register("reasonglTouchDrag", fn);


