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
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
#include <Powerup.h>
#include <math.h>

using namespace std;
using namespace sf;

BackgroundMusic::BackgroundMusic(string fn_menu, string fn_normal, string fn_dope, string fn_slow) {
    menu.openFromFile(fn_menu);
    menu.setLoop(true);
    normal.openFromFile(fn_normal);
    normal.setLoop(true);
    dope.openFromFile(fn_dope);
    dope.setLoop(true);
    slow.openFromFile(fn_slow);
    slow.setLoop(true);
}

GameLogic::GameLogic(GameState* s, bool c, BackgroundMusic *m) {
    state = s;
    last = Time::Zero;
    cheat = c;
    music = m;
}

void GameLogic::run()
{
    if (!state->player.initialized)
    {
        for (int i = 0; i < state->level.layers.size(); i++)
        {
            for (int j = 0; j < state->level.layers[i].size(); j++) {
                const Block* b = state->level.layers[i][j];
                if (b != nullptr)
                {
                    if (b->powerupType == PowerupType::PlayerStart)
                    {
                        state->level.layers[i][j] = nullptr;
                        state->player.initialized = true;
                        state->player.pos.x = i;
                        state->player.pos.y = j;
                    }
                }
            }
        }
    }
    if (last == Time::Zero) {
        last = state->timer.getElapsedTime();
        return;
    }

    // Check direction keystate.
    bool left = Keyboard::isKeyPressed(Keyboard::Left);
    bool right = Keyboard::isKeyPressed(Keyboard::Right);
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
    double acc = acc_scale * (1. + state->player.speedPower);
    if (state->player.pizza)
    {
        if (state->player.pizza_start == Time::Zero)
        {
            state->player.pizza_start = current;
        }
        double pizza_elapsed = (current - state->player.pizza_start).asSeconds();
        if (pizza_elapsed > pizza_time)
        {
            state->player.pizza = false;
            state->player.pizza_start = Time::Zero;
        } else {
            acc *= pizza_speed;
        }
    }
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
    if (state->player.schnitzel) {
        if (state->player.schnitzel_start == Time::Zero)
        {
            state->player.schnitzel_start = current;
        }
        double schnitzel_elapsed = (current - state->player.schnitzel_start).asSeconds();
        if (schnitzel_elapsed > schnitzel_time)
        {
            state->player.schnitzel = false;
            state->player.schnitzel_start = Time::Zero;
        } else {
            state->player.v.x = 0;
        }
    }
    state->player.pos += state->player.v * elapsed;


    // Check for collisions.
    double x = state->player.pos.x;
    double y = state->player.pos.y;
    bool col_left = state->level.collides(x, y + 0.5)
        || state->level.collides(x, y + 0.9)
        || state->level.collides(x, y + 0.1);
    bool col_right = state->level.collides(x + 1, y + 0.5)
        || state->level.collides(x + 1, y + 0.1)
        || state->level.collides(x + 1, y + 0.9);
    bool col_top = state->level.collides(x + 0.5, y + 1)
        || state->level.collides(x + 0.1, y + 1)
        || state->level.collides(x + 0.9, y + 1);
    bool col_bottom = state->level.collides(x + 0.5, y - 0.01)
        || state->level.collides(x + 0.1, y - 0.01)
        || state->level.collides(x + 0.9, y - 0.01);
    int x_i = floor(x + 0.5);
    int y_i = floor(y + 0.5);
    if (col_left && (state->player.v.x < 0))
    {
        state->player.v.x = 0;
    }
    if (col_right && (state->player.v.x > 0))
    {
        state->player.v.x = 0;
    }
    if (col_top && (state->player.v.y > 0))
    {
        state->player.v.y = 0;
    }
    if (col_bottom && (state->player.v.y < 0))
    {
        state->player.v.y = 0;
        state->player.inair = false;
        state->player.jump_count = 0;
        state->player.pos.y = y_i;
    }
    if (!(col_left || col_right || col_top || col_bottom))
    {
        state->player.inair = true;
    }

    if (state->player.pos.y < 0)
    {
        state->ingame = false;
        state->player.pos.x = 10;
        state->player.pos.y = 10;
    }

    double x_center = x_i + 0.5;
    double y_center = y_i + 0.5;
    if ((x_center >= x) && (x_center <= x + 1) && (y_center >= y) && (y_center <= y + 1))
    {
        if (state->level.layers.size() > x_i)
        {
            if (state->level.layers[x_i].size() > y_i)
            {
                const Block* b = state->level.layers[x_i][y_i];
                if (b != nullptr)
                {
                    switch (b->powerupType)
                    {
                    case (SpeedPowerup):
                        state->player.speedPower += 0.1;
                        if (state->player.speedPower > 1) state->player.speedPower = 1;
                        state->level.layers[x_i][y_i] = nullptr;
                        break;
                    case (JumpForwardPowerup):
                        state->player.forwardPower += 0.1;
                        if (state->player.forwardPower > 1) state->player.forwardPower = 1;
                        state->level.layers[x_i][y_i] = nullptr;
                        break;
                    case (JumpUpwardPowerup):
                        state->player.upwardPower += 0.1;
                        if (state->player.upwardPower > 1) state->player.upwardPower = 1;
                        state->level.layers[x_i][y_i] = nullptr;
                        break;
                    case (Schnitzel):
                        state->player.schnitzel = true;
                        state->level.layers[x_i][y_i] = nullptr;
                        break;
                    case (Pizza):
                        state->player.pizza = true;
                        state->level.layers[x_i][y_i] = nullptr;
                        break;
                    case (IceCream):
                        state->player.brainfreeze += 0.1;
                        if (state->player.brainfreeze > 1) state->player.brainfreeze = 1;
                        state->level.layers[x_i][y_i] = nullptr;
                        break;
                    case (Money):
                        // TODO
                        break;
                    case (Alcohol):
                        // TODO
                        break;
                    }
                }
            }
        }
    }

    // Decrease powerups.
    state->player.speedPower -= power_decrease * elapsed;
    if (state->player.speedPower < 0) state->player.speedPower = 0;
    state->player.forwardPower -= power_decrease * elapsed;
    if (state->player.forwardPower < 0) state->player.forwardPower = 0;
    state->player.upwardPower -= power_decrease * elapsed;
    if (state->player.upwardPower < 0) state->player.upwardPower = 0;

    // Update music.
    if (music->menu.getStatus() == Music::Status::Playing) {
        music->menu.stop();
    }
    if (state->player.pizza || state->player.schnitzel) {
        if (music->normal.getStatus() == Music::Status::Playing) {
            music->normal.pause();
        }
        if (music->dope.getStatus() == Music::Status::Playing) {
            music->dope.pause();
        }
        if (music->slow.getStatus() != Music::Status::Playing) {
            music->slow.play();
        }
    } else if ((state->player.speedPower + state->player.forwardPower + state->player.upwardPower) > 1.5) {
        if (music->normal.getStatus() == Music::Status::Playing) {
            music->normal.pause();
        }
        if (music->dope.getStatus() != Music::Status::Playing) {
            music->dope.play();
        }
        if (music->slow.getStatus() == Music::Status::Playing) {
            music->slow.pause();
        }
    } else {
        if (music->normal.getStatus() != Music::Status::Playing) {
            music->normal.play();
        }
        if (music->dope.getStatus() == Music::Status::Playing) {
            music->dope.pause();
        }
        if (music->slow.getStatus() == Music::Status::Playing) {
            music->slow.pause();
        }
    }

    last = current;
}

