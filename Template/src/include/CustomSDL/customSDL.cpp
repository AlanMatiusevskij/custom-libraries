#include <CustomSDL/customSDL.h>
using namespace customcpp;

////////////////////////////////

customsdl::UI *customsdl::UI::ui = nullptr;

///////////////////////////////////////////

SDL_Surface *customsdl::UI::text::getSurface(std::string text, color rgba, SDL_Rect textBox, font *fontData, Uint32 flags){
    if(last.text == text && compareRects(textBox, last.TextBox) && last.fontData.fontPath == fontData->fontPath && last.fontData.fontSize == fontData->fontSize && last.rgba.r == rgba.r && last.rgba.g == rgba.g && last.rgba.b == rgba.b && last.rgba.a == rgba.a && last.surface != nullptr)
        return last.surface;
    wordLength.clear();
    SDL_DestroyTexture(last.texture);
    SDL_FreeSurface(last.surface);
    last.texture = nullptr;
    std::vector<std::string> words;
    std::string ind_word{""};

    FT_FaceRec_* FACE = ui->useFont(fontData->fontPath, fontData->fontSize);

    //Save each word and whitespaces in a vector
    for(char symb : text){
        if(symb == '\n'){
            words.push_back(ind_word);
            ind_word = "\n";
        }
        else if(symb == ' '){
            words.push_back(ind_word);
            ind_word = "";
        }
        else ind_word += symb;
    }
    //save the last one.
    words.push_back(ind_word);

    //Find the total width and height.
    int cWidth = 1;
    int cHeight = 1;
    int wordWidth = 0;
    int maxWidth = 0;
    int maxHeight = 0;

    for(std::string word : words){
        for(char symb : word){
            FT_Load_Char(FACE, symb, FT_LOAD_BITMAP_METRICS_ONLY);
            cWidth+=FACE->glyph->bitmap.width;
            wordWidth+=FACE->glyph->bitmap.width;
            if(((flags & AUTO_NEW_LINES) && cWidth >= textBox.w) || symb == '\n'){
                cHeight += fontData->fontSize+1;
                cWidth = wordWidth+1;
            }
        }
        wordLength.push_back(wordWidth);
        wordWidth = 0;
        cWidth+=fontData->fontSize/4;
        maxWidth = std::max(maxWidth, cWidth);
    }
    maxHeight = cHeight+fontData->fontSize+4;

    //Create a surface where the sentence will be stored.
    surfaceManipulation manip;
    manip.createSurface(maxWidth, maxHeight, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_UnlockSurface(manip.active_surface);
    SDL_SetSurfaceBlendMode(manip.active_surface, SDL_BlendMode::SDL_BLENDMODE_BLEND);

    //Load each glyph surface and merge them into a single surface.
    cWidth = 1; cHeight = 1;
    int belowBaseLine{};

    for(int i = 0; i < words.size(); i++){
        if(((flags & AUTO_NEW_LINES) && i != 0 && cWidth+wordLength[i] >= textBox.w) || (words[i][0] == '\n' && i!=0)){
            cHeight+=fontData->fontSize+1;
            cWidth = 1;
        }
        for(char symb : words[i]){
            FT_Load_Char(FACE, symb, FT_LOAD_RENDER);
            belowBaseLine = (FACE->glyph->metrics.height - FACE->glyph->metrics.horiBearingY)/55;
            SDL_Surface *glyph;

            glyph = ui->surf8bitTo32bit(SDL_CreateRGBSurfaceFrom(FACE->glyph->bitmap.buffer, FACE->glyph->bitmap.width, FACE->glyph->bitmap.rows, 8, FACE->glyph->bitmap.pitch, 0, 0, 0, 0xFF));
            blitSurface({cWidth, cHeight-glyph->h+fontData->fontSize-fontData->fontSize/5+belowBaseLine}, manip.active_surface, glyph);
            SDL_FreeSurface(glyph);

            //Update some info.
            cWidth+=FACE->glyph->bitmap.width;
        }
        cWidth+=fontData->fontSize/4;
    }
    
    //Store values
    last.rgba = rgba;
    last.surface = manip.active_surface;
    last.fontData = *fontData;
    last.text = text;
    last.textDimensions = {textBox.x, textBox.y, maxWidth, maxHeight};
    last.TextBox = textBox;

    //Modulate RGB values.
    for(int x = 0; x < maxWidth; x++){
        for(int y = 0; y < maxHeight; y++){
            color src = getSurfaceColors({x, y}, manip.active_surface, false);
            src.r = src.g = src.b = 255;
            manip.drawToSurface({x, y}, {(Uint8)(int(src.r*(float)rgba.r/255) << 0), (Uint8)(int(src.g*(float)rgba.g/255) << 0), (Uint8)(int(src.b*(float)rgba.b/255) << 0), (Uint8)(int(src.a*(float)rgba.a/255) << 0)});
        }
    }

    return manip.active_surface;
}

 SDL_Texture *customsdl::UI::text::getTexture(std::string text, color rgba, SDL_Rect textBox, font *fontData, Uint32 flags){
    if(last.text == text && compareRects(textBox, last.TextBox) && last.fontData.fontPath == fontData->fontPath && last.fontData.fontSize == fontData->fontSize && last.rgba.r == rgba.r && last.rgba.g == rgba.g && last.rgba.b == rgba.b && last.rgba.a == rgba.a && last.texture != nullptr)
        return last.texture;
    last.texture = SDL_CreateTextureFromSurface(vars.renderer, getSurface(text, rgba, textBox, fontData, flags));
    SDL_SetTextureBlendMode(last.texture, SDL_BlendMode::SDL_BLENDMODE_BLEND);

    return last.texture;
 }

SDL_Surface* customsdl::UI::surf8bitTo32bit(SDL_Surface* _8bit){
    SDL_SetPaletteColors(_8bit->format->palette, _8bitpalletecolors, 0, 256);
    //SDL_SetSurfaceBlendMode(_8bit, SDL_BlendMode::SDL_BLENDMODE_BLEND); 
    SDL_Surface* _return = SDL_ConvertSurfaceFormat(_8bit, SDL_PIXELFORMAT_RGBA32, 0);

    SDL_FreeSurface(_8bit);
    return _return;
}

void customsdl::UI::text::renderText(std::string text, color rgba, SDL_Rect textBox, font *fontData, Uint32 flags){
    last.texture = getTexture(text, rgba, textBox, fontData, flags);
    SDL_Rect dest = textBox;
    if(flags & UIFLAGS::XCENTERED) dest.x = textBox.x + textBox.w/2 - last.textDimensions.w/2;
    if(flags & UIFLAGS::YCENTERED) dest.y = textBox.y + textBox.h/2 - last.textDimensions.h/2;
    
    dest.x += 1;
    dest.y += 1;
    dest.w = last.textDimensions.w;
    dest.h = last.textDimensions.h;
    SDL_RenderCopy(vars.renderer, last.texture, NULL, &dest);
    return;
}

void customsdl::UI::scrollBox::renderTextureScrollBox(SDL_Rect boxToRenderIn, int *textureWidth, int *textureHeight, SDL_Texture *texture, Uint32 flags){
    if(flags & XCENTERED) boxToRenderIn.x = ui->vars.WIDTH/2 - (*textureWidth)/2;
    if(flags & YCENTERED) boxToRenderIn.y = ui->vars.HEIGHT/2 - (*textureHeight)/2;

    if(!(flags & NO_VISUALS)){
        background->renderPane(boxToRenderIn, vars.background_color, 0);
        SDL_SetRenderDrawColor(vars.renderer, vars.outline_color.r, vars.outline_color.g, vars.outline_color.b, vars.outline_color.a);
        SDL_RenderDrawRect(vars.renderer, &boxToRenderIn);
    }

    scrollBoxCore(boxToRenderIn, textureWidth, textureHeight, texture, flags);

    if(!(flags & UI::UIFLAGS::NO_BUTTON_TEXT)){
        SDL_Rect renderArea = {int(shiftx*ratiox), int(shifty*ratioy), std::min(boxToRenderIn.w, *textureWidth), std::min(boxToRenderIn.h, *textureHeight)};
        SDL_Rect destArea = {boxToRenderIn.x, boxToRenderIn.y, std::min(boxToRenderIn.w, *textureWidth), std::min(boxToRenderIn.h, *textureHeight)};
        SDL_RenderCopy(vars.renderer, texture, &renderArea, &destArea);
    }
    return;
}

void customsdl::UI::scrollBox::renderButtonScrollBox(SDL_Rect boxToRenderIn, std::vector<std::string> entries, color rgba, font *fontData, void (*onClick)(std::string), Uint32 flags){
    if(prev_in != entries){
        prev_compact_in = "";
        prev_in = entries;
        for(std::string word : entries){
            for(char symb : word) if(symb != '\n') prev_compact_in += symb;
            prev_compact_in += '\n';
        }
            //Remove the last un-intended new line.
        prev_compact_in.pop_back();
    }

    renderTextureScrollBox(boxToRenderIn, &text_->last.textDimensions.w, &text_->last.textDimensions.h, text_->getTexture(prev_compact_in, rgba, boxToRenderIn, fontData, 0), flags);

    SDL_GetMouseState(&x, &y);
    if(onRect(boxToRenderIn, x, y)){        
        //Hover
            //Which button in the list.
        int n = (y-boxToRenderIn.y+shifty*ratioy)/(fontData->fontSize+1);
        if(n < entries.size() && !(flags & NO_BUTTON_EFFECT)){
            SDL_SetRenderDrawColor(vars.renderer, vars.hovered_color.r, vars.hovered_color.g, vars.hovered_color.b, vars.hovered_color.a);

            int y_hover = std::max(int(boxToRenderIn.y + n*(fontData->fontSize+1)-shifty*ratioy), boxToRenderIn.y);
            int h_hover = fontData->fontSize + 1;
            int offsetTop = int(shifty*ratioy)%(fontData->fontSize+1);
            int offsetBottom = boxToRenderIn.h%(fontData->fontSize+1);
            
            if(y < boxToRenderIn.y + fontData->fontSize + 1 - offsetTop) h_hover -= offsetTop;

            //TODO: This sometimes breaks
            else if(y >= boxToRenderIn.y + boxToRenderIn.h - offsetBottom + offsetTop) h_hover -= offsetBottom - offsetTop;

            SDL_Rect hover_area = {boxToRenderIn.x, y_hover, boxToRenderIn.w, h_hover};
            SDL_RenderFillRect(vars.renderer, &hover_area);
        }

        //Click
        if(vars.event->type == SDL_MOUSEBUTTONDOWN && vars.event->button.button == SDL_BUTTON_LEFT && n < entries.size())
            onClick(entries[n]);
    }

    return;
}

void customsdl::UI::scrollBox::scrollBoxCore(SDL_Rect boxToRenderIn, int *textureWidth, int *textureHeight, SDL_Texture *texture, Uint32 flags){
    //VERTICAL
    ratioy = double(*textureHeight)/boxToRenderIn.h;
    int bar_height = boxToRenderIn.h/ratioy;

    y_bar = {boxToRenderIn.x+boxToRenderIn.w, boxToRenderIn.y+shifty, 8, bar_height};

    if(vars.event->type == SDL_MOUSEBUTTONDOWN && vars.event->button.button == SDL_BUTTON_LEFT) if(onRect(y_bar) && !(flags & UIFLAGS::LOCK_VERTICAL_SCROLLBAR)){state_clickedY = true; SDL_GetMouseState(NULL, &clickedy);}
    if(vars.event->type == SDL_MOUSEBUTTONUP && vars.event->button.button == SDL_BUTTON_LEFT) state_clickedY = false;
    if(state_clickedY){
        SDL_GetMouseState(NULL, &y);
        shifty += y-clickedy;
        clickedy = y;

        stickY = false;
        if(shifty < 0) shifty = 0;
        else if(shifty+bar_height >= boxToRenderIn.h){
            shifty = boxToRenderIn.h-bar_height-1; 
            //Scroll bar is "sticked" to the bottom of the rect.
            stickY = true;
        }
    }

    if(stickY)
        if(flags & UIFLAGS::VERTICAL_SCROLLBAR_STICK)
            shifty = boxToRenderIn.h-bar_height-1;

    if(!(flags & UIFLAGS::NO_VERTICAL_SCROLLBAR)){
        SDL_SetRenderDrawColor(vars.renderer, vars.bar_color.r, vars.bar_color.g, vars.bar_color.b, vars.bar_color.a);
        if(ratioy > 1) SDL_RenderFillRect(vars.renderer, &y_bar);
    }

    //Horizontal

    ratiox = double(*textureWidth)/boxToRenderIn.w;
    int bar_width = boxToRenderIn.w/ratiox;

    x_bar = {boxToRenderIn.x + shiftx, boxToRenderIn.y+boxToRenderIn.h, bar_width, 8};

    if(vars.event->type == SDL_MOUSEBUTTONDOWN && vars.event->button.button == SDL_BUTTON_LEFT) if(onRect(x_bar) && !(flags & UIFLAGS::LOCK_HORIZONTAL_SCROLLBAR)){state_clickedX = true; SDL_GetMouseState(&clickedx, NULL);}
    if(vars.event->type == SDL_MOUSEBUTTONUP && vars.event->button.button == SDL_BUTTON_LEFT) state_clickedX = false;
    if(state_clickedX){
        SDL_GetMouseState(&x, NULL);
        shiftx += x - clickedx;
        clickedx = x;

        stickX = false;
        if(shiftx < 0) shiftx = 0;
        else if(shiftx + bar_width >= boxToRenderIn.w){
            shiftx = boxToRenderIn.w-bar_width-1;
            stickX = true;
        }
    }

    if(stickX)
        if(flags & UIFLAGS::HORIZONTAL_SCROLLBAR_STICK)
            shiftx = boxToRenderIn.w-bar_width-1;


    if(!(flags & UIFLAGS::NO_HORIZONTAL_SCROLLBAR)){
        if(ratiox > 1) SDL_RenderFillRect(vars.renderer, &x_bar);    
    }
    
    return;
}

FT_FaceRec* customsdl::UI::useFont(std::string path, int fontsize){
    //Check if a specific sized font is already loaded.
    for(activeFaces_struct &obj : active_faces)
        if(obj.path == path && obj.fontSize == fontsize) return obj.face;
    
    //Otherwise, load a font of specific size.
    int i = active_faces.size();
    active_faces.push_back({path, fontsize});

    FT_Open_Args args;
    args.flags = FT_OPEN_PATHNAME;
    args.pathname = path.data();

    FT_Init_FreeType(&active_faces[i].ft);
    if(FT_Open_Face(active_faces[i].ft, &args, 0, &active_faces[i].face)) std::cout << "Failed to load font ('" << path << "') of size " << fontsize << ". Tried index: " << i <<"\n";
    
    active_faces[i].face->glyph->format = FT_GLYPH_FORMAT_BITMAP;
    FT_Set_Pixel_Sizes(active_faces[i].face, fontsize, fontsize);

    return active_faces[i].face;
}

void customsdl::UI::button::renderButton(SDL_Rect rect, void (*onClick)(void*), void* click_data, void (*onHold)(void*), void* hold_data, Uint32 flags){
    static int time_held = 0;
    static std::chrono::steady_clock::time_point then;

    if(flags & UIFLAGS::XCENTERED) rect.x = vars.WIDTH/2 - rect.w/2; 
    if(flags & UIFLAGS::YCENTERED) rect.y = vars.HEIGHT/2 - rect.h/2;
    
    if(!(flags & UIFLAGS::NO_VISUALS)){
        background->renderPane(rect, vars.background_color, 0);
        SDL_SetRenderDrawColor(vars.renderer, vars.outline_color.r, vars.outline_color.g, vars.outline_color.b, vars.outline_color.a);
        SDL_RenderDrawRect(vars.renderer, &rect);
    }

    if(initialized){
        initialized = false;
        ui->layers.push_back({this, rect});
    }

    if(vars.event->type == SDL_MOUSEBUTTONUP && vars.event->button.button == SDL_BUTTON_LEFT){
        if((flags & UIFLAGS::BUTTON_CLICK_ON_UP)) if(onRect(rect) && onClick != nullptr) if(ui->checkLayers(this, rect)) onClick(click_data);
        held = false;
    }
    if(vars.event->type == SDL_MOUSEBUTTONDOWN && vars.event->button.button == SDL_BUTTON_LEFT)
        if(onRect(rect))
            if(ui->checkLayers(this, rect)){
                if(onClick != nullptr && !(flags & UIFLAGS::BUTTON_CLICK_ON_UP)) onClick(click_data);
                held = true;
                time_held = 0;
                then = std::chrono::steady_clock::now();
            }

    if(held) if(onHold != nullptr){
        if(time_held >= this->vars.button_hold_wait){
            onHold(hold_data);
        }
        else{
            time_held += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-then).count();
            then = std::chrono::steady_clock::now();
        }
    }

    if(onRect(rect)){
        if(ui->checkLayers(this, rect)){
            int alfa = vars.hovered_color.a;
            if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK){
                alfa=alfa*3/2;
                if(alfa > 255) alfa = 255;
            }
            if(!(flags & UIFLAGS::NO_BUTTON_EFFECT)){
                SDL_SetRenderDrawColor(vars.renderer, vars.hovered_color.r, vars.hovered_color.g, vars.hovered_color.b, alfa);
                SDL_RenderFillRect(vars.renderer, &rect);
            }                       
        }
    }
    else if(held) if(!(flags & UIFLAGS::BUTTON_HOLD_NOT_RELATIVE)) held = false;
}

