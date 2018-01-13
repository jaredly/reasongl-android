
let first = ref(true);
let checkRebuild = (filePath) => {
  if (first^) {
    first := false;

    MLforJava.setHotReloaded(cmaFile => {
      Capi.logAndroid("Hot Reloading!!!!");
      try {
        Dynlink.loadfile(cmaFile);
      } {
        | err => {
          Capi.logAndroid("Failed loading: " ++ Printexc.to_string(err));
        }
      }
    });
    MLforJava.setMain(context => {
      Capi.startHotReloading(~context, ~host=[%env ("LOCAL_IP", "127.0.0.1")], ~baseFile=filePath);
    });
  }
};
