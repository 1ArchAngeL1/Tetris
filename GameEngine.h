#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>


struct coordinates{
    int x;
    int y;
    coordinates(){};
    coordinates(int x_co,int y_co){
        x = x_co;
        y = y_co;
    }
};

class tetris{
    public:
    int index;
    coordinates coordinate;
    int** type;
    int side_length;
    tetris(std::string blockType,int x,int y);
    void draw_on_screen(sf::RenderWindow & wind,int size_of_block);
    void figure_rotation();
    void move_right();
    void move_left();
    void move_down();
};

