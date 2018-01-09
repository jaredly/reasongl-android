
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
      Capi.startHotReloading(~context, ~host="192.168.0.100", ~baseFile=filePath);
    });
  }
};
