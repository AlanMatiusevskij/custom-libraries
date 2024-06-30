#include<iostream>

#include<CustomSDL/customSDL.h>

const int WIDTH = 800, HEIGHT = 600;
customsdl::UI obj;

int main(int argc, char *argv[]){
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,WIDTH, HEIGHT, 0);
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, 0);
    SDL_SetRenderDrawBlendMode(rend, SDL_BlendMode::SDL_BLENDMODE_BLEND);
    SDL_Event evt;

    customsdl::UI::text phrase1(obj);
    customsdl::UI::text phrase2(obj);
    customsdl::UI::scrollBox scroll1;

    while(true){
        SDL_SetRenderDrawColor(rend, 30, 30, 30, 255);
        SDL_RenderClear(rend);
        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);

        if(SDL_PollEvent(&evt)){
            if(SDL_QUIT == evt.type)
            break;
        }

        phrase1.renderText(rend, "COOL?", {100,100,200,200}, 15, "./assets/fonts/Raleway-SemiBold.ttf", false, false);
        scroll1.renderTextureScrollBox(rend, evt, {200,200,400,200}, &phrase2.last.textDimensions.w, &phrase2.last.textDimensions.h, phrase2.getTexture(rend, "adnwi\ndaw\nadwa\nadwaw\nadawa\nwda\nadwdadwada\nadwada\n\n\ndahndawh\nadwjahd", {200,200,400,200}, 20, "./assets/fonts/Raleway-SemiBold.ttf", false));
        SDL_Rect test = {10,10, phrase2.last.surface->w, phrase2.last.surface->h};
        SDL_RenderCopy(rend, phrase2.last.texture, NULL, &test);

        SDL_RenderPresent(rend);
    }
    SDL_Quit();
    return EXIT_SUCCESS;
}

