#include <fstream>
#include <sstream>
#include <vector>

#include "uhcio.hh"
#include "mapLoader.hpp"

namespace MapLoader {

    static std::vector<std::string> splitLine(const std::string& line, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream stream(line);
        
        while (std::getline(stream, token, delimiter))
            tokens.push_back(token);
            
        return tokens;
    }

    Map load(std::string filePath) {
        Map map;

        std::ifstream file(filePath);
        std::string line;

        while (std::getline(file, line)) {
            auto split = splitLine(line, '|');

            if (line.rfind("L", 0) == 0) {
                MapLayer layer;
                layer.name = split[1];
                layer.gridSize = std::stoi(split[2]);
                layer.textureSize = std::stoi(split[3]);
                layer.r = std::stof(split[4]);
                layer.g = std::stof(split[5]);
                layer.b = std::stof(split[6]);
                layer.visible = (split[8] == "true");
                List::add(map.layers, layer);
            }

            if (line.rfind("T", 0) == 0) {
                MapTile tile;
                tile.layer = std::stoi(split[1]);
                tile.texture = split[2];
                tile.x = std::stof(split[3]);
                tile.y = std::stof(split[4]);
                tile.width = std::stof(split[5]);
                tile.height = std::stof(split[6]);
                tile.r = std::stof(split[7]);
                tile.g = std::stof(split[8]);
                tile.b = std::stof(split[9]);
                tile.a = std::stof(split[10]);
                List::add(List::get(map.layers, tile.layer)->tiles, tile);
            }
        }

        Log::debug("Map %s loaded", filePath.c_str());
        return map;
    }
}
