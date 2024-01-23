#include <bits/stdc++.h>
#include <SDL2/SDL.h>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BLOCK_SIZE = 20;
const int BORDER_SIZE=5;

struct SnakeSegment {
    int x, y;
};

void render(SDL_Renderer* renderer, const vector<SnakeSegment>& snake,const SDL_Point& food);
void update(vector<SnakeSegment>& snake,SDL_Point& food, SDL_Keycode direction, bool& running);
bool checkCollision(const vector<SnakeSegment>& snake, int x, int y);
int main(int argc,char* argv[]){
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    vector<SnakeSegment> snake{{5, 5}};
    SDL_Point food;
    food.x=10;
    food.y=10; 
    SDL_Keycode direction = SDLK_RIGHT; 
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                            direction = SDLK_UP;
                        break;
                    case SDLK_DOWN:
                            direction = SDLK_DOWN;
                        break;
                    case SDLK_LEFT:
                            direction = SDLK_LEFT;
                        break;
                    case SDLK_RIGHT:
                            direction = SDLK_RIGHT;
                        break;
                    default:
                        break;
                }
            }
        }
        update(snake,food, direction, running);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render(renderer, snake, food);
        SDL_RenderPresent(renderer);
        SDL_Delay(300);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
void update(vector<SnakeSegment>& snake,SDL_Point& food, SDL_Keycode direction, bool& running){
    int headx=snake.front().x;
    int heady=snake.front().y;
    switch(direction){
        case SDLK_UP:
        heady--;
        break;
        case SDLK_DOWN:
        heady++;
        break;
        case SDLK_LEFT:
        headx--;
        break;
        case SDLK_RIGHT:
        headx++;
        break;
        default:
        break;
    }
    headx=(headx+SCREEN_WIDTH/BLOCK_SIZE)%(SCREEN_WIDTH/BLOCK_SIZE);
    heady=(heady+SCREEN_HEIGHT/BLOCK_SIZE)%(SCREEN_HEIGHT/BLOCK_SIZE);
    if(headx==food.x && heady==food.y){
        SnakeSegment newSegment={headx,heady};
        snake.insert(snake.begin(),newSegment);
        food.x = rand() % (SCREEN_WIDTH / BLOCK_SIZE);
        food.y = rand() % (SCREEN_HEIGHT / BLOCK_SIZE);

    }
    else{
        if (checkCollision(snake, headx, heady) ) {
            running = false;
            return;
        }
        SnakeSegment newHead={headx,heady};
        snake.insert(snake.begin(),newHead);
        snake.pop_back();
    }
}
bool checkCollision(const vector<SnakeSegment>& snake, int x, int y) {
    for (size_t i = 1; i < snake.size(); ++i) {
        if (snake[i].x == x && snake[i].y == y) {
            return true;
        }
    }
    return false;
}
void render(SDL_Renderer* renderer,const vector<SnakeSegment>& snake,const SDL_Point& food){
for (const auto& segment : snake) {
        SDL_Rect blockRect = {segment.x * BLOCK_SIZE, segment.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &blockRect);
    }

SDL_Rect foodRect={food.x*BLOCK_SIZE,food.y*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &foodRect);
}
