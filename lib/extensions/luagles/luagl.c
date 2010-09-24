/*************************************************
*  LuaGL - an OpenGL binding for Lua
*  2003-2004(c) Fabio Guerra, Cleyde Marlyse
*  http://luagl.sourceforge.net
*-------------------------------------------------
*  Description: This file implements the OpenGL
*               binding for Lua 5
*-------------------------------------------------
* Mantained by Antonio Scuri since 2009
*-------------------------------------------------
*  See Copyright Notice in LuaGL.h
*************************************************/

/*************************************************
 * Tailored for OpenGL ES 1.0 and iPhone
 * 2010(c) Coconut Island Studio 
 * Author: Ye Feng 
 *************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <OpenGLES/ES1/gl.h>

#include "lua.h"
#include "lauxlib.h"

#include "luagl.h"
#include "luagl_util.h"


#define LUAGL_VERSION "1.5"


static const luaglConst luagl_const[] = {
  { "ADD"                             , GL_ADD                            },
  { "NEVER"                           , GL_NEVER                          },
  { "LESS"                            , GL_LESS                           },
  { "EQUAL"                           , GL_EQUAL                          },
  { "LEQUAL"                          , GL_LEQUAL                         },
  { "GREATER"                         , GL_GREATER                        },
  { "NOTEQUAL"                        , GL_NOTEQUAL                       },
  { "GEQUAL"                          , GL_GEQUAL                         },
  { "ALWAYS"                          , GL_ALWAYS                         },
  { "POINTS"                          , GL_POINTS                         },
  { "LINES"                           , GL_LINES                          },
  { "LINE_LOOP"                       , GL_LINE_LOOP                      },
  { "LINE_STRIP"                      , GL_LINE_STRIP                     },
  { "TRIANGLES"                       , GL_TRIANGLES                      },
  { "TRIANGLE_STRIP"                  , GL_TRIANGLE_STRIP                 },
  { "TRIANGLE_FAN"                    , GL_TRIANGLE_FAN                   },
  { "ZERO"                            , GL_ZERO                           },
  { "ONE"                             , GL_ONE                            },
  { "SRC_COLOR"                       , GL_SRC_COLOR                      },
  { "ONE_MINUS_SRC_COLOR"             , GL_ONE_MINUS_SRC_COLOR            },
  { "SRC_ALPHA"                       , GL_SRC_ALPHA                      },
  { "ONE_MINUS_SRC_ALPHA"             , GL_ONE_MINUS_SRC_ALPHA            },
  { "DST_ALPHA"                       , GL_DST_ALPHA                      },
  { "ONE_MINUS_DST_ALPHA"             , GL_ONE_MINUS_DST_ALPHA            },
  { "DST_COLOR"                       , GL_DST_COLOR                      },
  { "ONE_MINUS_DST_COLOR"             , GL_ONE_MINUS_DST_COLOR            },
  { "SRC_ALPHA_SATURATE"              , GL_SRC_ALPHA_SATURATE             },
  { "TRUE"                            , GL_TRUE                           },
  { "FALSE"                           , GL_FALSE                          },
  { "CLIP_PLANE0"                     , GL_CLIP_PLANE0                    },
  { "CLIP_PLANE1"                     , GL_CLIP_PLANE1                    },
  { "CLIP_PLANE2"                     , GL_CLIP_PLANE2                    },
  { "CLIP_PLANE3"                     , GL_CLIP_PLANE3                    },
  { "CLIP_PLANE4"                     , GL_CLIP_PLANE4                    },
  { "CLIP_PLANE5"                     , GL_CLIP_PLANE5                    },
  { "BYTE"                            , GL_BYTE                           },
  { "UNSIGNED_BYTE"                   , GL_UNSIGNED_BYTE                  },
  { "SHORT"                           , GL_SHORT                          },
  { "UNSIGNED_SHORT"                  , GL_UNSIGNED_SHORT                 },
  { "FLOAT"                           , GL_FLOAT                          },
  { "FRONT"                           , GL_FRONT                          },
  { "BACK"                            , GL_BACK                           },
  { "FRONT_AND_BACK"                  , GL_FRONT_AND_BACK                 },
  { "NO_ERROR"                        , GL_NO_ERROR                       },
  { "INVALID_ENUM"                    , GL_INVALID_ENUM                   },
  { "INVALID_VALUE"                   , GL_INVALID_VALUE                  },
  { "INVALID_OPERATION"               , GL_INVALID_OPERATION              },
  { "STACK_OVERFLOW"                  , GL_STACK_OVERFLOW                 },
  { "STACK_UNDERFLOW"                 , GL_STACK_UNDERFLOW                },
  { "OUT_OF_MEMORY"                   , GL_OUT_OF_MEMORY                  },
  { "EXP"                             , GL_EXP                            },
  { "EXP2"                            , GL_EXP2                           },
  { "CW"                              , GL_CW                             },
  { "CCW"                             , GL_CCW                            },
  { "CURRENT_COLOR"                   , GL_CURRENT_COLOR                  },
  { "CURRENT_NORMAL"                  , GL_CURRENT_NORMAL                 },
  { "CURRENT_TEXTURE_COORDS"          , GL_CURRENT_TEXTURE_COORDS         },
  { "POINT_SMOOTH"                    , GL_POINT_SMOOTH                   },
  { "POINT_SIZE"                      , GL_POINT_SIZE                     },
  { "LINE_SMOOTH"                     , GL_LINE_SMOOTH                    },
  { "LINE_WIDTH"                      , GL_LINE_WIDTH                     },
  { "CULL_FACE"                       , GL_CULL_FACE                      },
  { "CULL_FACE_MODE"                  , GL_CULL_FACE_MODE                 },
  { "FRONT_FACE"                      , GL_FRONT_FACE                     },
  { "LIGHTING"                        , GL_LIGHTING                       },
  { "LIGHT_MODEL_TWO_SIDE"            , GL_LIGHT_MODEL_TWO_SIDE           },
  { "LIGHT_MODEL_AMBIENT"             , GL_LIGHT_MODEL_AMBIENT            },
  { "SHADE_MODEL"                     , GL_SHADE_MODEL                    },
  { "COLOR_MATERIAL"                  , GL_COLOR_MATERIAL                 },
  { "FOG"                             , GL_FOG                            },
  { "FOG_DENSITY"                     , GL_FOG_DENSITY                    },
  { "FOG_START"                       , GL_FOG_START                      },
  { "FOG_END"                         , GL_FOG_END                        },
  { "FOG_MODE"                        , GL_FOG_MODE                       },
  { "FOG_COLOR"                       , GL_FOG_COLOR                      },
  { "DEPTH_RANGE"                     , GL_DEPTH_RANGE                    },
  { "DEPTH_TEST"                      , GL_DEPTH_TEST                     },
  { "DEPTH_WRITEMASK"                 , GL_DEPTH_WRITEMASK                },
  { "DEPTH_CLEAR_VALUE"               , GL_DEPTH_CLEAR_VALUE              },
  { "DEPTH_FUNC"                      , GL_DEPTH_FUNC                     },
  { "STENCIL_TEST"                    , GL_STENCIL_TEST                   },
  { "STENCIL_CLEAR_VALUE"             , GL_STENCIL_CLEAR_VALUE            },
  { "STENCIL_FUNC"                    , GL_STENCIL_FUNC                   },
  { "STENCIL_VALUE_MASK"              , GL_STENCIL_VALUE_MASK             },
  { "STENCIL_FAIL"                    , GL_STENCIL_FAIL                   },
  { "STENCIL_PASS_DEPTH_FAIL"         , GL_STENCIL_PASS_DEPTH_FAIL        },
  { "STENCIL_PASS_DEPTH_PASS"         , GL_STENCIL_PASS_DEPTH_PASS        },
  { "STENCIL_REF"                     , GL_STENCIL_REF                    },
  { "STENCIL_WRITEMASK"               , GL_STENCIL_WRITEMASK              },
  { "MATRIX_MODE"                     , GL_MATRIX_MODE                    },
  { "NORMALIZE"                       , GL_NORMALIZE                      },
  { "VIEWPORT"                        , GL_VIEWPORT                       },
  { "MODELVIEW_STACK_DEPTH"           , GL_MODELVIEW_STACK_DEPTH          },
  { "PROJECTION_STACK_DEPTH"          , GL_PROJECTION_STACK_DEPTH         },
  { "TEXTURE_STACK_DEPTH"             , GL_TEXTURE_STACK_DEPTH            },
  { "MODELVIEW_MATRIX"                , GL_MODELVIEW_MATRIX               },
  { "PROJECTION_MATRIX"               , GL_PROJECTION_MATRIX              },
  { "TEXTURE_MATRIX"                  , GL_TEXTURE_MATRIX                 },
  { "ALPHA_TEST"                      , GL_ALPHA_TEST                     },
  { "ALPHA_TEST_FUNC"                 , GL_ALPHA_TEST_FUNC                },
  { "ALPHA_TEST_REF"                  , GL_ALPHA_TEST_REF                 },
  { "DITHER"                          , GL_DITHER                         },
  { "BLEND_DST"                       , GL_BLEND_DST                      },
  { "BLEND_SRC"                       , GL_BLEND_SRC                      },
  { "BLEND"                           , GL_BLEND                          },
  { "LOGIC_OP_MODE"                   , GL_LOGIC_OP_MODE                  },
  { "COLOR_LOGIC_OP"                  , GL_COLOR_LOGIC_OP                 },
  { "SCISSOR_BOX"                     , GL_SCISSOR_BOX                    },
  { "SCISSOR_TEST"                    , GL_SCISSOR_TEST                   },
  { "COLOR_CLEAR_VALUE"               , GL_COLOR_CLEAR_VALUE              },
  { "COLOR_WRITEMASK"                 , GL_COLOR_WRITEMASK                },
  { "PERSPECTIVE_CORRECTION_HINT"     , GL_PERSPECTIVE_CORRECTION_HINT    },
  { "POINT_SMOOTH_HINT"               , GL_POINT_SMOOTH_HINT              },
  { "LINE_SMOOTH_HINT"                , GL_LINE_SMOOTH_HINT               },
  { "FOG_HINT"                        , GL_FOG_HINT                       },
  { "UNPACK_ALIGNMENT"                , GL_UNPACK_ALIGNMENT               },
  { "PACK_ALIGNMENT"                  , GL_PACK_ALIGNMENT                 },
  { "ALPHA_SCALE"                     , GL_ALPHA_SCALE                    },
  { "MAX_LIGHTS"                      , GL_MAX_LIGHTS                     },
  { "MAX_CLIP_PLANES"                 , GL_MAX_CLIP_PLANES                },
  { "MAX_TEXTURE_SIZE"                , GL_MAX_TEXTURE_SIZE               },
  { "MAX_MODELVIEW_STACK_DEPTH"       , GL_MAX_MODELVIEW_STACK_DEPTH      },
  { "MAX_PROJECTION_STACK_DEPTH"      , GL_MAX_PROJECTION_STACK_DEPTH     },
  { "MAX_TEXTURE_STACK_DEPTH"         , GL_MAX_TEXTURE_STACK_DEPTH        },
  { "MAX_VIEWPORT_DIMS"               , GL_MAX_VIEWPORT_DIMS              },
  { "SUBPIXEL_BITS"                   , GL_SUBPIXEL_BITS                  },
  { "RED_BITS"                        , GL_RED_BITS                       },
  { "GREEN_BITS"                      , GL_GREEN_BITS                     },
  { "BLUE_BITS"                       , GL_BLUE_BITS                      },
  { "ALPHA_BITS"                      , GL_ALPHA_BITS                     },
  { "DEPTH_BITS"                      , GL_DEPTH_BITS                     },
  { "STENCIL_BITS"                    , GL_STENCIL_BITS                   },
  { "TEXTURE_2D"                      , GL_TEXTURE_2D                     },
  { "DONT_CARE"                       , GL_DONT_CARE                      },
  { "FASTEST"                         , GL_FASTEST                        },
  { "NICEST"                          , GL_NICEST                         },
  { "LIGHT0"                          , GL_LIGHT0                         },
  { "LIGHT1"                          , GL_LIGHT1                         },
  { "LIGHT2"                          , GL_LIGHT2                         },
  { "LIGHT3"                          , GL_LIGHT3                         },
  { "LIGHT4"                          , GL_LIGHT4                         },
  { "LIGHT5"                          , GL_LIGHT5                         },
  { "LIGHT6"                          , GL_LIGHT6                         },
  { "LIGHT7"                          , GL_LIGHT7                         },
  { "AMBIENT"                         , GL_AMBIENT                        },
  { "DIFFUSE"                         , GL_DIFFUSE                        },
  { "SPECULAR"                        , GL_SPECULAR                       },
  { "POSITION"                        , GL_POSITION                       },
  { "SPOT_DIRECTION"                  , GL_SPOT_DIRECTION                 },
  { "SPOT_EXPONENT"                   , GL_SPOT_EXPONENT                  },
  { "SPOT_CUTOFF"                     , GL_SPOT_CUTOFF                    },
  { "CONSTANT_ATTENUATION"            , GL_CONSTANT_ATTENUATION           },
  { "LINEAR_ATTENUATION"              , GL_LINEAR_ATTENUATION             },
  { "QUADRATIC_ATTENUATION"           , GL_QUADRATIC_ATTENUATION          },
  { "CLEAR"                           , GL_CLEAR                          },
  { "AND"                             , GL_AND                            },
  { "AND_REVERSE"                     , GL_AND_REVERSE                    },
  { "COPY"                            , GL_COPY                           },
  { "AND_INVERTED"                    , GL_AND_INVERTED                   },
  { "NOOP"                            , GL_NOOP                           },
  { "XOR"                             , GL_XOR                            },
  { "OR"                              , GL_OR                             },
  { "NOR"                             , GL_NOR                            },
  { "EQUIV"                           , GL_EQUIV                          },
  { "INVERT"                          , GL_INVERT                         },
  { "OR_REVERSE"                      , GL_OR_REVERSE                     },
  { "COPY_INVERTED"                   , GL_COPY_INVERTED                  },
  { "OR_INVERTED"                     , GL_OR_INVERTED                    },
  { "NAND"                            , GL_NAND                           },
  { "SET"                             , GL_SET                            },
  { "EMISSION"                        , GL_EMISSION                       },
  { "SHININESS"                       , GL_SHININESS                      },
  { "AMBIENT_AND_DIFFUSE"             , GL_AMBIENT_AND_DIFFUSE            },
  { "MODELVIEW"                       , GL_MODELVIEW                      },
  { "PROJECTION"                      , GL_PROJECTION                     },
  { "TEXTURE"                         , GL_TEXTURE                        },
  { "ALPHA"                           , GL_ALPHA                          },
  { "RGB"                             , GL_RGB                            },
  { "RGBA"                            , GL_RGBA                           },
  { "LUMINANCE"                       , GL_LUMINANCE                      },
  { "LUMINANCE_ALPHA"                 , GL_LUMINANCE_ALPHA                },
  { "FLAT"                            , GL_FLAT                           },
  { "SMOOTH"                          , GL_SMOOTH                         },
  { "KEEP"                            , GL_KEEP                           },
  { "REPLACE"                         , GL_REPLACE                        },
  { "INCR"                            , GL_INCR                           },
  { "DECR"                            , GL_DECR                           },
  { "VENDOR"                          , GL_VENDOR                         },
  { "RENDERER"                        , GL_RENDERER                       },
  { "VERSION"                         , GL_VERSION                        },
  { "EXTENSIONS"                      , GL_EXTENSIONS                     },
  { "MODULATE"                        , GL_MODULATE                       },
  { "DECAL"                           , GL_DECAL                          },
  { "TEXTURE_ENV_MODE"                , GL_TEXTURE_ENV_MODE               },
  { "TEXTURE_ENV_COLOR"               , GL_TEXTURE_ENV_COLOR              },
  { "TEXTURE_ENV"                     , GL_TEXTURE_ENV                    },
  { "NEAREST"                         , GL_NEAREST                        },
  { "LINEAR"                          , GL_LINEAR                         },
  { "NEAREST_MIPMAP_NEAREST"          , GL_NEAREST_MIPMAP_NEAREST         },
  { "LINEAR_MIPMAP_NEAREST"           , GL_LINEAR_MIPMAP_NEAREST          },
  { "NEAREST_MIPMAP_LINEAR"           , GL_NEAREST_MIPMAP_LINEAR          },
  { "LINEAR_MIPMAP_LINEAR"            , GL_LINEAR_MIPMAP_LINEAR           },
  { "TEXTURE_MAG_FILTER"              , GL_TEXTURE_MAG_FILTER             },
  { "TEXTURE_MIN_FILTER"              , GL_TEXTURE_MIN_FILTER             },
  { "TEXTURE_WRAP_S"                  , GL_TEXTURE_WRAP_S                 },
  { "TEXTURE_WRAP_T"                  , GL_TEXTURE_WRAP_T                 },
  { "REPEAT"                          , GL_REPEAT                         },
  { "CLAMP_TO_EDGE"                   , GL_CLAMP_TO_EDGE                  },
  { "POLYGON_OFFSET_FACTOR"           , GL_POLYGON_OFFSET_FACTOR          },
  { "POLYGON_OFFSET_UNITS"            , GL_POLYGON_OFFSET_UNITS           },
  { "POLYGON_OFFSET_FILL"             , GL_POLYGON_OFFSET_FILL            },
  { "TEXTURE_BINDING_2D"              , GL_TEXTURE_BINDING_2D             },
  { "VERTEX_ARRAY"                    , GL_VERTEX_ARRAY                   },
  { "NORMAL_ARRAY"                    , GL_NORMAL_ARRAY                   },
  { "COLOR_ARRAY"                     , GL_COLOR_ARRAY                    },
  { "TEXTURE_COORD_ARRAY"             , GL_TEXTURE_COORD_ARRAY            },
  { "VERTEX_ARRAY_SIZE"               , GL_VERTEX_ARRAY_SIZE              },
  { "VERTEX_ARRAY_TYPE"               , GL_VERTEX_ARRAY_TYPE              },
  { "VERTEX_ARRAY_STRIDE"             , GL_VERTEX_ARRAY_STRIDE            },
  { "NORMAL_ARRAY_TYPE"               , GL_NORMAL_ARRAY_TYPE              },
  { "NORMAL_ARRAY_STRIDE"             , GL_NORMAL_ARRAY_STRIDE            },
  { "COLOR_ARRAY_SIZE"                , GL_COLOR_ARRAY_SIZE               },
  { "COLOR_ARRAY_TYPE"                , GL_COLOR_ARRAY_TYPE               },
  { "COLOR_ARRAY_STRIDE"              , GL_COLOR_ARRAY_STRIDE             },
  { "TEXTURE_COORD_ARRAY_SIZE"        , GL_TEXTURE_COORD_ARRAY_SIZE       },
  { "TEXTURE_COORD_ARRAY_TYPE"        , GL_TEXTURE_COORD_ARRAY_TYPE       },
  { "TEXTURE_COORD_ARRAY_STRIDE"      , GL_TEXTURE_COORD_ARRAY_STRIDE     },
  { "VERTEX_ARRAY_POINTER"            , GL_VERTEX_ARRAY_POINTER           },
  { "NORMAL_ARRAY_POINTER"            , GL_NORMAL_ARRAY_POINTER           },
  { "COLOR_ARRAY_POINTER"             , GL_COLOR_ARRAY_POINTER            },
  { "TEXTURE_COORD_ARRAY_POINTER"     , GL_TEXTURE_COORD_ARRAY_POINTER    },
  { "DEPTH_BUFFER_BIT"                , GL_DEPTH_BUFFER_BIT               },
  { "STENCIL_BUFFER_BIT"              , GL_STENCIL_BUFFER_BIT             },
  { "COLOR_BUFFER_BIT"                , GL_COLOR_BUFFER_BIT               },
  { 0, 0}
};

int luagl_checkboolean (lua_State *L, int narg) 
{
  int d = lua_toboolean(L, narg);
  if (d == 0 && !lua_isboolean(L, narg))  /* avoid extra test when d is not 0 */
    luaL_typerror(L, narg, lua_typename(L, LUA_TBOOLEAN));
  return d;
}

