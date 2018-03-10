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

GameLogic::GameLogic(GameState* s, bool c) {
    state = s;
    last = Time::Zero;
    cheat = c;
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
    double max_speed = acc * acc_time;

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
            state->player.v.y -= grav_acc * elapsed;
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
    case (LEFT):
        if (state->player.v.x > 0)
        state->player.v.x = 0;
        state->player.v.x -= acc * elapsed;
        break;
    case (RIGHT):
        if (state->player.v.x < 0)
        state->player.v.x = 0;
        state->player.v.x += acc * elapsed;
        break;
    case (NONE):
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


    // Check for collisions.
    // TODO get powerups
    vec2 pos_foot_1 = state->player.pos;
    pos_foot_1.x -= 0.25;
    pos_foot_1.y -= 0.5;
    vec2 pos_foot_2 = state->player.pos;
    pos_foot_2.x += 0.25;
    pos_foot_2.y -= 0.5;

    if ((state->level.collides(pos_foot_1)) || (state->level.collides(pos_foot_1)))
    {
        state->player.pos.y = 0;
        state->player.v.y = 0;
        state->player.inair = false;
        state->player.jump_count = 0;
    }

    // DEBUG
    if (state->player.pos.y < 0)
    {
        state->player.pos.y = 0;
        state->player.v.y = 0;
        state->player.inair = false;
        state->player.jump_count = 0;
    }

    last = current;
}

void GameLogic::keyPressed(sf::Keyboard::Key key) {
    switch (key)
    {
    case (sf::Keyboard::Space):
        state->player.jumping = true;
        break;
    case (sf::Keyboard::Escape):
        state->ingame = false;
        break;
    }
    if (cheat)
    {
        switch (key)
        {
        case (sf::Keyboard::Q):
            state->player.speedPower += 0.1;
            if (state->player.speedPower > 1) state->player.speedPower = 1;
            break;
        case (sf::Keyboard::A):
            state->player.speedPower -= 0.1;
            if (state->player.speedPower < 1) state->player.speedPower = 1;
            break;
        case (sf::Keyboard::W):
            state->player.forwardPower += 0.1;
            if (state->player.forwardPower > 1) state->player.forwardPower = 1;
            break;
        case (sf::Keyboard::S):
            state->player.forwardPower -= 0.1;
            if (state->player.forwardPower < 1) state->player.forwardPower = 1;
            break;
        case (sf::Keyboard::E):
            state->player.upwardPower += 0.1;
            if (state->player.upwardPower > 1) state->player.upwardPower = 1;
            break;
        case (sf::Keyboard::D):
            state->player.upwardPower -= 0.1;
            if (state->player.upwardPower < 1) state->player.upwardPower = 1;
            break;
        case (sf::Keyboard::R):
            state->player.brainfreeze += 0.1;
            if (state->player.brainfreeze > 1) state->player.brainfreeze = 1;
            break;
        case (sf::Keyboard::F):
            state->player.brainfreeze -= 0.1;
            if (state->player.brainfreeze < 1) state->player.brainfreeze = 1;
            break;
        }
    }
}
