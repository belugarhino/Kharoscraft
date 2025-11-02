#version 410 core

in VS_OUT {
    vec3 normal;
    vec3 world_pos;
    vec3 color;
} fs_in;

uniform vec3 uLightDir;
uniform vec3 uCameraPos;

out vec4 fragColor;

void main() {
    vec3 normal = normalize(fs_in.normal);
    vec3 light_dir = normalize(-uLightDir);
    float diff = max(dot(normal, light_dir), 0.0);

    vec3 view_dir = normalize(uCameraPos - fs_in.world_pos);
    vec3 half_dir = normalize(light_dir + view_dir);
    float spec = pow(max(dot(normal, half_dir), 0.0), 32.0);

    vec3 ambient = fs_in.color * 0.25;
    vec3 diffuse = fs_in.color * diff;
    vec3 specular = vec3(0.25) * spec;

    fragColor = vec4(ambient + diffuse + specular, 1.0);
}
