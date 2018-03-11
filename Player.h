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

#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/System/Time.hpp>

struct vec2 {
    double x;
    double y;

    vec2 operator+(vec2 const&);
    void operator+=(vec2 const&);
    vec2 operator*(double);
    void operator*=(double);
    void operator/=(double);
};

typedef enum { LEFT = -1, NONE = 0, RIGHT = 1 } direction;

class Player
{
public:
    Player();
    virtual ~Player() = default;

    void reset();

    vec2 pos, v;
    direction a, face;

    double forwardPower, upwardPower, speedPower, money, brainfreeze, drunken;
    bool schnitzel, inair, jumping, pizza, initialized;
    int jump_count;

    sf::Time pizza_start;
    sf::Time schnitzel_start;
};

#endif
