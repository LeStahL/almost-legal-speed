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

#include <SFML/Graphics.hpp>

#include <GameLogic.h>
#include <GameState.h>

#include <MenuLogic.h>

#include "Renderer.h"

#include <string.h>

int main(int argc, char **args)
{
    const char *fontpath = 0;
    for(int i=1; i<argc; ++i)
    {
        if(!strcmp(args[i], "--font") || !strcmp(args[i], "-f"))
        {
            ++i;
            fontpath = args[i];
        }
    }

    if(0 == fontpath)
    {
        fprintf(stderr, "ERROR: No font specified. Use -f flag.\n");
        return -1;
    }

    printf("Using font: %s\n", fontpath);

    sf::RenderWindow window(sf::VideoMode(800, 600), "Almost legal speed");
    GameState state;

    Renderer r(&window, fontpath);
    GameLogic gameLogic(&state);
    MenuLogic menuLogic;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case (sf::Event::Closed):
                window.close();
                break;
            case (sf::Event::KeyPressed):
                switch (event.key.code)
                {
                case(sf::Keyboard::Space):
                    if (state.ingame)
                    {
                        state.player.jumping = true;
                    }
                }
            }
        }

        window.clear(sf::Color::Black);

        if(state.ingame) {
            r.render();
            gameLogic.run();
        } else {
            r.renderMenu();
            menuLogic.run();
        }

        window.display();
    }

    return 0;
}
