#pragma once

#include <cstdint>

namespace khc::voxel {

enum class BlockType : std::uint8_t {
    Air = 0,
    Grass,
    Dirt,
    Stone,
};

inline bool is_solid(BlockType type) {
    return type != BlockType::Air;
}

} // namespace khc::voxel