unsigned int luagl_get_gl_enum(lua_State *L, int index)
{
  return luagl_get_enum(L, index, luagl_const);
}

static const char *luagl_get_str_gl_enum(GLenum num)
{
  unsigned int i = 0;

  /* works only for simple enums */
  while(luagl_const[i].str != 0)
  {
    if(num == luagl_const[i].value)
      return luagl_const[i].str;

    i++;
  }
  return NULL;
}

static void luagl_pushenum(lua_State *L, GLenum num)
{
  const char* str = luagl_get_str_gl_enum(num);
  if (str)
    lua_pushstring(L, str);
  else
    lua_pushinteger(L, num);
}

/*AlphaFunc (func, ref) -> none*/
static int luagl_alpha_func(lua_State *L)
{
  glAlphaFunc(luagl_get_gl_enum(L, 1), (GLclampf)luaL_checknumber(L, 2));
  return 0;
}

/*BindTexture (target, texture) -> none*/
static int luagl_bind_texture(lua_State *L)
{
  glBindTexture(luagl_get_gl_enum(L, 1), luaL_checkinteger(L, 2));
  return 0;
}

/*BlendFunc (sfactor, dfactor) -> none*/
static int luagl_blend_func(lua_State *L)
{
  glBlendFunc(luagl_get_gl_enum(L, 1), luagl_get_gl_enum(L, 2));
  return 0;
}

