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

#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>
#include <vector>

class Player;
class GameState;

using namespace sf;

class Renderer
{
public:
    Renderer(RenderWindow *w, const char *font_path);
    virtual ~Renderer();

    void render(GameState *state);

    RenderWindow *window;
    Font *font;

    void renderMenu(int);
    std::vector<sf::Text*> texts;
    sf::Text title;

    void renderHighscore(GameState *state);
    
    float tile_width, tile_height;

private:
    std::vector<sf::Texture> t, p_run_left, p_run_right;
    sf::Texture texture, t_red_powerup, t_green_powerup, t_blue_powerup, p_stand_left, p_stand_right, p_jump_left, p_jump_right, up_pictogram, jump_pictogram, run_pictogram;
    int drogen_counter, player_counter;
    sf::Time last_bg_change, last_player_change;
    sf::Shader brainfreeze;
};

#endif
