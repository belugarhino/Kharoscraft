#include "Game.hpp"

#include <chrono>
#include <iostream>
#include <numbers>
#include <random>

Game::Game() = default;

Game::~Game() {
    shutdown();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << '\n';
        return false;
    }

    m_window = SDL_CreateWindow("Kharoscraft", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                m_windowWidth, m_windowHeight, SDL_WINDOW_SHOWN);
    if (!m_window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << '\n';
        return false;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << '\n';
        return false;
    }

    m_renderer2D.initialize(m_renderer);

    std::random_device rd;
    unsigned int seed = rd();
    m_world.regenerate(160, 160, seed);

    m_player.setPosition(m_world.width() / 2.0f, m_world.height() / 2.0f);
    m_player.setSpeed(6.5f);

    m_timeOfDay = 0.0f;
    m_running = true;
    return true;
}

void Game::shutdown() {
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    SDL_Quit();
}

void Game::run() {
    if (!init()) {
        return;
    }

    Uint64 previousTicks = SDL_GetPerformanceCounter();
    const double frequency = static_cast<double>(SDL_GetPerformanceFrequency());

    while (m_running) {
        Uint64 currentTicks = SDL_GetPerformanceCounter();
        float deltaTime = static_cast<float>((currentTicks - previousTicks) / frequency);
        previousTicks = currentTicks;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            handleEvent(event);
        }

        update(deltaTime);
        render();
    }
}

void Game::handleEvent(const SDL_Event& event) {
    switch (event.type) {
    case SDL_QUIT:
        m_running = false;
        break;
    case SDL_KEYDOWN:
        if (event.key.repeat == 0) {
            m_pressedKeys.insert(event.key.keysym.scancode);
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            m_running = false;
        }
        break;
    case SDL_KEYUP:
        m_pressedKeys.erase(event.key.keysym.scancode);
        break;
    default:
        break;
    }
}

void Game::update(float deltaTime) {
    m_timeOfDay += deltaTime;
    const float cycle = std::numbers::pi_v<float> * 2.0f;
    if (m_timeOfDay > cycle) {
        m_timeOfDay -= cycle;
    }

    m_player.update(deltaTime, m_pressedKeys, m_world);
}

void Game::render() {
    if (!m_renderer) {
        return;
    }

    SDL_SetRenderDrawColor(m_renderer, 18, 28, 45, 255);
    SDL_RenderClear(m_renderer);

    m_renderer2D.drawScene(m_world, m_player, m_timeOfDay, m_windowWidth, m_windowHeight);

    SDL_RenderPresent(m_renderer);
}
