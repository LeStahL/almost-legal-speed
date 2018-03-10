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
#include "GameState.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

Renderer::Renderer(sf::RenderWindow* w, const char* font_path)
    : font(new Font)
    , window(w)
{
    if (!font->loadFromFile(font_path))
    {
        fprintf(stderr, "ERROR: Could not load font %s.\n", font_path);
        exit(0);
    }
    texts.push_back(sf::Text("Start Game", *font));
    texts.push_back(sf::Text("Quit Game", *font));
}

Renderer::~Renderer()
{
    delete font;
}

void Renderer::render(GameState *state)
{
    Player *player = &(state->player);
    //backdrop
    if(player->forwardPower + player->upwardPower + player->speedPower > 1.5)
    {
        std::vector<sf::Texture> t;
        t.resize(14);
        for(int i=0; i<14; ++i)
        {
            if (!t.at(i).loadFromFile((std::string("../gfx/backdrop/DrogenBG_")+std::to_string(i)+std::string(".png")).c_str()))
            {
                fprintf(stderr, "ERROR: Could not load DrogenBG*.png\n");
                exit(0);
            }
        }
    }
    else
    {
        sf::Texture texture;
        if (!texture.loadFromFile("../gfx/backdrop/BG.png"))
        {
            fprintf(stderr, "ERROR: Could not load BG.png\n");
            exit(0);
        }
        
        sf::Sprite sprite;
        sprite.setTexture(texture);
        window->draw(sprite);
    }
    
    CircleShape p(20.);
    p.setFillColor(sf::Color(111.,111.,111.));
    p.setPosition(player->pos.x*800., 560.-player->pos.y*600.);
    window->draw(p);
}

void Renderer::renderMenu(int selected)
{
    texts[0].setPosition(.45*800.,.45*600.);
    texts[0].setFillColor(Color(255.,0.,0.));

    texts[1].setPosition(.45*800.,.55*600.);
    texts[1].setFillColor(Color(255.,0.,0.));

    for (int i = 0; i < texts.size(); i++) {
    	if (selected == i) {
    		texts[i].setStyle(Text::Bold);
    		texts[i].setFillColor(Color(255.,255.,0.));
    	} else
    		texts[i].setFillColor(Color(255.,0,0));
    }

    window->draw(texts[0]);
    window->draw(texts[1]);
}

