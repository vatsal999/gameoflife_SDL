#include <SDL2/SDL.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
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
    x = x + 1, y = y + 1;
    int n = 0;
    if (population[x-1][y-1]) n++;
    if (population[x][y-1]) n++;
    if (population[x+1][y-1]) n++;

    if (population[x-1][y]) n++;
    // if (population[x][y]) n++;
    if (population[x+1][y]) n++;

    if (population[x-1][y+1]) n++;
    if (population[x][y+1]) n++;
    if (population[x+1][y+1]) n++;
    return n;
}

bool calculateNextState(int n, int x, int y){
    if (n < 2){
        return false;
    } else if (n > 3){
        return false;
    }else{
        if (!population[x+1][y+1]){
            return (n==3) ? true : false;
        }else{
            return true;
        }
    }
}

void calculate(){
    int temp[GRID_SIZE + 2][GRID_SIZE + 2];
    for (int i = 0; i < GRID_SIZE; i++){
        for (int j = 0; j < GRID_SIZE; j++){
            temp[i+1][j+1] = calculate_neighbours(i,j);
        }
    }
    for (int i = 0; i < GRID_SIZE; i++){
        for (int j = 0; j < GRID_SIZE; j++){
            population[i+1][j+1] = calculateNextState(temp[i][j], i, j);
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
    const int FPS = 5;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frametime;

    game.simulate = false;

    if (SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }
    if (SDL_Init(SDL_INIT_TIMER) < 0 ) {
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
        frameStart = SDL_GetTicks();
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
            // int n = calculate_neighbours(1,1);
            //     printf("%d\n",n);
            //     fflush(stdin);
            // if (calculateNextState(n, 0, 0)){
            //     // printf("nextstatetrue N: %d\n", n);
            //     printf("Lives");
            //     fflush(stdin);
            // }else{
            //     printf("dies");
            //     fflush(stdin);
            // }
        for ( int i = 0; i < GRID_SIZE ; i++ ) {
            for ( int j = 0; j < GRID_SIZE ; j++) {
                if ( population[i+1][j+1] ) {
                    SDL_SetRenderDrawColor( renderer, 10, 10, 10, 255 );
                    r.x = i * (SCREEN_WIDTH / GRID_SIZE);
                    r.y = j * (SCREEN_HEIGHT / GRID_SIZE);
                    SDL_RenderFillRect( renderer, &r );
                } else {
                    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
                }
            }
        }
        if (game.simulate){
            calculate();
        }

        SDL_RenderPresent(renderer);
        frametime = SDL_GetTicks() - frameStart;

        if (frameDelay > frametime){
            SDL_Delay(frameDelay - frametime);
        }
    }
    // for (int i = 0; i < GRID_SIZE; i++){
    //     free(rects[i]);
    // }
    // free(rects);

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
