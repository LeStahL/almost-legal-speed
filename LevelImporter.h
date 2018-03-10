// Almost Legal Speed - platform running game
// Copyright (C) 2018  Christoph Hess <christoph.s.hess@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef LEVEL_IMPORTER_H
#define LEVEL_IMPORTER_H

#include <memory>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Powerup.h"
using namespace sf;
using namespace std;


class Block {
public:
    char id;
    std::string pathToFile;
    size_t width, heigth;
    bool solid;
    PowerupType powerupType;
    shared_ptr<Texture> texture;

    Block(char _name, std::string _pathToFile, size_t _width, size_t _heigth, bool _solid, PowerupType p);
//     Block() = delete;
//     Block(const Block &other) = delete;
//     Block(const Block &&other) = delete;
    
    void init();
};



class Level {
public:
    bool definePlayerPosition;
    size_t id;
    std::vector<const Block*> blocks;
    std::vector<std::vector<const Block*>> layers;

    Level() { }
    Level(const Level& other) :
        definePlayerPosition(other.definePlayerPosition),
        id(other.id),
        blocks(other.blocks),
        layers(other.layers)
    {
    }

    Level(size_t id, std::vector<std::vector<const Block*>> layers);
    Level(size_t id, std::vector<const Level*>& list);

    void addLevel(Level& layers);

    bool collides(double, double);
};


class GfxManager {
public:
    GfxManager() { }
    ~GfxManager() {
        for(int i=0; i<blocks.size(); ++i)
            delete blocks[i];
    }

    Block* loadBlock(char name, std::string& pathToFile, size_t w, size_t h, bool solid, PowerupType type);
    std::vector<Block*> blocks;
};


class LevelImporter
{
public:
    std::map<size_t, Level> levels;
    std::map<char, Block*> blocks;

    LevelImporter() {
        // constraint
        blocks[' '] = nullptr;
    }

    const Level* loadLevel(std::string& pathToFile, GfxManager& gfxManager);
};
#endif
