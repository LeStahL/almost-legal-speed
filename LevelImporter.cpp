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

#include "LevelImporter.h"

#include <fstream>
#include <map>

#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <math.h>

#include <Player.h>
#include <iostream>

#include <SFML/Graphics.hpp>

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

const Level* LevelImporter::loadLevel(std::string& pathToFile, GfxManager& gfxManager)
{
    std::ifstream infile(pathToFile);
    Level* current_level = nullptr;
    Level* start_level = nullptr;

    std::vector<size_t> added_levels;

    for(std::string line; getline( infile, line ); )
    {
        if (line.find("block") == 0)
        {
            // name, path, w, h, solid
            // not efficient but maybe works
            std::vector<std::string> split_line = split(line, ' ');

            if (split_line.size() == 7) {
                char name = split_line[1][0];

                size_t w = stoull(split_line[3]), h = stoull(split_line[4]);
                bool solid = (bool)stoi(split_line[5]);

                PowerupType type = static_cast<PowerupType>(stoul(split_line[6]));
                Block* block = gfxManager.loadBlock(name, split_line[2], w, h, solid, type);
                blocks.insert ( std::pair<char, Block*>(name,block) );
            } else {
                cout<< "wrong count of parameters in " << line << endl;
            }
        } else if (line.find("level") == 0)
        {
            size_t id = stoull(line.substr(line.find_first_of(" "), line.size()));
            added_levels.push_back(id);
            current_level = &levels[id];
        } else if (line.find("clevel") == 0)
        {
            // clevel id <ids>
            std::vector<std::string> split_line = split(line, ' ');
            if (split_line.size() < 4)
                continue;

            size_t id = stoull(split_line[1]);
            Level& level = levels[id];
            level.id = id;
            for (size_t i = 2; i < split_line.size(); ++i) {
                size_t cid = stoull(split_line[i]);
                level.addLevel(levels[cid]);
            }
        } else if (line.find("slevel") == 0)
        {
            size_t id = stoull(line.substr(7, line.size()));
            start_level = &levels[id];
        } else if (current_level != nullptr){
            for (size_t i = 0; i < line.size(); ++i) {
                current_level->layers.resize(std::max(current_level->layers.size(), line.size()));
                current_level->layers[i].push_back(blocks[line[i]]);
            }
        }
    }

    for (auto id : added_levels) {
        Level& level = levels[id];

        for (size_t i = 0; i < level.layers.size(); ++i) {
            level.layers[i] = std::vector<const Block*>(level.layers[i].rbegin(),level.layers[i].rend());
        }

    }
    return start_level;
}

Block::Block(char _name, string &_pathToFile, size_t _width, size_t _heigth, bool _solid, PowerupType p)
    : id(_name), pathToFile(_pathToFile), width(_width), heigth(_heigth), solid(_solid), powerupType(p), texture()
{
    if (!texture.loadFromFile(pathToFile))
    {
        // Fail
        // FIXME throw an exception
        fprintf(stderr, "could not load texture file from %s", pathToFile.c_str());
    }
}

void Level::addLevel(Level &level)
{
    for (std::vector<const Block*>& row : level.layers) {
        this->layers.push_back(row);
    }
}

bool Level::collides(double xf, double yf)
{
    int x = floor(xf);
    int y = floor(yf);

    if (layers.size() <= x)
    {
        return false;
    }
    if (layers[x].size() <= y)
    {
        return false;
    }
    const Block* b = layers[x][y];
    if (b != nullptr)
    {
        return b->solid;
    }
    return false;
}

Block *GfxManager::loadBlock(char name, string &pathToFile, size_t w, size_t h, bool solid, PowerupType type) {
    for (size_t i = 0; i < blocks.size(); ++i)
    {
        auto& b = blocks[i];
        if (pathToFile.compare(b.pathToFile) == 0) {
            // same
            return &blocks[i];
        }
    }

    blocks.emplace_back(name, pathToFile, w, h, solid, type);
    return &blocks[blocks.size() - 1];

}
