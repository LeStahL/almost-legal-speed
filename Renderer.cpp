// Almost Legal Speed - platform running game
// Copyright (C) 2018  Alexander Kraus <nr4@z10.info>
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

#include "Renderer.h"
#include "Player.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Renderer::Renderer(sf::RenderWindow* w, const char* font_path, Player *p)
    : font(new Font)
    , window(w)
    , player(p)
{
    if (!font->loadFromFile(font_path))
    {
        fprintf(stderr, "ERROR: Could not load font %s.\n", font_path);
        exit(0);
    }
}

Renderer::~Renderer()
{
    delete font;
}

void Renderer::render()
{
//     sf::Text text("Hello, World", *font);

//     text.setCharacterSize(30);
//     text.setStyle(sf::Text::Bold);
    //text.setColor(sf::Color::Red);
    
//     window->draw(/*text*/);
    CircleShape p(20.);
    p.setFillColor(sf::Color(111.,111.,111.));
    p.setPosition(player->pos.x*800., 560.-player->pos.y*600.);
    window->draw(p);
}

void Renderer::renderMenu()
{
    sf::Text startgame("Start Game", *font);
    startgame.setPosition(.45*800.,.45*600.);
    
    sf::Text endgame("Quit Game", *font);
    endgame.setPosition(.45*800.,.55*600.);
    
    window->draw(startgame);
    window->draw(endgame);
}

