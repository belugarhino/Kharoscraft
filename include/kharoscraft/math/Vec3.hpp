#pragma once

#include <cmath>
#include <ostream>

namespace khc::math {

struct Vec3 {
    float x{0.0f};
    float y{0.0f};
    float z{0.0f};

    constexpr Vec3() = default;
    constexpr Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    [[nodiscard]] constexpr Vec3 operator+(const Vec3& rhs) const {
        return {x + rhs.x, y + rhs.y, z + rhs.z};
    }

    [[nodiscard]] constexpr Vec3 operator-(const Vec3& rhs) const {
        return {x - rhs.x, y - rhs.y, z - rhs.z};
    }

    [[nodiscard]] constexpr Vec3 operator*(float scalar) const {
        return {x * scalar, y * scalar, z * scalar};
    }

    [[nodiscard]] constexpr Vec3 operator/(float scalar) const {
        return {x / scalar, y / scalar, z / scalar};
    }

    constexpr Vec3& operator+=(const Vec3& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    constexpr Vec3& operator-=(const Vec3& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    constexpr Vec3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    [[nodiscard]] float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    [[nodiscard]] Vec3 normalized() const {
        float len = length();
        if (len <= 0.0f) {
            return {};
        }
        return {x / len, y / len, z / len};
    }
};

[[nodiscard]] inline float dot(const Vec3& a, const Vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

[[nodiscard]] inline Vec3 cross(const Vec3& a, const Vec3& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
    };
}

inline std::ostream& operator<<(std::ostream& os, const Vec3& vec) {
    os << "Vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}

} // namespace khc::math
