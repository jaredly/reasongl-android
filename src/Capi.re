
type window;
type bitmap;

external logAndroid : string => unit = "logAndroid";
external showAlert : (~context: window, ~title: string, ~message: string) => unit = "showAlert";
external getWindowHeight : window => int = "getWindowHeight";
external getWindowWidth : window => int = "getWindowWidth";

external texImage2DWithBitmap : (~context: window, ~target: int, ~level: int, ~bitmap: bitmap, ~border: int) => unit = "texImage2DWithBitmap";

external saveData : (~context: window, ~key: string, ~value: bytes) => unit = "saveData";
external loadData : (~context: window, ~key: string) => option(bytes) = "loadData";

external loadImage : (~context: window, ~filename: string) => option('a) = "loadImage";

external loadFile : (~context: window, ~filename: string) => option(string) = "loadFile";

external getTimeMs : unit => float = "getTimeMs";