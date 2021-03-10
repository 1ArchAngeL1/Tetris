
#include "GameEngine.h"

using namespace sf;

using namespace std;

extern Texture txts[8];

tetris::tetris(std::string BlockType,int x,int y){
    index = rand() % 8;
    side_length = sqrt(BlockType.length());
    if(BlockType.length() == 9){
        type = new int*[3];
        for(int i = 0;i < 3;i++)type[i] = new int[3];
    }else if(BlockType.length() == 16){
        type = new int*[4];
        for(int i = 0;i < 4;i++)type[i] = new int[4];
    }
    coordinate.x = x;
    coordinate.y = y;
    for(int i = 0; i < BlockType.length();++i){
        if(BlockType[i] == 'x'){
            int y = i / side_length;
            int x = i % side_length;
            type[y][x] = 1;
        }else{
            int y = i / side_length;
            int x = i % side_length;
            type[y][x] = 0;
        }   
    }
}

void tetris::figure_rotation(){
    if(side_length == 3){
        int forRet[3][3];
        for(int i = 0;i < 3;++i){
            for(int j = 0;j < 3;++j){
                forRet[i][j] = 0;
            }
        }
        for(int i = 0;i < 3;++i){
            for(int j = 0;j < 3;j++){
                if(type[i][j] == 1){
                    forRet[j][2-i] = 1;
                }
            }
        }
        for(int i = 0;i < 3;i++){
            for(int j = 0;j < 3;j++){
                type[i][j] = forRet[i][j];
            }
        }      
    }else if(side_length == 4){
        int forRet[4][4];
        for(int i = 0;i < 4;++i){
            for(int j = 0;j < 4;++j){
                forRet[i][j] = 0;
            }
        }
        for(int i = 0;i < 4;++i){
            for(int j = 0;j < 4;j++){
                if(type[i][j] == 1){
                    forRet[j][3-i] = 1;
                }
            }
        }
        for(int i = 0;i < 4;i++){
            for(int j = 0;j < 4;j++){
                type[i][j] = forRet[i][j];
            }
        }      
    }
    
}


void tetris::draw_on_screen(sf::RenderWindow & wind,int size_of_block){
    std::vector<Sprite> shapes;
    for(int i = 0;i < side_length;i++){
        for(int j = 0;j < side_length;j++){
            if(type[i][j] == 1){
                int x = coordinate.x + j;
                int y = coordinate.y + i;
                Sprite todraw;
                todraw.setTexture(txts[index]);
                todraw.scale(Vector2f(size_of_block/284.f,size_of_block/284.f));
                todraw.setPosition(Vector2f(x * size_of_block,y * size_of_block));
                shapes.push_back(todraw);
            }
        }
    }
    for(int i = 0;i < shapes.size();i++)wind.draw(shapes[i]);
}

void tetris:: move_right(){
    coordinate.x++;
}
void tetris::move_down(){
    coordinate.y++;
}
void tetris::move_left(){
    coordinate.x--;
}



































