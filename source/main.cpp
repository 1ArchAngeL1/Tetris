#include "GameEngine.h"
#include <SFML/Audio.hpp>
#include <random>
#include "input.h"

using namespace sf;
using namespace std;

#define block_size 30
#define x_width  15
#define y_width  30

int window_width = block_size * x_width;
int window_height = block_size * y_width;

std::vector<std::string> type_strings;
std::vector<coordinates> saved_cord;
std::vector<Sprite> saved_rects;
Texture txts[8];


float delay = 0.2f;

float time_saver = 0.2f;
bool exploded = false;

void txts_init(){
    Texture tmp;
    tmp.loadFromFile("giraffe.png");
    txts[0] = tmp;
    tmp.loadFromFile("hippo.png");
    txts[1] = tmp;
    tmp.loadFromFile("monkey.png");
    txts[2] = tmp;
    tmp.loadFromFile("parrot.png");
    txts[3] = tmp;
    tmp.loadFromFile("panda.png");
    txts[4] = tmp;
    tmp.loadFromFile("pig.png");
    txts[5] = tmp;
    tmp.loadFromFile("penguin.png");
    txts[6] = tmp;
    tmp.loadFromFile("snake.png");
    txts[7] = tmp;
}


void type_strings_init(){
    type_strings.push_back(".x...x...x...x..");
    type_strings.push_back(".....xx..xx.....");
//     type_strings.push_back(".x.xx.x..");
//     type_strings.push_back(".x.xx..x.");
//     type_strings.push_back("x..x..xx.");
//     type_strings.push_back(".x..x.xx.");
}


bool game_finished(){
    for(int i = 0;i < saved_cord.size();i++){
        if(saved_cord[i].y <= 1)return true;
    }
    return false;
}

bool spawnCheker(tetris * curr){
    for(int i = 0;i < curr->side_length;++i){
        for(int j = 0;j < curr->side_length;++j){
            if(curr->type[i][j] == 1){
                int x = curr->coordinate.x + j;
                int y = curr->coordinate.y + i;
                for(int i = 0;i < saved_cord.size();i++){
                    if(saved_cord[i].x == x && saved_cord[i].y == y)return true;
                }
            }
        }
    }
    return false;
}



bool collision(tetris * curr){
    for(int i = 0;i < curr->side_length;i++){
        for(int j = 0;j < curr->side_length;j++){
            if(curr->type[i][j] == 1){
                int x = curr->coordinate.x + j;
                int y = curr->coordinate.y + i;
                for(int z = 0;z < saved_cord.size();z++){
                    if(saved_cord[z].x == x && saved_cord[z].y == y + 1)return true;
                }
            }
        }
    }
    return false;
}

bool collisionLeft(tetris * curr){
    for(int i = 0;i < curr->side_length;i++){
        for(int j = 0;j < curr->side_length;j++){
            if(curr->type[i][j] == 1){
                int x = curr->coordinate.x + j;
                int y = curr->coordinate.y + i;
                if(x == 0)return true;
                for(int z = 0;z < saved_cord.size();z++){
                    if(saved_cord[z].x == x - 1 && saved_cord[z].y == y)return true;
                }
            }
        }
    }
    return false;
}

bool collisionRight(tetris * curr){
    for(int i = 0;i < curr->side_length;i++){
        for(int j = 0;j < curr->side_length;j++){
            if(curr->type[i][j] == 1){
                int x = curr->coordinate.x + j;
                int y = curr->coordinate.y + i;
                if(x == x_width-1)return true;
                for(int z = 0;z < saved_cord.size();z++){
                    if(saved_cord[z].x == x + 1 && saved_cord[z].y == y)return true;
                }
            }
        }
    }
    return false;
}


void tetris_cheker(){
    int rows[y_width];
    for(int i = 0; i < y_width;i++)rows[i] = 0;
    for(int i = 0;i < saved_cord.size();i++){
        rows[saved_cord[i].y]++;
    }
    for(int i = 0;i < y_width;i++){
        if(rows[i] == x_width){
            exploded = true;
            for(int j = saved_rects.size() - 1;j >= 0;j--){
                if(saved_rects[j].getPosition().y == (float)(i * block_size)){
                    saved_rects.erase(saved_rects.begin() + j);
                }else if(saved_rects[j].getPosition().y < (float)(i * block_size)){
                    saved_rects[j].setPosition(Vector2f(saved_rects[j].getPosition().x,saved_rects[j].getPosition().y + block_size));
                }
            }
            for(int j = saved_cord.size() - 1;j >= 0;j--){
                if(saved_cord[j].y == i){
                    saved_cord.erase(saved_cord.begin() + j);
                }else if(saved_cord[j].y < i){
                    saved_cord[j].y++;
                }   
            }
        }
    }
}