/*Clear (mask) -> none*/
static int luagl_clear(lua_State *L)
{
  glClear(luagl_get_gl_enum(L, 1));
  return 0;
}

/*ClearColor (red, green, blue, alpha) -> none*/
static int luagl_clear_color(lua_State *L)
{
  glClearColor((GLclampf)luaL_checknumber(L, 1), (GLclampf)luaL_checknumber(L, 2),
               (GLclampf)luaL_checknumber(L, 3), (GLclampf)luaL_checknumber(L, 4));
  return 0;
}

/*ClearDepth (depth) -> none*/
static int luagl_clear_depth(lua_State *L)
{
  glClearDepthf((GLclampf)luaL_checknumber(L, 1));
  return 0;
}

/*ClearStencil (s) -> none*/
static int luagl_clear_stencil(lua_State *L)
{
  glClearStencil(luaL_checkinteger(L, 1));
  return 0;
}

/*ClipPlane (plane, equationArray) -> none*/
static int luagl_clip_plane(lua_State *L)
{
  GLfloat *equation;

  luagl_get_arrayf(L, 2, &equation);

  glClipPlanef(luagl_get_gl_enum(L, 1), equation);

  LUAGL_DELETE_ARRAY(equation);
  return 0;
}

/*Color (red, green, blue [, alpha]) -> none */
static int luagl_color(lua_State *L)
{
    switch(lua_gettop(L))
    {
    case 3:  glColor4f(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), 1.0f);
      break;
    case 4:  glColor4f(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));
      break;
    }

  return 0;
}

