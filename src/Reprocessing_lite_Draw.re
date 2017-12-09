open Reprocessing_lite;

open Tgls;

module Utils = Reprocessing_lite_Utils;

let drawArc =
    (
      env,
      (xCenterOfCircle: float, yCenterOfCircle: float),
      radx: float,
      rady: float,
      start: float,
      stop: float,
      isPie: bool,
      {r, g, b}
    ) => {
  let noOfFans = int_of_float(radx +. rady) / 4 + 10;
  /*maybeFlushBatch texture::None vert::(8 * noOfFans) el::(3 * noOfFans) env;*/
  let pi = 4.0 *. atan(1.0);
  let anglePerFan = 2. *. pi /. float_of_int(noOfFans);
  let (r, g, b) = (toColorFloat(r), toColorFloat(g), toColorFloat(b));
  /*print_endline @@ "r " ^ (string_of_float r) ^ "g " ^ (string_of_float g) ^ "b " ^ string_of_float b;*/
  let verticesData = env.batch.vertexArray;
  let elementData = env.batch.elementArray;
  let setFloat32 = Bigarray.Array1.set;
  let setUint16 = Bigarray.Array1.set;
  let getUint16 = Bigarray.Array1.set;
  let vertexArrayOffset = env.batch.vertexPtr;
  let elementArrayOffset = env.batch.elementPtr;
  let start_i =
    if (isPie) {
      /* Start one earlier and force the first point to be the center */
      int_of_float(start /. anglePerFan)
      - 2
    } else {
      int_of_float(start /. anglePerFan) - 1
    };
  let stop_i = int_of_float(stop /. anglePerFan) - 1;
  for (i in start_i to stop_i) {
    let (xCoordinate, yCoordinate) =
      if (isPie && i - start_i == 0) {
        (
          /* force the first point to be the center */
          xCenterOfCircle,
          yCenterOfCircle
        )
      } else {
        let angle = anglePerFan *. float_of_int(i + 1);
        (xCenterOfCircle +. cos(angle) *. radx, yCenterOfCircle +. sin(angle) *. rady)
      };
    let ii = (i - start_i) * vertexSize + vertexArrayOffset;
    setFloat32(verticesData, ii + 0, xCoordinate);
    setFloat32(verticesData, ii + 1, yCoordinate);
    setFloat32(verticesData, ii + 2, r);
    setFloat32(verticesData, ii + 3, g);
    setFloat32(verticesData, ii + 4, b);
    setFloat32(verticesData, ii + 5, 1.0);
    setFloat32(verticesData, ii + 6, 0.0);
    setFloat32(verticesData, ii + 7, 0.0);
    /* For the first three vertices, we don't do any deduping. Then for the subsequent ones, we'll actually
       have 3 elements, one pointing at the first vertex, one pointing at the previously added vertex and one
       pointing at the current vertex. This mimicks the behavior of triangle_fan. */
    if (i - start_i < 3) {
      setUint16(elementData, i - start_i + elementArrayOffset, ii / vertexSize)
    } else {
      /* We've already added 3 elements, for i = 0, 1 and 2. From now on, we'll add 3 elements _per_ i.
         To calculate the correct offset in `elementData` we remove 3 from i as if we're starting from 0 (the
         first time we enter this loop i = 3), then for each i we'll add 3 elements (so multiply by 3) BUT for
         i = 3 we want `jj` to be 3 so we shift everything by 3 (so add 3). Everything's also shifted by
         `elementArrayOffset` */
      let jj = (i - start_i - 3) * 3 + elementArrayOffset + 3;
      setUint16(elementData, jj, vertexArrayOffset / vertexSize);
      setUint16(elementData, jj + 1, Bigarray.Array1.get(elementData, jj - 1));
      setUint16(elementData, jj + 2, ii / vertexSize)
    }
  };
  env.batch.vertexPtr = env.batch.vertexPtr + noOfFans * vertexSize;
  env.batch.elementPtr = env.batch.elementPtr + (stop_i - start_i - 3) * 3 + 3
};

let fill = (color, env: glEnv) => env.style = {...env.style, fillColor: Some(color)};

let background = (color, env: glEnv) => {
  clear(~context=env.gl, ~mask=16384 lor 256);
  let w = float_of_int(Env.width(env));
  let h = float_of_int(Env.height(env));
  addRectToGlobalBatch(
    env,
    ~bottomRight=(w, h),
    ~bottomLeft=(0., h),
    ~topRight=(w, 0.),
    ~topLeft=(0., 0.),
    ~color
  )
};

let drawEllipse = (env, center, radx: float, rady: float, c) =>
  drawArc(env, center, radx, rady, 0., Utils.tau, false, c);

let ellipsef = (~center, ~radx, ~rady, env: glEnv) =>
  switch env.style.fillColor {
  | None => () /* Don't draw fill */
  | Some(fill) => drawEllipse(env, center, radx, rady, fill)
  /*switch env.style.strokeColor {
    | None => () /* Don't draw stroke */
    | Some stroke =>
      drawArcStroke
        env
        center
        radx
        rady
        0.
        Utils.tau
        false
        false
        env.matrix
        stroke
        env.style.strokeWeight
    }*/
  };

let ellipse = (~center as (cx, cy), ~radx, ~rady, env: glEnv) =>
  ellipsef(
    ~center=(float_of_int(cx), float_of_int(cy)),
    ~radx=float_of_int(radx),
    ~rady=float_of_int(rady),
    env
  );