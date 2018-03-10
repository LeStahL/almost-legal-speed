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

#include <string>
#include <iostream>

Renderer::Renderer(sf::RenderWindow* w, const char* font_path)
    : font(new Font)
    , window(w)
    , drogen_counter(0)
{
    if (!font->loadFromFile(font_path))
    {
        fprintf(stderr, "ERROR: Could not load font %s.\n", font_path);
        exit(0);
    }
    texts.push_back(sf::Text("Start Game", *font));
    texts.push_back(sf::Text("Quit Game", *font));
    
    t.resize(14);
    for(int i=0; i<14; ++i)
    {
        std::string s = (std::string("../gfx/backdrop/DrogenBG_")+std::to_string(i)+std::string(".png")).c_str();
        const char *s2 = s.c_str();
        if (!t.at(i).loadFromFile(s2))
        {
            fprintf(stderr, "ERROR: Could not load DrogenBG*.png\n");
            exit(0);
        }
    }
    
    if (!texture.loadFromFile("../gfx/backdrop/BG.png"))
    {
        fprintf(stderr, "ERROR: Could not load BG.png\n");
        exit(0);
    }
    
    if (!t_red_powerup.loadFromFile("../gfx/p_red.png"))
    {
        fprintf(stderr, "ERROR: Could not load p_red.png\n");
        exit(0);
    }

    if (!t_green_powerup.loadFromFile("../gfx/p_green.png"))
    {
        fprintf(stderr, "ERROR: Could not load p_red.png\n");
        exit(0);
    }
    
    if (!t_blue_powerup.loadFromFile("../gfx/p_blue.png"))
    {
        fprintf(stderr, "ERROR: Could not load p_red.png\n");
        exit(0);
    }

    last_bg_change = sf::Time::Zero;
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
        sf::Time current = state->timer.getElapsedTime();
        if ((current - last_bg_change).asMilliseconds() > 70) {
            ++drogen_counter;
            last_bg_change = current;
        }
        if(drogen_counter == 14) drogen_counter = 0;
        sf::Sprite sprite;
        sprite.setTexture(t.at(drogen_counter));
        window->draw(sprite);
    }
    else
    {
        sf::Sprite sprite;
        sprite.setTexture(texture);
        window->draw(sprite);
    }
    
    CircleShape p(20.);
    p.setFillColor(sf::Color(111.,111.,111.));
    p.setPosition(player->pos.x*800., 560.-player->pos.y*600.);
    window->draw(p);
    
    //ui overlay
    RectangleShape speedDopingBar(Vector2f(150.,20.));
    speedDopingBar.setOutlineColor(Color(255.,255.,255.));
    speedDopingBar.setFillColor(Color(0.,0.,0.,0.));
    speedDopingBar.setOutlineThickness(2.);
    speedDopingBar.setPosition(260.,10.);
    window->draw(speedDopingBar);
    
    RectangleShape speedDopingProgress(Vector2f(140*state->player.speedPower, 14.));
    speedDopingProgress.setFillColor(Color(255.,0.,0.));
    speedDopingProgress.setPosition(265.,13.);
    window->draw(speedDopingProgress);
    
    Text speedText("Speed doping", *font);
    speedText.setPosition(10.,0.);
    window->draw(speedText);
    
    RectangleShape upwardDopingBar(Vector2f(150.,20.));
    upwardDopingBar.setOutlineColor(Color(255.,255.,255.));
    upwardDopingBar.setFillColor(Color(0.,0.,0.,0.));
    upwardDopingBar.setOutlineThickness(2.);
    upwardDopingBar.setPosition(260.,40.);
    window->draw(upwardDopingBar);
    
    RectangleShape upwardDopingProgress(Vector2f(140*state->player.upwardPower, 14.));
    upwardDopingProgress.setFillColor(Color(0.,0.,255.));
    upwardDopingProgress.setPosition(265.,43.);
    window->draw(upwardDopingProgress);
    
    Text upwardText("Upward doping", *font);
    upwardText.setPosition(10.,30.);
    window->draw(upwardText);
    
    RectangleShape forwardDopingBar(Vector2f(150.,20.));
    forwardDopingBar.setOutlineColor(Color(255.,255.,255.));
    forwardDopingBar.setFillColor(Color(0.,0.,0.,0.));
    forwardDopingBar.setOutlineThickness(2.);
    forwardDopingBar.setPosition(260.,70.);
    window->draw(forwardDopingBar);
    
    RectangleShape forwardDopingProgress(Vector2f(140*state->player.forwardPower, 14.));
    forwardDopingProgress.setFillColor(Color(0.,255.,0.));
    forwardDopingProgress.setPosition(265.,73.);
    window->draw(forwardDopingProgress);
    
    Text forwardText("Forward doping", *font);
    forwardText.setPosition(10.,60.);
    window->draw(forwardText);
    
    sf::Sprite sprite_red_powerup;
    sprite_red_powerup.setTexture(t_red_powerup);
    sprite_red_powerup.setPosition(425, 0.);
    window->draw(sprite_red_powerup);
    
    sf::Sprite sprite_blue_powerup;
    sprite_blue_powerup.setTexture(t_blue_powerup);
    sprite_blue_powerup.setPosition(425, 30.);
    window->draw(sprite_blue_powerup);
    
    sf::Sprite sprite_green_powerup;
    sprite_green_powerup.setTexture(t_green_powerup);
    sprite_green_powerup.setPosition(425, 60.);
    window->draw(sprite_green_powerup);
    
    //level
    Level level = state->level;
    
    
}

void Renderer::renderMenu(int selected)
{
    texts[0].setPosition(.45*800.,.45*600.);
    texts[0].setColor(Color(255.,0.,0.));

    texts[1].setPosition(.45*800.,.55*600.);
    texts[1].setColor(Color(255.,0.,0.));

    for (int i = 0; i < texts.size(); i++) {
    	if (selected == i) {
    		texts[i].setStyle(Text::Bold);
    		texts[i].setColor(Color(255.,255.,0.));
    	} else {
    		texts[i].setStyle(Text::Regular);
    		texts[i].setColor(Color(255.,0,0));
    	}
    }

    window->draw(texts[0]);
    window->draw(texts[1]);
}

