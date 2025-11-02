#pragma once

#include <array>
#include <cmath>

#include "kharoscraft/math/Vec3.hpp"

namespace khc::math {

struct Mat4 {
    std::array<float, 16> data{};

    constexpr Mat4() : data{1.0f, 0.0f, 0.0f, 0.0f,
                             0.0f, 1.0f, 0.0f, 0.0f,
                             0.0f, 0.0f, 1.0f, 0.0f,
                             0.0f, 0.0f, 0.0f, 1.0f} {}

    explicit constexpr Mat4(std::array<float, 16> values) : data(values) {}

    [[nodiscard]] static Mat4 identity() { return Mat4{}; }

    [[nodiscard]] static Mat4 perspective(float fovRadians, float aspect, float nearPlane, float farPlane) {
        float f = 1.0f / std::tan(fovRadians / 2.0f);
        float nf = 1.0f / (nearPlane - farPlane);

        return Mat4{std::array<float, 16>{
            f / aspect, 0.0f, 0.0f, 0.0f,
            0.0f, f, 0.0f, 0.0f,
            0.0f, 0.0f, (farPlane + nearPlane) * nf, -1.0f,
            0.0f, 0.0f, (2.0f * farPlane * nearPlane) * nf, 0.0f,
        }};
    }

    [[nodiscard]] static Mat4 look_at(const Vec3& eye, const Vec3& center, const Vec3& up) {
        Vec3 f = (center - eye).normalized();
        Vec3 s = cross(f, up.normalized()).normalized();
        Vec3 u = cross(s, f);

        return Mat4{std::array<float, 16>{
            s.x, u.x, -f.x, 0.0f,
            s.y, u.y, -f.y, 0.0f,
            s.z, u.z, -f.z, 0.0f,
            -dot(s, eye), -dot(u, eye), dot(f, eye), 1.0f,
        }};
    }

    [[nodiscard]] Mat4 operator*(const Mat4& rhs) const {
        Mat4 result;
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                float sum = 0.0f;
                for (int k = 0; k < 4; ++k) {
                    sum += (*this)(row, k) * rhs(k, col);
                }
                result(row, col) = sum;
            }
        }
        return result;
    }

    [[nodiscard]] float& operator()(int row, int col) {
        return data[col * 4 + row];
    }

    [[nodiscard]] float operator()(int row, int col) const {
        return data[col * 4 + row];
    }

    [[nodiscard]] const float* raw() const { return data.data(); }
};

} // namespace khc::math
