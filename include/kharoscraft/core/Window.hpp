#pragma once

#include <SDL2/SDL.h>
#include <string>

namespace khc::core {

struct WindowConfig {
    std::string title{"Kharoscraft"};
    int width{1600};
    int height{900};
    bool fullscreen{false};
};

class Window {
public:
    Window() = default;
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) noexcept = delete;
    Window& operator=(Window&&) noexcept = delete;

    bool create(const WindowConfig& config);
    void destroy();

    [[nodiscard]] SDL_Window* handle() const { return window_; }
    [[nodiscard]] SDL_GLContext context() const { return context_; }

    void swap_buffers();

private:
    SDL_Window* window_{nullptr};
    SDL_GLContext context_{nullptr};
};

} // namespace khc::core
