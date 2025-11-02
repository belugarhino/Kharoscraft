#pragma once

#include <SDL.h>
#include <unordered_set>

class World;

class Player {
public:
    Player();

    void setPosition(float x, float y);
    void setSpeed(float speed);

    void update(float deltaTime, const std::unordered_set<SDL_Scancode>& keys,
                const World& world);

    float x() const { return m_position.x; }
    float y() const { return m_position.y; }
    float speed() const { return m_speed; }

private:
    SDL_FPoint m_position{0.0f, 0.0f};
    float m_speed{5.0f};
};
