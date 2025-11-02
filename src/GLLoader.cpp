#include "kharoscraft/render/GLLoader.hpp"

#include <SDL2/SDL.h>
#include <iostream>

namespace khc::render::gl {

PFNGLCREATESHADERPROC CreateShader = nullptr;
PFNGLSHADERSOURCEPROC ShaderSource = nullptr;
PFNGLCOMPILESHADERPROC CompileShader = nullptr;
PFNGLGETSHADERIVPROC GetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog = nullptr;
PFNGLDELETESHADERPROC DeleteShader = nullptr;
PFNGLCREATEPROGRAMPROC CreateProgram = nullptr;
PFNGLATTACHSHADERPROC AttachShader = nullptr;
PFNGLLINKPROGRAMPROC LinkProgram = nullptr;
PFNGLGETPROGRAMIVPROC GetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC GetProgramInfoLog = nullptr;
PFNGLDELETEPROGRAMPROC DeleteProgram = nullptr;
PFNGLUSEPROGRAMPROC UseProgram = nullptr;
PFNGLGETUNIFORMLOCATIONPROC GetUniformLocation = nullptr;
PFNGLUNIFORMMATRIX4FVPROC UniformMatrix4fv = nullptr;
PFNGLUNIFORM3FPROC Uniform3f = nullptr;
PFNGLUNIFORM1FPROC Uniform1f = nullptr;
PFNGLUNIFORM1IPROC Uniform1i = nullptr;
PFNGLGENBUFFERSPROC GenBuffers = nullptr;
PFNGLBINDBUFFERPROC BindBuffer = nullptr;
PFNGLBUFFERDATAPROC BufferData = nullptr;
PFNGLDELETEBUFFERSPROC DeleteBuffers = nullptr;
PFNGLGENVERTEXARRAYSPROC GenVertexArrays = nullptr;
PFNGLBINDVERTEXARRAYPROC BindVertexArray = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC VertexAttribPointer = nullptr;
PFNGLDELETEVERTEXARRAYSPROC DeleteVertexArrays = nullptr;
PFNGLDRAWELEMENTSPROC DrawElements = nullptr;

namespace {

template <typename T>
bool load_function(T& target, const char* name) {
    target = reinterpret_cast<T>(SDL_GL_GetProcAddress(name));
    if (!target) {
        std::cerr << "Failed to load GL function: " << name << '\n';
        return false;
    }
    return true;
}

} // namespace

bool load_gl_functions() {
    bool ok = true;
    ok &= load_function(CreateShader, "glCreateShader");
    ok &= load_function(ShaderSource, "glShaderSource");
    ok &= load_function(CompileShader, "glCompileShader");
    ok &= load_function(GetShaderiv, "glGetShaderiv");
    ok &= load_function(GetShaderInfoLog, "glGetShaderInfoLog");
    ok &= load_function(DeleteShader, "glDeleteShader");
    ok &= load_function(CreateProgram, "glCreateProgram");
    ok &= load_function(AttachShader, "glAttachShader");
    ok &= load_function(LinkProgram, "glLinkProgram");
    ok &= load_function(GetProgramiv, "glGetProgramiv");
    ok &= load_function(GetProgramInfoLog, "glGetProgramInfoLog");
    ok &= load_function(DeleteProgram, "glDeleteProgram");
    ok &= load_function(UseProgram, "glUseProgram");
    ok &= load_function(GetUniformLocation, "glGetUniformLocation");
    ok &= load_function(UniformMatrix4fv, "glUniformMatrix4fv");
    ok &= load_function(Uniform3f, "glUniform3f");
    ok &= load_function(Uniform1f, "glUniform1f");
    ok &= load_function(Uniform1i, "glUniform1i");
    ok &= load_function(GenBuffers, "glGenBuffers");
    ok &= load_function(BindBuffer, "glBindBuffer");
    ok &= load_function(BufferData, "glBufferData");
    ok &= load_function(DeleteBuffers, "glDeleteBuffers");
    ok &= load_function(GenVertexArrays, "glGenVertexArrays");
    ok &= load_function(BindVertexArray, "glBindVertexArray");
    ok &= load_function(EnableVertexAttribArray, "glEnableVertexAttribArray");
    ok &= load_function(VertexAttribPointer, "glVertexAttribPointer");
    ok &= load_function(DeleteVertexArrays, "glDeleteVertexArrays");
    ok &= load_function(DrawElements, "glDrawElements");
    return ok;
}

} // namespace khc::render::gl
