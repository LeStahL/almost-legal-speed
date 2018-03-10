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

    // Check direction keystate.
    bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    if (left)
    {
        if (right)
        {
            state->player.a = NONE;
        } else {
            state->player.a = LEFT;
            state->player.face = LEFT;
        }
    } else {
        if (right)
        {
            state->player.a = RIGHT;
            state->player.face = RIGHT;
        } else {
            state->player.a = NONE;
        }
    }

    // Update speed vector.
    auto current = state->timer.getElapsedTime();
    double elapsed = (current - last).asSeconds();
    double acc = acc_scale * (1. + state->player.speedPower) * state->player.pizzaslow;
    double max_speed = acc * 5;

    if (state->player.inair)
    {
        if (state->player.jumping)
        {
            if (state->player.jump_count < max_jumps * state->player.upwardPower)
            {
                state->player.jump_count++;
                state->player.v.y = jump_speed;
            }
            state->player.jumping = false;
        } else {
            state->player.v.y -= grav_acc;
        }
        max_speed *= 1 + state->player.forwardPower;
    } else {
        if (state->player.jumping)
        {
            state->player.inair = true;
            state->player.v.y = jump_speed;
            state->player.jumping = false;
        } else {
            state->player.v.y = 0;
        }
    }
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

    // Apply speed vector.
    if (!state->player.stuck) {
        state->player.pos += state->player.v * elapsed;
    }

    // DEBUG
    if (state->player.pos.y < 0)
    {
        state->player.pos.y = 0;
        state->player.v.y = 0;
        state->player.inair = false;
        state->player.jump_count = 0;
    }

    // Check for collision.
    // TODO
    // TODO reset double_jumped

    last = current;
}

void GameLogic::keyPressed(sf::Keyboard::Key key) {
    switch (key)
    {
    case(sf::Keyboard::Space):
        state->player.jumping = true;
        break;
    case(sf::Keyboard::Escape):
        state->ingame = false;
        break;
    }
}
