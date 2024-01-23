#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BLOCK_SIZE = 20;
struct SnakeSegment
{
    int x, y;
};
struct Border
{
    int x, y;
};
void render(SDL_Renderer *renderer, const vector<SnakeSegment> &snake, const vector<Border> &gameborder, const SDL_Point &food,int &score,TTF_Font* font);
void update(vector<SnakeSegment> &snake, const vector<Border> &gameborder, SDL_Point &food, SDL_Keycode direction, bool &running,int &score);
bool checkCollision(const vector<SnakeSegment> &snake, int x, int y);
bool checkgameborderCollision(const vector<Border> &gameborder, int x, int y);
int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDL_Window *window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    TTF_Font* font = TTF_OpenFont("shikha.ttf", 30);
    vector<SnakeSegment> snake = {{5, 5}};
    SDL_Point food;
    food.x = 10;
    food.y = 10;
    SDL_Keycode direction = SDLK_RIGHT;
    bool running = true;
    int score=0;
    vector<Border> gameborder;
    for (int i = 0; i <= 31; i++)
    {
        gameborder.push_back({i, 0});
        gameborder.push_back({i, 23});
    }
    for (int i = 1; i <= 23; i++)
    {
        gameborder.push_back({0, i});
        gameborder.push_back({31, i});
    }
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    if (direction != SDLK_DOWN)
                        direction = SDLK_UP;
                    break;
                case SDLK_DOWN:
                    if (direction != SDLK_UP)
                        direction = SDLK_DOWN;
                    break;
                case SDLK_LEFT:
                    if (direction != SDLK_RIGHT)
                        direction = SDLK_LEFT;
                    break;
                case SDLK_RIGHT:
                    if (direction != SDLK_LEFT)
                        direction = SDLK_RIGHT;
                    break;
                default:
                    break;
                }
            }
        }
        update(snake, gameborder, food, direction, running,score);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render(renderer, snake, gameborder, food,score,font);
        SDL_RenderPresent(renderer);
        SDL_Delay(200);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
void update(vector<SnakeSegment> &snake, const vector<Border> &gameborder, SDL_Point &food, SDL_Keycode direction, bool &running,int &score)
{
    int headx = snake.front().x;
    int heady = snake.front().y;
    switch (direction)
    {
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
    if (headx == 31)
            {
                headx = 1;
            }
    else if(headx==0){
        headx = 30;
    }
    else if(heady==23){
        heady=1;
    }
    else if(heady==0){
        heady=22;
    }
    if (headx == food.x && heady == food.y)
    {
        SnakeSegment newSegment = {headx, heady};
        snake.insert(snake.begin(), newSegment);
        score +=5;
        food.x = rand()% ((SCREEN_WIDTH - 2 * BLOCK_SIZE) / BLOCK_SIZE)+1 ; 
        food.y = rand() % ((SCREEN_HEIGHT - 2 * BLOCK_SIZE) / BLOCK_SIZE)+1;
        ///cout<<food.x<<" "<<food.y<<endl;
    }
    else
    {
        if (checkCollision(snake, headx, heady))
        {
            running = false;
            return;
        }
        else if (checkgameborderCollision(gameborder, headx, heady))
        {
            running = true;
            
        }
        SnakeSegment newHead = {headx, heady};
        snake.insert(snake.begin(), newHead);
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
bool checkgameborderCollision(const vector<Border> &gameborder, int x, int y)
{
    for (const auto &Border : gameborder)
    {
        if (Border.x == x && Border.y == y)
        {
            return true;
        }
    }
    return false;
}
void render(SDL_Renderer *renderer, const vector<SnakeSegment> &snake, const vector<Border> &gameborder, const SDL_Point &food,int &score,TTF_Font* font)
{
    for (const auto &Border : gameborder)
    {
        SDL_Rect gameborder = {Border.x * BLOCK_SIZE, Border.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderFillRect(renderer, &gameborder);
    }
    for (const auto &segment : snake)
    {
        SDL_Rect blockRect = {segment.x * BLOCK_SIZE, segment.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &blockRect);
    }
    SDL_Rect foodRect = {food.x * BLOCK_SIZE, food.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &foodRect);
     SDL_Color textColor = {255, 0, 50, 255};
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
