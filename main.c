#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL_events.h>
#include <stdio.h>

#define GRID_SIZE 50
const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 900;

// pad with extra cells to calculate efficiently
bool population[GRID_SIZE+2][GRID_SIZE+2];

typedef struct {
    bool simulate;
} Game;

Game game;

typedef struct {
    int row,col;
    bool live;
} Cell;

int calculate_neighbours(int x, int y){
    int n = 0;
    if (population[x-1][y-1]) n++;
    if (population[x][y-1]) n++;
    if (population[x+1][y-1]) n++;

    if (population[x-1][y]) n++;
    if (population[x][y]) n++;
    if (population[x+1][y]) n++;

    if (population[x-1][y+1]) n++;
    if (population[x][y+1]) n++;
    if (population[x+1][y+1]) n++;
    return n;
}

void calculate(){
    for (int i = 0; i < GRID_SIZE; i++){
        for (int j = 0; j < GRID_SIZE; j++){
            int n = calculate_neighbours(i,j);
        }
    }
}

// initialize population
// void initialize_population(SDL_Rect** rects){
void initialize_population(){
    for (int i = 0; i < GRID_SIZE + 2; i++){
        for (int j = 0; j < GRID_SIZE + 2; j++){
            population[i][j] = false;
        }
    }

    // rects = (SDL_Rect**)malloc(sizeof(SDL_Rect*)*GRID_SIZE);
    // for (int i = 0; i < GRID_SIZE; i++){
    //     rects[i] = (SDL_Rect*)malloc(sizeof(SDL_Rect)*GRID_SIZE);
    // }
    //
    // for (int i = 0; i < GRID_SIZE ; i++){
    //     for (int j = 0; j < GRID_SIZE ; j++){
    //         SDL_Rect r;
    //         r.x = 0, r.y = 0;
    //         r.w = SCREEN_WIDTH / GRID_SIZE, r.h = SCREEN_HEIGHT / GRID_SIZE;
    //         rects[i][j] = r;
    //     }
    // }
}

int main(){
    game.simulate = false;

    if (SDL_Init(SDL_INIT_VIDEO) < 0 ){
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window *window = NULL;
    window = SDL_CreateWindow("GAME OF LIFE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event event;
    int xMouse, yMouse;
    bool run = true;

    initialize_population();

    while (run){
        SDL_Rect r;
        r.w = SCREEN_WIDTH/GRID_SIZE;
        r.h = SCREEN_HEIGHT/GRID_SIZE;

        if (game.simulate){
            //white
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }else{
            //grey
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        }
        // renderbg
        SDL_RenderClear(renderer);

        if (game.simulate){
            //grey
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        }else{
            //black
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }
        // render lines
        for (int i = 0; i <= SCREEN_WIDTH; i = i + SCREEN_WIDTH/GRID_SIZE){
            SDL_RenderDrawLine(renderer, i, 0, i, SCREEN_HEIGHT);
        }
        for (int i = 0; i <= SCREEN_WIDTH; i = i + SCREEN_HEIGHT/GRID_SIZE){
            SDL_RenderDrawLine(renderer, 0, i, SCREEN_WIDTH, i);
        }

        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    run = false;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_SPACE){
                        game.simulate = !game.simulate;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (!game.simulate){
                        SDL_GetMouseState(&xMouse,&yMouse);
                        int x = xMouse / (SCREEN_WIDTH / GRID_SIZE);
                        // int x = xMouse - xMouse % (SCREEN_WIDTH / GRID_SIZE);
                        // int y = yMouse - yMouse % (SCREEN_HEIGHT / GRID_SIZE);
                        int y = yMouse / (SCREEN_HEIGHT / GRID_SIZE);
                        population[x + 1][y + 1] = !population[x + 1][y + 1];
                        // bool next_state = !population[(x/(SCREEN_WIDTH/GRID_SIZE)) + 1][(y/(SCREEN_HEIGHT/GRID_SIZE)) + 1];
                        printf("%d,%d\n", x,y);
                        fflush(stdin);
                    }else{
                        game.simulate = false;
                    }
                    break;
            }
        }
        if (game.simulate){
            SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
        }else{
            SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
        }
        for (int i = 0; i < GRID_SIZE ; i++){
            for (int j = 0; j < GRID_SIZE ; j++){
                if (population[i+1][j+1]){
                    SDL_SetRenderDrawColor( renderer, 10, 10, 10, 255 );
                    r.x = i * (SCREEN_WIDTH / GRID_SIZE);
                    r.y = j * (SCREEN_HEIGHT / GRID_SIZE);
                    SDL_RenderFillRect( renderer, &r );
                }else{
                    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
                }
            }
        }

        SDL_RenderPresent(renderer);
    }
    // for (int i = 0; i < GRID_SIZE; i++){
    //     free(rects[i]);
    // }
    // free(rects);

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
