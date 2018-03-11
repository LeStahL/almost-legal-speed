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
#include <curl/curl.h>

using namespace std;
using namespace sf;

BackgroundMusic::BackgroundMusic(vector<pair<string, double>> d)
{
    music.clear();
    measure_time.clear();
    for (auto p: d)
    {
        Music* m = new Music();
        m->openFromFile(get<0>(p));
        m->setLoop(true);
        music.push_back(m);
        measure_time.push_back(get<1>(p));
    }
    last_i = -1;
}


void BackgroundMusic::update(Time current)
{
    int i = music_index % music.size();
    if (i == last_i) return;

    Music* m = music.at(i);
    double time = m->getPlayingOffset().asSeconds();
    int count = floor(time / measure_time.at(i));
    double bar_time = time - count*measure_time.at(i);

    if (change_time == Time::Zero)
    {
        change_time = current;
    }

    if ((bar_time < 0.1) || ((current - change_time).asSeconds() > 2))
    {
        play(i);
        change_time = Time::Zero;
    }
}

void BackgroundMusic::play(int ind)
{
    music_index = ind;
    for (int i=0; i<music.size(); i++)
    {
        Music* m = music.at(i);
        if (i == ind)
        {
            double time = m->getPlayingOffset().asSeconds();
            int count = floor(time / measure_time.at(i));
            m->setPlayingOffset(seconds(count * measure_time.at(i)));
            m->play();
        } else {
            m->pause();
        }
    }
    last_i = ind;
}

GameLogic::GameLogic(GameState* s, bool c, BackgroundMusic *m) {
    state = s;
    last = Time::Zero;
    cheat = c;
    music = m;
}

