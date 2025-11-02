#include "kharoscraft/voxel/Noise.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>

namespace khc::voxel {

PerlinNoise::PerlinNoise(std::uint32_t seed) {
    std::array<int, 256> p;
    std::iota(p.begin(), p.end(), 0);

    std::mt19937 generator(seed);
    std::shuffle(p.begin(), p.end(), generator);

    for (int i = 0; i < 256; ++i) {
        permutation_[i] = p[i];
        permutation_[i + 256] = p[i];
    }
}

float PerlinNoise::noise(float x, float y, float z) const {
    int xi = static_cast<int>(std::floor(x)) & 255;
    int yi = static_cast<int>(std::floor(y)) & 255;
    int zi = static_cast<int>(std::floor(z)) & 255;

    float xf = x - std::floor(x);
    float yf = y - std::floor(y);
    float zf = z - std::floor(z);

    float u = fade(xf);
    float v = fade(yf);
    float w = fade(zf);

    int aaa = permutation_[permutation_[permutation_[xi] + yi] + zi];
    int aba = permutation_[permutation_[permutation_[xi] + yi + 1] + zi];
    int aab = permutation_[permutation_[permutation_[xi] + yi] + zi + 1];
    int abb = permutation_[permutation_[permutation_[xi] + yi + 1] + zi + 1];
    int baa = permutation_[permutation_[permutation_[xi + 1] + yi] + zi];
    int bba = permutation_[permutation_[permutation_[xi + 1] + yi + 1] + zi];
    int bab = permutation_[permutation_[permutation_[xi + 1] + yi] + zi + 1];
    int bbb = permutation_[permutation_[permutation_[xi + 1] + yi + 1] + zi + 1];

    float x1 = lerp(u, grad(aaa, xf, yf, zf), grad(baa, xf - 1.0f, yf, zf));
    float x2 = lerp(u, grad(aba, xf, yf - 1.0f, zf), grad(bba, xf - 1.0f, yf - 1.0f, zf));
    float y1 = lerp(v, x1, x2);

    x1 = lerp(u, grad(aab, xf, yf, zf - 1.0f), grad(bab, xf - 1.0f, yf, zf - 1.0f));
    x2 = lerp(u, grad(abb, xf, yf - 1.0f, zf - 1.0f), grad(bbb, xf - 1.0f, yf - 1.0f, zf - 1.0f));
    float y2 = lerp(v, x1, x2);

    return (lerp(w, y1, y2) + 1.0f) / 2.0f;
}

float PerlinNoise::fractal_noise(float x, float y, float z, int octaves, float persistence) const {
    float total = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;
    float max_value = 0.0f;

    for (int i = 0; i < octaves; ++i) {
        total += noise(x * frequency, y * frequency, z * frequency) * amplitude;
        max_value += amplitude;
        amplitude *= persistence;
        frequency *= 2.0f;
    }

    return total / max_value;
}

float PerlinNoise::fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float PerlinNoise::lerp(float t, float a, float b) {
    return a + t * (b - a);
}

float PerlinNoise::grad(int hash, float x, float y, float z) {
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

} // namespace khc::voxel
