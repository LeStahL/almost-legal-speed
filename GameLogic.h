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

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <GameState.h>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>

class GameLogic {
public:
    GameLogic(GameState*);
    virtual ~GameLogic() = default;

    void run();
    void keyPressed(sf::Keyboard::Key);

private:
    GameState* state;
    Time last;
    const double grav_acc = 0.003;
    const double jump_speed_x = 0.4;
    const double jump_speed_y = 0.6;
    const double acc_scale = 0.05;
};

#endif
