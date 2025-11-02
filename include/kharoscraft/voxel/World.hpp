#pragma once

#include <memory>
#include <unordered_map>

#include "kharoscraft/render/Mesh.hpp"
#include "kharoscraft/voxel/Chunk.hpp"
#include "kharoscraft/voxel/ChunkMesher.hpp"
#include "kharoscraft/voxel/Noise.hpp"

namespace khc::voxel {

struct ChunkRenderData {
    Chunk chunk;
    render::Mesh mesh;
};

class World {
public:
    World();

    void generate_region(int radius);
    void rebuild_chunk_mesh(const ChunkPosition& position);

    [[nodiscard]] const std::unordered_map<ChunkPosition, ChunkRenderData, ChunkPositionHasher>& chunks() const {
        return chunks_;
    }

private:
    void generate_chunk(ChunkRenderData& render_data, const ChunkPosition& position);

    std::unordered_map<ChunkPosition, ChunkRenderData, ChunkPositionHasher> chunks_{};
    ChunkMesher mesher_{};
    PerlinNoise noise_;
};

} // namespace khc::voxel