bool customsdl::onRect(SDL_Rect rect){
    int x, y;
    SDL_GetMouseState(&x, &y);
    return onRect(rect, x, y);
}

bool customsdl::onRect(SDL_Rect rect, int x, int y){
    if(x >= rect.x && x < rect.x+rect.w && y >= rect.y && y < rect.y+rect.h) return true;
    return false;
}

bool customsdl::compareRects(SDL_Rect rect1, SDL_Rect rect2){
    if(rect1.x == rect2.x && rect1.y == rect2.y && rect1.h == rect2.h && rect1.w == rect2.w) return true;
    return false;
}

color customsdl::getSurfaceColors(pixel coords, SDL_Surface *surface, bool freeSurface){
    Uint8 red{}, green{}, blue{}, alfa{};
    Uint32 values = *(Uint32*)((Uint8*)((Uint8*)surface->pixels + coords.y*surface->pitch + coords.x*surface->format->BytesPerPixel));
    SDL_GetRGBA(values, surface->format, (Uint8*)(&red), (Uint8*)(&green), (Uint8*)(&blue), (Uint8*)(&alfa));
    if(freeSurface) SDL_FreeSurface(surface);
    return {red, green, blue, alfa};
}

void customsdl::blitSurface(pixel coords, SDL_Surface *to, SDL_Surface *from){
    SDL_Rect areaTo = {coords.x, coords.y, to->w, to->h};
    SDL_BlitSurface(from, NULL, to, &areaTo);
}

