#pragma once

#include "kharoscraft/math/Mat4.hpp"
#include "kharoscraft/math/Vec3.hpp"

namespace khc::render {

class Camera {
public:
    void set_perspective(float fov_degrees, float aspect_ratio, float near_plane, float far_plane);

    void set_position(const math::Vec3& position) { position_ = position; }
    void set_orientation(float yaw, float pitch);

    void move(const math::Vec3& delta) { position_ += delta; }
    void add_yaw(float delta);
    void add_pitch(float delta);

    [[nodiscard]] math::Vec3 forward() const;
    [[nodiscard]] math::Vec3 right() const;
    [[nodiscard]] math::Vec3 up() const;

    [[nodiscard]] math::Mat4 view_matrix() const;
    [[nodiscard]] const math::Mat4& projection_matrix() const { return projection_; }
    [[nodiscard]] math::Vec3 position() const { return position_; }

private:
    math::Vec3 position_{0.0f, 64.0f, 0.0f};
    float yaw_{-90.0f};
    float pitch_{0.0f};
    math::Mat4 projection_{};
};

} // namespace khc::render