void GameLogic::run()
{
    auto current = state->timer.getElapsedTime();
    music->update(current);
    if (state->finished) {
        music->music_index = 4;
        return;
    }
    if (!state->player.initialized)
    {
        for (int i = 0; i < state->level->layers.size(); i++)
        {
            for (int j = 0; j < state->level->layers[i].size(); j++) {
                if (state->getBlockType(i, j) == PowerupType::PlayerStart)
                {
                    state->level->layers[i][j] = nullptr;
                    state->player.initialized = true;
                    state->player.pos.x = i;
                    state->player.pos.y = j;
                }
            }
        }
    }
    if (last == Time::Zero) {
        last = current;
        state->time = 0;
        return;
    }
    state->time += (current - last).asSeconds();

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
            if ((state->player.jump_count < max_jumps * state->player.upwardPower) && !state->player.schnitzel)
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
        if (state->player.jumping && !state->player.schnitzel)
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


    // Check for collisions.
    double x = state->player.pos.x;
    double y = state->player.pos.y;
    bool col_left = state->level->collides(x + 0.5, y)
        || state->level->collides(x + 0.5, y + 0.6)
        || state->level->collides(x + 0.5, y - 0.9);
    bool col_right = state->level->collides(x + 1.5, y)
        || state->level->collides(x + 1.5, y - 0.9)
        || state->level->collides(x + 1.5, y + 0.6);
    bool col_top = state->level->collides(x + 1., y + 0.7)
        || state->level->collides(x + 0.6, y + 0.7)
        || state->level->collides(x + 1.4, y + 0.7);
    bool col_bottom = state->level->collides(x + 1., y - 1.01)
        || state->level->collides(x + 0.6, y - 1.01)
        || state->level->collides(x + 1.4, y - 1.01);
    int x_i = floor(x + 0.5);
    int y_i = floor(y - 0.5);
    if ((col_left && (state->player.v.x < 0)) || (col_right && (state->player.v.x > 0)))
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

    double x_center = x_i + 0.5;
    double y_center = y_i + 0.5;
    if ((x_center >= x + 0.5) && (x_center <= x + 1.5) && (y_center >= y - 1) && (y_center <= y + 1))
    {
        switch (state->getBlockType(x_i, y_i))
        {
        case (SpeedPowerup):
            state->player.speedPower += powerup_value;
            if (state->player.speedPower > 1) state->player.speedPower = 1;
            state->level->layers[x_i][y_i] = nullptr;
            break;
        case (JumpForwardPowerup):
            state->player.forwardPower += powerup_value;
            if (state->player.forwardPower > 1) state->player.forwardPower = 1;
            state->level->layers[x_i][y_i] = nullptr;
            break;
        case (JumpUpwardPowerup):
            state->player.upwardPower += powerup_value;
            if (state->player.upwardPower > 1) state->player.upwardPower = 1;
            state->level->layers[x_i][y_i] = nullptr;
            break;
        case (Schnitzel):
            state->player.schnitzel = true;
            state->level->layers[x_i][y_i] = nullptr;
            break;
        case (Pizza):
            state->player.pizza = true;
            state->level->layers[x_i][y_i] = nullptr;
            break;
        case (IceCream):
            state->player.brainfreeze += powerup_value;
            if (state->player.brainfreeze > .9) state->player.brainfreeze = .9;
            state->level->layers[x_i][y_i] = nullptr;
            break;
        case (Money):
            // TODO
            break;
        case (Alcohol):
            // TODO
            break;
        case (Finish):
            state->finished = true;
            state->player.v.x = 0;
            state->player.v.y = 0;
            state->player.brainfreeze = 0;
            state->player.speedPower = 0;
            state->player.upwardPower = 0;
            state->player.forwardPower = 0;
            music->music_index = 4;
            music->play(4);
            pushScore("Nobody", state->time);
            break;
        }
    }

    // Decrease powerups.
    state->player.speedPower -= power_decrease * elapsed;
    if (state->player.speedPower < 0) state->player.speedPower = 0;
    state->player.forwardPower -= power_decrease * elapsed;
    if (state->player.forwardPower < 0) state->player.forwardPower = 0;
    state->player.upwardPower -= power_decrease * elapsed;
    if (state->player.upwardPower < 0) state->player.upwardPower = 0;
    state->player.brainfreeze -= freeze_decrease * elapsed;
    if (state->player.brainfreeze < 0) state->player.brainfreeze = 0;

    // Update music.
    if (state->player.pizza || state->player.schnitzel) {
        music->music_index = 3;
    } else if ((state->player.speedPower + state->player.forwardPower + state->player.upwardPower) > 1.5) {
        music->music_index = 2;
    } else {
        music->music_index = 1;
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
        music->play(0);
        break;
    }
    if (cheat)
    {
        switch (key)
        {
        case (Keyboard::Q):
            state->player.speedPower += powerup_value;
            if (state->player.speedPower > 1) state->player.speedPower = 1;
            break;
        case (Keyboard::A):
            state->player.speedPower -= powerup_value;
            if (state->player.speedPower < 0) state->player.speedPower = 0;
            break;
        case (Keyboard::W):
            state->player.forwardPower += powerup_value;
            if (state->player.forwardPower > 1) state->player.forwardPower = 1;
            break;
        case (Keyboard::S):
            state->player.forwardPower -= powerup_value;
            if (state->player.forwardPower < 0) state->player.forwardPower = 0;
            break;
        case (Keyboard::E):
            state->player.upwardPower += powerup_value;
            if (state->player.upwardPower > 1) state->player.upwardPower = 1;
            break;
        case (Keyboard::D):
            state->player.upwardPower -= powerup_value;
            if (state->player.upwardPower < 0) state->player.upwardPower = 0;
            break;
        case (Keyboard::R):
            state->player.brainfreeze += powerup_value;
            if (state->player.brainfreeze > 1) state->player.brainfreeze = 1;
            break;
        case (Keyboard::F):
            state->player.brainfreeze -= powerup_value;
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

void GameLogic::pushScore(string name, int time) {
	CURL *curl = curl_easy_init();
    if (curl) {
        struct curl_slist *headers = nullptr;
        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_URL, "https://speed.jbtec.eu/score");
    	curl_easy_setopt(curl, CURLOPT_POST, 1L);
		headers = curl_slist_append(headers, "Expect:");
		headers = curl_slist_append(headers, "Content-Type: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		char body[100];
		sprintf(body, "{\"name\":\"%s\",\"score\":%d}", name.c_str(), time);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, -1L);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
}
