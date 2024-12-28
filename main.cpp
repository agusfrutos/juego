#include <iostream>
#include <SDL.h>

#define main SDL_main

struct Stats {
    float life = 100; 
    float mana = 100;
    float att = 10;
    float def = 5;
    float dex = 5;
    float wis = 10;
    float vit = 10;
    float speed = 0.9f;
};

struct Entity {
    float worldX, worldY;     // Posición en el mundo
    float screenX, screenY;   // Posición en pantalla
    int width, height;
    SDL_Rect figura;      
    Stats stats;   
};

int main(int argc, char* argv[]) {

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return 1;
    }

    const int SCREEN_WIDTH = 1600;
    const int SCREEN_HEIGHT = 900;

    //Ventana en la que se renderiza el juego
    SDL_Window* window = SDL_CreateWindow(
        "Rotmg like game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (window == NULL) {
        SDL_Quit();
    };

    bool isRunning = true;
    SDL_Event event;

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    // posición x (centro de la pantalla)
    // posición y (centro de la pantalla)
    // ancho del rectángulo
    // alto del rectángulo
    Entity player;
    player.worldX = 400;
    player.worldY = 300;
    player.width = 50;
    player.height = 100;
    player.figura = SDL_Rect{ (int)player.worldX, (int)player.worldY, player.width, player.height };

    Entity enemy;
    enemy.figura = SDL_Rect{ 400, 300, 35, 75 };

    float cameraX = 0;
    float cameraY = 0;


    const int SCREEN_CENTER_X = SCREEN_WIDTH / 2;
    const int SCREEN_CENTER_Y = SCREEN_HEIGHT / 2;

    Uint32 lastTime = SDL_GetTicks();

    while (isRunning) {
#pragma region INPUT

        // Procesar eventos y estado del teclado
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        // Calcular delta time (parte del sistema)
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime);
        lastTime = currentTime;

        const Uint8* keyState = SDL_GetKeyboardState(NULL);

#pragma endregion
#pragma region UPDATE

        // Actualizar posiciones y state del juego
        if (keyState[SDL_SCANCODE_W]) player.posY -= player.stats.speed * deltaTime;
        if (keyState[SDL_SCANCODE_A]) player.posX -= player.stats.speed * deltaTime;
        if (keyState[SDL_SCANCODE_S]) player.posY += player.stats.speed * deltaTime;
        if (keyState[SDL_SCANCODE_D]) player.posX += player.stats.speed * deltaTime;

        // Actualizar posición de la cámara para que siga al jugador
        cameraX = player.posX - SCREEN_CENTER_X;
        cameraY = player.posY - SCREEN_CENTER_Y;

        // Posición en pantalla relativa a la cámara
        player.figura.x = (int)(player.posX - cameraX);
        player.figura.y = (int)(player.posY - cameraY);


#pragma endregion
#pragma region RENDER
       
        SDL_SetRenderDrawColor(renderer, 0, 120, 33, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 150, 50, 255);

        for (int x = 0; x < 5000; x += 64) { 
            int screenX = x - (int)cameraX; 
            SDL_RenderDrawLine(renderer, screenX, 0, screenX, SCREEN_HEIGHT);
        }

        // Líneas horizontales
        for (int y = 0; y < 5000; y += 64) {
            int screenY = y - (int)cameraY;  // Ajustar a la posición de la cámara
            SDL_RenderDrawLine(renderer, 0, screenY, SCREEN_WIDTH, screenY);
        }


        //jugador
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &enemy.figura);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &player.figura);
        SDL_RenderPresent(renderer);

#pragma endregion
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}