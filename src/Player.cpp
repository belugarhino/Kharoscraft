#include "Player.hpp"

#include <algorithm>
#include <cmath>

#include "World.hpp"

Player::Player() = default;

void Player::setPosition(float x, float y) {
    m_position.x = x;
    m_position.y = y;
}

void Player::setSpeed(float speed) {
    m_speed = speed;
}

void Player::update(float deltaTime, const std::unordered_set<SDL_Scancode>& keys,
                    const World& world) {
    float dx = 0.0f;
    float dy = 0.0f;

    if (keys.contains(SDL_SCANCODE_W)) {
        dy -= 1.0f;
    }
    if (keys.contains(SDL_SCANCODE_S)) {
        dy += 1.0f;
    }
    if (keys.contains(SDL_SCANCODE_A)) {
        dx -= 1.0f;
    }
    if (keys.contains(SDL_SCANCODE_D)) {
        dx += 1.0f;
    }

    float length = std::sqrt(dx * dx + dy * dy);
    if (length > 0.0f) {
        dx /= length;
        dy /= length;
    }

    float sprintMultiplier = (keys.contains(SDL_SCANCODE_LSHIFT) || keys.contains(SDL_SCANCODE_RSHIFT)) ? 1.8f : 1.0f;

    float newX = m_position.x + dx * m_speed * sprintMultiplier * deltaTime;
    float newY = m_position.y + dy * m_speed * sprintMultiplier * deltaTime;

    if (world.isWalkable(static_cast<int>(std::floor(newX)), static_cast<int>(std::floor(m_position.y)))) {
        m_position.x = std::clamp(newX, 0.0f, static_cast<float>(world.width() - 1));
    }
    if (world.isWalkable(static_cast<int>(std::floor(m_position.x)), static_cast<int>(std::floor(newY)))) {
        m_position.y = std::clamp(newY, 0.0f, static_cast<float>(world.height() - 1));
    }
}
