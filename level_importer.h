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

#include <vector>
#include <SFML/Graphics/Texture.hpp>
#ifndef LEVEL_IMPORTER_H
#define LEVEL_IMPORTER_H


using namespace sf;
using namespace std;


class Block {
public:
    std::string name, pathToFile;
    size_t width, heigth;
    bool solid;
    Texture texture;

    Block(std::string& _name, std::string& _pathToFile, size_t _width, size_t _heigth, bool _solid)
        : name(_name), pathToFile(_pathToFile), width(_width), heigth(_heigth), solid(_solid)
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
    std::vector<const Block*> blocks;
    std::vector<std::vector<const Block*>> level;
};


class GfxManger {
public:
    const Block* LoadBlock(std::string& name, std::string& pathToFile, size_t w, size_t h, bool solid) {
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


class level_importer
{
public:
    level_importer() {}

    void LoadLevel(std::string& pathToFile, GfxManger& gfxManager) const;
};
#endif
