#pragma once

#include <array>
#include <cstdint>

namespace khc::voxel {

class PerlinNoise {
public:
    explicit PerlinNoise(std::uint32_t seed = 0);

    [[nodiscard]] float noise(float x, float y, float z) const;
    [[nodiscard]] float fractal_noise(float x, float y, float z, int octaves, float persistence) const;

private:
    [[nodiscard]] static float fade(float t);
    [[nodiscard]] static float lerp(float t, float a, float b);
    [[nodiscard]] static float grad(int hash, float x, float y, float z);

    std::array<int, 512> permutation_{};
};

} // namespace khc::voxel
