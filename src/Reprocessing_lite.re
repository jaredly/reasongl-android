open GLConstants;

open Bindings;

open Tgls;



/*let render
      ::window
      ::mouseDown
      ::mouseUp
      ::mouseMove
      ::keyDown
      ::keyUp
      ::windowResize
      ::displayFunc
      () => {
    Callback.register "update" displayFunc;
  };
  */
let vertexShaderSource = {|
  attribute vec3 aVertexPosition;
  attribute vec3 aVertexColor;

  uniform mat4 uPMatrix;
  uniform mat3 normalMatrix;

  varying lowp vec4 vColor;

  void main(void) {
    gl_Position = uPMatrix * vec4(aVertexPosition, 1.0);
    vColor = vec4(aVertexColor, 1.0);
  }
|};

let fragmentShaderSource = {|
  varying lowp vec4 vColor;

  void main(void) {
    gl_FragColor = vColor;
  }
|};

/*let vertexData =
  Bigarray.of_array
    Bigarray.Float32
    [|
      0.5,
      (-0.5),
      0.,
      255.,
      0.0,
      0.0,
      (-0.5),
      (-0.5),
      0.,
      255.,
      0.0,
      0.0,
      0.5,
      0.5,
      0.,
      255.,
      0.0,
      0.0,
      (-0.5),
      (-0.5),
      0.,
      255.,
      0.0,
      0.0,
      0.5,
      0.5,
      0.,
      255.,
      0.0,
      0.0,
      (-0.5),
      0.5,
      0.,
      255.,
      0.0,
      0.0
    |];*/
/*let setupGL vc context => {
    setCurrentContext context;
    let program = createProgram ::context;
    let vertexShader = createShader ::context shaderType::gl_vertex_shader;
    shaderSource ::context shader::vertexShader source::[|vertexShaderSource|];
    compileShader ::context shader::vertexShader;
    print_endline (getShaderInfoLog ::context shader::vertexShader);
    attachShader ::context ::program shader::vertexShader;
    let fragmentShader = createShader ::context shaderType::gl_fragment_shader;
    shaderSource ::context shader::fragmentShader source::[|fragmentShaderSource|];
    compileShader ::context shader::fragmentShader;
    print_endline (getShaderInfoLog ::context shader::fragmentShader);
    attachShader ::context ::program shader::fragmentShader;
    linkProgram ::context ::program;
    print_endline (getProgramInfoLog ::context ::program);
    let umvp = getUniformLocation ::context ::program name::"uPMatrix";
    deleteShader ::context shader::vertexShader;
    deleteShader ::context shader::fragmentShader;
    setProgram vc program;
    setUmvp vc umvp;
    enable ::context gl_depth_test;
    let vertexArray = genVertexArray ::context;
    bindVertexArray ::context ::vertexArray;
    setVertexArray vc vertexArray;
    let vertexBuffer = (createBuffer ::context 1).(0);
    bindBuffer ::context target::gl_array_buffer buffer::vertexBuffer;
    setVertexBuffer vc vertexBuffer;
    bufferData
      ::context target::gl_array_buffer data::vertexData usage::gl_static_draw;
    let aVertexPosition = getAttribLocation ::context ::program name::"aVertexPosition";
    enableVertexAttribArray ::context attribute::aVertexPosition;
    vertexAttribPointer
      ::context
      attribute::aVertexPosition
      size::3
      type_::gl_float
      normalize::false
      stride::24
      offset::0;
    let aVertexColor = getAttribLocation ::context ::program name::"aVertexColor";
    enableVertexAttribArray ::context attribute::aVertexColor;
    vertexAttribPointer
      ::context
      attribute::aVertexColor
      size::3
      type_::gl_float
      normalize::false
      stride::24
      offset::12;
    /* unbindVertexArray ::context; */
    print_endline @@ "program log info " ^ getProgramInfoLog ::context ::program
  };*/
module Mat4 = {
  type t;
  external create : unit => t = "Mat4_create";
  external ortho :
    (
      ~out: t,
      ~left: float,
      ~right: float,
      ~bottom: float,
      ~top: float,
      ~near: float,
      ~far: float
    ) =>
    unit =
    "Mat4_ortho_byte" "Mat4_ortho_native";
};

