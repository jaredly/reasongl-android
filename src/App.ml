
let echo str = str ^ " from ocaml"

external logAndroid : string -> unit = "logAndroid"

let sayHi text = logAndroid(">>>>>> OCAML <<<<<< " ^ text)


