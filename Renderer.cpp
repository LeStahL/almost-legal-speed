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
    , tile_width(32.)
    , tile_height(32.)
    , player_counter(0)
{
    if (!font->loadFromFile(font_path))
    {
        fprintf(stderr, "ERROR: Could not load font %s.\n", font_path);
        exit(0);
    }
    texts.push_back(new sf::Text("Start Game", *font));
    texts.push_back(new sf::Text("Show Highscores", *font));
    texts.push_back(new sf::Text("Quit Game", *font));
    title = sf::Text("Almost Legal Speed", *font);
    title.setCharacterSize(60);
    float title_w = title.getGlobalBounds().width;
    float title_h = title.getGlobalBounds().height;
    title.setPosition(0.5*(800. - title_w), 40. + 0.5*title_h);

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
    
    if(!up_pictogram.loadFromFile("../gfx/piktogram_jump_up.png"))
    {
        fprintf(stderr, "ERROR: Could not load piktogram_jump_up.png\n");
        exit(0);
    }

    if(!run_pictogram.loadFromFile("../gfx/piktogram_run.png"))
    {
        fprintf(stderr, "ERROR: Could not load piktogram_jump_up.png\n");
        exit(0);
    }

    if(!jump_pictogram.loadFromFile("../gfx/piktogram_jump.png"))
    {
        fprintf(stderr, "ERROR: Could not load piktogram_jump_up.png\n");
        exit(0);
    }

    //brainfreeze
    if (!brainfreeze.loadFromFile("../shaders/brainfreeze.frag", sf::Shader::Fragment))
    {
        fprintf(stderr, "ERROR: Could not load brainfreeze fragment shader\n");
//         exit(0);
    }
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

    //ui overlay
    RectangleShape speedDopingBar(Vector2f(150.,20.));
    speedDopingBar.setOutlineColor(Color(255.,255.,255.));
    speedDopingBar.setFillColor(Color(0.,0.,0.,0.));
    speedDopingBar.setOutlineThickness(2.);
    speedDopingBar.setPosition(60.,10.);
    window->draw(speedDopingBar);

    RectangleShape speedDopingProgress(Vector2f(140*state->player.speedPower, 14.));
    speedDopingProgress.setFillColor(Color(255.,0.,0.));
    speedDopingProgress.setPosition(65.,13.);
    window->draw(speedDopingProgress);

    /*Text speedText("Speed doping", *font);
    speedText.setPosition(10.,0.);
    window->draw(speedText);*/
    sf::Sprite sprite_pictogram_speed;
    sprite_pictogram_speed.setTexture(run_pictogram);
    sprite_pictogram_speed.setPosition(10., 0.);
    window->draw(sprite_pictogram_speed);

    RectangleShape upwardDopingBar(Vector2f(150.,20.));
    upwardDopingBar.setOutlineColor(Color(255.,255.,255.));
    upwardDopingBar.setFillColor(Color(0.,0.,0.,0.));
    upwardDopingBar.setOutlineThickness(2.);
    upwardDopingBar.setPosition(60.,40.);
    window->draw(upwardDopingBar);

    RectangleShape upwardDopingProgress(Vector2f(140*state->player.upwardPower, 14.));
    upwardDopingProgress.setFillColor(Color(0.,0.,255.));
    upwardDopingProgress.setPosition(65.,43.);
    window->draw(upwardDopingProgress);

    /*Text upwardText("Upward doping", *font);
    upwardText.setPosition(10.,30.);
    window->draw(upwardText);*/
    sf::Sprite sprite_pictogram_up;
    sprite_pictogram_up.setTexture(up_pictogram);
    sprite_pictogram_up.setPosition(10., 30.);
    window->draw(sprite_pictogram_up);

    RectangleShape forwardDopingBar(Vector2f(150.,20.));
    forwardDopingBar.setOutlineColor(Color(255.,255.,255.));
    forwardDopingBar.setFillColor(Color(0.,0.,0.,0.));
    forwardDopingBar.setOutlineThickness(2.);
    forwardDopingBar.setPosition(60.,70.);
    window->draw(forwardDopingBar);

    RectangleShape forwardDopingProgress(Vector2f(140*state->player.forwardPower, 14.));
    forwardDopingProgress.setFillColor(Color(0.,255.,0.));
    forwardDopingProgress.setPosition(65.,73.);
    window->draw(forwardDopingProgress);

/*    Text forwardText("Forward doping", *font);
    forwardText.setPosition(10.,60.);
    window->draw(forwardText);*/
    sf::Sprite sprite_pictogram_jump;
    sprite_pictogram_jump.setTexture(jump_pictogram);
    sprite_pictogram_jump.setPosition(10., 60.);
    window->draw(sprite_pictogram_jump);



    sf::Sprite sprite_red_powerup;
    sprite_red_powerup.setTexture(t_red_powerup);
    sprite_red_powerup.setPosition(225, 0.);
    window->draw(sprite_red_powerup);

    sf::Sprite sprite_blue_powerup;
    sprite_blue_powerup.setTexture(t_blue_powerup);
    sprite_blue_powerup.setPosition(225, 30.);
    window->draw(sprite_blue_powerup);

    sf::Sprite sprite_green_powerup;
    sprite_green_powerup.setTexture(t_green_powerup);
    sprite_green_powerup.setPosition(225, 60.);
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
        sprite_p_right_jumping.setScale(2.*tile_width/32., 2.*tile_height/32.);
        window->draw(sprite_p_right_jumping);
    }
    else if(player->inair && player->face == LEFT) //jump left
    {
        sf::Sprite sprite_p_left_jumping;
        sprite_p_left_jumping.setTexture(p_jump_left);
        sprite_p_left_jumping.setPosition(400, 300.);
        sprite_p_left_jumping.setScale(2.*tile_width/32., 2.*tile_height/32.);
        window->draw(sprite_p_left_jumping);
    }
    else if(!player->inair && player->face == LEFT)
    {
        if(player->v.x < 0.) //run left
        {
            sf::Sprite sprite_p_left_running;
            sprite_p_left_running.setTexture(p_run_left.at(player_counter));
            sprite_p_left_running.setPosition(400, 300.);
            sprite_p_left_running.setScale(2.*tile_width/32., 2.*tile_height/32.);
            window->draw(sprite_p_left_running);
        }
        else if(player->v.x == 0.) //stand left
        {
            sf::Sprite sprite_p_left_standing;
            sprite_p_left_standing.setTexture(p_stand_left);
            sprite_p_left_standing.setPosition(400, 300.);
            sprite_p_left_standing.setScale(2.*tile_width/32., 2.*tile_height/32.);
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
            sprite_p_right_running.setScale(2.*tile_width/32., 2.*tile_height/32.);
            window->draw(sprite_p_right_running);
        }
        else if(player->v.x == 0.) //stand right
        {
            sf::Sprite sprite_p_right_standing;
            sprite_p_right_standing.setTexture(p_stand_right);
            sprite_p_right_standing.setPosition(400, 300.);
            sprite_p_right_standing.setScale(2.*tile_width/32., 2.*tile_height/32.);
            window->draw(sprite_p_right_standing);
        }
    }
}

void Renderer::renderMenu(int selected)
{
    sf::Sprite sprite;
    sprite.setTexture(t.at(0));
    window->draw(sprite);

	for (int i = 0; i < texts.size(); i++) {
        float txtWidth = texts[i]->getGlobalBounds().width;
        float txtHeight = texts[i]->getGlobalBounds().height;
	    texts[i]->setPosition(.5*(800. - txtWidth),.5*600. + 100. + (i - texts.size()/2.)*60.);
	    texts[i]->setColor(Color(255.,0.,0.));
	}

    for (int i = 0; i < texts.size(); i++) {
    	if (selected == i) {
    		texts[i]->setStyle(Text::Bold);
    		texts[i]->setColor(Color(50., 0., 0.));
    	} else {
    		texts[i]->setStyle(Text::Regular);
    		texts[i]->setColor(Color(50., 0., 0));
    	}
    }

    for (int i = 0; i < texts.size(); i++)
    	window->draw(*texts[i]);

    window->draw(title);
}

void Renderer::renderHighscore(GameState *state)
{
	Text scores(state->highscores, *font);
	scores.setPosition(0, 0);
	window->draw(scores);}