module Reprocessing_Shaders = {
  let vertexShaderSource = {|
  attribute vec2 aVertexPosition;
  attribute vec4 aVertexColor;

  uniform mat4 uPMatrix;

  varying vec4 vColor;

  void main(void) {
    gl_Position = uPMatrix * vec4(aVertexPosition, 0.0, 1.0);
    vColor = aVertexColor;
  }
|};
  let fragmentShaderSource = {|
  varying vec4 vColor;
  varying vec2 vTextureCoord;

  void main(void) {
    gl_FragColor = vColor;
  }
|};
};

type glCamera = {projectionMatrix: Mat4.t};

type batchT = {
  vertexArray: Bigarray.Array1.t(float, Bigarray.float32_elt, Bigarray.c_layout),
  elementArray: Bigarray.Array1.t(int, Bigarray.int16_unsigned_elt, Bigarray.c_layout),
  mutable vertexPtr: int,
  mutable elementPtr: int
};

type strokeCapT =
  | Round
  | Square
  | Project;

type colorT = {
  r: int,
  g: int,
  b: int
};

type styleT = {
  strokeColor: option(colorT),
  strokeWeight: int,
  strokeCap: strokeCapT,
  fillColor: option(colorT)
};

type glEnv = {
  camera: glCamera,
  gl: contextT,
  vertexArrayPtr: vertexArrayT,
  pMatrixUniform: uniformT,
  vertexBuffer: bufferT,
  elementBuffer: bufferT,
  aVertexColor: attributeT,
  aVertexPosition: attributeT,
  batch: batchT,
  mutable style: styleT
};

module Env = {
  let width = (_) => 375;
  let height = (_) => 667;
};

let circularBufferSize = 6 * 10000;

let vertexSize = 8;

