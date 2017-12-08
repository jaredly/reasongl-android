let module Gl
/* Uncomment this to see what still needs to be done */
: RGLInterface.t
 = {
  include Bindings;
  /* include Tgls_new; */
  type contextT = Capi.window;
  type programT = Tgls_new.programT;
  type attributeT = Tgls_new.attribT;

  let target = "native-android";
  module type FileT = {type t; let readFile: (~filename: string, ~cb: string => unit) => unit;
    let saveUserData: (~key: string, ~value: 'a) => bool;
    let loadUserData: (~key: string) => option('a);
  };
  module File: FileT = {
    type t;
    let readFile = (~filename, ~cb) => {
      switch (loadFile(filename)) {
      | None => failwith("File not found in resources: " ++ filename)
      | Some(text) => cb(text)
      }
    };
    let loadUserData = (~key) => None;
    let saveUserData = (~key, ~value) => false;
  };

  module type WindowT = {
    type t;
    let getWidth: t => int;
    let getHeight: t => int;
    let getPixelWidth: t => int;
    let getPixelHeight: t => int;
    let getPixelScale: t => float;
    let init: (~title: string=?, ~argv: array(string), (t) => unit) => unit;
    let setWindowSize: (~window: t, ~width: int, ~height: int) => unit;
    let getContext: t => Capi.window;
  };

  module Window: WindowT = {
    type t = Capi.window;
    let getWidth = (window) => Capi.getWindowWidth(window);
    let getHeight = (window) => Capi.getWindowHeight(window);
    /** TODO these are probably wrong */
    let getPixelWidth = (window) => getWidth(window);
    let getPixelHeight = (window) => getHeight(window);
    let getPixelScale = (window) => 1.;

    let setWindowSize = (~window, ~width, ~height) => {
      /* umm this can't happen.
      one thing we could do is have an "intrinsic size" and a "real size",
      and this makes it so we scale whatever they're drawing to the actual
      size of the phone.
      But I don't love that idea.
       */
      ()
    };

    let getContext = t => t;
    let init = (~title=?, ~argv: array(string), cb) => {
      MLforJava.setMain((vc: Capi.window) => {
          /* setPreferredFramesPerSecond(vc, 60); */
          /* let view = getView(vc); */
          /* setContext(view, context); */
          /* setDrawableDepthFormat(view, GLKViewDrawableDepthFormat24); */
          /* setCurrentContext(context); */
        try {
          cb(Obj.magic(vc))
        } {
          | Failure(text) => {
            Capi.logAndroid("Dying with failure: " ++ text)
          }
          | err => {
            Capi.logAndroid("Dying with unknown exception")
          }
        }
      })
    };
  };

  let module Events: RGLEvents.t = {
    include RGLEvents.T;
    let keycodeMap = (keycode) => {
      Nothing
    }
  };

  /* let getTimeMs = () => 0.; */
  let render = (~window, ~mouseDown=?, ~mouseUp=?, ~mouseMove=?, ~keyDown=?, ~keyUp=?, ~windowResize=?, ~displayFunc, ()) => {
    MLforJava.setUpdate((time) => {
      try {displayFunc(time *. 1000.)} {
      | Failure(text) => {
        Capi.logAndroid("Dying in update with failure " ++ text)
      }
      | _ => {
        Capi.logAndroid("Dying for unknown reaslong")
      }
      }
    });
    MLforJava.setTouchDrag(switch mouseMove {
    | None => (x, y) => ()
    | Some(fn) => (x, y) => ignore(fn(~x=int_of_float(x), ~y=int_of_float(y)))
    });
    MLforJava.setTouchPress(switch mouseDown {
    | None => (x, y) => ()
    | Some(fn) => (x, y) => ignore(fn(~button=Events.LeftButton, ~state=Events.MouseDown, ~x=int_of_float(x), ~y=int_of_float(y)))
    });
    MLforJava.setTouchRelease(switch mouseUp {
    | None => (x, y) => ()
    | Some(fn) => (x, y) => ignore(fn(~button=Events.LeftButton, ~state=Events.MouseUp, ~x=int_of_float(x), ~y=int_of_float(y)))
    });
  };

  type shaderParamsT =
    | Shader_delete_status
    | Compile_status
    | Shader_type;
  type programParamsT =
    | Program_delete_status
    | Link_status
    | Validate_status;
  let getProgramParameter = (~context, ~program: programT, ~paramName) =>
    switch paramName {
    | Program_delete_status =>
      Tgls_new.getProgramiv(~program, ~pname=GLConstants.gl_delete_status)
    | Link_status => Tgls_new.getProgramiv(~program, ~pname=GLConstants.gl_link_status)
    | Validate_status => Tgls_new.getProgramiv(~program, ~pname=GLConstants.gl_validate_status)
    };
  let getShaderParameter = (~context, ~shader, ~paramName) =>
    switch paramName {
    | Shader_delete_status =>
      Tgls_new.getShaderiv(~shader, ~pname=GLConstants.gl_delete_status)
    | Compile_status => Tgls_new.getShaderiv(~shader, ~pname=GLConstants.gl_compile_status)
    | Shader_type => Tgls_new.getShaderiv(~shader, ~pname=GLConstants.gl_shader_type)
};
  let shaderSource = (~context, ~shader, ~source) => {
    Tgls_new.shaderSource(shader, [|source|])
  };

  let readPixels_RGBA = (~context as _, ~x, ~y, ~width, ~height) =>
    Tgls_new.readPixels_RGBA(~x, ~y, ~width, ~height);
  let createTexture = (~context: contextT) => Tgls_new.genTexture();

  let texImage2D_RGBA =
    (
      ~context: contextT,
      ~target: int,
      ~level: int,
      ~width: int,
      ~height: int,
      ~border: int,
      ~data: Bigarray.Array1.t('a, 'b, Bigarray.c_layout)
    ) =>
    Tgls_new.texImage2D_RGBA(~target, ~level, ~width, ~height, ~border, ~data);

  let drawElementsInstanced = (~context: contextT, ~mode: int, ~count: int, ~type_: int, ~indices: int, ~primcount: int) =>
    failwith("We're using opengles 2, which doesn't support drawElementsInstanced");

  type loadOptionT =
    | LoadAuto
    | LoadL
    | LoadLA
    | LoadRGB
    | LoadRGBA;

  type imageT = {
    width: int,
    height: int,
    channels: int,
    data: Bigarray.Array1.t(int, Bigarray.int8_unsigned_elt, Bigarray.c_layout)
  };
  let getImageWidth = (image) => image.width;
  let getImageHeight = (image) => image.height;

  let loadImage = (~context: Capi.window, ~filename: string, ~loadOption=?, ~callback: option(imageT) => unit, unit) => {
    callback(Bindings.loadImage(~context, ~filename))
  };

  let texImage2DWithImage = (~context, ~target, ~level, ~image) =>
    texImage2D_RGBA(
      ~context,
      ~target,
      ~level,
      ~width=image.width,
      ~height=image.height,
      ~border=0,
      ~data=image.data
  );

  let vertexAttribDivisor = (~context: contextT, ~attribute: attributeT, ~divisor: int) => failwith("OpenGL ES 2.0 Doesn't support vertexattribdivisor");

  module type Bigarray = {
    type t('a, 'b);
    type float64_elt;
    type float32_elt;
    type int16_unsigned_elt;
    type int16_signed_elt;
    type int8_unsigned_elt;
    type int8_signed_elt;
    type int_elt;
    type int32_elt;
    type int64_elt;
    type kind('a, 'b) =
      | Float64: kind(float, float64_elt)
      | Float32: kind(float, float32_elt)
      | Int16: kind(int, int16_signed_elt)
      | Uint16: kind(int, int16_unsigned_elt)
      | Int8: kind(int, int8_signed_elt)
      | Uint8: kind(int, int8_unsigned_elt)
      | Char: kind(char, int8_unsigned_elt)
      | Int: kind(int, int_elt)
      | Int64: kind(int64, int64_elt)
      | Int32: kind(int32, int32_elt);
    let create: (kind('a, 'b), int) => t('a, 'b);
    let of_array: (kind('a, 'b), array('a)) => t('a, 'b);
    let dim: t('a, 'b) => int;
    let blit: (t('a, 'b), t('a, 'b)) => unit;
    let unsafe_blit: (t('a, 'b), t('a, 'b), ~offset: int, ~numOfBytes: int) => unit;
    let get: (t('a, 'b), int) => 'a;
    let unsafe_get: (t('a, 'b), int) => 'a;
    let set: (t('a, 'b), int, 'a) => unit;
    let unsafe_set: (t('a, 'b), int, 'a) => unit;
    let sub: (t('a, 'b), ~offset: int, ~len: int) => t('a, 'b);
  };

    module Bigarray = {
      type t('a, 'b) = Bigarray.Array1.t('a, 'b, Bigarray.c_layout);
      type float64_elt = Bigarray.float64_elt;
      type float32_elt = Bigarray.float32_elt;
      type int16_unsigned_elt = Bigarray.int16_unsigned_elt;
      type int16_signed_elt = Bigarray.int16_signed_elt;
      type int8_unsigned_elt = Bigarray.int8_unsigned_elt;
      type int8_signed_elt = Bigarray.int8_signed_elt;
      type int_elt = Bigarray.int_elt;
      type int32_elt = Bigarray.int32_elt;
      type int64_elt = Bigarray.int64_elt;
      type kind('a, 'b) =
        | Float64: kind(float, float64_elt)
        | Float32: kind(float, float32_elt)
        | Int16: kind(int, int16_signed_elt)
        | Uint16: kind(int, int16_unsigned_elt)
        | Int8: kind(int, int8_signed_elt)
        | Uint8: kind(int, int8_unsigned_elt)
        | Char: kind(char, int8_unsigned_elt)
        | Int: kind(int, int_elt)
        | Int64: kind(int64, int64_elt)
        | Int32: kind(int32, int32_elt);
      let create = (type a, type b, kind: kind(a, b), size) : t(a, b) =>
        switch kind {
        | Float64 => Bigarray.Array1.create(Bigarray.Float64, Bigarray.c_layout, size)
        | Float32 => Bigarray.Array1.create(Bigarray.Float32, Bigarray.c_layout, size)
        | Int16 => Bigarray.Array1.create(Bigarray.Int16_signed, Bigarray.c_layout, size)
        | Uint16 => Bigarray.Array1.create(Bigarray.Int16_unsigned, Bigarray.c_layout, size)
        | Int8 => Bigarray.Array1.create(Bigarray.Int8_signed, Bigarray.c_layout, size)
        | Uint8 => Bigarray.Array1.create(Bigarray.Int8_unsigned, Bigarray.c_layout, size)
        | Char => Bigarray.Array1.create(Bigarray.Char, Bigarray.c_layout, size)
        | Int => Bigarray.Array1.create(Bigarray.Int, Bigarray.c_layout, size)
        | Int64 => Bigarray.Array1.create(Bigarray.Int64, Bigarray.c_layout, size)
        | Int32 => Bigarray.Array1.create(Bigarray.Int32, Bigarray.c_layout, size)
        };
      let of_array = (type a, type b, kind: kind(a, b), arr: array(a)) : t(a, b) =>
        switch kind {
        | Float64 => Bigarray.Array1.of_array(Bigarray.Float64, Bigarray.c_layout, arr)
        | Float32 => Bigarray.Array1.of_array(Bigarray.Float32, Bigarray.c_layout, arr)
        | Int16 => Bigarray.Array1.of_array(Bigarray.Int16_signed, Bigarray.c_layout, arr)
        | Uint16 => Bigarray.Array1.of_array(Bigarray.Int16_unsigned, Bigarray.c_layout, arr)
        | Int8 => Bigarray.Array1.of_array(Bigarray.Int8_signed, Bigarray.c_layout, arr)
        | Uint8 => Bigarray.Array1.of_array(Bigarray.Int8_unsigned, Bigarray.c_layout, arr)
        | Char => Bigarray.Array1.of_array(Bigarray.Char, Bigarray.c_layout, arr)
        | Int => Bigarray.Array1.of_array(Bigarray.Int, Bigarray.c_layout, arr)
        | Int64 => Bigarray.Array1.of_array(Bigarray.Int64, Bigarray.c_layout, arr)
        | Int32 => Bigarray.Array1.of_array(Bigarray.Int32, Bigarray.c_layout, arr)
        };
      let dim = Bigarray.Array1.dim;
      let blit = Bigarray.Array1.blit;
      /* "What is going on here" you may ask.
         Well we kinda sorta profiled the app and noticed that ba_caml_XYZ was called a lot.
         This is an attempt at reducing the cost of those calls. We implemented our own C blit (
         which is just memcpy)
                  Ben - August 28th 2017
            */
      [@noalloc]
      external unsafe_blit :
        (
          Bigarray.Array1.t('a, 'b, 'c),
          Bigarray.Array1.t('a, 'b, 'c),
          ~offset: int,
          ~numOfBytes: int
        ) =>
        unit =
        "bigarray_unsafe_blit";
      let get = Bigarray.Array1.get;
      let unsafe_get = Bigarray.Array1.unsafe_get;
      let set = Bigarray.Array1.set;
      let unsafe_set = Bigarray.Array1.unsafe_set;
      let sub = (type a, type b, arr: t(a, b), ~offset, ~len) : t(a, b) =>
        Bigarray.Array1.sub(arr, offset, len);
  };

  module type Mat4T = {
    type t;
    let to_array: t => array(float);
    let create: unit => t;
    let identity: (~out: t) => unit;
    let translate: (~out: t, ~matrix: t, ~vec: array(float)) => unit;
    let scale: (~out: t, ~matrix: t, ~vec: array(float)) => unit;
    let rotate: (~out: t, ~matrix: t, ~rad: float, ~vec: array(float)) => unit;
    let ortho:
      (
        ~out: t,
        ~left: float,
        ~right: float,
        ~bottom: float,
        ~top: float,
        ~near: float,
        ~far: float
      ) =>
      unit;
  };

  module Mat4: Mat4T = {
    type t = array(float);
    let to_array = (a) => a;
    let epsilon = 0.00001;
    let create = () => [|
      1.0,
      0.0,
      0.0,
      0.0,
      0.0,
      1.0,
      0.0,
      0.0,
      0.0,
      0.0,
      1.0,
      0.0,
      0.0,
      0.0,
      0.0,
      1.0
    |];
    let identity = (~out: t) => {
      out[0] = 1.0;
      out[1] = 0.0;
      out[2] = 0.0;
      out[3] = 0.0;
      out[4] = 0.0;
      out[5] = 1.0;
      out[6] = 0.0;
      out[7] = 0.0;
      out[8] = 0.0;
      out[9] = 0.0;
      out[10] = 1.0;
      out[11] = 0.0;
      out[12] = 0.0;
      out[13] = 0.0;
      out[14] = 0.0;
      out[15] = 1.0
    };
    let translate = (~out: t, ~matrix: t, ~vec: array(float)) => {
      let x = vec[0];
      let y = vec[1];
      let z = vec[2];
      if (matrix === out) {
        out[12] = matrix[0] *. x +. matrix[4] *. y +. matrix[8] *. z +. matrix[12];
        out[13] = matrix[1] *. x +. matrix[5] *. y +. matrix[9] *. z +. matrix[13];
        out[14] = matrix[2] *. x +. matrix[6] *. y +. matrix[10] *. z +. matrix[14];
        out[15] = matrix[3] *. x +. matrix[7] *. y +. matrix[11] *. z +. matrix[15]
      } else {
        let a00 = matrix[0];
        let a01 = matrix[1];
        let a02 = matrix[2];
        let a03 = matrix[3];
        let a10 = matrix[4];
        let a11 = matrix[5];
        let a12 = matrix[6];
        let a13 = matrix[7];
        let a20 = matrix[8];
        let a21 = matrix[9];
        let a22 = matrix[10];
        let a23 = matrix[11];
        out[0] = a00;
        out[1] = a01;
        out[2] = a02;
        out[3] = a03;
        out[4] = a10;
        out[5] = a11;
        out[6] = a12;
        out[7] = a13;
        out[8] = a20;
        out[9] = a21;
        out[10] = a22;
        out[11] = a23;
        out[12] = a00 *. x +. a10 *. y +. a20 *. z +. matrix[12];
        out[13] = a01 *. x +. a11 *. y +. a21 *. z +. matrix[13];
        out[14] = a02 *. x +. a12 *. y +. a22 *. z +. matrix[14];
        out[15] = a03 *. x +. a13 *. y +. a23 *. z +. matrix[15]
      }
    };
    let scale = (~out: t, ~matrix: t, ~vec: array(float)) => {
      let x = vec[0];
      let y = vec[1];
      let z = vec[2];
      out[0] = matrix[0] *. x;
      out[1] = matrix[1] *. x;
      out[2] = matrix[2] *. x;
      out[3] = matrix[3] *. x;
      out[4] = matrix[4] *. y;
      out[5] = matrix[5] *. y;
      out[6] = matrix[6] *. y;
      out[7] = matrix[7] *. y;
      out[8] = matrix[8] *. z;
      out[9] = matrix[9] *. z;
      out[10] = matrix[10] *. z;
      out[11] = matrix[11] *. z;
      out[12] = matrix[12];
      out[13] = matrix[13];
      out[14] = matrix[14];
      out[15] = matrix[15]
    };
    let rotate = (~out: t, ~matrix: t, ~rad: float, ~vec: array(float)) => {
      let x = vec[0];
      let y = vec[1];
      let z = vec[2];
      let len = sqrt(x *. x +. y *. y +. z *. z);
      if (abs_float(len) > epsilon) {
        let len = 1. /. sqrt(x *. x +. y *. y +. z *. z);
        let x = matrix[0] *. len;
        let y = matrix[1] *. len;
        let z = matrix[2] *. len;
        let s = sin(rad);
        let c = cos(rad);
        let t = 1. -. c;
        let a00 = matrix[0];
        let a01 = matrix[1];
        let a02 = matrix[2];
        let a03 = matrix[3];
        let a10 = matrix[4];
        let a11 = matrix[5];
        let a12 = matrix[6];
        let a13 = matrix[7];
        let a20 = matrix[8];
        let a21 = matrix[9];
        let a22 = matrix[10];
        let a23 = matrix[11];
        let b00 = x *. x *. t +. c;
        let b01 = y *. x *. t +. z *. s;
        let b02 = z *. x *. t -. y *. s;
        let b10 = x *. y *. t -. y *. s;
        let b11 = y *. y *. t -. c;
        let b12 = z *. y *. t +. x *. s;
        let b20 = x *. z *. t +. y *. s;
        let b21 = y *. z *. t -. x *. s;
        let b22 = z *. z *. t +. c;
        matrix[0] = a00 *. b00 +. a10 *. b01 +. a20 *. b02;
        matrix[1] = a01 *. b00 +. a11 *. b01 +. a21 *. b02;
        matrix[2] = a02 *. b00 +. a12 *. b01 +. a22 *. b02;
        matrix[3] = a03 *. b00 +. a13 *. b01 +. a23 *. b02;
        matrix[4] = a00 *. b10 +. a10 *. b11 +. a20 *. b12;
        matrix[5] = a01 *. b10 +. a11 *. b11 +. a21 *. b12;
        matrix[6] = a02 *. b10 +. a12 *. b11 +. a22 *. b12;
        matrix[7] = a03 *. b10 +. a13 *. b11 +. a23 *. b12;
        matrix[8] = a00 *. b20 +. a10 *. b21 +. a20 *. b22;
        matrix[9] = a01 *. b20 +. a11 *. b21 +. a21 *. b22;
        matrix[10] = a02 *. b20 +. a12 *. b21 +. a22 *. b22;
        matrix[11] = a03 *. b20 +. a13 *. b21 +. a23 *. b22
      };
      if (matrix !== out) {
        out[12] = matrix[12];
        out[13] = matrix[13];
        out[14] = matrix[14];
        out[15] = matrix[15]
      }
    };
    let ortho =
        (
          ~out: t,
          ~left: float,
          ~right: float,
          ~bottom: float,
          ~top: float,
          ~near: float,
          ~far: float
        ) => {
      let lr = 1. /. (left -. right);
      let bt = 1. /. (bottom -. top);
      let nf = 1. /. (near -. far);
      out[0] = (-2.) *. lr;
      out[1] = 0.;
      out[2] = 0.;
      out[3] = 0.;
      out[4] = 0.;
      out[5] = (-2.) *. bt;
      out[6] = 0.;
      out[7] = 0.;
      out[8] = 0.;
      out[9] = 0.;
      out[10] = 2. *. nf;
      out[11] = 0.;
      out[12] = (left +. right) *. lr;
      out[13] = (top +. bottom) *. bt;
      out[14] = (far +. near) *. nf;
      out[15] = 1.
    };
  };

  let uniformMatrix4fv = (~context, ~location, ~value: Mat4.t) =>
  Tgls_new.uniformMatrix4fv(~location, ~transpose=false, ~value=Mat4.to_array(value));

  let drawElements = (~context) => Tgls_new.drawElements;
  let drawArrays = (~context) => Tgls_new.drawArrays;
  let getShaderSource = (~context) => Tgls_new.getShaderSource;
  let getProgramInfoLog = (~context) => Tgls_new.getProgramInfoLog;
  let getShaderInfoLog = (~context) => Tgls_new.getShaderInfoLog;
  let vertexAttribPointer = (~context) => Tgls_new.vertexAttribPointer;
  let enableVertexAttribArray = (~context, ~attribute) => Tgls_new.enableVertexAttribArray(attribute);
  let getAttribLocation = (~context) => Tgls_new.getAttribLocation;
  let getUniformLocation = (~context) => Tgls_new.getUniformLocation;
  let clear = (~context, ~mask) => Tgls_new.clear(mask);
  let viewport = (~context) => Tgls_new.viewport;
  let bufferData = (~context) => Tgls_new.bufferData;
  let uniform4f = (~context) => Tgls_new.uniform4f;
  let uniform3f = (~context) => Tgls_new.uniform3f;
  let uniform2f = (~context) => Tgls_new.uniform2f;
  let uniform1f = (~context) => Tgls_new.uniform1f;
  let uniform1i = (~context) => Tgls_new.uniform1i;
  let texSubImage2D = (~context) => Tgls_new.texSubImage2D;
  let blendFunc = (~context, sfactor, dfactor) => Tgls_new.blendFunc(~sfactor, ~dfactor);
  let disable = (~context) => Tgls_new.disable;
  let enable = (~context) => Tgls_new.enable;
  let texParameteri = (~context) => Tgls_new.texParameteri;
  let bindTexture = (~context) => Tgls_new.bindTexture;
  let activeTexture = (~context) => Tgls_new.activeTexture;
  /* '<,'>s/(\w+)'.*$/$1 = (~context) => Tgls_new.$1;/g */
  type textureT = Tgls_new.textureT;
  type uniformT = Tgls_new.uniformT;
  type bufferT = Tgls_new.bufferT;
  type shaderT = Tgls_new.shaderT;
  let bindBuffer = (~context) => Tgls_new.bindBuffer;
  let createBuffer = (~context) => Tgls_new.genBuffer();
  let useProgram = (~context) => Tgls_new.useProgram;
  let linkProgram = (~context) => Tgls_new.linkProgram;
  let compileShader = (~context) => Tgls_new.compileShader;
  let deleteShader = (~context) => Tgls_new.deleteShader;
  let attachShader = (~context) => Tgls_new.attachShader;
  let createShader = (~context) => Tgls_new.createShader;
  let createProgram = (~context) => Tgls_new.createProgram();
  let clearColor = (~context) => Tgls_new.clearColor;

};


