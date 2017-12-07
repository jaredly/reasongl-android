(* register *)

Callback.register "reasonglMain"
(fun () -> Capi.logAndroid("MAIN NOT IMPLEMENTED"));
Callback.register "reasonglUpdate"
(fun () -> Capi.logAndroid("UPDATE NOT IMPLEMENTED"));
Callback.register "reasonglTouchPress"
(fun () -> Capi.logAndroid("TOUCH PRESS NOT IMPLEMENTED"));
Callback.register "reasonglTouchDrag"
(fun () -> Capi.logAndroid("TOUCH DRAG NOT IMPLEMENTED"));
Callback.register "reasonglTouchRelease"
(fun () -> Capi.logAndroid("TOUCH RELEASE NOT IMPLEMENTED"))
