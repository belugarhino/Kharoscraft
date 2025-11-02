#include "kharoscraft/render/Camera.hpp"

#include <algorithm>
#include <cmath>

namespace khc::render {

void Camera::set_perspective(float fov_degrees, float aspect_ratio, float near_plane, float far_plane) {
    float fov_radians = fov_degrees * static_cast<float>(M_PI) / 180.0f;
    projection_ = math::Mat4::perspective(fov_radians, aspect_ratio, near_plane, far_plane);
}

void Camera::set_orientation(float yaw, float pitch) {
    yaw_ = yaw;
    pitch_ = std::clamp(pitch, -89.0f, 89.0f);
}

void Camera::add_yaw(float delta) {
    yaw_ += delta;
}

void Camera::add_pitch(float delta) {
    pitch_ = std::clamp(pitch_ + delta, -89.0f, 89.0f);
}

math::Vec3 Camera::forward() const {
    float yaw_rad = yaw_ * static_cast<float>(M_PI) / 180.0f;
    float pitch_rad = pitch_ * static_cast<float>(M_PI) / 180.0f;

    return math::Vec3{
        std::cos(yaw_rad) * std::cos(pitch_rad),
        std::sin(pitch_rad),
        std::sin(yaw_rad) * std::cos(pitch_rad),
    }.normalized();
}

math::Vec3 Camera::right() const {
    return math::cross(forward(), {0.0f, 1.0f, 0.0f}).normalized();
}

math::Vec3 Camera::up() const {
    return math::cross(right(), forward()).normalized();
}

math::Mat4 Camera::view_matrix() const {
    return math::Mat4::look_at(position_, position_ + forward(), up());
}

} // namespace khc::render
