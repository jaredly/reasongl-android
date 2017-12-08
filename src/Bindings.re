/* type contextT; */

type gameViewControllerT;

type openGLT =
  | EAGLRenderingAPIOpenGLES1
  | EAGLRenderingAPIOpenGLES2
  | EAGLRenderingAPIOpenGLES3;

type _GLKViewDrawableDepthFormatT =
  | GLKViewDrawableDepthFormatNone
  | GLKViewDrawableDepthFormat16
  | GLKViewDrawableDepthFormat24;

/* external makeContext : openGLT => option(contextT) = "newEAGLContext"; */

type _GLKViewT;

/* [@noalloc] external setContext : ('GLKViewT, contextT) => unit = "setContext"; */

/* [@noalloc] external setPreferredFramesPerSecond : ('GLKViewT, int) => unit = "setPreferredFramesPerSecond"; */

/* external getView : gameViewControllerT => _GLKViewT = "getGLKView"; */

/* [@noalloc] external setDrawableDepthFormat : (_GLKViewT, _GLKViewDrawableDepthFormatT) => unit = "setDrawableDepthFormat"; */

/*external isNil : 'a => bool = "isNil";*/

/* [@noalloc] external setCurrentContext : contextT => unit = "setCurrentContext"; */

