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

#include <GameLogic.h>
#include <GameState.h>
#include <Player.h>
#include <SFML/System/Clock.hpp>

GameLogic::GameLogic(GameState* s) {
    state = s;
    last = Time::Zero;
}

void GameLogic::run()
{
    if (last == Time::Zero) {
        last = state->timer.getElapsedTime();
        return;
    }

    // Update speed vector.
    double acc = (1. + state->player.speedPower) * state->player.pizzaslow;
    switch (state->player.a) {
    case(LEFT):
        state->player.v.x -= acc;
        break;
    case(RIGHT):
        state->player.v.x += acc;
        break;
    }

    // Apply speed vector.
    auto current = state->timer.getElapsedTime();
    double elapsed = (current - last).asSeconds();
    state->player.pos += state->player.v * elapsed;
}