void customsdl::blitSurface(pixel coords, SDL_Surface *to, SDL_Rect areaFrom, SDL_Surface *from){
    SDL_Rect areaTo = {coords.x, coords.y, to->w, to->h};
    SDL_BlitSurface(from, &areaFrom, from, &areaTo);
}

void customsdl::surfaceManipulation::createSurface(int width, int height, int depth, SDL_PixelFormatEnum format){
    active_surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, depth, format);
    SDL_SetSurfaceBlendMode(active_surface, SDL_BlendMode::SDL_BLENDMODE_ADD);    
    SDL_LockSurface(active_surface);
}

SDL_Texture* customsdl::surfaceManipulation::createTextureFromSurface(SDL_Renderer *renderer){
    SDL_Texture* _return;
    SDL_UnlockSurface(active_surface);
    _return = SDL_CreateTextureFromSurface(renderer, active_surface);
    SDL_FreeSurface(active_surface);
    return _return;
}

void customsdl::surfaceManipulation::drawToSurface(pixel coords, color RGBA){
    if(RGBA.a == 0) return;
    data = (Uint8*)active_surface->pixels + coords.y*active_surface->pitch + coords.x * active_surface->format->BytesPerPixel;
    *((Uint32*)data) = (Uint32)(RGBA.r << 0 | RGBA.g << 8 | RGBA.b << 16 | RGBA.a << 24);
}

