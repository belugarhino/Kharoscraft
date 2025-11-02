#pragma once

#include <vector>

#include "kharoscraft/render/Mesh.hpp"
#include "kharoscraft/voxel/Chunk.hpp"

namespace khc::voxel {

struct MeshData {
    std::vector<render::Vertex> vertices;
    std::vector<unsigned int> indices;
};

class ChunkMesher {
public:
    MeshData build_mesh(const Chunk& chunk) const;
};

} // namespace khc::voxel
