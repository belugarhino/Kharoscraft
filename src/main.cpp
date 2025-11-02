#include "kharoscraft/core/Clock.hpp"
#include "kharoscraft/core/Input.hpp"
#include "kharoscraft/core/Window.hpp"
#include "kharoscraft/math/Mat4.hpp"
#include "kharoscraft/render/Camera.hpp"
#include "kharoscraft/render/GLLoader.hpp"
#include "kharoscraft/render/Shader.hpp"
#include "kharoscraft/voxel/World.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>

using namespace khc;

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    core::Window window;
    core::WindowConfig config;
    config.title = "Kharoscraft";
    config.width = 1600;
    config.height = 900;

    if (!window.create(config)) {
        return 1;
    }

    if (!render::gl::load_gl_functions()) {
        std::cerr << "Failed to load OpenGL functions" << std::endl;
        return 1;
    }

    glViewport(0, 0, config.width, config.height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    render::Shader shader;
    if (!shader.load_from_files("assets/shaders/voxel.vert", "assets/shaders/voxel.frag")) {
        return 1;
    }

    render::Camera camera;
    camera.set_perspective(70.0f, static_cast<float>(config.width) / static_cast<float>(config.height), 0.1f, 512.0f);
    camera.set_position({0.0f, 80.0f, 0.0f});

    voxel::World world;
    world.generate_region(2);

    core::Input input;
    core::Clock clock;

    SDL_SetRelativeMouseMode(SDL_TRUE);

    bool running = true;
    while (running) {
        float dt = clock.tick();
        input.begin_frame();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            input.handle_event(event);
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F5) {
                shader.reload();
            } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                int width = event.window.data1;
                int height = event.window.data2;
                if (height == 0) {
                    height = 1;
                }
                glViewport(0, 0, width, height);
                camera.set_perspective(70.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 512.0f);
            }
        }

        const float mouse_sensitivity = 0.1f;
        camera.add_yaw(input.mouse_dx() * mouse_sensitivity);
        camera.add_pitch(-input.mouse_dy() * mouse_sensitivity);

        float speed = 30.0f;
        if (input.is_key_down(SDL_SCANCODE_LSHIFT)) {
            speed = 90.0f;
        }
        float velocity = speed * dt;

        math::Vec3 move_delta{0.0f, 0.0f, 0.0f};
        if (input.is_key_down(SDL_SCANCODE_W)) {
            move_delta += camera.forward();
        }
        if (input.is_key_down(SDL_SCANCODE_S)) {
            move_delta -= camera.forward();
        }
        if (input.is_key_down(SDL_SCANCODE_A)) {
            move_delta -= camera.right();
        }
        if (input.is_key_down(SDL_SCANCODE_D)) {
            move_delta += camera.right();
        }
        if (input.is_key_down(SDL_SCANCODE_SPACE)) {
            move_delta.y += 1.0f;
        }
        if (input.is_key_down(SDL_SCANCODE_LCTRL)) {
            move_delta.y -= 1.0f;
        }

        if (move_delta.length() > 0.0f) {
            camera.move(move_delta.normalized() * velocity);
        }

        glClearColor(0.45f, 0.67f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        const auto& projection = camera.projection_matrix();
        shader.set_uniform_mat4("uProjection", projection.raw());
        auto view = camera.view_matrix();
        shader.set_uniform_mat4("uView", view.raw());
        shader.set_uniform_vec3("uCameraPos", camera.position().x, camera.position().y, camera.position().z);
        shader.set_uniform_vec3("uLightDir", -0.3f, -1.0f, -0.4f);

        for (const auto& [position, chunk] : world.chunks()) {
            (void)position;
            chunk.mesh.draw();
        }

        window.swap_buffers();
    }

    SDL_SetRelativeMouseMode(SDL_FALSE);

    return 0;
}
