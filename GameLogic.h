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
#include <SFML/Audio.hpp>
#include <string>
#include <vector>

class BackgroundMusic {
public:
    BackgroundMusic(std::vector<std::pair<std::string, double>>);
    virtual ~BackgroundMusic() = default;

    void update(sf::Time);
    void play(int);
    int music_index;

private:
    int last_i;
    std::vector<sf::Music*> music;
    std::vector<double> measure_time;
    sf::Music menu, normal, slow, dope;
    sf::Time change_time;
};

class GameLogic {
public:
    GameLogic(GameState*, bool, BackgroundMusic*);
    virtual ~GameLogic() = default;

    void run();
    void keyPressed(sf::Keyboard::Key);

private:
    GameState* state;
    Time last;
    bool cheat;
    const double grav_acc = 20;
    const double jump_speed = 10;
    const double acc_scale = 30;
    const double acc_time = 0.2;
    const int max_jumps = 4;
    const double pizza_speed = 0.2;
    const double pizza_time = 3;
    const double schnitzel_time = 1;
    const double power_decrease = 0.02;
    const double powerup_value = 0.2;
    BackgroundMusic* music;
};

#endif
