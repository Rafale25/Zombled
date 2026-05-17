#pragma once

#include <string>
#include "uhcstd.hh"

namespace MapLoader {

    struct MapTile {
        int layer = 0;
        std::string texture = "default.png";
        int x = 0.0f;
        int y = 0.0f;
        int width = 100.0f;
        int height = 100.0f;
        float r = 1.0f;
        float g = 1.0f;
        float b = 1.0f;
        float a = 1.0f;
    };

    struct MapLayer {
        std::string name = "default";
        int gridSize = 100;
        int textureSize = 16;
        float r = 1.0f;
        float g = 1.0f;
        float b = 1.0f;
        bool visible = true;
        List::It<MapTile> tiles;
    };

    struct Map {
        List::It<MapLayer> layers;
    };

    Map load(std::string filePath);
}
