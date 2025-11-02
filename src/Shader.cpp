#include "kharoscraft/render/Shader.hpp"

#include "kharoscraft/render/GLLoader.hpp"

#include <SDL2/SDL.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace khc::render {

namespace {

std::string read_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool check_shader_compile(GLuint shader, const std::string& name) {
    GLint status = 0;
    gl::GetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint log_length = 0;
        gl::GetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        std::string log(log_length, '\0');
        gl::GetShaderInfoLog(shader, log_length, nullptr, log.data());
        std::cerr << "Shader compilation failed (" << name << "):\n" << log << '\n';
        return false;
    }
    return true;
}

bool check_program_link(GLuint program) {
    GLint status = 0;
    gl::GetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint log_length = 0;
        gl::GetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
        std::string log(log_length, '\0');
        gl::GetProgramInfoLog(program, log_length, nullptr, log.data());
        std::cerr << "Program link failed:\n" << log << '\n';
        return false;
    }
    return true;
}

} // namespace

Shader::~Shader() {
    destroy();
}

Shader::Shader(Shader&& other) noexcept {
    program_id_ = other.program_id_;
    vertex_path_ = std::move(other.vertex_path_);
    fragment_path_ = std::move(other.fragment_path_);
    other.program_id_ = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        destroy();
        program_id_ = other.program_id_;
        vertex_path_ = std::move(other.vertex_path_);
        fragment_path_ = std::move(other.fragment_path_);
        other.program_id_ = 0;
    }
    return *this;
}

bool Shader::load_from_files(const std::string& vertex_path, const std::string& fragment_path) {
    vertex_path_ = vertex_path;
    fragment_path_ = fragment_path;
    return compile();
}

bool Shader::reload() {
    return compile();
}

bool Shader::compile() {
    destroy();

    std::string vertex_source;
    std::string fragment_source;
    try {
        vertex_source = read_file(vertex_path_);
        fragment_source = read_file(fragment_path_);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return false;
    }

    GLuint vertex_shader = gl::CreateShader(GL_VERTEX_SHADER);
    const char* vertex_data = vertex_source.c_str();
    gl::ShaderSource(vertex_shader, 1, &vertex_data, nullptr);
    gl::CompileShader(vertex_shader);
    if (!check_shader_compile(vertex_shader, vertex_path_)) {
        gl::DeleteShader(vertex_shader);
        return false;
    }

    GLuint fragment_shader = gl::CreateShader(GL_FRAGMENT_SHADER);
    const char* fragment_data = fragment_source.c_str();
    gl::ShaderSource(fragment_shader, 1, &fragment_data, nullptr);
    gl::CompileShader(fragment_shader);
    if (!check_shader_compile(fragment_shader, fragment_path_)) {
        gl::DeleteShader(vertex_shader);
        gl::DeleteShader(fragment_shader);
        return false;
    }

    program_id_ = gl::CreateProgram();
    gl::AttachShader(program_id_, vertex_shader);
    gl::AttachShader(program_id_, fragment_shader);
    gl::LinkProgram(program_id_);

    gl::DeleteShader(vertex_shader);
    gl::DeleteShader(fragment_shader);

    if (!check_program_link(program_id_)) {
        destroy();
        return false;
    }

    return true;
}

void Shader::destroy() {
    if (program_id_ != 0) {
        gl::DeleteProgram(program_id_);
        program_id_ = 0;
    }
}

void Shader::use() const {
    gl::UseProgram(program_id_);
}

void Shader::set_uniform_mat4(std::string_view name, const float* value) const {
    std::string uniform{name};
    GLint location = gl::GetUniformLocation(program_id_, uniform.c_str());
    if (location >= 0) {
        gl::UniformMatrix4fv(location, 1, GL_FALSE, value);
    }
}

void Shader::set_uniform_vec3(std::string_view name, float x, float y, float z) const {
    std::string uniform{name};
    GLint location = gl::GetUniformLocation(program_id_, uniform.c_str());
    if (location >= 0) {
        gl::Uniform3f(location, x, y, z);
    }
}

void Shader::set_uniform_float(std::string_view name, float value) const {
    std::string uniform{name};
    GLint location = gl::GetUniformLocation(program_id_, uniform.c_str());
    if (location >= 0) {
        gl::Uniform1f(location, value);
    }
}

} // namespace khc::render