/*ColorMask (red, green, blue, alpha) -> none*/
static int luagl_color_mask(lua_State *L)
{
  glColorMask((GLboolean)luagl_checkboolean(L, 1), (GLboolean)luagl_checkboolean(L, 2),
              (GLboolean)luagl_checkboolean(L, 3), (GLboolean)luagl_checkboolean(L, 4));
  return 0;
}

/*ColorPointer (colorArray) -> none*/
static int luagl_color_pointer(lua_State *L)
{
  GLint size;
  static GLfloat *parray = 0;

  LUAGL_DELETE_ARRAY(parray);

  if (lua_isnil(L,1))
    return 0;

  if (lua_isnumber(L, 2))
  {
    size = luaL_checkinteger(L, 2);
    luagl_get_arrayf_pointer(L, 1, &parray);
  }
  else 
    luagl_get_array2f_pointer(L, 1, &parray, &size);

  glColorPointer(size, GL_FLOAT, 0, parray);
  return 0;
}

/*CopyTexImage (level, internalFormat, border, x, y, width[, height]) -> none*/
static int luagl_copy_tex_image(lua_State *L)
{
  int num_args = lua_gettop(L);
  if (num_args > 6)
  {
    glCopyTexImage2D(GL_TEXTURE_2D, luaL_checkinteger(L, 1), luagl_get_gl_enum(L, 2),
      luaL_checkinteger(L, 4), luaL_checkinteger(L, 5),
      luaL_checkinteger(L, 6), luaL_checkinteger(L, 7),
      luaL_checkinteger(L, 3));
  }
  else
  {
    glCopyTexImage2D(GL_TEXTURE_2D, luaL_checkinteger(L, 1), luagl_get_gl_enum(L, 2),
      luaL_checkinteger(L, 4), luaL_checkinteger(L, 5),
      luaL_checkinteger(L, 6), 1,
      luaL_checkinteger(L, 3));
  }
  return 0;
}

/*CopyTexSubImage (level, x, y, xoffset, width[, yoffset, height]) -> none*/
static int luagl_copy_tex_sub_image(lua_State *L)
{
  int num_args = lua_gettop(L);
  if (num_args >= 7)
  {
    glCopyTexSubImage2D(GL_TEXTURE_2D,
      luaL_checkinteger(L, 1), luaL_checkinteger(L, 4),
      luaL_checkinteger(L, 6), luaL_checkinteger(L, 2),
      luaL_checkinteger(L, 3), luaL_checkinteger(L, 5),
      luaL_checkinteger(L, 7));
  }
  else
  {
	glCopyTexSubImage2D(GL_TEXTURE_2D,
      luaL_checkinteger(L, 1), luaL_checkinteger(L, 4),
      1, luaL_checkinteger(L, 2),
      luaL_checkinteger(L, 3), luaL_checkinteger(L, 5),
      1);
  }
  return 0;
}

/*CullFace (mode) -> none*/
static int luagl_cull_face(lua_State *L)
{
  glCullFace(luagl_get_gl_enum(L, 1));
  return 0;
}

/*DeleteTextures (texturesArray) -> none*/
static int luagl_delete_textures(lua_State *L)
{
  int n;
  GLuint *textures;

  n = luagl_get_arrayui(L, 1, &textures);

  glDeleteTextures((GLsizei)n, (GLuint *)textures);

  LUAGL_DELETE_ARRAY(textures);

  return 0;
}

/*DepthFunc (func) -> none*/
static int luagl_depth_func(lua_State *L)
{
  glDepthFunc(luagl_get_gl_enum(L, 1));
  return 0;
}

/*DepthMask (flag) -> none*/
static int luagl_depth_mask(lua_State *L)
{
  glDepthMask((GLboolean)luagl_checkboolean(L, 1));
  return 0;
}

/*DepthRange (zNear, zFar) -> none*/
static int luagl_depth_range(lua_State *L)
{
  glDepthRangef((GLclampf)luaL_checknumber(L, 1), (GLclampf)luaL_checknumber(L, 2));
  return 0;
}

/*Disable (cap) -> none*/
static int luagl_disable(lua_State *L)
{
  glDisable(luagl_get_gl_enum(L, 1));
  return 0;
}

/*DisableClientState (parray) -> none*/
static int luagl_disable_client_state(lua_State *L)
{
  glDisableClientState(luagl_get_gl_enum(L, 1));
  return 0;
}