let setupGL = (vc, context) => {
  setCurrentContext(context);
  viewport(~context, ~x=(-1), ~y=(-1), ~width=Env.width(), ~height=Env.height());
  clearColor(~context, ~r=0., ~g=0., ~b=0., ~a=1.);
  clear(~context, ~mask=16384 lor 256);

  /*** Camera is a simple record containing one matrix used to project a point in 3D onto the screen. **/
  let camera = {projectionMatrix: Mat4.create()};
  let buffers = createBuffers(~context, 2);
  let vertexBuffer = buffers[0];
  let elementBuffer = buffers[1];
  let program = createProgram(~context);
  let vertexShader = createShader(~context, gl_vertex_shader);
  shaderSource(~context, ~shader=vertexShader, ~source=[|vertexShaderSource|]);
  compileShader(~context, vertexShader);
  print_endline @@ "vertexShader logs: " ++ getShaderInfoLog(~context, vertexShader);
  attachShader(~context, ~program, ~shader=vertexShader);
  let fragmentShader = createShader(~context, gl_fragment_shader);
  shaderSource(~context, ~shader=fragmentShader, ~source=[|fragmentShaderSource|]);
  compileShader(~context, fragmentShader);
  print_endline @@ "fragmentShader logs: " ++ getShaderInfoLog(~context, fragmentShader);
  attachShader(~context, ~program, ~shader=fragmentShader);
  linkProgram(~context, program);
  print_endline @@ "program logs: " ++ getProgramInfoLog(~context, program);
  deleteShader(~context, vertexShader);
  deleteShader(~context, fragmentShader);
  /*setProgram vc program;*/
  useProgram(~context, program);

  /*** Get the attribs ahead of time to be used inside the render function **/
  let aVertexPosition = getAttribLocation(~context, ~program, ~name="aVertexPosition");
  enableVertexAttribArray(~context, ~attribute=aVertexPosition);
  let aVertexColor = getAttribLocation(~context, ~program, ~name="aVertexColor");
  enableVertexAttribArray(~context, ~attribute=aVertexColor);
  let pMatrixUniform = getUniformLocation(~context, ~program, ~name="uPMatrix");
  /* @HACK this function is fuckd */
  uniformMatrix4fv_glk(
    ~context,
    ~location=pMatrixUniform,
    ~transpose=false,
    ~value=camera.projectionMatrix
  );
  /*setUmvp vc pMatrixUniform;*/

  /*** Get attribute and uniform locations for later usage in the draw code. **/
  /*let aTextureCoord =
      getAttribLocation ::context ::program name::"aTextureCoord";
    enableVertexAttribArray ::context attribute::aTextureCoord;

    /** Generate texture buffer that we'll use to pass image data around. **/
    let texture = genTexture ::context;

    * This tells OpenGL that we're going to be using texture0. OpenGL imposes a limit on the number of
        texture we can manipulate at the same time. That limit depends on the device. We don't care as we'll just
        always use texture0. *
    activeTexture ::context RGLConstants.texture0;

    /** Bind `texture` to `texture_2d` to modify it's magnification and minification params. **/
    bindTexture ::context target::RGLConstants.texture_2d ::texture;
    let uSampler = Gl.getUniformLocation ::context ::program name::"uSampler";*/

  /*** Load a dummy texture. This is because we're using the same shader for things with and without a texture */
  /*Gl.texImage2D_RGBA
      ::context
      target::RGLConstants.texture_2d
      level::0
      width::1
      height::1
      border::0
      data::(Gl.Bigarray.of_array Gl.Bigarray.Uint8 [|0, 0, 0, 0|]);
    Gl.texParameteri
      ::context
      target::RGLConstants.texture_2d
      pname::RGLConstants.texture_mag_filter
      param::RGLConstants.linear;
    Gl.texParameteri
      ::context
      target::RGLConstants.texture_2d
      pname::RGLConstants.texture_min_filter
      param::RGLConstants.linear_mipmap_nearest;*/

  /*** Enable blend and tell OpenGL how to blend. */
  enable(~context, RGLConstants.blend);
  blendFunc(~context, RGLConstants.src_alpha, RGLConstants.one_minus_src_alpha);

  /***
   * Will mutate the projectionMatrix to be an ortho matrix with the given boundaries.
   * See this link for quick explanation of what this is.
   * https://shearer12345.github.io/graphics/assets/projectionPerspectiveVSOrthographic.png
   */
  Mat4.ortho(
    ~out=camera.projectionMatrix,
    ~left=0.,
    ~right=float_of_int @@ Env.width(),
    ~bottom=float_of_int @@ Env.height(),
    ~top=0.,
    ~near=(-1024.),
    ~far=1024.
  );
  {
    camera,
    vertexArrayPtr: genVertexArray(~context),
    gl: context,
    batch: {
      vertexArray: Bigarray.Array1.create(Bigarray.Float32, Bigarray.c_layout, circularBufferSize * vertexSize),
      elementArray: Bigarray.Array1.create(Bigarray.Int16_unsigned, Bigarray.c_layout, circularBufferSize),
      vertexPtr: 0,
      elementPtr: 0
    },
    style: {
      fillColor: Some({r: 0, g: 0, b: 0}),
      strokeWeight: 10,
      strokeCap: Round,
      strokeColor: Some({r: 0, g: 0, b: 0})
    },
    vertexBuffer,
    elementBuffer,
    aVertexPosition,
    aVertexColor,
    pMatrixUniform
  }
};

