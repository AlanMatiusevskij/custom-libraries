/**
 * A template file. The makefile command should be able to create an executable.
 * This .cpp file mostly contains some examples on how to use each function.
 * 
 * Yours truly, @AlanMatiusevskij
 */

#include<CustomSDL/customSDL.h>

const int WIDTH = 800, HEIGHT = 600;
const int UPS = 240;
//ui's struct to store font path and size.
customsdl::UI::font font_Data = {20, "./assets/fonts/Raleway-SemiBold.ttf"};

void quitButton(void* data);
void heldButton(void *data);

int main(int argc, char *argv[]){
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,WIDTH, HEIGHT, 0);
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, 0);
    SDL_SetRenderDrawBlendMode(rend, SDL_BlendMode::SDL_BLENDMODE_BLEND);
    SDL_Event evt;

    //Create parent/main `UI` class object, which holds some information used by GUI objects.
    customsdl::UI *ui = customsdl::UI::INITIALIZE(rend, &evt, WIDTH, HEIGHT, {255,255,255,255}, {255,255,255,100}, {255,255,255,50}, {90,90,90,255});

    bool do_quit = false;

    //UI Objects
    customsdl::UI::text _1_text;
    customsdl::UI::text _2_text;
    customsdl::UI::text _3_text;
    customsdl::UI::button _1_button;
    customsdl::UI::button _2_button;
    customsdl::UI::scrollBox _1_textScrollBox;

    //Boxes
    SDL_Rect _1_text_box = {WIDTH/2-50, 20, 100, 100};
    SDL_Rect _1_button_box = {0, 50, 60, font_Data.fontSize + 10};
    SDL_Rect _2_button_box = {0, 70, 60, font_Data.fontSize + 10};
    SDL_Rect _2_text_box = {WIDTH/2-50, 54, 100, 100};
    SDL_Rect _3_scrolbox_box = {0, 100, 100, 100};

    while(true){
        SDL_SetRenderDrawColor(rend, 20, 20, 30, 255);
        SDL_RenderClear(rend);
        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
        SDL_PollEvent(&evt);

        _1_text.renderText("Hello world.", {0,255,100, 255}, _1_text_box, &font_Data, customsdl::UI::UIFLAGS::XCENTERED);
                                                    //      bool to void*
        _1_button.renderButton(_1_button_box, &quitButton, (void*)(&do_quit), &heldButton, nullptr, customsdl::UI::UIFLAGS::XCENTERED | ui->BUTTON_CLICK_ON_UP);
        
        _2_text.renderText("Quit", {240, 240, 240, 255}, _2_text_box, &font_Data, ui->XCENTERED);
        /*button on top of another button*/ _2_button.renderButton(_2_button_box, nullptr, nullptr, nullptr, nullptr, customsdl::UI::UIFLAGS::XCENTERED);
        _1_textScrollBox.renderTextureScrollBox(_3_scrolbox_box, &_3_text.last.surface->w, &_3_text.last.surface->h, _3_text.getTexture("I'm\nnever-rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr\ngoing\nto\ngive\nyou\nup\nNever\ngoing\nto\nlet\nyou\ndown\nNever\ngonna\nrun\naround\nand\ndesert\nyou", {255,255,255,255}, _3_scrolbox_box, &font_Data, ui->XCENTERED), ui->XCENTERED | ui->HORIZONTAL_SCROLLBAR_STICK);

        if(SDL_QUIT == evt.type || (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_q) || do_quit) break;

        //customcpp::upsAverage(true);

        SDL_RenderPresent(rend);
        SDL_Delay(1000/UPS);
    }
    SDL_Quit();
    return EXIT_SUCCESS;
}

void quitButton(void* data){
    //Example of converting a void* to any data type. Crashes if converts to a wrong one!
    bool *boolean = (bool*)data;
    *boolean = true;
    std::cout << "Quitting!";
}

void heldButton(void *data){
    std::cout << ">";
}