HWND *customsdl::getWindowHandle(SDL_Window *window, int indx){
    //assumes we dont need more than 20 active windows;
    if(indx >= 20 || indx < 0) return nullptr;

    static bool firstLoad = true;
    static HWND* handles[20];

    if(firstLoad){
        std::fill_n(handles, 20, nullptr);
        firstLoad = false;
    }
    if(handles[indx] == nullptr){
        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);
        SDL_GetWindowWMInfo(window, &wmInfo);
        handles[indx] = &wmInfo.info.win.window;
    }
    return handles[indx];
}

bool customsdl::UI::checkLayers(void *obj_pointer, SDL_Rect &obj_area){
    static int x, y;

    //Update rect.
    for(UI::layer_Struct &layer : layers)
        if(layer.pointer == obj_pointer)
            layer.rect = obj_area;

    SDL_GetMouseState(&x, &y);
    for(int i = layers.size() - 1; i>=0; i--){
        if(onRect(layers[i].rect, x, y)){
            if(compareRects(obj_area, layers[i].rect) && layers[i].pointer == obj_pointer)
                return true;
            else
                return false;
        }
    }

    std::cout << "shouldn't reach this point?\n";
    return true;
}

void customsdl::UI::pane::renderPane(SDL_Rect rect, color clr, int flags){
    //Create the 'wall'
    if(initialized){
        initialized = false;
        index = ui->layers.size();
        ui->layers.push_back({this, rect});
    }

    if(!(flags & UIFLAGS::NO_VISUALS)){
        SDL_SetRenderDrawColor(vars.renderer, clr.r, clr.g, clr.b, clr.a);
        SDL_RenderFillRect(vars.renderer, &rect);
    }

}