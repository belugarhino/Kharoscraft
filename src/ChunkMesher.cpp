#include "kharoscraft/voxel/ChunkMesher.hpp"

#include "kharoscraft/voxel/Block.hpp"

namespace khc::voxel {

namespace {

struct FaceDefinition {
    int dx;
    int dy;
    int dz;
    float normal[3];
    float corners[4][3];
};

constexpr FaceDefinition FACES[6] = {
    {1, 0, 0, {1.f, 0.f, 0.f}, {{1.f, 0.f, 0.f}, {1.f, 0.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 0.f}}},
    {-1, 0, 0, {-1.f, 0.f, 0.f}, {{0.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 1.f, 1.f}, {0.f, 0.f, 1.f}}},
    {0, 1, 0, {0.f, 1.f, 0.f}, {{0.f, 1.f, 0.f}, {1.f, 1.f, 0.f}, {1.f, 1.f, 1.f}, {0.f, 1.f, 1.f}}},
    {0, -1, 0, {0.f, -1.f, 0.f}, {{0.f, 0.f, 0.f}, {0.f, 0.f, 1.f}, {1.f, 0.f, 1.f}, {1.f, 0.f, 0.f}}},
    {0, 0, 1, {0.f, 0.f, 1.f}, {{0.f, 0.f, 1.f}, {0.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 0.f, 1.f}}},
    {0, 0, -1, {0.f, 0.f, -1.f}, {{0.f, 0.f, 0.f}, {1.f, 0.f, 0.f}, {1.f, 1.f, 0.f}, {0.f, 1.f, 0.f}}},
};

render::Vertex make_vertex(float x, float y, float z, const float* normal, const float* color) {
    render::Vertex vertex{};
    vertex.position[0] = x;
    vertex.position[1] = y;
    vertex.position[2] = z;
    vertex.normal[0] = normal[0];
    vertex.normal[1] = normal[1];
    vertex.normal[2] = normal[2];
    vertex.color[0] = color[0];
    vertex.color[1] = color[1];
    vertex.color[2] = color[2];
    return vertex;
}

void color_for_block(BlockType type, float (&out)[3]) {
    switch (type) {
    case BlockType::Grass:
        out[0] = 0.45f;
        out[1] = 0.72f;
        out[2] = 0.34f;
        break;
    case BlockType::Dirt:
        out[0] = 0.52f;
        out[1] = 0.32f;
        out[2] = 0.18f;
        break;
    case BlockType::Stone:
        out[0] = 0.6f;
        out[1] = 0.6f;
        out[2] = 0.62f;
        break;
    default:
        out[0] = out[1] = out[2] = 1.0f;
        break;
    }
}

} // namespace

MeshData ChunkMesher::build_mesh(const Chunk& chunk) const {
    MeshData data;
    data.vertices.reserve(4096);
    data.indices.reserve(6144);

    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int y = 0; y < CHUNK_HEIGHT; ++y) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                BlockType block = chunk.get(x, y, z);
                if (!is_solid(block)) {
                    continue;
                }

                float color[3];
                color_for_block(block, color);

                for (const auto& face : FACES) {
                    int nx = x + face.dx;
                    int ny = y + face.dy;
                    int nz = z + face.dz;

                    bool neighbor_solid = false;
                    if (nx >= 0 && nx < CHUNK_SIZE && ny >= 0 && ny < CHUNK_HEIGHT && nz >= 0 && nz < CHUNK_SIZE) {
                        neighbor_solid = is_solid(chunk.get(nx, ny, nz));
                    }

                    if (neighbor_solid) {
                        continue;
                    }

                    unsigned int start_index = static_cast<unsigned int>(data.vertices.size());
                    for (int i = 0; i < 4; ++i) {
                        const auto& corner = face.corners[i];
                        data.vertices.push_back(make_vertex(
                            static_cast<float>(x) + corner[0],
                            static_cast<float>(y) + corner[1],
                            static_cast<float>(z) + corner[2],
                            face.normal,
                            color));
                    }

                    data.indices.push_back(start_index + 0);
                    data.indices.push_back(start_index + 1);
                    data.indices.push_back(start_index + 2);
                    data.indices.push_back(start_index + 2);
                    data.indices.push_back(start_index + 3);
                    data.indices.push_back(start_index + 0);
                }
            }
        }
    }

    return data;
}

} // namespace khc::voxel
