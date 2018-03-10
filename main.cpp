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
#include "LevelImporter.h"

#include <string.h>
#include <math.h>

#include <iostream>

int main(int argc, char **args)
{
    const char *fontpath = 0, *levelpath = 0;
    bool cheat = false;
    for(int i=1; i<argc; ++i)
    {
        if(!strcmp(args[i], "--font") || !strcmp(args[i], "-f"))
        {
            ++i;
            fontpath = args[i];
        }
        else if(!strcmp(args[i], "--level") || !strcmp(args[i], "-l"))
        {
            ++i;
            levelpath = args[i];
        }
        else if(!strcmp(args[i], "--cheat") || !strcmp(args[i], "-c"))
        {
            ++i;
            cheat = true;
        }
    }

    if(0 == fontpath)
    {
        fprintf(stderr, "ERROR: No font specified. Use -f flag.\n");
        return -1;
    }
    printf("Using font: %s\n", fontpath);

    if(0 == levelpath)
    {
        fprintf(stderr, "ERROR: No level specified. Use -l flag.\n");
        return -1;
    }
    printf("Rendering level: %s\n", levelpath);

    GfxManager m;
    LevelImporter imp;
    std::string fn(levelpath);
    const Level *level = imp.loadLevel(fn, m);
    sf::RenderWindow window(sf::VideoMode(800, 600), "Almost legal speed");
    GameState state;
    state.level = *level;
    Renderer r(&window, fontpath);
    BackgroundMusic music(
        "../matzesmagicmusic/Chilly.ogg",
        "../matzesmagicmusic/Chilly.ogg",
        "../matzesmagicmusic/MoreDopeThanHope.ogg",
        "../matzesmagicmusic/TooMuchFood.ogg"
    );
    music.menu.play();
    GameLogic gameLogic(&state, cheat, &music);
    MenuLogic menuLogic(&state, r.texts);

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
                if (state.ingame)
                    gameLogic.keyPressed(event.key.code);
                else
                    menuLogic.keyPressed(event.key.code);
                break;
            case (sf::Event::MouseMoved):
                if (!state.ingame)
                    menuLogic.mouseMoved(event.mouseMove.x, event.mouseMove.y);
                break;
            case (sf::Event::MouseButtonPressed):
                if (!state.ingame)
                    menuLogic.mouseButtonPressed(event.mouseButton.button);
                break;
            }
        }

        window.clear(sf::Color::Black);

        if(state.ingame) {
            gameLogic.run();
            r.render(&state);
        } else {
            if (state.onscores)
                r.renderHighscore(&state);
            else
                r.renderMenu(menuLogic.selectedItem);
        }

        window.display();
    }

    return 0;
}