/*DrawArrays (mode, first, count) -> none*/
static int luagl_draw_arrays(lua_State *L)
{
  glDrawArrays(luagl_get_gl_enum(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
  return 0;
}

/*DrawElements (mode, indicesArray) -> none*/
static int luagl_draw_elements(lua_State *L)
{
  int n;
  GLuint *indices;

  n = luagl_get_arrayui(L, 2, &indices);

  glDrawElements(luagl_get_gl_enum(L, 1), n, GL_UNSIGNED_SHORT, indices);

  LUAGL_DELETE_ARRAY(indices);
  return 0;
}

/*Enable (cap) -> none*/
static int luagl_enable(lua_State *L)
{
  glEnable(luagl_get_gl_enum(L, 1));
  return 0;
}

/*EnableClientState (parray) -> none*/
static int luagl_enable_client_state(lua_State *L)
{
  glEnableClientState(luagl_get_gl_enum(L, 1));
  return 0;
}

/*Finish () -> none*/
static int luagl_finish(lua_State *L)
{
  glFinish();
  return 0;
}

/*Flush () -> none*/
static int luagl_flush(lua_State *L)
{
  glFlush();
  return 0;
}

/*Fog (pname, param) -> none
  Fog (pname, paramsArray) -> none*/
static int luagl_fog(lua_State *L)
{
  GLfloat *param;

  if(lua_istable(L, 2))
  {
    luagl_get_arrayf(L, 2, &param);

    glFogfv(luagl_get_gl_enum(L, 1), (GLfloat*)param);

    LUAGL_DELETE_ARRAY(param);
    return 0;
  }
  else
  {
    glFogf(luagl_get_gl_enum(L, 1), (GLfloat)luaL_checknumber(L, 2));
  }

  return 0;
}

/*FrontFace (mode) -> none*/
static int luagl_front_face(lua_State *L)
{
  glFrontFace(luagl_get_gl_enum(L, 1));
  return 0;
}

/*Frustum (left, right, bottom, top, zNear, zFar) -> none*/
static int luagl_frustum(lua_State *L)
{
  glFrustumf(luaL_checknumber(L, 1), luaL_checknumber(L, 2),
            luaL_checknumber(L, 3), luaL_checknumber(L, 4),
            luaL_checknumber(L, 5), luaL_checknumber(L, 6));
  return 0;
}

/*GenTextures (n) -> texturesArray*/
static int luagl_gen_textures(lua_State *L)
{
  GLsizei size;
  GLuint *textures;

  size = luaL_checkinteger(L, 1);
  textures = LUAGL_NEW_ARRAY(GLuint, size);

  glGenTextures(size, textures);

  luagl_push_arrayui(L, textures, size);

  LUAGL_DELETE_ARRAY(textures);

  return 1;
}

/*Get (pname) -> params*/
static int luagl_get(lua_State *L)
{
  int i, size=1;
  GLenum e;
  GLfloat *params;
  int mask;

  e = luagl_get_gl_enum(L, 1);

  switch(e)
  {
  case GL_STENCIL_VALUE_MASK:
  case GL_STENCIL_WRITEMASK:
    mask = 0;
    glGetIntegerv(e, &mask);
    lua_pushstring(L, luagl_mask2str(mask));
    return 1;
  case GL_FRONT_FACE: 
    /* looks like there's still a lot of glGet* not implemented, including 
       GL_FRONT_FACE
    */
    glGetIntegerv(e, &mask);
    lua_pushinteger(L, mask);
    return 1;

  case GL_DEPTH_RANGE:
  case GL_MAX_VIEWPORT_DIMS:
    size = 2;
    break;

  case GL_CURRENT_NORMAL:
    size = 3;
    break;

  case GL_COLOR_CLEAR_VALUE:
  case GL_COLOR_WRITEMASK:
  case GL_CURRENT_COLOR:
  case GL_CURRENT_TEXTURE_COORDS:
  case GL_FOG_COLOR:
  case GL_LIGHT_MODEL_AMBIENT:
  case GL_SCISSOR_BOX:
  case GL_TEXTURE_ENV_COLOR:
  case GL_VIEWPORT:
    size = 4;
    break;

  case GL_MODELVIEW_MATRIX:
  case GL_PROJECTION_MATRIX:
  case GL_TEXTURE_MATRIX:
    size = 16;
    break;

  default:
    luaL_argerror(L, 1, "unknown enumeration.");
    break;
  }

  params = LUAGL_NEW_ARRAY(GLfloat, size);

  glGetFloatv(e, params);

  for(i = 0; i < size; i++)
    lua_pushnumber(L, params[i]);

  LUAGL_DELETE_ARRAY(params);

  return size;
}

/*GetConst (pname) -> constant string*/
static int luagl_get_const(lua_State *L)
{
  int i, size=1;
  GLenum e;
  GLenum *params;

  e = luagl_get_gl_enum(L, 1);

  switch(e)
  {
  case GL_DEPTH_RANGE:
  case GL_MAX_VIEWPORT_DIMS:
    size = 2;
    break;

  case GL_CURRENT_NORMAL:
    size = 3;
    break;

  case GL_COLOR_CLEAR_VALUE:
  case GL_COLOR_WRITEMASK:
  case GL_CURRENT_COLOR:
  case GL_CURRENT_TEXTURE_COORDS:
  case GL_FOG_COLOR:
  case GL_LIGHT_MODEL_AMBIENT:
  case GL_SCISSOR_BOX:
  case GL_TEXTURE_ENV_COLOR:
  case GL_VIEWPORT:
    size = 4;
    break;

  case GL_MODELVIEW_MATRIX:
  case GL_PROJECTION_MATRIX:
  case GL_TEXTURE_MATRIX:
    size = 16;
    break;
  }

  params = LUAGL_NEW_ARRAY(GLenum, size);

  glGetIntegerv(e, (GLint*)params);

  for(i = 0; i < size; i++)
    luagl_pushenum(L, params[i]);

  LUAGL_DELETE_ARRAY(params);

  return size;
}

/*GetArray (pname) -> paramsArray*/
static int luagl_get_array(lua_State *L)
{
  int size = 1;
  GLenum e;
  GLfloat *params;

  e = luagl_get_gl_enum(L, 1);

  switch(e)
  {
  case GL_DEPTH_RANGE:
  case GL_MAX_VIEWPORT_DIMS:
    size = 2;
    break;

  case GL_CURRENT_NORMAL:
    size = 3;
    break;

  case GL_COLOR_CLEAR_VALUE:
  case GL_COLOR_WRITEMASK:
  case GL_CURRENT_COLOR:
  case GL_CURRENT_TEXTURE_COORDS:
  case GL_FOG_COLOR:
  case GL_LIGHT_MODEL_AMBIENT:
  case GL_SCISSOR_BOX:
  case GL_TEXTURE_ENV_COLOR:
  case GL_VIEWPORT:
    size = 4;
    break;

  case GL_MODELVIEW_MATRIX:
  case GL_PROJECTION_MATRIX:
  case GL_TEXTURE_MATRIX:
    size = 16;
    break;
  }

  params = LUAGL_NEW_ARRAY(GLfloat, size);

  glGetFloatv(e, params);

  luagl_push_arrayf(L, params, size);

  LUAGL_DELETE_ARRAY(params);

  return 1;
}

/*GetClipPlane (plane) -> equationArray*/
static int luagl_get_clip_plane(lua_State *L)
{
  GLfloat *equation;

  equation = LUAGL_NEW_ARRAY(GLfloat, 4);

  glGetClipPlanef(luagl_get_gl_enum(L, 1), equation);

  luagl_push_arrayf(L, equation, 4);

  LUAGL_DELETE_ARRAY(equation);

  return 1;
}

/*GetError () -> error flag*/
static int luagl_get_error(lua_State *L)
{
  GLenum error = glGetError();
  if(error == GL_NO_ERROR)
    lua_pushnil(L);
  else
    luagl_pushenum(L, error);
  return 1;
}

/*GetLight (light, pname) -> paramsArray*/
static int luagl_get_light(lua_State *L)
{
  int size = 1;
  GLenum e1, e2;
  GLfloat *params;

  e1 = luagl_get_gl_enum(L, 1);
  e2 = luagl_get_gl_enum(L, 2);

  switch(e2)
  {
  case GL_AMBIENT:
  case GL_DIFFUSE:
  case GL_SPECULAR:
  case GL_POSITION:
    size = 4;
    break;
  case GL_SPOT_DIRECTION :
    size = 3;
    break;
  case GL_SPOT_EXPONENT:
  case GL_SPOT_CUTOFF:
  case GL_CONSTANT_ATTENUATION:
  case GL_LINEAR_ATTENUATION:
  case GL_QUADRATIC_ATTENUATION:
    size = 1;
    break;
  }

  params = LUAGL_NEW_ARRAY(GLfloat, size);

  glGetLightfv(e1, e2, params);

  luagl_push_arrayf(L, params, size);

  LUAGL_DELETE_ARRAY(params);

  return 1;
}

/*GetMaterial (face, pname) -> paramsArray*/
static int luagl_get_material(lua_State *L)
{
  int size = 1;
  GLenum e1, e2;
  GLfloat *params;

  /* get string parameters */
  e1 = luagl_get_gl_enum(L, 1);
  e2 = luagl_get_gl_enum(L, 2);

  switch(e2)
  {
  case GL_AMBIENT:
  case GL_DIFFUSE:
  case GL_SPECULAR:
  case GL_EMISSION:
    size = 4;
    break;
  case GL_SHININESS:
    size = 1;
    break;
  }

  params = LUAGL_NEW_ARRAY(GLfloat, size);

  glGetMaterialfv(e1, e2, params);

  luagl_push_arrayf(L, params, size);

  LUAGL_DELETE_ARRAY(params);

  return 1;
}

/*GetPointer (pname, n) -> valuesArray*/
static int luagl_get_pointer(lua_State *L)
{
  int n;
  GLenum e;

  e = luagl_get_gl_enum(L, 1);
  n = luaL_checkinteger(L, 2);

  GLfloat *params;
  glGetPointerv(e, (void *)&params);
  if(params == 0)
    return 0;

  luagl_push_arrayf(L, params, n);

  return 1;
}

/*GetString (name) -> string*/
static int luagl_get_string(lua_State *L)
{
  lua_pushstring(L, (const char*)glGetString(luagl_get_gl_enum(L, 1)));
  return 1;
}

/*GetTexEnv (pname) -> paramsArray*/
static int luagl_get_tex_env(lua_State *L)
{
  GLenum e1;

  e1 = luagl_get_gl_enum(L, 1);

  if (e1 != GL_TEXTURE_ENV_MODE)
  {
    GLfloat *params;

    params = LUAGL_NEW_ARRAY(GLfloat, 4);

    glGetTexEnvfv(GL_TEXTURE_ENV, e1, params);

    luagl_push_arrayf(L, params, 4);

    LUAGL_DELETE_ARRAY(params);
  }
  else 
  {
    GLint e2;
    glGetTexEnviv(GL_TEXTURE_ENV, e1, &e2);
    luagl_pushenum(L, e2);
  }

  return 1;
}

static int luagl_get_depth(GLenum format)
{
  int depth = 0;
  switch(format)
  {
  case GL_ALPHA:
  case GL_LUMINANCE:
    depth = 1;
    break;

  case GL_LUMINANCE_ALPHA:
    depth = 2;
    break;

  case GL_RGB:
    depth = 3;
    break;

  case GL_RGBA:
    depth = 4;
    break;
  }

  return depth;
}

/*GetTexParameter (target, pname) -> paramsArray*/
static int luagl_get_tex_parameter(lua_State *L)
{
  GLenum target, pname;

  target = luagl_get_gl_enum(L, 1);
  pname = luagl_get_gl_enum(L, 2);

  GLint e;
  glGetTexParameteriv(target, pname, &e);
  luagl_pushenum(L, e);

  return 1;
}

/*Hint (target, mode) -> none*/
static int luagl_hint(lua_State *L)
{
  glHint(luagl_get_gl_enum(L, 1), luagl_get_gl_enum(L, 2));
  return 0;
}

/*IsEnabled (cap) -> true/false*/
static int luagl_is_enabled(lua_State *L)
{
  lua_pushboolean(L, glIsEnabled(luagl_get_gl_enum(L, 1)));
  return 1;
}

/*IsTexture (texture) -> true/false*/
static int luagl_is_texture(lua_State *L)
{
  lua_pushboolean(L, glIsTexture(luaL_checkinteger(L, 1)));
  return 1;
}

/*Light (light, pname, param) -> none
  Light (light, pname, paramsArray) -> none*/
static int luagl_light(lua_State *L)
{
  if(lua_istable(L, 3))
  {
    GLfloat *params;
    luagl_get_arrayf(L, 3, &params);

    glLightfv(luagl_get_gl_enum(L, 1), luagl_get_gl_enum(L, 2), (GLfloat *)params);

    LUAGL_DELETE_ARRAY(params);
  }
  else 
    glLightf(luagl_get_gl_enum(L, 1), luagl_get_gl_enum(L, 2), (GLfloat)luaL_checknumber(L, 3));
  return 0;
}

/*LightModel (pname, param) -> none
  LightModel (pname, paramsArray) -> none*/
static int luagl_light_model(lua_State *L)
{
  if (lua_istable(L, 2))
  {
    GLfloat *params;
    luagl_get_arrayf(L, 2, &params);

    glLightModelfv(luagl_get_gl_enum(L, 1), (GLfloat *)params);

    LUAGL_DELETE_ARRAY(params);
  }
  else 
    glLightModelf(luagl_get_gl_enum(L, 1), (GLfloat)luaL_checknumber(L, 2));
  return 0;
}

/*LineWidth (width) -> none*/
static int luagl_line_width(lua_State *L)
{
  glLineWidth((GLfloat)luaL_checknumber(L, 1));
  return 0;
}

/*LoadIdentity () -> none*/
static int luagl_load_identity(lua_State *L)
{
  glLoadIdentity();
  return 0;
}

/*LoadMatrix (mArray) -> none*/
static int luagl_load_matrix(lua_State *L)
{
  GLfloat *m;
  int n;

  n = luagl_get_arrayf(L, 1, &m);
  if (n < 16)
  {
    LUAGL_DELETE_ARRAY(m);
    luaL_argerror(L, 1, "invalid number of elements in the matrix table (n<16).");
  }

  glLoadMatrixf(m);

  LUAGL_DELETE_ARRAY(m);

  return 0;
}

/*LogicOp (opcode) -> none*/
static int luagl_logic_op(lua_State *L)
{
  glLogicOp(luagl_get_gl_enum(L, 1));
  return 0;
}

/*Material (face, pname, param) -> none*/
static int luagl_material(lua_State *L)
{
  GLenum e1, e2;
  GLfloat *params;

  e1 = luagl_get_gl_enum(L, 1);
  e2 = luagl_get_gl_enum(L, 2);

  if (lua_istable(L, 3))
  {
    luagl_get_arrayf(L, 3, &params);

    glMaterialfv(e1, e2, (GLfloat *)params);

    LUAGL_DELETE_ARRAY(params);
  }
  else 
    glMaterialf(e1, e2, (GLfloat)luaL_checknumber(L, 3));
  return 0;
}

/*MatrixMode (mode) -> none*/
static int luagl_matrix_mode(lua_State *L)
{
  glMatrixMode(luagl_get_gl_enum(L, 1));
  return 0;
}

/*MultMatrix (mArray) -> none*/
static int luagl_mult_matrix(lua_State *L)
{
  GLfloat *m;
  int n;

  n = luagl_get_arrayf(L, 1, &m);
  if (n < 16)
  {
    LUAGL_DELETE_ARRAY(m);
    luaL_argerror(L, 1, "invalid number of elements in the matrix table (n<16).");
  }

  glMultMatrixf(m);

  LUAGL_DELETE_ARRAY(m);

  return 0;
}

/*Normal (nx, ny, nz) -> none*/
static int luagl_normal(lua_State *L)
{
  glNormal3f(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3));

  return 0;
}

/*NormalPointer (normalArray) -> none*/
static int luagl_normal_pointer(lua_State *L)
{
  GLint size;
  static GLfloat *parray = 0;

  LUAGL_DELETE_ARRAY(parray);

  if(lua_isnil(L,1))
    return 0;

  if (lua_isnumber(L, 2))
  {
    size = luaL_checkinteger(L, 2);
    luagl_get_arrayf_pointer(L, 1, &parray);
  }
  else 
    luagl_get_array2f_pointer(L, 1, &parray, &size);

  glNormalPointer(GL_FLOAT, 0, parray);

  return 0;
}

/*Ortho (left, right, bottom, top, zNear, zFar) -> none*/
static int luagl_ortho(lua_State *L)
{
  glOrthof(luaL_checknumber(L, 1), luaL_checknumber(L, 2),
          luaL_checknumber(L, 3), luaL_checknumber(L, 4),
          luaL_checknumber(L, 5), luaL_checknumber(L, 6));
  return 0;
}

/*PixelStore (pname, param) -> none*/
static int luagl_pixel_store(lua_State *L)
{
  if (lua_isboolean(L,2))
    glPixelStorei(luagl_get_gl_enum(L, 1), (GLfloat)luagl_checkboolean(L, 2));
  else 
    glPixelStorei(luagl_get_gl_enum(L, 1), (GLfloat)luaL_checknumber(L, 2));
  return 0;
}

/*PointSize (size) -> none*/
static int luagl_point_size(lua_State *L)
{
  glPointSize((GLfloat)luaL_checknumber(L, 1));
  return 0;
}

/*PolygonOffset (factor, units) -> none*/
static int luagl_polygon_offset(lua_State *L)
{
  glPolygonOffset((GLfloat)luaL_checknumber(L, 1), (GLfloat)luaL_checknumber(L, 2));
  return 0;
}

/*PopMatrix () -> none*/
static int luagl_pop_matrix(lua_State *L)
{
  glPopMatrix();
  return 0;
}

/*PushMatrix () -> none*/
static int luagl_push_matrix(lua_State *L)
{
  glPushMatrix();
  return 0;
}

/*ReadPixels (x, y, width, height, format, pixelsArray) -> none*/
static int luagl_read_pixels(lua_State *L)
{
  GLenum format;
  GLubyte *pixels;
  int width, height, size, depth=1;

  format = luagl_get_gl_enum(L, 5);
  depth = luagl_get_depth(format);
  if (depth == 0)
    luaL_argerror(L, 5, "unknown format");

  width = luaL_checkinteger(L, 3);
  height = luaL_checkinteger(L, 4);
  size = width*height*depth;

  pixels = LUAGL_NEW_ARRAY(GLubyte, size);

  glReadPixels(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2),
               width, height, format, GL_UNSIGNED_BYTE, pixels);

  luagl_push_arrayuc(L, pixels, size);

  LUAGL_DELETE_ARRAY(pixels);

  return 1;
}

