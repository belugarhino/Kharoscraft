#pragma once

#include <cstddef>
#include <vector>

namespace khc::render {

struct Vertex {
    float position[3];
    float normal[3];
    float color[3];
};

class Mesh {
public:
    Mesh() = default;
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void upload(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    void draw() const;
    [[nodiscard]] bool empty() const { return index_count_ == 0; }

private:
    void destroy();

    unsigned int vao_{0};
    unsigned int vbo_{0};
    unsigned int ibo_{0};
    unsigned int index_count_{0};
};

} // namespace khc::render
