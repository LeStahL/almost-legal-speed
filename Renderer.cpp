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
    , tile_width(64.)
    , tile_height(64.)
    , player_counter(0)
{
    if (!font->loadFromFile(font_path))
    {
        fprintf(stderr, "ERROR: Could not load font %s.\n", font_path);
        exit(0);
    }
    texts.push_back(sf::Text("Start Game", *font));
    texts.push_back(sf::Text("Show highscores", *font));
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
        fprintf(stderr, "ERROR: Could not load p_green.png\n");
        exit(0);
    }

    if (!t_blue_powerup.loadFromFile("../gfx/p_blue.png"))
    {
        fprintf(stderr, "ERROR: Could not load p_blue.png\n");
        exit(0);
    }

    if(!p_jump_left.loadFromFile("../gfx/Matzario_jump_left.png"))
    {
        fprintf(stderr, "ERROR: Could not load Matzario_jump_left.png\n");
        exit(0);
    }
    
    if(!p_jump_right.loadFromFile("../gfx/Matzario_jump_right.png"))
    {
        fprintf(stderr, "ERROR: Could not load Matzario_jump_right.png\n");
        exit(0);
    }
    
    if(!p_jump_left.loadFromFile("../gfx/Matzario_jump_left.png"))
    {
        fprintf(stderr, "ERROR: Could not load Matzario_jump_left.png\n");
        exit(0);
    }
    
    if(!p_stand_left.loadFromFile("../gfx/matzario_left.png"))
    {
        fprintf(stderr, "ERROR: Could not load matzario_left.png\n");
        exit(0);
    }
    
    if(!p_stand_right.loadFromFile("../gfx/matzario_right.png"))
    {
        fprintf(stderr, "ERROR: Could not load matzario_right.png\n");
        exit(0);
    }
    
    p_run_left.resize(2);
    p_run_right.resize(2);
    
    if(!p_run_left.at(0).loadFromFile("../gfx/matzario_run_left.png", sf::IntRect(0.,0.,32.,32.)))
    {
        fprintf(stderr, "ERROR: Could not load matzario_run_left.png\n");
        exit(0);
    }
    
    if(!p_run_left.at(1).loadFromFile("../gfx/matzario_run_left.png", sf::IntRect(0.,32.,32.,32.)))
    {
        fprintf(stderr, "ERROR: Could not load matzario_run_left.png\n");
        exit(0);
    }
    
    if(!p_run_right.at(0).loadFromFile("../gfx/matzario_run_right.png", sf::IntRect(0.,0.,32.,32.)))
    {
        fprintf(stderr, "ERROR: Could not load matzario_run_right.png\n");
        exit(0);
    }
    
    if(!p_run_right.at(1).loadFromFile("../gfx/matzario_run_right.png", sf::IntRect(0.,32.,32.,32.)))
    {
        fprintf(stderr, "ERROR: Could not load matzario_run_right.png\n");
        exit(0);
    }
    
    last_bg_change = sf::Time::Zero;
    last_player_change = sf::Time::Zero;
}

Renderer::~Renderer()
{
    delete font;
}

void Renderer::render(GameState *state)
{
    Player *player = &(state->player);
    sf::Time current = state->timer.getElapsedTime();
    
    //backdrop
    if(player->forwardPower + player->upwardPower + player->speedPower > 1.5)
    {
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

//     CircleShape p(.5*tile_width);
//     p.setFillColor(sf::Color(111.,111.,111.));
//     p.setPosition((player->pos.x)*tile_width, 600.-(player->pos.y+1.)*tile_height);
//     p.setPosition(400., 300.);
//     window->draw(p);

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
    vector<vector<const Block*> > level = state->level.layers;
    for(int i=0; i<level.size(); ++i)
    {
        for(int j=0; j<level.at(i).size(); ++j)
        {
            const Block *block = level.at(i).at(j);
            if (0 == block) continue;
            if (block->powerupType == PowerupType::PlayerStart) continue;
            sf::Sprite block_sprite;
            block_sprite.setScale(tile_width/block->texture->getSize().x, tile_height/block->texture->getSize().y);
            block_sprite.setTexture(*(block->texture));
            block_sprite.setPosition((i-player->pos.x)*tile_width+400.,600.-(j-player->pos.y)*tile_height-300.);
            window->draw(block_sprite);
        }
    }
    
    //matze
    if((current - last_player_change).asMilliseconds() > 100)
    {
        ++player_counter;
        player_counter %= 2;
        last_player_change = current;
    }
    
    if(player->inair && player->face == RIGHT) //jump right
    {
        sf::Sprite sprite_p_right_jumping;
        sprite_p_right_jumping.setTexture(p_jump_right);
        sprite_p_right_jumping.setPosition(400, 300.);
        sprite_p_right_jumping.setScale(tile_width/32., tile_height/32.);
        window->draw(sprite_p_right_jumping);
    }
    else if(player->inair && player->face == LEFT) //jump left
    {
        sf::Sprite sprite_p_left_jumping;
        sprite_p_left_jumping.setTexture(p_jump_left);
        sprite_p_left_jumping.setPosition(400, 300.);
        sprite_p_left_jumping.setScale(tile_width/32., tile_height/32.);
        window->draw(sprite_p_left_jumping);
    }
    else if(!player->inair && player->face == LEFT) 
    {
        if(player->v.x < 0.) //run left
        {
            sf::Sprite sprite_p_left_running;
            sprite_p_left_running.setTexture(p_run_left.at(player_counter));
            sprite_p_left_running.setPosition(400, 300.);
            sprite_p_left_running.setScale(tile_width/32., tile_height/32.);
            window->draw(sprite_p_left_running);
        }
        else if(player->v.x == 0.) //stand left
        {
            sf::Sprite sprite_p_left_standing;
            sprite_p_left_standing.setTexture(p_stand_left);
            sprite_p_left_standing.setPosition(400, 300.);
            sprite_p_left_standing.setScale(tile_width/32., tile_height/32.);
            window->draw(sprite_p_left_standing);
        }
    }
    else if(!player->inair && player->face == RIGHT)
    {
        if(player->v.x > 0.) //run right
        {
            sf::Sprite sprite_p_right_running;
            sprite_p_right_running.setTexture(p_run_right.at(player_counter));
            sprite_p_right_running.setPosition(400, 300.);
            sprite_p_right_running.setScale(tile_width/32., tile_height/32.);
            window->draw(sprite_p_right_running);
        }
        else if(player->v.x == 0.) //stand right
        {
            sf::Sprite sprite_p_right_standing;
            sprite_p_right_standing.setTexture(p_stand_right);
            sprite_p_right_standing.setPosition(400, 300.);
            sprite_p_right_standing.setScale(tile_width/32., tile_height/32.);
            window->draw(sprite_p_right_standing);
        }
    }
}

void Renderer::renderMenu(int selected)
{
	for (int i = 0; i < texts.size(); i++) {
	    texts[i].setPosition(.45*800.,.45*600. + i*60.);
	    texts[i].setColor(Color(255.,0.,0.));
	}

    for (int i = 0; i < texts.size(); i++) {
    	if (selected == i) {
    		texts[i].setStyle(Text::Bold);
    		texts[i].setColor(Color(255.,255.,0.));
    	} else {
    		texts[i].setStyle(Text::Regular);
    		texts[i].setColor(Color(255.,0,0));
    	}
    }

    for (int i = 0; i < texts.size(); i++)
    	window->draw(texts[i]);
}

void Renderer::renderHighscore(GameState *state)
{
	Text scores(state->highscores, *font);
	scores.setPosition(0, 0);
	window->draw(scores);}
