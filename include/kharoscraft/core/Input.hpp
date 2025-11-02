#pragma once

#include <SDL2/SDL.h>
#include <array>

namespace khc::core {

class Input {
public:
    void begin_frame();
    void handle_event(const SDL_Event& event);

    [[nodiscard]] bool is_key_down(SDL_Scancode key) const { return keyboard_state_[key]; }
    [[nodiscard]] float mouse_dx() const { return mouse_delta_x_; }
    [[nodiscard]] float mouse_dy() const { return mouse_delta_y_; }

private:
    std::array<bool, SDL_NUM_SCANCODES> keyboard_state_{};
    float mouse_delta_x_{0.0f};
    float mouse_delta_y_{0.0f};
};

} // namespace khc::core
