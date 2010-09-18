/*************************************************
*  LuaGL - an OpenGL binding for Lua
*  2007(c) Fabio Guerra
*  luagl.sourceforge.net
*-------------------------------------------------
*  Description: This file implements the GLU
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
#include <math.h>

#include <OpenGLES/ES1/gl.h>

#include "lua.h"
#include "lauxlib.h"

#include "luaglu.h"
#include "luagl_util.h"

/* old headers does not have this */
#ifndef GLU_INCOMPATIBLE_GL_VERSION
#define GLU_INCOMPATIBLE_GL_VERSION     100903
#endif

static
void gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{	
	GLfloat xmin, xmax, ymin, ymax;
	
	ymax = zNear * (GLfloat)tanf(fovy * (float)M_PI / 360);
	ymin = -ymax;
	xmin = ymin * aspect;
	xmax = ymax * aspect;
	
	glFrustumf(xmin, xmax,
			   ymin, ymax,
			   zNear, zFar);	
}

static
void gluOrtho2D(GLfloat  	left,
				GLfloat  	right,
				GLfloat  	bottom,
				GLfloat  	top)
{
	// gluOrtho2D sets up a two-dimensional orthographic viewing region. This is equivalent 
	// to calling glOrtho with near = -1 and far = 1 .
	glOrthof(left, right, bottom, top, -1.0f, 1.0f);
}

static
void gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
			   GLfloat centerx, GLfloat centery, GLfloat centerz,
			   GLfloat upx, GLfloat upy, GLfloat upz)
{
    GLfloat m[16];
    GLfloat x[3], y[3], z[3];
    GLfloat mag;
	
    /* Make rotation matrix */
	
    /* Z vector */
    z[0] = eyex - centerx;
    z[1] = eyey - centery;
    z[2] = eyez - centerz;
    mag = (float)sqrtf(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
    if (mag) {
        z[0] /= mag;
        z[1] /= mag;
        z[2] /= mag;
    }
	
    /* Y vector */
    y[0] = upx;
    y[1] = upy;
    y[2] = upz;
	
    /* X vector = Y cross Z */
    x[0] = y[1] * z[2] - y[2] * z[1];
    x[1] = -y[0] * z[2] + y[2] * z[0];
    x[2] = y[0] * z[1] - y[1] * z[0];
	
    /* Recompute Y = Z cross X */
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = -z[0] * x[2] + z[2] * x[0];
    y[2] = z[0] * x[1] - z[1] * x[0];
	
    /* cross product gives area of parallelogram, which is < 1.0 for
     * non-perpendicular unit-length vectors; so normalize x, y here
     */
	
    mag = (float)sqrtf(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
    if (mag) {
        x[0] /= mag;
        x[1] /= mag;
        x[2] /= mag;
    }
	
    mag = (float)sqrtf(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
    if (mag) {
        y[0] /= mag;
        y[1] /= mag;
        y[2] /= mag;
    }
	
#define M(row,col)  m[col*4+row]
    M(0, 0) = x[0];
    M(0, 1) = x[1];
    M(0, 2) = x[2];
    M(0, 3) = 0.0f;
    M(1, 0) = y[0];
    M(1, 1) = y[1];
    M(1, 2) = y[2];
    M(1, 3) = 0.0f;
    M(2, 0) = z[0];
    M(2, 1) = z[1];
    M(2, 2) = z[2];
    M(2, 3) = 0.0f;
    M(3, 0) = 0.0f;
    M(3, 1) = 0.0f;
    M(3, 2) = 0.0f;
    M(3, 3) = 1.0f;
#undef M
	
	glMultMatrixf(m);
	
	
    /* Translate Eye to Origin */
    glTranslatef(-eyex, -eyey, -eyez);
}



/* Ortho2D(left, right, bottom, top) -> none */
static int luaglu_ortho_2D(lua_State *L)
{
  gluOrtho2D(luaL_checknumber(L, 1), luaL_checknumber(L, 2), 
             luaL_checknumber(L, 3), luaL_checknumber(L, 4));
  return 0;
}

/* Perspective(fovy, aspect, near, far) -> none */
static int luaglu_perspective(lua_State *L)
{
  gluPerspective(luaL_checknumber(L, 1), luaL_checknumber(L, 2), 
                 luaL_checknumber(L, 3), luaL_checknumber(L, 4));
  return 0;
}

/* LookAt(Ex, Ey, Ez, Lx, Ly, Lz, Ux, Uy, Uz) -> none */
static int luaglu_look_at(lua_State *L)
{
  gluLookAt(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3),
            luaL_checknumber(L, 4), luaL_checknumber(L, 5), luaL_checknumber(L, 6),
            luaL_checknumber(L, 7), luaL_checknumber(L, 8), luaL_checknumber(L, 9));
  return 0;
}



static const luaL_reg luaglu_lib[] = {
  {"Ortho2D", luaglu_ortho_2D},
  {"Perspective", luaglu_perspective},
  {"LookAt", luaglu_look_at},
  {NULL, NULL}
};


int luaopen_luaglu(lua_State *L)
{
  luaL_register(L, "glu", luaglu_lib);
  return 1;
}
