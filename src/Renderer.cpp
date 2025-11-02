#include "Renderer.hpp"

#include <algorithm>
#include <cmath>

#include "World.hpp"

void Renderer2D::initialize(SDL_Renderer* renderer) {
    m_renderer = renderer;
    if (m_renderer) {
        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
    }
}

void Renderer2D::drawScene(const World& world, const Player& player, float timeOfDay,
                           int windowWidth, int windowHeight) {
    if (!m_renderer) {
        return;
    }

    const int tileSize = World::TILE_SIZE;
    const int worldPixelWidth = world.width() * tileSize;
    const int worldPixelHeight = world.height() * tileSize;

    float playerPixelX = player.x() * tileSize;
    float playerPixelY = player.y() * tileSize;

    int cameraX = static_cast<int>(playerPixelX) - windowWidth / 2;
    int cameraY = static_cast<int>(playerPixelY) - windowHeight / 2;

    cameraX = std::clamp(cameraX, 0, std::max(0, worldPixelWidth - windowWidth));
    cameraY = std::clamp(cameraY, 0, std::max(0, worldPixelHeight - windowHeight));

    int startTileX = std::max(0, cameraX / tileSize);
    int startTileY = std::max(0, cameraY / tileSize);
    int endTileX = std::min(world.width(), (cameraX + windowWidth) / tileSize + 2);
    int endTileY = std::min(world.height(), (cameraY + windowHeight) / tileSize + 2);

    for (int y = startTileY; y < endTileY; ++y) {
        for (int x = startTileX; x < endTileX; ++x) {
            SDL_Rect rect{};
            rect.x = x * tileSize - cameraX;
            rect.y = y * tileSize - cameraY;
            rect.w = tileSize;
            rect.h = tileSize;

            SDL_Color color = colorForTile(world.tileAt(x, y));
            SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, 255);
            SDL_RenderFillRect(m_renderer, &rect);
        }
    }

    SDL_Rect playerRect{};
    playerRect.w = static_cast<int>(tileSize * 0.5f);
    playerRect.h = static_cast<int>(tileSize * 0.5f);
    playerRect.x = static_cast<int>(playerPixelX) - cameraX - playerRect.w / 2;
    playerRect.y = static_cast<int>(playerPixelY) - cameraY - playerRect.h / 2;

    SDL_SetRenderDrawColor(m_renderer, 255, 244, 163, 255);
    SDL_RenderFillRect(m_renderer, &playerRect);

    float daylight = 0.5f + 0.5f * std::sin(timeOfDay * 0.25f);
    daylight = std::clamp(daylight, 0.0f, 1.0f);
    Uint8 overlayAlpha = static_cast<Uint8>((1.0f - daylight) * 160.0f);

    if (overlayAlpha > 0) {
        SDL_SetRenderDrawColor(m_renderer, 10, 20, 45, overlayAlpha);
        SDL_Rect screen{0, 0, windowWidth, windowHeight};
        SDL_RenderFillRect(m_renderer, &screen);
    }

    SDL_Rect panel{20, 20, 220, 60};
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 120);
    SDL_RenderFillRect(m_renderer, &panel);

    SDL_Rect dayBar{panel.x + 12, panel.y + 20, 196, 12};
    SDL_SetRenderDrawColor(m_renderer, 40, 40, 60, 200);
    SDL_RenderFillRect(m_renderer, &dayBar);

    SDL_Rect fill = dayBar;
    fill.w = static_cast<int>(daylight * static_cast<float>(dayBar.w));
    SDL_SetRenderDrawColor(m_renderer, 255, 216, 102, 220);
    SDL_RenderFillRect(m_renderer, &fill);

    SDL_Rect indicator{panel.x + 12, panel.y + 40, 24, 12};
    SDL_SetRenderDrawColor(m_renderer, 120, 210, 255, 220);
    SDL_RenderFillRect(m_renderer, &indicator);
}

SDL_Color Renderer2D::colorForTile(World::TileType type) const {
    switch (type) {
    case World::TileType::Water:
        return SDL_Color{55, 134, 214, 255};
    case World::TileType::Sand:
        return SDL_Color{196, 178, 128, 255};
    case World::TileType::Grass:
        return SDL_Color{87, 181, 74, 255};
    case World::TileType::Forest:
        return SDL_Color{34, 120, 62, 255};
    case World::TileType::Mountain:
    default:
        return SDL_Color{150, 150, 160, 255};
    }
}
