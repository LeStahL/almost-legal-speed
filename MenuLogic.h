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

#ifndef MENU_LOGIC_H
#define MENU_LOGIC_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <curl/curl.h>

#include <GameState.h>

class MenuLogic {
public:
    MenuLogic(GameState*, std::vector<sf::Text*>);
    virtual ~MenuLogic() = default;

    void run();
    void keyPressed(sf::Keyboard::Key);
    void mouseMoved(int, int);
    void mouseButtonPressed(sf::Mouse::Button);

    int selectedItem;
private:
	void activateMenuItem(int);
	GameState* state;
	std::vector<sf::Text*> texts;
	int menu_item_count;
    CURL *curl;
};

#endif
