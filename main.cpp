#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
using namespace std;
Mix_Chunk *eat;
Mix_Chunk *bonus;
Mix_Chunk *pause;
Mix_Chunk *gameover;
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

struct obstacles
{
    int x, y;
};

void update(vector<SnakeSegment> &snake, SDL_Point &food, const vector<Border> &gameborder, SDL_Keycode direction, bool &running, int &score, const vector<obstacles> &obstacleborder, SDL_Rect &bonusfood, int &active, Uint32 &bonusfoodCreationTime, int &count);
void render(SDL_Renderer *renderer, const vector<SnakeSegment> &snake, const SDL_Point &food, const vector<Border> &gameborder, TTF_Font *font, int &score, const vector<obstacles> &obstacleborder, SDL_Rect &bonusfood, int &active, Uint32 &bonusfoodCreationTime);
bool checkCollision(const vector<SnakeSegment> &snake, int x, int y);
bool checkbordercollision(const vector<Border> &gameborder, int x, int y);
bool checkObstacleCollision(const vector<obstacles> &obstacleborder, int x, int y);
void GameOver(auto score);

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 1024);
    
    eat = Mix_LoadWAV("mixer.mp3");
    bonus = Mix_LoadWAV("bell.mp3");
    pause= Mix_LoadWAV("pause.mp3");
    gameover= Mix_LoadWAV("gameover.mp3");
    SDL_Window *window = SDL_CreateWindow("snake game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    bool running = true;
    TTF_Font *font = TTF_OpenFont("shikha.ttf", 20);
    SDL_Keycode direction = SDLK_RIGHT;
    vector<SnakeSegment> snake;
    snake.insert(snake.begin(), {5, 5});
    SDL_Point food;
    food.x = 10;
    food.y = 10;

    SDL_Rect bonusfood;
    int active = 0;
    Uint32 bonusfoodCreationTime = 0;

    vector<Border> gameborder;
    for (int i = 0; i < (SCREEN_WIDTH / BLOCK_SIZE); i++)
    {
        gameborder.push_back({i, 0});
        gameborder.push_back({i, (SCREEN_HEIGHT / BLOCK_SIZE) - 1});
    }
    for (int i = 1; i < (SCREEN_HEIGHT / BLOCK_SIZE); i++)
    {
        gameborder.push_back({0, i});
        gameborder.push_back({(SCREEN_WIDTH / BLOCK_SIZE) - 1, i});
    }

    vector<obstacles> obstacleborder;
    for (int i = 23; i < 29; i++)
    {
        obstacleborder.push_back({i, 3});
    }
    for (int i = 4; i < 9; i++)
    {
        obstacleborder.push_back({28, i});
    }
    for (int i = 3; i < 9; i++)
    {
        obstacleborder.push_back({i, 20});
    }

    for (int i = 15; i < 20; i++)
    {
        obstacleborder.push_back({3, i});
    }

    int score = 0;
    int count = 0;
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
                case SDLK_RIGHT:
                    if (direction != SDLK_LEFT)
                        direction = SDLK_RIGHT;
                    break;
                case SDLK_LEFT:
                    if (direction != SDLK_RIGHT)
                        direction = SDLK_LEFT;
                    break;
                default:
                    break;
                }
            }
        }
        update(snake, food, gameborder, direction, running, score, obstacleborder, bonusfood, active, bonusfoodCreationTime, count);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render(renderer, snake, food, gameborder, font, score, obstacleborder, bonusfood, active, bonusfoodCreationTime);
        SDL_RenderPresent(renderer);
        SDL_Delay(200);
    }
    GameOver(score);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

