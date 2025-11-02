#pragma once

#include <unordered_set>

#include <SDL.h>

#include "Player.hpp"
#include "Renderer.hpp"
#include "World.hpp"

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    bool init();
    void shutdown();

    void handleEvent(const SDL_Event& event);
    void update(float deltaTime);
    void render();

    bool m_running{false};
    int m_windowWidth{1280};
    int m_windowHeight{720};

    SDL_Window* m_window{nullptr};
    SDL_Renderer* m_renderer{nullptr};

    World m_world;
    Player m_player;
    Renderer2D m_renderer2D;

    std::unordered_set<SDL_Scancode> m_pressedKeys;
    float m_timeOfDay{0.0f};
};
