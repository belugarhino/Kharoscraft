#include "kharoscraft/render/Mesh.hpp"

#include "kharoscraft/render/GLLoader.hpp"

namespace khc::render {

Mesh::~Mesh() {
    destroy();
}

Mesh::Mesh(Mesh&& other) noexcept {
    vao_ = other.vao_;
    vbo_ = other.vbo_;
    ibo_ = other.ibo_;
    index_count_ = other.index_count_;
    other.vao_ = 0;
    other.vbo_ = 0;
    other.ibo_ = 0;
    other.index_count_ = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        destroy();
        vao_ = other.vao_;
        vbo_ = other.vbo_;
        ibo_ = other.ibo_;
        index_count_ = other.index_count_;
        other.vao_ = 0;
        other.vbo_ = 0;
        other.ibo_ = 0;
        other.index_count_ = 0;
    }
    return *this;
}

void Mesh::upload(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    if (vertices.empty() || indices.empty()) {
        destroy();
        return;
    }

    if (vao_ == 0) {
        gl::GenVertexArrays(1, &vao_);
        gl::GenBuffers(1, &vbo_);
        gl::GenBuffers(1, &ibo_);
    }

    gl::BindVertexArray(vao_);

    gl::BindBuffer(GL_ARRAY_BUFFER, vbo_);
    gl::BufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), vertices.data(), GL_STATIC_DRAW);

    gl::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    gl::BufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)), indices.data(), GL_STATIC_DRAW);

    gl::EnableVertexAttribArray(0);
    gl::VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
    gl::EnableVertexAttribArray(1);
    gl::VertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
    gl::EnableVertexAttribArray(2);
    gl::VertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));

    gl::BindVertexArray(0);

    index_count_ = static_cast<unsigned int>(indices.size());
}

void Mesh::draw() const {
    if (vao_ == 0 || index_count_ == 0) {
        return;
    }

    gl::BindVertexArray(vao_);
    gl::DrawElements(GL_TRIANGLES, static_cast<GLsizei>(index_count_), GL_UNSIGNED_INT, nullptr);
    gl::BindVertexArray(0);
}

void Mesh::destroy() {
    if (vao_ != 0) {
        gl::DeleteVertexArrays(1, &vao_);
        vao_ = 0;
    }
    if (vbo_ != 0) {
        gl::DeleteBuffers(1, &vbo_);
        vbo_ = 0;
    }
    if (ibo_ != 0) {
        gl::DeleteBuffers(1, &ibo_);
        ibo_ = 0;
    }
    index_count_ = 0;
}

} // namespace khc::render
