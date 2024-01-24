#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BLOCK_SIZE = 20;
const int SPEED = 10;

struct SnakeSegment
{
    int x, y;
};

void update(vector<SnakeSegment> &snake, SDL_Keycode direction, bool &running, SDL_Point &food, SDL_Rect &rect);
void render(const vector<SnakeSegment> &snake, SDL_Renderer *renderer, SDL_Point &food, SDL_Rect &rect);
bool checkcollisionwithrectangle(SDL_Rect &rect, const SnakeSegment &head);
int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    vector<SnakeSegment> snake;
    snake.insert(snake.begin(), {5, 5});

    SDL_Point food;
    food.x = 10;
    food.y = 10;

    SDL_Rect rect = {0, 220, 120, 20};

    bool running = true;
    SDL_Keycode direction = SDLK_RIGHT;

    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running = false;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
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
        update(snake, direction, running, food, rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render(snake, renderer, food, rect);
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void update(vector<SnakeSegment> &snake, SDL_Keycode direction, bool &running, SDL_Point &food, SDL_Rect &rect)
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

    headx = (headx + (SCREEN_WIDTH / BLOCK_SIZE)) % (SCREEN_WIDTH / BLOCK_SIZE);
    heady = (heady + (SCREEN_HEIGHT / BLOCK_SIZE)) % (SCREEN_HEIGHT / BLOCK_SIZE);
    if (headx == food.x && heady == food.y)
    {
        SnakeSegment newhead = {headx, heady};
        snake.insert(snake.begin(), newhead);
        food.x = rand() % (SCREEN_WIDTH / BLOCK_SIZE);
        food.y = rand() % (SCREEN_HEIGHT / BLOCK_SIZE);
    }
    SnakeSegment newhead = {headx, heady};
    snake.insert(snake.begin(), newhead);
    snake.pop_back();
    if (checkcollisionwithrectangle(rect, {headx,heady}))
    {
        cout<<"Collision ditected"<<endl;
        
    }
}

void render(const vector<SnakeSegment> &snake, SDL_Renderer *renderer, SDL_Point &food, SDL_Rect &rect)
{
    for (const auto &segment : snake)
    {
        SDL_Rect snakerect = {
            segment.x * BLOCK_SIZE,
            segment.y * BLOCK_SIZE,
            BLOCK_SIZE,
            BLOCK_SIZE,
        };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &snakerect);
    }
    SDL_Rect foodrect = {food.x * BLOCK_SIZE, food.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
    SDL_RenderFillRect(renderer, &foodrect);

    rect.x += SPEED;
    if (rect.x > SCREEN_WIDTH)
    {
        rect.x = -rect.w;
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    SDL_RenderFillRect(renderer, &rect);
}

bool checkcollisionwithrectangle(SDL_Rect &rect, const SnakeSegment &head)
{
    bool collision = (head.x * BLOCK_SIZE < rect.x + rect.w) &&
                     ((head.x + 1) * BLOCK_SIZE > rect.x) &&
                     (head.y * BLOCK_SIZE < rect.y + rect.h) &&
                     ((head.y + 1) * BLOCK_SIZE > rect.y);

    return collision;
}