bool collisionRotation(tetris * curr){
    int size = curr->side_length;
    int save[size][size];
    for(int i = 0;i < size;i++){
        for(int j = 0;j < size;j++){
            save[i][j] = curr->type[i][j];
        }
    }
    for(int i = 0;i < size;i++){
        for(int j = 0;j < size;++j){
            int x = curr->coordinate.x + j;
            int y = curr->coordinate.y + i;
            if(x >= x_width || x < 0 || y > y_width)return true;
            for(int z = 0;z < saved_cord.size();z++){
                if(saved_cord[z].x == x && saved_cord[z].y == y)return true;
            }
        }
    }
    for(int i = 0;i < size;i++){
        for(int j = 0;j < size;j++){
            curr->type[i][j] = save[i][j];
        }
    }
    return false;
}


int main(){
    sf::SoundBuffer animalSound;
    animalSound.loadFromFile("animal.wav");
    sf::SoundBuffer backSongbuffer;
    backSongbuffer.loadFromFile("Tetris.wav");
    sf::Sound musicsound;
    musicsound.setBuffer(backSongbuffer);
    musicsound.setLoop(true);
    musicsound.play();
    sf::Sound sound;
    sf::SoundBuffer loosingbuffer;
    loosingbuffer.loadFromFile("losing.wav");
    sf::SoundBuffer swappingbuffer;
    swappingbuffer.loadFromFile("explosion.wav");
    txts_init();
    for(int i =0;i < x_width;i++){
        coordinates tempo(i,y_width);
        saved_cord.push_back(tempo);
    }
    RectangleShape back_block;
    back_block.setFillColor(Color::White);
    back_block.setOutlineThickness(1);
    back_block.setOutlineColor(Color::Black);
    back_block.setSize(Vector2f(block_size,block_size));
    float action = 2;
    bool is_free = true;
    type_strings_init();
    srand(time(nullptr));
    RenderWindow window(VideoMode(window_width,window_height), "Tetris");
    Clock clock;
    float chrono = 0;
    tetris * curr;
    window.setFramerateLimit(60);
    std::vector<RectangleShape> background_blocks;
    for(int i = 0;i < x_width;i++){
            for(int j = 0;j < y_width;j++){
                if((i + j) % 2 == 0){
                    back_block.setFillColor(Color(141,186,127));
                }else{
                    back_block.setFillColor(Color(208,221,203));
                }
                back_block.setPosition(Vector2f(i * block_size,j * block_size));
                background_blocks.push_back(back_block);
            }
    }
    while (window.isOpen()){
        if(exploded){
            sound.setBuffer(swappingbuffer);
            sound.play();
            exploded = false;
        }
        Input::Listen();
        float time = clock.getElapsedTime().asSeconds();
        chrono += time;
        clock.restart();
        Event event;
        if(is_free){
            int index = rand() % type_strings.size();
            curr = new tetris(type_strings[index],7,0);
            if(spawnCheker(curr) || game_finished()){
                sound.setBuffer(loosingbuffer);
                sound.play();
                musicsound.stop();
                sleep(seconds(1.5));
                window.close();
            }
            is_free = false;
        }
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        tetris_cheker();
        if(Input::Pressed(Keyboard::Left)){
        action = -1;
        }if(Input::Pressed(Keyboard::Right)){
            action = 1;
        }if(Input::Pressed(Keyboard::Up)){
            action = 0;
        } if(Input::Pressed(Keyboard::Down)){
            delay = 0.01;
        } 
        if(chrono >= delay){
            chrono = 0;
            if(action == -1){
                if(!collisionLeft(curr))curr->move_left();   
            }else
            if(action == 1){
                if(!collisionRight(curr))curr->move_right();  
            }else
            if(action == 0){
                if(!collisionRotation(curr))curr->figure_rotation();  
            }
            action = 2;
            if(collision(curr)){
                sound.setBuffer(animalSound);
                sound.play();
                delay = time_saver;
                for(int i = 0;i < curr->side_length;i++){
                    for(int j = 0;j < curr->side_length;j++){
                        if(curr->type[i][j] == 1){
                            int x = curr->coordinate.x + j;
                            int y = curr->coordinate.y + i;
                            Sprite currs_sp;
                            currs_sp.setTexture(txts[curr->index]);
                            currs_sp.scale(Vector2f(block_size/284.f,block_size/284.f));
                            currs_sp.setPosition(Vector2f(x * block_size,y * block_size));
                            saved_rects.push_back(currs_sp);
                            coordinates cord(x,y);
                            saved_cord.push_back(cord);
                        }
                    }
                }
                for(int i = 0;i < curr->side_length;i++){
                    free(curr->type[i]);
                }
                free(curr->type);
                free(curr);
                is_free = true;
            }else{
                curr->move_down();
            }
          
        }
        window.clear();
        for(int i = 0;i < background_blocks.size();i++){
             window.draw(background_blocks[i]);
        }
       
        if(!is_free)curr->draw_on_screen(window,block_size);
        for(int i = 0;i < saved_rects.size();i++)window.draw(saved_rects[i]);
        window.display();
    }

    return 0;
}









