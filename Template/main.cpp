#include<iostream>

#include<CustomSDL/customSDL.h>

const int WIDTH = 800, HEIGHT = 600;
customsdl::UI obj;

int main(int argc, char *argv[]){
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,WIDTH, HEIGHT, 0);
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, 0);
    SDL_Event evt;

    customsdl::UI::text phrase1(obj);

    while(true){
        SDL_SetRenderDrawColor(rend, 30, 30, 30, 255);
        SDL_RenderClear(rend);
        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);

        if(SDL_PollEvent(&evt)){
            if(SDL_QUIT == evt.type)
            break;
        }

        phrase1.renderText(rend, "COOL?", {100,100,200,200}, 15, "./assets/fonts/Raleway-SemiBold.ttf", false, true);

        SDL_RenderPresent(rend);
    }
    SDL_Quit();
    return EXIT_SUCCESS;
}

