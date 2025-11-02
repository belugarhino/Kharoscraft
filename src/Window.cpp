#include "kharoscraft/core/Window.hpp"

#include <SDL2/SDL_opengl.h>
#include <iostream>

namespace khc::core {

Window::~Window() {
    destroy();
}

bool Window::create(const WindowConfig& config) {
    destroy();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << '\n';
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    Uint32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    if (config.fullscreen) {
        window_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    window_ = SDL_CreateWindow(
        config.title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        config.width,
        config.height,
        window_flags);

    if (!window_) {
        std::cerr << "Failed to create window: " << SDL_GetError() << '\n';
        return false;
    }

    context_ = SDL_GL_CreateContext(window_);
    if (!context_) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << '\n';
        destroy();
        return false;
    }

    SDL_GL_SetSwapInterval(0);

    return true;
}

void Window::destroy() {
    if (context_) {
        SDL_GL_DeleteContext(context_);
        context_ = nullptr;
    }
    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
    SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_Quit();
}

void Window::swap_buffers() {
    SDL_GL_SwapWindow(window_);
}

} // namespace khc::core
