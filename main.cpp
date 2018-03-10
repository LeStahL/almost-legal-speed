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

#include "Renderer.h"

#include <string.h>

int main(int argc, char **args)
{
    const char *fontpath = 0;
    for(int i=0; i<argc; ++i)
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
    GameLogic logic(&state);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        r.render();
        logic.run();

        window.display();
    }

    return 0;
}
