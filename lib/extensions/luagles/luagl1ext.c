/*
 *  luagl1ext.c
 *  TestRapidWax2
 *
 *  Created by Feng Ye on 10-9-23.
 *  Copyright 2010 Coconut Island Studio. All rights reserved.
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

#include "lua.h"
#include "lauxlib.h"

#include "luagl1ext.h"
#include "luagl_util.h"

static const luaglConst luagl1ext_const[] = {
	{ "FRAMEBUFFER_OES"							, GL_FRAMEBUFFER_OES			},
    { "RENDERBUFFER_OES"						, GL_RENDERBUFFER_OES			},
    /* for glGet */
    { "FRAMEBUFFER_BINDING_OES"					, GL_FRAMEBUFFER_BINDING_OES	},
    { "RENDERBUFFER_BINDING_OES"				, GL_RENDERBUFFER_BINDING_OES	},
    /* for glGetRenderbufferParameter */
    { "RENDERBUFFER_WIDTH_OES"					, GL_RENDERBUFFER_WIDTH_OES		},
    { "RENDERBUFFER_HEIGHT_OES"					, GL_RENDERBUFFER_HEIGHT_OES	},
    /* for attachment */
    { "COLOR_ATTACHMENT0_OES"					, GL_COLOR_ATTACHMENT0_OES		},
    { "DEPTH_ATTACHMENT_OES"					, GL_DEPTH_ATTACHMENT_OES		},
    { "STENCIL_ATTACHMENT_OES"					, GL_STENCIL_ATTACHMENT_OES		},
	/* extended formats */
    { "DEPTH_COMPONENT24_OES"					, GL_DEPTH_COMPONENT24_OES		},
	{ "STENCIL_INDEX8_OES"						, GL_STENCIL_INDEX8_OES			},
    { "DEPTH_STENCIL_OES"						, GL_DEPTH_STENCIL_OES			},
	{ "UNSIGNED_INT_24_8_OES"					, GL_UNSIGNED_INT_24_8_OES		},
	{ "DEPTH24_STENCIL8_OES"					, GL_DEPTH24_STENCIL8_OES		},
    /* standard formats */
    { "RGB8_OES"								, GL_RGB8_OES					},
    { "RGBA8_OES"								, GL_RGBA8_OES					},
	{ "RGBA4_OES"								, GL_RGBA4_OES					},
	{ "RGB5_A1_OES"								, GL_RGB5_A1_OES				},
	{ "RGB565_OES"								, GL_RGB565_OES					},
	{ "DEPTH_COMPONENT16_OES"					, GL_DEPTH_COMPONENT16_OES		},
    /* for check framebuffer results */
    { "FRAMEBUFFER_COMPLETE_OES"				, GL_FRAMEBUFFER_COMPLETE_OES	},
	{ "FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES"	, GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES},
	{ "FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES", GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES},
	{ "FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES"	, GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES},
	{ "FRAMEBUFFER_INCOMPLETE_FORMATS_OES"		, GL_FRAMEBUFFER_INCOMPLETE_FORMATS_OES},
	{ "FRAMEBUFFER_UNSUPPORTED_OES"				, GL_FRAMEBUFFER_UNSUPPORTED_OES},
    
	{ 0, 0 },
};

/* will need gl enum in extensions */
extern unsigned int luagl_get_gl_enum(lua_State *L, int index);
unsigned int luagl1ext_get_gl_enum(lua_State *L, int index)
{
    return luagl_get_enum(L, index, luagl1ext_const);
}

static const char *luagl1ext_get_str_gl_enum(GLenum num)
{
    unsigned int i = 0;
    
    /* works only for simple enums */
    while(luagl1ext_const[i].str != 0)
    {
        if(num == luagl1ext_const[i].value)
            return luagl1ext_const[i].str;
        
        i++;
    }
    return NULL;
}

static void luagl1ext_pushenum(lua_State *L, GLenum num)
{
    const char* str = luagl1ext_get_str_gl_enum(num);
    if (str)
        lua_pushstring(L, str);
    else
        lua_pushinteger(L, num);
}


/******************************************************************************/

/* GenFramebuffersOES () -> framebuffer */
static int luagl1ext_gen_framebuffers(lua_State *L)
{
    /* FIXME: support delete multiple framebuffers */
    GLuint framebuffer;
    glGenFramebuffersOES(1, &framebuffer);
    lua_pushinteger(L, framebuffer);
    return 1;
}

/* BindFramebufferOES (gl.FRAMEBUFFER_OES, framebuffer) -> none */
static int luagl1ext_bind_framebuffer(lua_State* L)
{
    glBindFramebufferOES(luagl1ext_get_gl_enum(L, 1), luaL_checkinteger(L, 2));
    return 0;
}

/* DeleteFramebuffersOES(framebuffer) -> none */
static int luagl1ext_delete_framebuffers(lua_State* L)
{
    /* FIXME: support delete multiple framebuffers */
    GLuint framebuffer = luaL_checkinteger(L, 1);
    glDeleteFramebuffersOES(1, &framebuffer);
    return 0;
}

/* FramebufferTexture2DOES(gl.FRAMEBUFFER_OES, attachment, texture_target, 
   texture, level) -> none */
static int luagl1ext_framebuffer_texture2d(lua_State* L)
{
    /* attach a texture to framebuffer object */
    glFramebufferTexture2DOES(luagl1ext_get_gl_enum(L, 1), 
                              luagl1ext_get_gl_enum(L, 2), 
                              luagl_get_gl_enum(L, 3), 
                              luaL_checkinteger(L, 4), 
                              luaL_checkinteger(L, 5));
    return 0;
}

