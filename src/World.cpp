#include "World.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <stdexcept>

namespace {
float hashedNoise(int x, int y, unsigned int seed) {
    std::uint32_t h = static_cast<std::uint32_t>(seed);
    h ^= static_cast<std::uint32_t>(x) * 0x27d4eb2dU;
    h = (h << 15U) | (h >> 17U);
    h ^= static_cast<std::uint32_t>(y) * 0x85ebca6bU;
    h ^= (h >> 13U);
    h *= 0xc2b2ae35U;
    h ^= (h >> 16U);
    return static_cast<float>(h & 0xFFFFFFU) / static_cast<float>(0xFFFFFFU);
}

float valueNoise(float x, float y, unsigned int seed) {
    int xi = static_cast<int>(std::floor(x));
    int yi = static_cast<int>(std::floor(y));
    float tx = x - static_cast<float>(xi);
    float ty = y - static_cast<float>(yi);

    float c00 = hashedNoise(xi, yi, seed);
    float c10 = hashedNoise(xi + 1, yi, seed);
    float c01 = hashedNoise(xi, yi + 1, seed);
    float c11 = hashedNoise(xi + 1, yi + 1, seed);

    float nx0 = std::lerp(c00, c10, tx);
    float nx1 = std::lerp(c01, c11, tx);
    return std::lerp(nx0, nx1, ty);
}

float fractalNoise(float x, float y, unsigned int seed) {
    float total = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;
    float maxValue = 0.0f;

    for (int octave = 0; octave < 5; ++octave) {
        total += valueNoise(x * frequency, y * frequency, seed + static_cast<unsigned int>(octave) * 53U) * amplitude;
        maxValue += amplitude;
        amplitude *= 0.5f;
        frequency *= 2.0f;
    }

    return total / maxValue;
}
} // namespace

World::World(int width, int height, unsigned int seed)
    : m_width(width), m_height(height), m_seed(seed) {
    if (width <= 0 || height <= 0) {
        throw std::runtime_error("World dimensions must be positive");
    }
    generate(seed);
}

void World::regenerate(int width, int height, unsigned int seed) {
    if (width <= 0 || height <= 0) {
        throw std::runtime_error("World dimensions must be positive");
    }
    m_width = width;
    m_height = height;
    m_seed = seed;
    generate(seed);
}

World::TileType World::tileAt(int x, int y) const {
    if (x < 0 || y < 0 || x >= m_width || y >= m_height) {
        return TileType::Mountain;
    }
    return m_tiles[static_cast<std::size_t>(y) * static_cast<std::size_t>(m_width) + static_cast<std::size_t>(x)];
}

bool World::isWalkable(int x, int y) const {
    if (x < 0 || y < 0 || x >= m_width || y >= m_height) {
        return false;
    }

    TileType tile = tileAt(x, y);
    return tile == TileType::Grass || tile == TileType::Forest || tile == TileType::Sand;
}

void World::generate(unsigned int seed) {
    m_tiles.assign(static_cast<std::size_t>(m_width) * static_cast<std::size_t>(m_height), TileType::Water);

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            float nx = static_cast<float>(x) / static_cast<float>(std::max(1, m_width - 1));
            float ny = static_cast<float>(y) / static_cast<float>(std::max(1, m_height - 1));

            float elevation = fractalNoise(nx * 6.0f, ny * 6.0f, seed);
            float moisture = fractalNoise(nx * 4.0f + 128.0f, ny * 4.0f + 256.0f, seed ^ 0x5bd1e995U);

            float dx = nx * 2.0f - 1.0f;
            float dy = ny * 2.0f - 1.0f;
            float distance = std::sqrt(dx * dx + dy * dy);
            float falloff = std::clamp(1.0f - distance * distance, 0.0f, 1.0f);

            elevation = std::clamp(elevation * 0.6f + falloff * 0.4f, 0.0f, 1.0f);

            TileType tile = TileType::Grass;

            if (elevation < 0.28f) {
                tile = TileType::Water;
            } else if (elevation < 0.34f) {
                tile = TileType::Sand;
            } else if (elevation < 0.66f) {
                tile = moisture > 0.55f ? TileType::Forest : TileType::Grass;
            } else if (elevation < 0.82f) {
                tile = moisture > 0.6f ? TileType::Forest : TileType::Grass;
            } else {
                tile = TileType::Mountain;
            }

            m_tiles[static_cast<std::size_t>(y) * static_cast<std::size_t>(m_width) + static_cast<std::size_t>(x)] = tile;
        }
    }
}
