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

const Level* LevelImporter::LoadLevel(std::string& pathToFile, GfxManager& gfxManager)
{
    std::ifstream infile(pathToFile);
    Level* current_level = nullptr;
    Level* start_level = nullptr;
    for(std::string line; getline( infile, line ); )
    {
        cout << line << endl;
        if (line.find("block") == 0)
        {
            // name, path, w, h, solid
            // not efficient but maybe works
            std::vector<std::string> split_line = split(line, ' ');

            if (split_line.size() == 5) {
                char name = split_line[1][0];

                size_t w, h;
                bool solid;
                Block* block = gfxManager.LoadBlock(name, split_line[2], w, h, solid);
                blocks.insert ( std::pair<char, Block*>(name,block) );
            }
        } else if (line.find("level") == 0)
        {
            size_t id = stoull(line.substr(line.find_first_of(" "), line.size()));
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
                level.AddLevel(levels[cid]);
            }
        } else if (line.find("slevel") == 0)
        {
            size_t id = stoull(line.substr(7, line.size()));
            start_level = &levels[id];
        } else if (current_level != nullptr){
            for (size_t i = 0; i < line.size(); ++i) {
                current_level->level[i].push_back(blocks[line[i]]);
            }
        }

    }
    return start_level;

}

void Level::AddLevel(Level &level) const
{

}

bool Level::collides(vec2 pos)
{
    int x = floor(pos.x);
    int y = floor(pos.y);

    if (level.size() <= x)
    {
        return false;
    }
    if (level[x].size() <= y)
    {
        return false;
    }
    const Block* b = level[x][y];
    if (b != nullptr)
    {
        return b->solid;
    }
    return false;
}
