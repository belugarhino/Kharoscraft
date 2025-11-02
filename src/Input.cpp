#include "kharoscraft/core/Input.hpp"

namespace khc::core {

void Input::begin_frame() {
    mouse_delta_x_ = 0.0f;
    mouse_delta_y_ = 0.0f;
}

void Input::handle_event(const SDL_Event& event) {
    switch (event.type) {
    case SDL_KEYDOWN:
        if (!event.key.repeat) {
            keyboard_state_[event.key.keysym.scancode] = true;
        }
        break;
    case SDL_KEYUP:
        keyboard_state_[event.key.keysym.scancode] = false;
        break;
    case SDL_MOUSEMOTION:
        mouse_delta_x_ += static_cast<float>(event.motion.xrel);
        mouse_delta_y_ += static_cast<float>(event.motion.yrel);
        break;
    default:
        break;
    }
}

} // namespace khc::core
