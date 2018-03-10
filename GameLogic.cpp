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
#include <SFML/Graphics.hpp>

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

    bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    if (left)
    {
        if (right)
        {
            state->player.a = NONE;
        } else {
            state->player.a = LEFT;
        }
    } else {
        if (right)
        {
            state->player.a = RIGHT;
        } else {
            state->player.a = NONE;
        }
    }

    // Update speed vector.
    auto current = state->timer.getElapsedTime();
    double elapsed = (current - last).asSeconds();
    double acc = (1. + state->player.speedPower) * state->player.pizzaslow;
    double max_speed = acc * 1;
    if (state->player.jumping) {

    }

    if (state->player.inair) {
        state->player.v.y -= grav_acc;
    } else {
        state->player.v.y = 0;
        switch (state->player.a) {
        case(LEFT):
            state->player.v.x -= acc;
            break;
        case(RIGHT):
            state->player.v.x += acc;
            break;
        case(NONE):
            state->player.v.x = 0;
            break;
        }
        if (state->player.v.x > max_speed) {
            state->player.v.x = max_speed;
        } else if (state->player.v.x < -max_speed) {
            state->player.v.x = -max_speed;
        }
    }

    // Apply speed vector.
    if (!state->player.stuck) {
        state->player.pos += state->player.v * elapsed;
    }

    // Check for collision.
    // TODO
}

void GameLogic::keyPressed(sf::Keyboard::Key key) {
    switch (key)
    {
    case(sf::Keyboard::Space):
        state->player.jumping = true;
        break;
    }
}
