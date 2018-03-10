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

#include "level_importer.h"

#include <fstream>
#include <map>

#include <string>
#include <sstream>
#include <vector>
#include <iterator>

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


enum ImporterState {
    BlockDescriptor,
    LevelDescriptor
};

void level_importer::LoadLevel(std::string& pathToFile, GfxManger& gfxManager) const
{
    std::ifstream infile(pathToFile);

    std::vector<Level> level;

    std::map<string, const Block*> blocks;
    ImporterState state;
    for(std::string line; getline( infile, line ); )
    {
        if (state == BlockDescriptor) {
            // name, path, w, h, solid
            // not efficient but maybe works
            std::vector<std::string> split_line = split(line, ' ');

            if (split_line.size() == 5) {
                auto& name = split_line[0];

                size_t w, h;
                bool solid;
                auto* block = gfxManager.LoadBlock(name, split_line[1], w, h, solid);
                blocks[name] = block;
            }
        }
    }

}
