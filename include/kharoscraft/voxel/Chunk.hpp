#pragma once

#include <array>
#include <vector>
#include <cstdint>

#include "kharoscraft/voxel/Block.hpp"

namespace khc::voxel {

constexpr int CHUNK_SIZE = 32;
constexpr int CHUNK_HEIGHT = 128;
constexpr int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE;

struct ChunkPosition {
    int x{0};
    int z{0};

    [[nodiscard]] bool operator==(const ChunkPosition& other) const {
        return x == other.x && z == other.z;
    }
};

struct ChunkPositionHasher {
    [[nodiscard]] std::size_t operator()(const ChunkPosition& pos) const noexcept {
        std::uint64_t ux = static_cast<std::uint64_t>(static_cast<std::uint32_t>(pos.x));
        std::uint64_t uz = static_cast<std::uint64_t>(static_cast<std::uint32_t>(pos.z));
        return static_cast<std::size_t>((ux << 32) ^ uz);
    }
};

class Chunk {
public:
    Chunk() = default;

    BlockType get(int x, int y, int z) const;
    void set(int x, int y, int z, BlockType type);

private:
    [[nodiscard]] static int index(int x, int y, int z);

    std::array<BlockType, CHUNK_VOLUME> blocks_{};
};

} // namespace khc::voxel