let drawGeometry =
    (
      ~vertexArray: Bigarray.Array1.t(float, Bigarray.float32_elt, Bigarray.c_layout),
      ~elementArray: Bigarray.Array1.t(int, Bigarray.int16_unsigned_elt, Bigarray.c_layout),
      ~mode,
      ~count,
      /*::textureBuffer*/
      env
    ) => {
  /* Bind `vertexBuffer`, a pointer to chunk of memory to be sent to the GPU to the "register" called
     `array_buffer` */
  bindBuffer(~context=env.gl, ~target=gl_array_buffer, ~buffer=env.vertexBuffer);

  /*** Copy all of the data over into whatever's in `array_buffer` (so here it's `vertexBuffer`) **/
  bufferData(~context=env.gl, ~target=gl_array_buffer, ~data=vertexArray, ~usage=gl_stream_draw);

  /*** Tell the GPU about the shader attribute called `aVertexPosition` so it can access the data per vertex */
  vertexAttribPointer(
    ~context=env.gl,
    ~attribute=env.aVertexPosition,
    ~size=2,
    ~type_=gl_float,
    ~normalize=false,
    ~stride=vertexSize * 4,
    ~offset=0
  );

  /*** Same as above but for `aVertexColor` **/
  vertexAttribPointer(
    ~context=env.gl,
    ~attribute=env.aVertexColor,
    ~size=4,
    ~type_=gl_float,
    ~normalize=false,
    ~stride=vertexSize * 4,
    ~offset=2 * 4
  );

  /*** Same as above but for `aTextureCoord` **/
  /*vertexAttribPointer
    context::env.gl
    attribute::env.aTextureCoord
    size::2
    type_::gl_float
    normalize::false
    stride::(vertexSize * 4)
    offset::(6 * 4);*/

  /*** Tell OpenGL about what the uniform called `uSampler` is pointing at, here it's given 0 which is what
       texture0 represent.  **/
  /*uniform1i context::env.gl location::env.uSampler value::0;*/

  /*** Bind `elementBuffer`, a pointer to GPU memory to `element_array_buffer`. That "register" is used for
       the data representing the indices of the vertex. **/
  bindBuffer(~context=env.gl, ~target=gl_element_array_buffer, ~buffer=env.elementBuffer);

  /*** Copy the `elementArray` into whatever buffer is in `element_array_buffer` **/
  bufferData(
    ~context=env.gl,
    ~target=gl_element_array_buffer,
    ~data=elementArray,
    ~usage=gl_stream_draw
  );
  uniformMatrix4fv_glk(
    ~context=env.gl,
    ~location=env.pMatrixUniform,
    ~transpose=false,
    ~value=env.camera.projectionMatrix
  );

  /*** We bind `texture` to texture_2d, like we did for the vertex buffers in some ways (I think?) **/
  /*bindTexture
    context::env.gl target::gl_texture_2d texture::textureBuffer;*/
  /*bindVertexArray context::env.gl vertexArray::env.vertexArrayPtr;*/

  /*** Final call which actually tells the GPU to draw. **/
  drawElements(~context=env.gl, ~mode, ~count, ~type_=gl_unsigned_short, ~offset=0)
};

let flushGlobalBatch = (env) =>
  if (env.batch.elementPtr > 0) {
    /*let textureBuffer =
      switch env.batch.currTex {
      | None => env.batch.nullTex
      | Some textureBuffer => textureBuffer
      };*/
    clearColor(~context=env.gl, ~r=1., ~g=0., ~b=0., ~a=1.);
    clear(~context=env.gl, ~mask=16384 lor 256);
    drawGeometry(
      ~vertexArray=Bigarray.Array1.sub(env.batch.vertexArray, 0, env.batch.vertexPtr),
      ~elementArray=
        Bigarray.Array1.sub(env.batch.elementArray, 0, env.batch.elementPtr),
      ~mode=gl_triangles,
      ~count=env.batch.elementPtr,
      /*::textureBuffer*/
      env
    );
    /*env.batch.currTex = None;*/
    env.batch.vertexPtr = 0;
    env.batch.elementPtr = 0
  };

let toColorFloat = (i) => float_of_int(i) /. 255.;

/*
 * This array packs all of the values that the shaders need: vertices, colors and texture coordinates.
 * We put them all in one as an optimization, so there are less back and forths between us and the GPU.
 *
 * The vertex array looks like:
 *
 * |<--------  8 * 4 bytes  ------->|
 *  --------------------------------
 * |  x  y  |  r  g  b  a  |  s  t  |  x2  y2  |  r2  g2  b2  a2  |  s2  t2  | ....
 *  --------------------------------
 * |           |              |
 * +- offset: 0 bytes, stride: 8 * 4 bytes (because we need to move by 8*4 bytes to get to the next x)
 *             |              |
 *             +- offset: 3 * 4 bytes, stride: 8 * 4 bytes
 *                            |
 *                            +- offset: (3 + 4) * 4 bytes, stride: 8 * 4 bytes
 *
 *
 * The element array is just an array of indices of vertices given that each vertex takes 8 * 4 bytes.
 * For example, if the element array looks like [|0, 1, 2, 1, 2, 3|], we're telling the GPU to draw 2
 * triangles: one with the vertices 0, 1 and 2 from the vertex array, and one with the vertices 1, 2 and 3.
 * We can "point" to duplicated vertices in our geometry to avoid sending those vertices.
 */
