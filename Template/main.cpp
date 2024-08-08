#include<customSDL/customSDL.h>

const int WIDTH = 1000, HEIGHT = 600;
bool hide = true;

void testingButton(void *data);
void testingScorllBox(std::string data);

int main(int argc, char *argv[]){
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_Window *wind = SDL_CreateWindow("Template", 1920-WIDTH, 1080-HEIGHT, WIDTH, HEIGHT, 0);
    SDL_Renderer *rend = SDL_CreateRenderer(wind, -1, 0);
    SDL_Event evt;

    customsdl::UI *ui = customsdl::UI::init(rend, &evt, WIDTH, HEIGHT, (float)1000/30, {255,255,255,255},{30,30,30,255},{30,30,30,100},{40,40,40,150}, "./assets/fonts/Raleway-SemiBold.ttf");
    
    SDL_SetRenderDrawColor(rend,20,20,20,255);
    while(true){
        SDL_RenderClear(rend);
        ui->update();

        if(evt.type == SDL_QUIT) break;
        
        ui->button.renderButton({600,100,50,50}, &testingButton, nullptr, nullptr,nullptr, 1, 0);
        ui->button.renderButton({625,125,50,50}, &testingButton, nullptr, nullptr,nullptr, 1, 0);
        ui->button.renderButton({600,125,50,50}, &testingButton, nullptr, nullptr,nullptr, 1, 0);

        ui->scrollbox.renderTextScrollBox("testing scrollbox.I\ndia\nwhdiaud ghuadg igauwd giuawgd i", {100,100,200,40}, CLR.BLACK, 2, 20, 0);
        ui->scrollbox.renderButtonScrollBox({"test", "test1", "test2", "test3", "test4", "test5", "test6", "test7", "test8", "test9", "test10", "test11", "test12", "test13", "test14"},{200,200,100,100}, CLR.WHITE, 0,15,&testingScorllBox, 0);

        ui->text.renderText("hello world", {0,0,50,50}, CLR.LIME, 2, 25, 0);

        ui->text.renderText("ups: " + intToString(upsAverage(false)), {WIDTH-100, 10, 100, 10}, CLR.WHITE, 1, 16, ui->XCENTERED);

        ui->present();
        SDL_RenderPresent(rend);
    }
    SDL_Quit();
    return 0;
}

void testingButton(void *data){
    hide =!hide;
}

void testingScorllBox(std::string data){
    std::cout << data << "\n";
}