/* GenRenderbuffersOES() -> renderbuffer */
static int luagl1ext_gen_renderbuffers(lua_State* L)
{
    /* FIXME: support generate multiple renderbuffers */
    GLuint renderbuffer;
    glGenRenderbuffersOES(1, &renderbuffer);
    lua_pushinteger(L, renderbuffer);
    return 1;
}

/* DeleteRenderbuffersOES(renderbuffer) -> none */
static int luagl1ext_delete_renderbuffers(lua_State* L)
{
    /* FIXME: support delete multiple renderbuffers */
    GLuint renderbuffer = luaL_checkinteger(L, 1);
    glDeleteRenderbuffersOES(1, &renderbuffer);
    return 0;
}

/* BindRenderbufferOES(gl.RENDERBUFFER_OES, renderbuffer) -> none */
static int luagl1ext_bind_renderbuffer(lua_State* L)
{
    glBindRenderbufferOES(luagl1ext_get_gl_enum(L, 1), luaL_checkinteger(L, 2));
    return 0;
}

/* FramebufferRenderbufferOES(gl.FRAMEBUFFER_OES, attachment, 
   renderbuffer_target, renderbuffer) -> none */
static int luagl1ext_framebuffer_renderbuffer(lua_State* L)
{
    glFramebufferRenderbufferOES(luagl1ext_get_gl_enum(L, 1), 
                                 luagl1ext_get_gl_enum(L, 2), 
                                 luagl1ext_get_gl_enum(L, 3), 
                                 luaL_checkinteger(L, 4));
    return 0;
}

/* RenderbufferStorageOES(gl.RENDERBUFFER_OES, format, width, height) -> none */
static int luagl1ext_renderbuffer_storage(lua_State* L)
{
    glRenderbufferStorageOES(luagl1ext_get_gl_enum(L, 1), 
                             luagl1ext_get_gl_enum(L, 2), 
                             luaL_checkinteger(L, 3), 
                             luaL_checkinteger(L, 4));
    return 0;
}

/* GetRenderbufferParameter(gl.RENDERBUFFER_OES, pname) -> param */
static int luagl1ext_get_renderbuffer_parameter(lua_State* L)
{
    GLint result;
    
    GLenum e = luagl1ext_get_gl_enum(L, 2);
    switch(e)
    {
        case GL_RENDERBUFFER_WIDTH_OES:
        case GL_RENDERBUFFER_HEIGHT_OES:
            glGetRenderbufferParameterivOES(luagl1ext_get_gl_enum(L, 1), 
                                            e, &result);
            lua_pushinteger(L, result);
            return 1;
        default:
            luaL_argerror(L, 2, "unknown enumeration.");
            break;
    }
	
    return 0;
}

/* CheckFramebufferStatusOES(gl.FRAMEBUFFER_OES) -> status */
static int luagl1ext_check_framebuffer_status(lua_State* L)
{
    GLenum status = glCheckFramebufferStatusOES(luagl1ext_get_gl_enum(L, 1));
    lua_pushinteger(L, (lua_Integer)status);
    return 1;
}

/*Get (pname) -> params*/
static int luagl1ext_get(lua_State *L)
{
    int i, size=1;
    GLenum e;
    GLfloat *params;
    int result;
    
    e = luagl1ext_get_gl_enum(L, 1);
    
    switch(e)
    {
		case GL_FRAMEBUFFER_BINDING_OES:
        case GL_RENDERBUFFER_BINDING_OES:
            glGetIntegerv(e, &result);
            lua_pushinteger(L, result);
            return 1;
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

/* GenCBuffer (length) -> userdata */
static int luagl1ext_gen_c_buffer(lua_State* L)
{
    size_t length = (size_t)luaL_checkinteger(L, 1);
    void* memblock = malloc(length);
    lua_pushlightuserdata(L, memblock);
    return 1;
}

/* DeleteCBuffer (userdata) -> none */
static int luagl1ext_delete_c_buffer(lua_State* L)
{
    if (lua_islightuserdata(L, 1))
    {
    	free(lua_touserdata(L, 1));
    }
    else {
        luaL_argerror(L, 1, "not light userdata.");
    }

    return 0;
}

/******************************************************************************/

static const luaL_reg luagl1ext_lib[] = {
    {"GenFramebuffersOES"						, luagl1ext_gen_framebuffers	},
    {"DeleteFramebuffersOES"					, luagl1ext_delete_framebuffers	},
    {"BindFramebufferOES"						, luagl1ext_bind_framebuffer	},
    {"FramebufferTexture2DOES"					, luagl1ext_framebuffer_texture2d},
    
    {"GenRenderbuffersOES"						, luagl1ext_gen_renderbuffers	},
    {"DeleteRenderbuffersOES"					, luagl1ext_delete_renderbuffers},
    {"BindRenderbufferOES"						, luagl1ext_bind_renderbuffer	},
    {"FramebufferRenderbufferOES"				, luagl1ext_framebuffer_renderbuffer},
    {"RenderbufferStorageOES"					, luagl1ext_renderbuffer_storage},
    {"GetRenderbufferParameterOES"				, luagl1ext_get_renderbuffer_parameter},
    
    {"CheckFramebufferStatusOES"				, luagl1ext_check_framebuffer_status},
    
    {"Get"										, luagl1ext_get					},
    
    // self-provided utility functions
    {"GenCBuffer"								, luagl1ext_gen_c_buffer		},
    {"DeleteCBuffer"							, luagl1ext_delete_c_buffer		},
    {NULL, NULL},
};

int luaopen_luagl1ext(lua_State *L)
{
	luaL_register(L, "gl1ext", luagl1ext_lib);
	luagl_initconst(L, luagl1ext_const);
    return 1;
}
