#include "kharoscraft/voxel/Chunk.hpp"

#include <cassert>

namespace khc::voxel {

BlockType Chunk::get(int x, int y, int z) const {
    assert(x >= 0 && x < CHUNK_SIZE);
    assert(y >= 0 && y < CHUNK_HEIGHT);
    assert(z >= 0 && z < CHUNK_SIZE);
    return blocks_[index(x, y, z)];
}

void Chunk::set(int x, int y, int z, BlockType type) {
    assert(x >= 0 && x < CHUNK_SIZE);
    assert(y >= 0 && y < CHUNK_HEIGHT);
    assert(z >= 0 && z < CHUNK_SIZE);
    blocks_[index(x, y, z)] = type;
}

int Chunk::index(int x, int y, int z) {
    return y * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE + x;
}

} // namespace khc::voxel