let addRectToGlobalBatch =
    (
      env,
      ~bottomRight as (x1, y1),
      ~bottomLeft as (x2, y2),
      ~topRight as (x3, y3),
      ~topLeft as (x4, y4),
      ~color as {r, g, b}
    ) => {
  /*maybeFlushBatch texture::None el::6 vert::32 env;*/
  let setFloat32 = Bigarray.Array1.set;
  let (r, g, b) = (toColorFloat(r), toColorFloat(g), toColorFloat(b));
  let i = env.batch.vertexPtr;
  let vertexArrayToMutate = env.batch.vertexArray;
  setFloat32(vertexArrayToMutate, i + 0, x1);
  setFloat32(vertexArrayToMutate, i + 1, y1);
  setFloat32(vertexArrayToMutate, i + 2, r);
  setFloat32(vertexArrayToMutate, i + 3, g);
  setFloat32(vertexArrayToMutate, i + 4, b);
  setFloat32(vertexArrayToMutate, i + 5, 1.);
  setFloat32(vertexArrayToMutate, i + 6, 0.0);
  setFloat32(vertexArrayToMutate, i + 7, 0.0);
  setFloat32(vertexArrayToMutate, i + 8, x2);
  setFloat32(vertexArrayToMutate, i + 9, y2);
  setFloat32(vertexArrayToMutate, i + 10, r);
  setFloat32(vertexArrayToMutate, i + 11, g);
  setFloat32(vertexArrayToMutate, i + 12, b);
  setFloat32(vertexArrayToMutate, i + 13, 1.);
  setFloat32(vertexArrayToMutate, i + 14, 0.0);
  setFloat32(vertexArrayToMutate, i + 15, 0.0);
  setFloat32(vertexArrayToMutate, i + 16, x3);
  setFloat32(vertexArrayToMutate, i + 17, y3);
  setFloat32(vertexArrayToMutate, i + 18, r);
  setFloat32(vertexArrayToMutate, i + 19, g);
  setFloat32(vertexArrayToMutate, i + 20, b);
  setFloat32(vertexArrayToMutate, i + 21, 1.);
  setFloat32(vertexArrayToMutate, i + 22, 0.0);
  setFloat32(vertexArrayToMutate, i + 23, 0.0);
  setFloat32(vertexArrayToMutate, i + 24, x4);
  setFloat32(vertexArrayToMutate, i + 25, y4);
  setFloat32(vertexArrayToMutate, i + 26, r);
  setFloat32(vertexArrayToMutate, i + 27, g);
  setFloat32(vertexArrayToMutate, i + 28, b);
  setFloat32(vertexArrayToMutate, i + 29, 1.);
  setFloat32(vertexArrayToMutate, i + 30, 0.0);
  setFloat32(vertexArrayToMutate, i + 31, 0.0);
  let ii = i / vertexSize;
  let j = env.batch.elementPtr;
  let elementArrayToMutate = env.batch.elementArray;
  let setUint16 = Bigarray.Array1.set;
  setUint16(elementArrayToMutate, j + 0, ii);
  setUint16(elementArrayToMutate, j + 1, ii + 1);
  setUint16(elementArrayToMutate, j + 2, ii + 2);
  setUint16(elementArrayToMutate, j + 3, ii + 1);
  setUint16(elementArrayToMutate, j + 4, ii + 2);
  setUint16(elementArrayToMutate, j + 5, ii + 3);
  env.batch.vertexPtr = i + 4 * vertexSize;
  env.batch.elementPtr = j + 6
};

let globalRender: ref(option((glEnv => unit))) = ref(None);

let reasonglMain = (vc: gameViewControllerT) =>
  switch (makeContext(EAGLRenderingAPIOpenGLES2)) {
  | None => failwith("Unable to setup OpenGL rendering context :(")
  | Some(context) =>
    setContext(vc, context);
    setPreferredFramesPerSecond(vc, 60);
    let view = getView(vc);
    setContext(view, context);
    setDrawableDepthFormat(view, GLKViewDrawableDepthFormat24);
    let env = setupGL(vc, context);
    switch globalRender^ {
    | None => failwith("You forgot to call Reasongl.run() -- no setup function defined.")
    | Some(render) => render(env)
    }
  };

Callback.register("reasonglMain", reasonglMain);

let run = (~setup, ~draw, ()) =>
  /* Register a global render. */
  globalRender :=
    Some(
      (env) => {
        let state = ref(setup(env));
        Callback.register(
          "reasonglUpdate",
          (_) => {
            state := draw(state^, env);

            /*** Flush buffers */
            flushGlobalBatch(env)
          }
        )
      }
    );
