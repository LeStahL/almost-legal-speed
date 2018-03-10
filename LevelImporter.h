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
#include <SFML/Graphics/Texture.hpp>

using namespace sf;
using namespace std;

class Block {
public:
    char id;
    std::string pathToFile;
    size_t width, heigth;
    bool solid;
    Texture texture;

    Block(char _name, std::string& _pathToFile, size_t _width, size_t _heigth, bool _solid)
        : id(_name), pathToFile(_pathToFile), width(_width), heigth(_heigth), solid(_solid), texture()
    {
        if (!texture.loadFromFile(pathToFile))
        {
            // Fail
            // FIXME throw an exception
            throw std::exception();
        }
    }
};

class Level {
public:
    bool definePlayerPosition;
    size_t id;
    std::vector<const Block*> blocks;
    std::vector<std::vector<const Block*>> level;

    Level() { }
    Level(size_t id, std::vector<std::vector<const Block*>> level);
    Level(size_t id, std::vector<const Level*>& list);

    void AddLevel(Level& level) const;
};


class GfxManger {
public:
    Block* LoadBlock(char name, std::string& pathToFile, size_t w, size_t h, bool solid) {
        for (size_t i = 0; i < blocks.size(); ++i)
        {
            auto& b = blocks[i];
            if (pathToFile.compare(b.pathToFile) == 0) {
                // same
                return &blocks[i];
            }
        }

        blocks.emplace_back(name, pathToFile, w, h, solid);
        return &blocks[blocks.size() - 1];

    }

    std::vector<Block> blocks;
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

    const Level* LoadLevel(std::string& pathToFile, GfxManger& gfxManager);
};
#endif
