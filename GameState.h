// Almost Legal Speed - platform running game
// Copyright (C) 2018  Alexander Kraus <nr4@z10.info>
//                     Daniel Hauck <mail@dhauck.eu>
//                     Jonas Blahut <darkphoenix@jbtec.eu>
//
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

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>

#include <SFML/System/Clock.hpp>
#include <Player.h>

using namespace sf;

class Renderer;

class Block;

class Platform;
class Powerup;

class Level;

using namespace std;

class GameState
{
public:
    GameState();
    virtual ~GameState();

    bool jumping;

    Player player;

    Level level;
    Clock timer;
};

#endif