/*ReadPixelsRaw (x, y, width, height, format, type, pixels) -> none*/
static int luagl_read_pixels_raw(lua_State *L)
{
  glReadPixels(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2),
               luaL_checkinteger(L, 3), luaL_checkinteger(L, 4), 
               luagl_get_gl_enum(L, 5), luagl_get_gl_enum(L, 6), lua_touserdata(L, 7));
  return 0;
}

/*Rotate (angle, x, y, z) -> none*/
static int luagl_rotate(lua_State *L)
{
  glRotatef(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));
  return 0;
}

/*Scale (x, y, z) -> none*/
static int luagl_scale(lua_State *L)
{
  glScalef(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
  return 0;
}

/*Scissor (x, y, width, height) -> none*/
static int luagl_scissor(lua_State *L)
{
  glScissor(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4));
  return 0;
}

static int luagl_get_select_buffer(lua_State *L)
{
  int size, i;
  GLuint *buffer;

  if(!lua_islightuserdata(L, 1))
    luaL_typerror(L, 1, lua_typename(L, LUA_TLIGHTUSERDATA));

  buffer=(GLuint *)lua_touserdata(L,1);
  i=luaL_checkinteger(L,2);
  if (buffer) 
  { 
    size = (int)buffer[0];
    if ((i<=size) && (i>0)) 
    {
      lua_pushnumber(L,buffer[i]); /*select buffer data begin at index i */
      return 1;
     }
  }
  return 0;
}

