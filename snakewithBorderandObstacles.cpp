#include<bits/stdc++.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
using namespace std;
const int SCREEN_WIDTH=640;
const int SCREEN_HEIGHT=480;
const int BLOCK_SIZE=20;
struct SnakeSegment{
    int x,y;
};

struct Border{
    int x,y;
};

struct obstacles {
    int x, y;
};

void update(vector<SnakeSegment> &snake, SDL_Point& food,const vector<Border>&gameborder,SDL_Keycode direction,bool &running,int &score,const vector<obstacles>&obstacleborder);
void render(SDL_Renderer *renderer,const vector<SnakeSegment>&snake,const SDL_Point& food,const vector<Border>&gameborder,TTF_Font *font,int &score,const vector<obstacles>&obstacleborder);
bool checkCollision(const vector<SnakeSegment> &snake, int x, int y);
bool checkbordercollision(const vector<Border>&gameborder,int x,int y);
bool checkObstacleCollision(const vector<obstacles>& obstacleborder, int x, int y);

int main(int argc,char *argv[]){
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDL_Window *window=SDL_CreateWindow("snake game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer=SDL_CreateRenderer(window,-1,0);
    bool running=true;
    TTF_Font *font=TTF_OpenFont("shikha.ttf",25);
    SDL_Keycode direction=SDLK_RIGHT;
    vector<SnakeSegment>snake;
    snake.insert(snake.begin(),{5,5});
    SDL_Point food;
    food.x=10;
    food.y=10;
    vector<Border>gameborder;
    for(int i=0;i<(SCREEN_WIDTH/BLOCK_SIZE);i++){
        gameborder.push_back({i,0});
        gameborder.push_back({i,(SCREEN_HEIGHT/BLOCK_SIZE)-1});
    }
    for(int i=1;i<(SCREEN_HEIGHT/BLOCK_SIZE);i++){
        gameborder.push_back({0,i});
        gameborder.push_back({(SCREEN_WIDTH/BLOCK_SIZE)-1,i});
    }

    vector<obstacles>obstacleborder;
    for(int i=23;i<29;i++){
        obstacleborder.push_back({i,3});
    }
    for(int i=4;i<9;i++){
        obstacleborder.push_back({28,i});
    }
     for(int i=3;i<9;i++){
        obstacleborder.push_back({i,20});
    }

    for(int i=15;i<20;i++){
        obstacleborder.push_back({3,i});
    }

    int score=0;
    while(running){
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            if(e.type==SDL_QUIT){
                running=false;
            }
            else if(e.type==SDL_KEYDOWN){
                switch(e.key.keysym.sym){
                    case SDLK_UP:
                    if(direction!=SDLK_DOWN)
                    direction=SDLK_UP;
                    break;
                    case SDLK_DOWN:
                    if(direction!=SDLK_UP)
                    direction=SDLK_DOWN;
                    break;
                    case SDLK_RIGHT:
                    if(direction!=SDLK_LEFT)
                    direction=SDLK_RIGHT;
                    break;
                    case SDLK_LEFT:
                    if(direction!=SDLK_RIGHT)
                    direction=SDLK_LEFT;
                    break;
                    default:
                    break;
                }
            }
        }
        update(snake,food,gameborder,direction,running,score,obstacleborder);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);
        render(renderer,snake,food,gameborder,font,score,obstacleborder);
        SDL_RenderPresent(renderer);
        SDL_Delay(200);
    }
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
void update(vector<SnakeSegment> &snake, SDL_Point& food,const vector<Border>&gameborder,SDL_Keycode direction,bool &running,int &score,const vector<obstacles>&obstacleborder){
    int headx=snake.front().x;
    int heady=snake.front().y;
    switch(direction){
        case SDLK_UP:
        heady--;
        break;
        case SDLK_DOWN:
        heady++;
        break;
        case SDLK_RIGHT:
        headx++;
        break;
        case SDLK_LEFT:
        headx--;
        break;
        default:
        break;
    }
    headx=(headx+(SCREEN_WIDTH/BLOCK_SIZE))%(SCREEN_WIDTH/BLOCK_SIZE);
    heady=(heady+(SCREEN_HEIGHT/BLOCK_SIZE))%(SCREEN_HEIGHT/BLOCK_SIZE);
    if(headx==31){
        headx=1;
    }
    else if(headx==0){
        headx=30;
    }
    else if(heady==23){
        heady=1;
    }
    else if(heady==0){
        heady=22;
    }
    if (headx == food.x && heady == food.y) {
        SnakeSegment newSegment = {headx, heady};
        snake.insert(snake.begin(),newSegment);
        food.x = rand() % (SCREEN_WIDTH -2* BLOCK_SIZE)/BLOCK_SIZE+1;
        food.y = rand() % (SCREEN_HEIGHT -2* BLOCK_SIZE)/BLOCK_SIZE+1;
        score+=5;
    }
    else{
        if(checkCollision(snake,headx,heady)){
            running=false;
            return;
        }
        else if(checkbordercollision(gameborder,headx,heady)){
            running=true;
        }
         else if(checkObstacleCollision(obstacleborder, headx, heady))
        {
            running=true;
            if(score>=10){
            score-=10;
            }
        }
        SnakeSegment newHead={headx,heady};
        snake.insert(snake.begin(),newHead);
        snake.pop_back();
    }
}
bool checkCollision(const vector<SnakeSegment> &snake, int x, int y)
{
    for (int i = 1; i < snake.size(); ++i)
    {
        if (snake[i].x == x && snake[i].y == y)
        {
            return true;
        }
    }
    return false;
}
bool checkbordercollision(const vector<Border>&gameborder,int x,int y){
    for(const auto &Border:gameborder){
        if(Border.x==x && Border.y==y){
            return true;
        }
    }
    return false;
}
bool checkObstacleCollision(const vector<obstacles>& obstacleborder, int x, int y) {
    for (const auto& obstacles : obstacleborder) {
        if (obstacles.x == x && obstacles.y == y) {
            return true;
        }
    }
    return false;
}
void render(SDL_Renderer *renderer,const vector<SnakeSegment>&snake,const SDL_Point& food,const vector<Border>&gameborder,TTF_Font *font,int &score,const vector<obstacles>&obstacleborder){
    for(const auto& segment:snake){
        SDL_Rect snakeRect={segment.x*BLOCK_SIZE,segment.y*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE};
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderFillRect(renderer,&snakeRect);
    }
    for(const auto &border:gameborder){
        SDL_Rect borderRect={border.x*BLOCK_SIZE,border.y*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE};
        SDL_SetRenderDrawColor(renderer,255,255,0,255);
        SDL_RenderFillRect(renderer,&borderRect);
    }

    for (const auto& obstacles :obstacleborder ) {
        SDL_Rect obstacleRect = {obstacles.x * BLOCK_SIZE, obstacles.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
        SDL_SetRenderDrawColor(renderer, 255, 5, 255, 255);
        SDL_RenderFillRect(renderer, &obstacleRect);
    }

    SDL_Rect foodRect={food.x*BLOCK_SIZE,food.y*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE};
    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    SDL_RenderFillRect(renderer,&foodRect);

    SDL_Color textColor = {0, 0, 255, 255};
    string scoreText = "Score: " + to_string(score);

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);  
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);
    int xPos = (SCREEN_WIDTH - textWidth) / 2;
    SDL_Rect scoreRect = {xPos, 20, textWidth, textHeight};
    SDL_RenderCopy(renderer, textTexture, nullptr, &scoreRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}