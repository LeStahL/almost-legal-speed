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

#include <SFML/Graphics.hpp>

MenuLogic::MenuLogic(GameState* s) {
    state = s;
    selectedItem = -1;
}

void MenuLogic::run()
{

}

void MenuLogic::keyPressed(sf::Keyboard::Key key) {
    switch (key)
    {
    case(sf::Keyboard::Return):
        state->ingame = true;
        break;
    case(sf::Keyboard::Down):
        selectedItem = (selectedItem < 0 || selectedItem > menu_item_count-2) ? 0 : selectedItem+1;
        break;
    case(sf::Keyboard::Up):
        selectedItem = selectedItem < 1 ? menu_item_count-1 : selectedItem-1;
        break;
    }
    fprintf(stderr, "%d\n", selectedItem);
}