static int luagl_free_select_buffer(lua_State *L)
{
  GLuint *buffer;
  if(!lua_islightuserdata(L, 1))
    luaL_typerror(L, 1, lua_typename(L, LUA_TLIGHTUSERDATA));
  buffer=(GLuint *)lua_touserdata(L,1);
  LUAGL_DELETE_ARRAY(buffer);
  return 0;
}

/*ShadeModel (mode) -> none*/
static int luagl_shade_model(lua_State *L)
{
  glShadeModel(luagl_get_gl_enum(L, 1));
  return 0;
}

/*StencilFunc (func, ref, mask) -> none*/
static int luagl_stencil_func(lua_State *L)
{
  if (lua_type(L,3) == LUA_TSTRING)
    glStencilFunc(luagl_get_gl_enum(L, 1), luaL_checkinteger(L, 2), luagl_str2mask(luaL_checkstring(L, 3)));
  else 
    glStencilFunc(luagl_get_gl_enum(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
  return 0;
}

/*StencilMask (mask) -> none*/
static int luagl_stencil_mask(lua_State *L)
{
  if(lua_type(L,1) == LUA_TSTRING)
    glStencilMask(luagl_str2mask(luaL_checkstring(L, 1)));
  else 
    glStencilMask(luaL_checkinteger(L, 1));
  return 0;
}

/*StencilOp (fail, zfail, zpass) -> none*/
static int luagl_stencil_op(lua_State *L)
{
  glStencilOp(luagl_get_gl_enum(L, 1), luagl_get_gl_enum(L, 2), luagl_get_gl_enum(L, 3));
  return 0;
}

/*TexCoord (s[, t, r, q]) -> none
  TexCoord (vArray) -> none*/
static int luagl_tex_coord(lua_State *L)
{
  int index;
  int num_args = lua_gettop(L);

  GLfloat *v = 0;

  if (lua_istable(L, 1))
    num_args = luagl_get_arrayf(L, 1, &v);
  else
  {
    v = LUAGL_NEW_ARRAY(GLfloat, num_args);

    for(index = 0; index < num_args; index++)
    {
      v[index] = luaL_checknumber(L, index+1);
    }
  }

  switch(num_args)
  {
  case 1:  glMultiTexCoord4f(GL_TEXTURE0, v[0], 0, 0, 0);  break;
  case 2:  glMultiTexCoord4f(GL_TEXTURE0, v[0], v[1], 0, 0);  break;
  case 3:  glMultiTexCoord4f(GL_TEXTURE0, v[0], v[1], v[2], 0);  break;
  case 4:  glMultiTexCoord4f(GL_TEXTURE0, v[0], v[1], v[2], v[3]);  break;
  }

  LUAGL_DELETE_ARRAY(v);

  return 0;
}

/*TexCoordPointer(vArray) -> none*/
static int luagl_tex_coord_pointer(lua_State *L)
{
  GLint size;
  static GLfloat *parray = 0;

  LUAGL_DELETE_ARRAY(parray);

  if(lua_isnil(L,1))
    return 0;

  if (lua_isnumber(L, 2))
  {
    size = luaL_checkinteger(L, 2);
    luagl_get_arrayf_pointer(L, 1, &parray);
  }
  else 
    luagl_get_array2f_pointer(L, 1, &parray, &size);

  glTexCoordPointer(size, GL_FLOAT, 0, parray);

  return 0;
}

/*TexEnv (pname, param) -> none
  TexEnv (pname, paramsArray) -> none*/
static int luagl_tex_env(lua_State *L)
{
  if(lua_istable(L, 2))
  {
    GLfloat *param;
    luagl_get_arrayf(L, 2, &param);

    glTexEnvfv(GL_TEXTURE_ENV, luagl_get_gl_enum(L, 1), (GLfloat *)param);

    LUAGL_DELETE_ARRAY(param);
  }
  else if(lua_isnumber(L, 2))
    glTexEnvf(GL_TEXTURE_ENV, luagl_get_gl_enum(L, 1), (GLfloat)luaL_checknumber(L, 2));
  else 
    glTexEnvi(GL_TEXTURE_ENV, luagl_get_gl_enum(L, 1), luagl_get_gl_enum(L, 2));

  return 0;
}

/*TexImage(level, internalformat, format, pixels) -> none*/
static int luagl_tex_image(lua_State *L)
{
  GLenum e;
  GLubyte *pixels;
  GLsizei width, height;
  int iformat;
  int index;

  if(lua_isnumber(L, 1) && lua_istable(L, 2))
  {
    /* undocumented parameter passing, 
       so it can be compatible with a texture created for glu.Build2DMipmaps */
    lua_getfield(L, 2, "components");
    iformat = luaL_checkinteger(L, -1);
    lua_remove(L, -1);

    lua_getfield(L, 2, "format");
    e = luagl_get_gl_enum(L, -1);
    lua_remove(L, -1);

    index = 2;
  }
  else
  {
    e = luagl_get_gl_enum(L, 3);
    iformat = luaL_checkinteger(L, 2);
    index = 4;
  }

  height = luagl_get_array2uc(L, index, &pixels, &width);
  if (height != -1)
  {
    glTexImage2D(GL_TEXTURE_2D, luaL_checkinteger(L, 1),
                 iformat, width/iformat, height, 0, e, GL_UNSIGNED_BYTE, pixels);
  }
  else  /* if not 2D, get 1D */
  {
    width = luagl_get_arrayuc(L, index, &pixels);
    glTexImage2D(GL_TEXTURE_2D, luaL_checkinteger(L, 1),
				   iformat, width/iformat, 1, 0, e, GL_UNSIGNED_BYTE, pixels);
  }
  LUAGL_DELETE_ARRAY(pixels);
  return 0;
}

/* TexImage2D(level, depth, width, height, border, format, type, pixels) -> none*/
static int luagl_tex_image_2d(lua_State *L)
{
  glTexImage2D(GL_TEXTURE_2D, luaL_checkinteger(L, 1),
              luaL_checkinteger(L, 2), (GLsizei)luaL_checkinteger(L, 3), 
              (GLsizei)luaL_checkinteger(L, 4), luaL_checkinteger(L, 5), 
              luagl_get_gl_enum(L, 6), luagl_get_gl_enum(L, 7), lua_touserdata(L, 8));
  return 0;
}

/*TexSubImage (level, format, pixels, xoffset) -> none
  TexSubImage (level, format, pixels, xoffset, yoffset) -> none*/
static int luagl_tex_sub_image(lua_State *L)
{
  GLenum format;
  GLfloat *pixels;
  GLsizei width, height;
  int depth;

  format = luagl_get_gl_enum(L, 2);
  depth = luagl_get_depth(format);
  if (depth == 0)
    luaL_argerror(L, 2, "unknown format");

  height = luagl_get_array2f(L, 3, &pixels, &width);
  if(height != -1)
  {
    glTexSubImage2D(GL_TEXTURE_2D, luaL_checkinteger(L, 1), luaL_checkinteger(L, 4),
                    luaL_checkinteger(L, 5), width/depth, height, format, GL_FLOAT, pixels);
  }
  else  /* if not 2D, get 1D */
  {
    width = luagl_get_arrayf(L, 3, &pixels);
    glTexSubImage2D(GL_TEXTURE_2D, luaL_checkinteger(L, 1), luaL_checkinteger(L, 4),
                    luaL_checkinteger(L, 5), width/depth, 1, format, GL_FLOAT, pixels);
  }

  LUAGL_DELETE_ARRAY(pixels);
  return 0;
}

/* TexSubImage2D (level, xoffset, yoffset, width, height, format, type, pixels) -> none*/
static int luagl_tex_sub_image_2d(lua_State *L)
{
  glTexSubImage2D(GL_TEXTURE_2D, luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), 
                  luaL_checkinteger(L, 3), luaL_checkinteger(L, 4), luaL_checkinteger(L, 5), 
                  luagl_get_gl_enum(L, 6), luagl_get_gl_enum(L, 7), lua_touserdata(L, 8));
  return 0;
}

/*TexParameter (target, pname, param) -> none
TexParameter (target, pname, paramsArray) -> none*/
static int luagl_tex_parameter(lua_State *L)
{
  GLenum e1, e2;

  e1 = luagl_get_gl_enum(L, 1);
  e2 = luagl_get_gl_enum(L, 2);

  if(lua_istable(L, 3))
  {
    GLfloat *param;
    luagl_get_arrayf(L, 3, &param);

    glTexParameterfv(e1, e2, (GLfloat *)param);

    LUAGL_DELETE_ARRAY(param);
  }
  else if(lua_isnumber(L, 3))
    glTexParameterf(e1, e2, (GLfloat)luaL_checknumber(L, 3));
  else 
    glTexParameteri(e1, e2, luagl_get_gl_enum(L, 3));

  return 0;
}

/*Translate (x, y, z) -> none*/
static int luagl_translate(lua_State *L)
{
  glTranslatef(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
  return 0;
}


/*VertexPointer (vertexArray) -> none*/
static int luagl_vertex_pointer(lua_State *L)
{
  GLint size;
  static GLfloat *parray = 0;

  LUAGL_DELETE_ARRAY(parray);

  if(lua_isnil(L,1))
    return 0;

  if (lua_isnumber(L, 2))
  {
    size = luaL_checkinteger(L, 2);
    luagl_get_arrayf_pointer(L, 1, &parray);
  }
  else 
    luagl_get_array2f_pointer(L, 1, &parray, &size);

  glVertexPointer(size, GL_FLOAT, 0, parray);

  return 0;
}

/*Viewport (x, y, width, height) -> none*/
static int luagl_viewport(lua_State *L)
{
  glViewport(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), 
             luaL_checkinteger(L, 3), luaL_checkinteger(L, 4));
  return 0;
}

static const luaL_reg luagl_lib[] = {
  {"AlphaFunc", luagl_alpha_func},
  {"BindTexture", luagl_bind_texture},
  {"BlendFunc", luagl_blend_func},
  {"Clear", luagl_clear},
  {"ClearColor", luagl_clear_color},
  {"ClearDepth", luagl_clear_depth},
  {"ClearStencil", luagl_clear_stencil},
  {"ClipPlane", luagl_clip_plane},
  {"Color", luagl_color},
  {"ColorMask", luagl_color_mask},
  {"ColorPointer", luagl_color_pointer},
  {"CopyTexImage", luagl_copy_tex_image},
  {"CopyTexSubImage", luagl_copy_tex_sub_image},
  {"CullFace",luagl_cull_face},
  {"DeleteTextures",luagl_delete_textures},
  {"DepthFunc",luagl_depth_func},
  {"DepthMask",luagl_depth_mask},
  {"DepthRange",luagl_depth_range},
  {"Disable",luagl_disable},
  {"DisableClientState",luagl_disable_client_state},
  {"DrawArrays",luagl_draw_arrays},
  {"DrawElements", luagl_draw_elements},
  {"Enable", luagl_enable},
  {"EnableClientState", luagl_enable_client_state},
  {"Finish", luagl_finish},
  {"Flush", luagl_flush},
  {"Fog", luagl_fog},
  {"FrontFace", luagl_front_face},
  {"Frustum", luagl_frustum},
  {"GenTextures", luagl_gen_textures},
  {"Get", luagl_get},
  {"GetArray", luagl_get_array},
  {"GetConst", luagl_get_const},
  {"GetClipPlane", luagl_get_clip_plane},
  {"GetError", luagl_get_error},
  {"GetLight", luagl_get_light},
  {"GetMaterial", luagl_get_material},
  {"GetPointer", luagl_get_pointer},
  {"GetString", luagl_get_string},
  {"GetTexEnv", luagl_get_tex_env},
  {"GetTexParameter", luagl_get_tex_parameter},
  {"Hint", luagl_hint},
  {"IsEnabled", luagl_is_enabled},
  {"IsTexture", luagl_is_texture},
  {"Light", luagl_light},
  {"LightModel", luagl_light_model},
  {"LineWidth", luagl_line_width},
  {"LoadIdentity", luagl_load_identity},
  {"LoadMatrix", luagl_load_matrix},
  {"LogicOp", luagl_logic_op},
  {"Material", luagl_material},
  {"MatrixMode", luagl_matrix_mode},
  {"MultMatrix", luagl_mult_matrix},
  {"Normal", luagl_normal},
  {"NormalPointer", luagl_normal_pointer},
  {"Ortho", luagl_ortho},
  {"PixelStore", luagl_pixel_store},
  {"PointSize", luagl_point_size},
  {"PolygonOffset", luagl_polygon_offset},
  {"PopMatrix", luagl_pop_matrix},
  {"PushMatrix", luagl_push_matrix},
  {"ReadPixels", luagl_read_pixels},
  {"ReadPixelsRaw", luagl_read_pixels_raw},
  {"Rotate", luagl_rotate},
  {"Scale", luagl_scale},
  {"Scissor", luagl_scissor},
  {"GetSelectBuffer", luagl_get_select_buffer},
  {"FreeSelectBuffer", luagl_free_select_buffer},
  {"ShadeModel", luagl_shade_model},
  {"StencilFunc", luagl_stencil_func},
  {"StencilMask", luagl_stencil_mask},
  {"StencilOp", luagl_stencil_op},
  {"TexCoord", luagl_tex_coord},
  {"TexCoordPointer", luagl_tex_coord_pointer},
  {"TexEnv", luagl_tex_env},
  {"TexImage", luagl_tex_image},
  {"TexImage2D", luagl_tex_image_2d},
  {"TexSubImage", luagl_tex_sub_image},
  {"TexSubImage2D", luagl_tex_sub_image_2d},
  {"TexParameter", luagl_tex_parameter},
  {"Translate", luagl_translate},
  {"VertexPointer", luagl_vertex_pointer},
  {"Viewport", luagl_viewport},
  {NULL, NULL}
};

int luaopen_luagl(lua_State *L) 
{
  luaL_register(L, "gl", luagl_lib);

  luagl_initconst(L, luagl_const);

  lua_pushstring(L, "_VERSION");
  lua_pushstring(L, LUAGL_VERSION);
  lua_settable(L,-3);

  return 1;
}
