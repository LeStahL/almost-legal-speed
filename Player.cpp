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

#include <Player.h>

vec2 vec2::operator+(vec2 const& rhs) {
    vec2 res;
    res.x = x + rhs.x;
    res.y = y + rhs.y;
    return res;
}

void vec2::operator+=(vec2 const& rhs) {
    x += rhs.x;
    y += rhs.y;
}

vec2 vec2::operator*(double f) {
    vec2 res;
    res.x = x * f;
    res.y = y * f;
    return res;
}

void vec2::operator*=(double f) {
    x *= f;
    y *= f;
}

void vec2::operator/=(double f) {
    x /= f;
    y /= f;
}
