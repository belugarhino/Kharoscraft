#pragma once

#include <SDL2/SDL_opengl_glext.h>

namespace khc::render::gl {

extern PFNGLCREATESHADERPROC CreateShader;
extern PFNGLSHADERSOURCEPROC ShaderSource;
extern PFNGLCOMPILESHADERPROC CompileShader;
extern PFNGLGETSHADERIVPROC GetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog;
extern PFNGLDELETESHADERPROC DeleteShader;
extern PFNGLCREATEPROGRAMPROC CreateProgram;
extern PFNGLATTACHSHADERPROC AttachShader;
extern PFNGLLINKPROGRAMPROC LinkProgram;
extern PFNGLGETPROGRAMIVPROC GetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC GetProgramInfoLog;
extern PFNGLDELETEPROGRAMPROC DeleteProgram;
extern PFNGLUSEPROGRAMPROC UseProgram;
extern PFNGLGETUNIFORMLOCATIONPROC GetUniformLocation;
extern PFNGLUNIFORMMATRIX4FVPROC UniformMatrix4fv;
extern PFNGLUNIFORM3FPROC Uniform3f;
extern PFNGLUNIFORM1FPROC Uniform1f;
extern PFNGLUNIFORM1IPROC Uniform1i;
extern PFNGLGENBUFFERSPROC GenBuffers;
extern PFNGLBINDBUFFERPROC BindBuffer;
extern PFNGLBUFFERDATAPROC BufferData;
extern PFNGLDELETEBUFFERSPROC DeleteBuffers;
extern PFNGLGENVERTEXARRAYSPROC GenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC BindVertexArray;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC VertexAttribPointer;
extern PFNGLDELETEVERTEXARRAYSPROC DeleteVertexArrays;
extern PFNGLDRAWELEMENTSPROC DrawElements;

bool load_gl_functions();

} // namespace khc::render::gl
