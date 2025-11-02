#pragma once

#include <cstddef>
#include <vector>

class World {
public:
    enum class TileType {
        Water,
        Sand,
        Grass,
        Forest,
        Mountain
    };

    static constexpr int TILE_SIZE = 32;

    World(int width = 128, int height = 128, unsigned int seed = 0);

    void regenerate(int width, int height, unsigned int seed);

    int width() const { return m_width; }
    int height() const { return m_height; }

    TileType tileAt(int x, int y) const;
    bool isWalkable(int x, int y) const;

private:
    void generate(unsigned int seed);

    int m_width{0};
    int m_height{0};
    unsigned int m_seed{0};
    std::vector<TileType> m_tiles;
};
