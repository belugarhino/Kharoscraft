#include "kharoscraft/voxel/World.hpp"

namespace khc::voxel {

World::World() : noise_(1337u) {}

void World::generate_region(int radius) {
    for (int x = -radius; x <= radius; ++x) {
        for (int z = -radius; z <= radius; ++z) {
            ChunkPosition position{x, z};
            auto& render_data = chunks_[position];
            generate_chunk(render_data, position);
            rebuild_chunk_mesh(position);
        }
    }
}

void World::rebuild_chunk_mesh(const ChunkPosition& position) {
    auto it = chunks_.find(position);
    if (it == chunks_.end()) {
        return;
    }

    MeshData mesh_data = mesher_.build_mesh(it->second.chunk);
    it->second.mesh.upload(mesh_data.vertices, mesh_data.indices);
}

void World::generate_chunk(ChunkRenderData& render_data, const ChunkPosition& position) {
    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int z = 0; z < CHUNK_SIZE; ++z) {
            float world_x = static_cast<float>(position.x * CHUNK_SIZE + x);
            float world_z = static_cast<float>(position.z * CHUNK_SIZE + z);
            float height_noise = noise_.fractal_noise(world_x * 0.01f, 0.0f, world_z * 0.01f, 4, 0.5f);
            int surface_height = static_cast<int>(height_noise * 40.0f) + 48;

            for (int y = 0; y < CHUNK_HEIGHT; ++y) {
                BlockType type = BlockType::Air;
                if (y <= surface_height) {
                    if (y == surface_height) {
                        type = BlockType::Grass;
                    } else if (y >= surface_height - 4) {
                        type = BlockType::Dirt;
                    } else {
                        type = BlockType::Stone;
                    }
                }

                render_data.chunk.set(x, y, z, type);
            }
        }
    }
}

} // namespace khc::voxel
