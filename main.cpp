#include <SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>


const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 560;

const int SNAKE_SIZE = 20;

enum Direction { UP, DOWN, LEFT, RIGHT };

struct Point {
    int x, y;
};

void Init();
void Close();
void HandleInput(Direction& dir, bool& quit);
void Update(std::vector<Point>& snake, Point& food, Direction dir, bool& gameOver);
void Render(SDL_Renderer* renderer, const std::vector<Point>& snake, const Point& food);

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

int main(int argc, char* args[]) {
    Init();

    bool quit = false;
    bool gameOver = false;
    Direction dir = RIGHT;
    std::vector<Point> snake = {{320, 240}, {300, 240}, {280, 240}};
    Point food = {rand() % (SCREEN_WIDTH / SNAKE_SIZE) * SNAKE_SIZE, rand() % (SCREEN_HEIGHT / SNAKE_SIZE) * SNAKE_SIZE};

    while (!quit) {
        HandleInput(dir, quit);
        if (!gameOver) {
            Update(snake, food, dir, gameOver);
        }
        Render(gRenderer, snake, food);
        SDL_Delay(100);
    }

    Close();

    return 0;
}

void Init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    gWindow = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == NULL) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    srand(static_cast<unsigned int>(time(0)));
}

void Close() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;

    SDL_Quit();
}

void HandleInput(Direction& dir, bool& quit) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                    if (dir != DOWN) dir = UP;
                    break;
                case SDLK_DOWN:
                    if (dir != UP) dir = DOWN;
                    break;
                case SDLK_LEFT:
                    if (dir != RIGHT) dir = LEFT;
                    break;
                case SDLK_RIGHT:
                    if (dir != LEFT) dir = RIGHT;
                    break;
            }
        }
    }
}

void Update(std::vector<Point>& snake, Point& food, Direction dir, bool& gameOver) {
    Point newHead = snake.front();
    switch (dir) {
        case UP: newHead.y -= SNAKE_SIZE; break;
        case DOWN: newHead.y += SNAKE_SIZE; break;
        case LEFT: newHead.x -= SNAKE_SIZE; break;
        case RIGHT: newHead.x += SNAKE_SIZE; break;
    }

    if (newHead.x < 0 || newHead.x >= SCREEN_WIDTH || newHead.y < 0 || newHead.y >= SCREEN_HEIGHT) {
        gameOver = true;
    }

    for (const Point& p : snake) {
        if (newHead.x == p.x && newHead.y == p.y) {
            gameOver = true;
        }
    }

    snake.insert(snake.begin(), newHead);

    if (newHead.x == food.x && newHead.y == food.y) {
        food = {rand() % (SCREEN_WIDTH / SNAKE_SIZE) * SNAKE_SIZE, rand() % (SCREEN_HEIGHT / SNAKE_SIZE) * SNAKE_SIZE};
    } else {
        snake.pop_back();
    }
}


void Render(SDL_Renderer* renderer, const std::vector<Point>& snake, const Point& food) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 0);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (const Point& p : snake) {
        SDL_Rect fillRect = { p.x, p.y, SNAKE_SIZE, SNAKE_SIZE };
        SDL_RenderFillRect(renderer, &fillRect);
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect fillRect = { food.x, food.y, SNAKE_SIZE, SNAKE_SIZE };
    SDL_RenderFillRect(renderer, &fillRect);

    SDL_RenderPresent(renderer);
}
