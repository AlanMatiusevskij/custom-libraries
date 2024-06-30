/**
 * A template file. The makefile command should be able to create an executable.
 * This cpp file mostly contains examples on how to use each function.
 * 
 * Yours truly, @AlanMatiusevskij
 */

#include<CustomSDL/customSDL.h>

const int WIDTH = 800, HEIGHT = 600;
const int UPS = 240;
customsdl::UI obj;
std::string path = "./assets/fonts/Raleway-SemiBold.ttf";

void testClick(std::string data);
void testButton(void* data);

int main(int argc, char *argv[]){
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,WIDTH, HEIGHT, 0);
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, 0);
    SDL_SetRenderDrawBlendMode(rend, SDL_BlendMode::SDL_BLENDMODE_BLEND);
    SDL_Event evt;

    customsdl::UI::text phrase1(obj);
    customsdl::UI::text phrase2(obj);
    customsdl::UI::scrollBox scroll1;
    customsdl::UI::scrollBox scroll2;
    customsdl::UI::button button1;

    while(true){
        SDL_SetRenderDrawColor(rend, 30, 30, 30, 255);
        SDL_RenderClear(rend);
        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);

        if(SDL_PollEvent(&evt)){
            if(SDL_QUIT == evt.type || (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_q)) break;
        }

        phrase1.renderText(rend, "COOL?", {100,100,200,200}, 15, path, false, false);
        scroll1.renderTextureScrollBox(rend, evt, {400,0,300,200}, &phrase2.last.textDimensions.w, &phrase2.last.textDimensions.h, phrase2.getTexture(rend, "adnwi\ndaw\nadwa\nadwddddddddddddd dddddddddddddddd dddddddd dddd d ddd ddd dddddddaw\nadawa\nwda\nadwdadwada\nadwada\n\n\ndahndawh\nadwjahd", {400,0,300,200}, 20, path, true));
        scroll2.renderButtonScrollBox(rend, evt, &obj, {200,200, 400, 200}, {" i", "know", "1", "have", "good", "judgement", " , 2", "know", "yep", "you", "will", "never", "know", "me", "nor", "the", "star", "inside", "me"}, 20, path, &testClick);
        button1.renderButton(rend, evt, {0, 400, 50, 50}, &testButton, (void*)(&WIDTH), nullptr);

        customcpp::upsAverage(true);

        SDL_RenderPresent(rend);
        SDL_Delay(1000/UPS);
    }
    SDL_Quit();
    return EXIT_SUCCESS;
}

void testClick(std::string data){
    std::cout << data << "\n";
}

void testButton(void* data){
    int value = *(int*)data;
    std::cout << value << "\n";
}