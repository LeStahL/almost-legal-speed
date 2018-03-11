// Almost Legal Speed - platform running game
// Copyright (C) 2018  Alexander Kraus <nr4@z10.info>
//                     Daniel Hauck <mail@dhauck.eu>
//                     Jonas Blahut <darkphoenix@jbtec.eu>
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

#include "GameState.h"
#include <Powerup.h>

GameState::GameState(string lp)
: ingame(false), onscores(false), finished(false), levelpath(lp)
{

}

void GameState::reload()
{
    GfxManager* m = new GfxManager();
    LevelImporter* imp = new LevelImporter();
    std::string fn(levelpath);
    level = imp->loadLevel(fn, m);
    if (level == nullptr) {
        fprintf(stderr, "ERROR: Could not load startlevel.\n");
    }
    player.reset();
    finished = false;
    name = "";
    time = 0;
}

PowerupType GameState::getBlockType(int x, int y)
{
    if (level->layers.size() > x)
    {
        if (level->layers[x].size() > y)
        {
            const Block* b = level->layers[x][y];
            if (b != nullptr)
            {
                return b->powerupType;
            }
        }
    }
    return None;
}
