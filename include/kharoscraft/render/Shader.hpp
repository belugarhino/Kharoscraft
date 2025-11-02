#pragma once

#include <string>
#include <string_view>

namespace khc::render {

class Shader {
public:
    Shader() = default;
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    bool load_from_files(const std::string& vertex_path, const std::string& fragment_path);
    bool reload();

    void use() const;

    void set_uniform_mat4(std::string_view name, const float* value) const;
    void set_uniform_vec3(std::string_view name, float x, float y, float z) const;
    void set_uniform_float(std::string_view name, float value) const;

    [[nodiscard]] unsigned int id() const { return program_id_; }

private:
    bool compile();
    void destroy();

    unsigned int program_id_{0};
    std::string vertex_path_;
    std::string fragment_path_;
};

} // namespace khc::render