void GameLogic::keyPressed(Keyboard::Key key) {
    switch (key)
    {
    case (Keyboard::Space):
        state->player.jumping = true;
        break;
    case (Keyboard::Escape):
        state->ingame = false;
        music->normal.stop();
        music->dope.stop();
        music->slow.stop();
        music->menu.play();
        break;
    }
    if (cheat)
    {
        switch (key)
        {
        case (Keyboard::Q):
            state->player.speedPower += 0.1;
            if (state->player.speedPower > 1) state->player.speedPower = 1;
            break;
        case (Keyboard::A):
            state->player.speedPower -= 0.1;
            if (state->player.speedPower < 0) state->player.speedPower = 0;
            break;
        case (Keyboard::W):
            state->player.forwardPower += 0.1;
            if (state->player.forwardPower > 1) state->player.forwardPower = 1;
            break;
        case (Keyboard::S):
            state->player.forwardPower -= 0.1;
            if (state->player.forwardPower < 0) state->player.forwardPower = 0;
            break;
        case (Keyboard::E):
            state->player.upwardPower += 0.1;
            if (state->player.upwardPower > 1) state->player.upwardPower = 1;
            break;
        case (Keyboard::D):
            state->player.upwardPower -= 0.1;
            if (state->player.upwardPower < 0) state->player.upwardPower = 0;
            break;
        case (Keyboard::R):
            state->player.brainfreeze += 0.1;
            if (state->player.brainfreeze > 1) state->player.brainfreeze = 1;
            break;
        case (Keyboard::F):
            state->player.brainfreeze -= 0.1;
            if (state->player.brainfreeze < 0) state->player.brainfreeze = 0;
            break;
        case (Keyboard::T):
            state->player.pizza = true;
            break;
        case (Keyboard::G):
            state->player.schnitzel = true;
            break;
        }
    }
}
