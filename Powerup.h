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

#ifndef POWERUP_H
#define POWERUP_H

typedef enum {
    SpeedPowerup,
    JumpForwardPowerup,
    JumpUpwardPowerup,
    Money,
    Schnitzel, //complete stop for x sec, buff: calories - you speed up a small bit
    Pizza, //stuck with cheese, slow down
    IceCream, //brainfreeze, you almost pass out, level is colored darker
    Alcohol //Level starts to tilt and blur
} PowerupType;

class Player;

class Powerup
{
public:
    Powerup(PowerupType t);
    virtual ~Powerup();

    PowerupType type;
    void apply(Player *p);
};

#endif