void update(vector<SnakeSegment> &snake, SDL_Point &food, const vector<Border> &gameborder, SDL_Keycode direction, bool &running, int &score, const vector<obstacles> &obstacleborder, SDL_Rect &bonusfood, int &active, Uint32 &bonusfoodCreationTime, int &count)
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
    case SDLK_RIGHT:
        headx++;
        break;
    case SDLK_LEFT:
        headx--;
        break;
    default:
        break;
    }
    headx = (headx + (SCREEN_WIDTH / BLOCK_SIZE)) % (SCREEN_WIDTH / BLOCK_SIZE);
    heady = (heady + (SCREEN_HEIGHT / BLOCK_SIZE)) % (SCREEN_HEIGHT / BLOCK_SIZE);
    if (headx == 31)
    {
        headx = 1;
    }
    else if (headx == 0)
    {
        headx = 30;
    }
    else if (heady == 23)
    {
        heady = 1;
    }
    else if (heady == 0)
    {
        heady = 22;
    }

    if (headx == food.x && heady == food.y)
    {
        SnakeSegment newSegment = {headx, heady};
        snake.insert(snake.begin(), newSegment);
        do
        {
            food.x = rand() % (SCREEN_WIDTH - 2 * BLOCK_SIZE) / BLOCK_SIZE + 1;
            food.y = rand() % (SCREEN_HEIGHT - 2 * BLOCK_SIZE) / BLOCK_SIZE + 1;

        } while (food.y==1 || checkCollision(snake, food.x, food.y) || checkObstacleCollision(obstacleborder, food.x, food.y));
        count++;

        Mix_PlayChannel(-1, eat, 0);
        score += 5;
        if (count % 7 == 0)
        {
            active = 1;
            do
            {
                bonusfood.x = rand() % (SCREEN_WIDTH - 2 * BLOCK_SIZE) / BLOCK_SIZE + 1;
                bonusfood.y = rand() % (SCREEN_HEIGHT - 2 * BLOCK_SIZE) / BLOCK_SIZE + 1;
            } while (bonusfood.y==1 || checkCollision(snake, bonusfood.x, bonusfood.y) || checkObstacleCollision(obstacleborder, bonusfood.x, bonusfood.y));
            bonusfoodCreationTime = SDL_GetTicks() + 4000;
            Mix_PlayChannel(-1, bonus, 0);
        }
    }
    else if (active == 1 && headx == bonusfood.x && heady == bonusfood.y)
    {
        score += 10;
        active = 0;
        Mix_PlayChannel(-1, eat, 0);
    }
    else
    {
        if (checkCollision(snake, headx, heady))
        {
            running = false;
            return;
        }
        else if (checkbordercollision(gameborder, headx, heady))
        {
            running = true;
        }
        else if (checkObstacleCollision(obstacleborder, headx, heady))
        {
            Mix_PlayChannel(-1, pause, 0);

            SDL_Window *pausewindow = SDL_CreateWindow("PAUSE GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
            SDL_Renderer *pauserenderer = SDL_CreateRenderer(pausewindow, -1, 0);
            TTF_Font *font = TTF_OpenFont("shikha.ttf", 30);
            SDL_SetRenderDrawColor(pauserenderer, 0, 255, 0, 255);
            SDL_RenderClear(pauserenderer);

            SDL_Color textcoloryes = {255, 255, 255, 255};
            SDL_Surface *textsurfaceyes = TTF_RenderText_Solid(font, "Yes", textcoloryes);
            SDL_Texture *texttextureyes = SDL_CreateTextureFromSurface(pauserenderer, textsurfaceyes);
            SDL_Rect scorerect = {210, 230, textsurfaceyes->w, textsurfaceyes->h};
            SDL_RenderCopy(pauserenderer, texttextureyes, nullptr, &scorerect);

            SDL_Color textcolorno = {255, 255, 255, 255};

            SDL_Surface *textsurfaceno = TTF_RenderText_Solid(font, "No", textcolorno);
            SDL_Texture *texttextureno = SDL_CreateTextureFromSurface(pauserenderer, textsurfaceno);
            SDL_Rect scorerectno = {420, 230, textsurfaceno->w, textsurfaceno->h};
            SDL_RenderCopy(pauserenderer, texttextureno, nullptr, &scorerectno);

            SDL_RenderPresent(pauserenderer);
            SDL_FreeSurface(textsurfaceyes);
            SDL_DestroyTexture(texttextureyes);

            SDL_FreeSurface(textsurfaceno);
            SDL_DestroyTexture(texttextureno);

            bool windowpause = true;

            while (windowpause)
            {
                SDL_Event e;
                while (SDL_PollEvent(&e))
                {
                    if (e.type == SDL_QUIT)
                    {
                        windowpause = false;
                    }
                    else if (e.type == SDL_KEYDOWN)
                    {
                        switch (e.key.keysym.sym)
                        {
                        case SDLK_LEFT:
                            
                            if (score >= 10)
                            {
                                score -= 10;
                            }
                            running = true;
                            windowpause = false;
                            break;
                        case SDLK_RIGHT:
                            
                            running = false;
                            windowpause = false;
                            break;
                        default:
                            break;
                        }
                    }
                }
            }

            SDL_DestroyRenderer(pauserenderer);
            SDL_DestroyWindow(pausewindow);
            TTF_CloseFont(font);
        }

        SnakeSegment newHead = {headx, heady};
        snake.insert(snake.begin(), newHead);
        snake.pop_back();
    }
    if (active == 1 && SDL_GetTicks() > bonusfoodCreationTime)
    {
        active = 0;
        Mix_PlayChannel(-1, bonus, 0);
    }
}
void render(SDL_Renderer *renderer, const vector<SnakeSegment> &snake, const SDL_Point &food, const vector<Border> &gameborder, TTF_Font *font, int &score, const vector<obstacles> &obstacleborder, SDL_Rect &bonusfood, int &active, Uint32 &bonusfoodCreationTime)
{
    for (const auto &segment : snake)
    {
        SDL_Rect snakeRect = {segment.x * BLOCK_SIZE, segment.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &snakeRect);
    }
    for (const auto &border : gameborder)
    {
        SDL_Rect borderRect = {border.x * BLOCK_SIZE, border.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
        SDL_SetRenderDrawColor(renderer, 255, 125, 125, 0);
        SDL_RenderFillRect(renderer, &borderRect);
    }

    for (const auto &obstacles : obstacleborder)
    {
        SDL_Rect obstacleRect = {obstacles.x * BLOCK_SIZE, obstacles.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
        SDL_SetRenderDrawColor(renderer, 255, 0, 125, 0);
        SDL_RenderFillRect(renderer, &obstacleRect);
    }
    if (active == 1)
    {
        SDL_Rect bonusfoodrect = {bonusfood.x * BLOCK_SIZE, bonusfood.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
        SDL_SetRenderDrawColor(renderer, 250, 250, 0, 0);
        SDL_RenderFillRect(renderer, &bonusfoodrect);
    }

    SDL_Rect foodRect = {food.x * BLOCK_SIZE, food.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &foodRect);

    SDL_Rect headrect = {snake.front().x * BLOCK_SIZE, snake.front().y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 0);
    SDL_RenderFillRect(renderer, &headrect);

    SDL_Color textcolor = {0, 0, 255, 255};
    string scoretext = "Score: " + to_string(score);
    SDL_Surface *textsurface = TTF_RenderText_Solid(font, scoretext.c_str(), textcolor);
    SDL_Texture *texttexture = SDL_CreateTextureFromSurface(renderer, textsurface);
    SDL_Rect scorerect = {(SCREEN_WIDTH - textsurface->w) / 2, 20, textsurface->w, textsurface->h};

    SDL_RenderCopy(renderer, texttexture, nullptr, &scorerect);
    SDL_FreeSurface(textsurface);
    SDL_DestroyTexture(texttexture);
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
bool checkbordercollision(const vector<Border> &gameborder, int x, int y)
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
bool checkObstacleCollision(const vector<obstacles> &obstacleborder, int x, int y)
{
    for (const auto &obstacle : obstacleborder)
    {
        if (obstacle.x == x && obstacle.y == y)
        {
            return true;
        }
    }
    return false;
}

void GameOver(auto score)
{
    SDL_Window *gameoverwindow = SDL_CreateWindow("Game Over", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *gameoverrenderer = SDL_CreateRenderer(gameoverwindow, -1, 0);
    TTF_Font *gameoverfont = TTF_OpenFont("shikha.ttf", 50);
    SDL_SetRenderDrawColor(gameoverrenderer, 0, 255, 0, 255);
    SDL_RenderClear(gameoverrenderer);
    SDL_Color textcolor = {255, 255, 255, 255};
    string gameovertext = "Game Over with score " + to_string(score);
    SDL_Surface *textsurface = TTF_RenderText_Solid(gameoverfont, gameovertext.c_str(), textcolor);
    SDL_Texture *texttexture = SDL_CreateTextureFromSurface(gameoverrenderer, textsurface);
    SDL_Rect scorerect = {60, 230, textsurface->w, textsurface->h};
    SDL_RenderCopy(gameoverrenderer, texttexture, nullptr, &scorerect);
    SDL_RenderPresent(gameoverrenderer);
     Mix_PlayChannel(-1, gameover, 0);
     SDL_Delay(2000);
    SDL_FreeSurface(textsurface);
    SDL_DestroyTexture(texttexture);
}