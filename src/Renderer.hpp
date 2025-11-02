#pragma once

#include <SDL.h>

#include "Player.hpp"
#include "World.hpp"

class Renderer2D {
public:
    Renderer2D() = default;

    void initialize(SDL_Renderer* renderer);
    void drawScene(const World& world, const Player& player, float timeOfDay,
                   int windowWidth, int windowHeight);

private:
    SDL_Color colorForTile(World::TileType type) const;

    SDL_Renderer* m_renderer{nullptr};
};
