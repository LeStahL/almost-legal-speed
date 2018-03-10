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

#include <MenuLogic.h>
#include <GameState.h>
#include <vector>

#include <SFML/Graphics.hpp>

MenuLogic::MenuLogic(GameState* s, std::vector<sf::Text> txts) {
    state = s;
    texts = txts;
    selectedItem = -1;
    menu_item_count = texts.size();
}

void MenuLogic::keyPressed(sf::Keyboard::Key key) {
    switch (key)
    {
    case(sf::Keyboard::Return):
        activateMenuItem(selectedItem);
        break;
    case (sf::Keyboard::Down):
        selectedItem = (selectedItem < 0 || selectedItem > menu_item_count-2) ? 0 : selectedItem+1;
        break;
    case (sf::Keyboard::Up):
        selectedItem = selectedItem < 1 ? menu_item_count-1 : selectedItem-1;
        break;
    }
}

void MenuLogic::mouseMoved(int x, int y) {
    for (int i = 0; i < texts.size(); i++) {
        float txtX = .45*800.;
        float txtY = .45*600. + i*60.;
        float txtWidth = texts[i].getGlobalBounds().width;
        float txtHeight = texts[i].getGlobalBounds().height;
        if (x > txtX && y > txtY + .5*txtHeight && x < txtX + txtWidth && y < txtY + 1.5*txtHeight) {
            selectedItem = i;
            return;
        }
    }
    selectedItem = -1;
}

void MenuLogic::mouseButtonPressed(sf::Mouse::Button btn) {
    if (btn == sf::Mouse::Left) {
        activateMenuItem(selectedItem);
    }
}

void MenuLogic::activateMenuItem(int index) {
    if (index == 0)
            state->ingame = true;
    else if (index == 1)
        exit(0);
}