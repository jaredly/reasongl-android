
type window;
type bitmap;

external logAndroid : string => unit = "logAndroid";
external showAlert : (~context: window, ~title: string, ~message: string) => unit = "showAlert";
external startHotReloading : (~context: window, ~host: string, ~baseFile: string) => unit = "startHotReloading";
external getWindowHeight : window => int = "getWindowHeight";
external getWindowWidth : window => int = "getWindowWidth";
external getDevicePixelRatio : window => float = "getDevicePixelRatio";

external texImage2DWithBitmap : (~context: window, ~target: int, ~level: int, ~bitmap: bitmap, ~border: int) => unit = "texImage2DWithBitmap";
external fillTextureWithColor : (~context: window, ~target: int, ~level: int, ~red: int, ~green: int, ~blue: int, ~alpha: int) => unit = "fillTextureWithColor_bytecode" "fillTextureWithColor_native";

external saveData : (~context: window, ~key: string, ~value: bytes) => unit = "saveData";
external loadData : (~context: window, ~key: string) => option(bytes) = "loadData";

external loadImage : (~context: window, ~filename: string) => option('a) = "loadImage";
external loadFile : (~context: window, ~filename: string) => option(string) = "loadFile";

external getTimeMs : unit => float = "getTimeMs";