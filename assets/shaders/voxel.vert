#version 410 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;

uniform mat4 uProjection;
uniform mat4 uView;

out VS_OUT {
    vec3 normal;
    vec3 world_pos;
    vec3 color;
} vs_out;

void main() {
    vec4 world_pos = vec4(aPosition, 1.0);
    vs_out.world_pos = world_pos.xyz;
    vs_out.normal = aNormal;
    vs_out.color = aColor;
    gl_Position = uProjection * uView * world_pos;
}
