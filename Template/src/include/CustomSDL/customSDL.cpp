#include <CustomSDL/customSDL.h>

//////////////////////

SDL_Surface *customsdl::UI::text::getSurface(SDL_Renderer *renderer, std::string text, SDL_Rect textBox, int fontsize, std::string fontpath, bool autoNewLines){
    if(last.text == text && compareRects(textBox, last.TextBox) && last.fontsize == fontsize && last.fontpath == fontpath && last.autonewlines == autoNewLines && last.surface != nullptr)
        return last.surface;
    wordLength.clear();
    SDL_DestroyTexture(last.texture);
    SDL_FreeSurface(last.surface);
    last.texture = nullptr;
    std::vector<std::string> words;
    std::string ind_word{""};

    FT_FaceRec_* FACE = parent_class.useFont(fontpath, fontsize);

    //Save each word and whitespaces in a vector
    for(char symb : text){
        if(symb == '\n'){
            words.push_back(ind_word);
            ind_word = "\n";
        }
        if(symb == ' '){
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
            if(autoNewLines && cWidth >= textBox.w || symb == '\n'){
                cHeight+=fontsize+1;
                cWidth = wordWidth+1;
            }
        }
        wordLength.push_back(wordWidth);
        wordWidth = 0;
        cWidth+=fontsize/4;
        maxWidth = std::max(maxWidth, cWidth);
    }
    maxHeight = cHeight+fontsize+4;

    //Create a surface where the sentence will be stored.
    surfaceManipulation manip;
    manip.createSurface(maxWidth, maxHeight, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_UnlockSurface(manip.active_surface);

    //Load each glyph surface and merge them into a single surface.
    cWidth = 1; cHeight = 1; wordWidth = 0;
    int belowBaseLine{};

    for(int i = 0; i < words.size(); i++){
        if((autoNewLines && i != 0 && wordWidth+wordLength[i] >= textBox.w) || (words[i][0] == '\n' && i!=0)){
            cHeight+=fontsize+1;
            wordWidth = 1;
            cWidth = 1;
        }
        for(char symb : words[i]){
            FT_Load_Char(FACE, symb, FT_LOAD_RENDER);
            belowBaseLine = (FACE->glyph->metrics.height - FACE->glyph->metrics.horiBearingY)/55;
            SDL_Surface *glyph;
            
            glyph = parent_class.surf8bitTo32bit(SDL_CreateRGBSurfaceFrom(FACE->glyph->bitmap.buffer, FACE->glyph->bitmap.width, FACE->glyph->bitmap.rows, 8, FACE->glyph->bitmap.pitch, 0, 0, 0, 0xFF));
            blitSurface({cWidth, cHeight-glyph->h+fontsize-fontsize/5+belowBaseLine}, manip.active_surface, glyph);
            SDL_FreeSurface(glyph);

            //Update some info.
            cWidth+=FACE->glyph->bitmap.width;
            wordWidth+=FACE->glyph->bitmap.width;
        }
        wordWidth+=fontsize/4;
        cWidth+=fontsize/4;
    }
    
    //Create a texture and store it.
    last.surface = manip.active_surface;
    last.autonewlines = autoNewLines;
    last.fontpath = fontpath;
    last.fontsize = fontsize;
    last.text = text;
    last.textDimensions = {textBox.x, textBox.y, maxWidth, maxHeight};
    last.TextBox = textBox;

    return manip.active_surface;
}

 SDL_Texture *customsdl::UI::text::getTexture(SDL_Renderer *renderer, std::string text, SDL_Rect textBox, int fontsize, std::string fontpath, bool autoNewLines){
    if(last.text == text && compareRects(textBox, last.TextBox) && last.fontsize == fontsize && last.fontpath == fontpath && last.autonewlines == autoNewLines && last.texture != nullptr)
        return last.texture;
    last.texture = SDL_CreateTextureFromSurface(renderer, getSurface(renderer, text, textBox, fontsize, fontpath, autoNewLines));
    return last.texture;
 }

SDL_Surface* customsdl::UI::surf8bitTo32bit(SDL_Surface* _8bit){
    SDL_SetPaletteColors(_8bit->format->palette, _8bitpalletecolors, 0, 256);
    SDL_SetSurfaceBlendMode(_8bit, SDL_BlendMode::SDL_BLENDMODE_ADD); 
    SDL_Surface* _return = SDL_ConvertSurfaceFormat(_8bit, SDL_PIXELFORMAT_RGBA32, 0);

    SDL_FreeSurface(_8bit);
    return _return;
}

void customsdl::UI::text::renderText(SDL_Renderer *renderer, std::string text, SDL_Rect textBox, int fontsize, std::string fontpath, bool autoNewLines, bool centered){
    last.texture = getTexture(renderer, text, textBox, fontsize, fontpath, autoNewLines);
    SDL_Rect dest = textBox;
    if(centered){
        dest.x = textBox.x + textBox.w/2 - last.textDimensions.w/2;
        dest.y = textBox.y + textBox.y/2 - last.textDimensions.y/2;
    }
    dest.w = last.textDimensions.w;
    dest.h = last.textDimensions.h;
    SDL_RenderCopy(renderer, last.texture, NULL, &dest);
    return;
}

void customsdl::UI::scrollBox::renderTextureScrollBox(SDL_Renderer *renderer, SDL_Event &evt, SDL_Rect boxToRenderIn, int *textureWidth, int *textureHeight, SDL_Texture *texture){
    scrollBoxCore(renderer, evt, boxToRenderIn, textureWidth, textureHeight, texture);

    SDL_SetRenderDrawColor(renderer, outline_color.r, outline_color.g, outline_color.b, outline_color.a);
    SDL_RenderDrawRect(renderer, &boxToRenderIn);

    SDL_Rect renderArea = {0, int(shifty*ratio), *textureWidth, *textureHeight};
    SDL_Rect destArea = {boxToRenderIn.x, boxToRenderIn.y, *textureWidth, *textureHeight};
    SDL_RenderCopy(renderer, texture, &renderArea, &destArea);
    return;
}

void customsdl::UI::scrollBox::renderButtonScrollBox(SDL_Renderer *renderer, SDL_Event &evt, SDL_Rect boxToRenderIn, int *textureWidth, int *textureHeight, SDL_Texture *texture, void (*onClick)(std::string), std::vector<std::string> entries, int fontSize, std::string fontpath){
    renderTextureScrollBox(renderer, evt, boxToRenderIn, textureWidth, textureHeight, texture);

    SDL_GetMouseState(&x, &y);
    if(onRect(boxToRenderIn, x, y)){
        SDL_SetRenderDrawColor(renderer, button_hovered.r, button_hovered.g, button_hovered.b, button_hovered.a);
        
        //Hover
            //Which button in the list.
        int n = (boxToRenderIn.y-y+shifty)/fontSize;
        SDL_Rect hover_area = {boxToRenderIn.x, boxToRenderIn.y + n*fontSize-shifty, boxToRenderIn.w, fontSize}; //todo
        SDL_RenderFillRect(renderer, &hover_area);

        //Click
        if(evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == SDL_BUTTON_LEFT && n < entries.size())
            onClick(entries[n]);
    }

    return;
}

void customsdl::UI::scrollBox::scrollBoxCore(SDL_Renderer *renderer, SDL_Event &evt, SDL_Rect boxToRenderIn, int *textureWidth, int *textureHeight, SDL_Texture *texture){
    //VERTICAL
    ratio = *textureHeight/boxToRenderIn.h;
    int bar_height = boxToRenderIn.h*boxToRenderIn.h/(*textureHeight);
    SDL_Rect bar = {boxToRenderIn.x+boxToRenderIn.w, boxToRenderIn.y+shifty, bar_width, bar_height};

    if(evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == SDL_BUTTON_LEFT) if(onRect(bar)){clicked = true; SDL_GetMouseState(NULL, &clickedy);}
    if(evt.type == SDL_MOUSEBUTTONUP && evt.button.button == SDL_BUTTON_LEFT) clicked = false;
    if(clicked){
        SDL_GetMouseState(NULL, &y);
        shifty += y-clickedy;
        clickedy = y;
    }

    SDL_SetRenderDrawColor(renderer, bar_color.r, bar_color.g, bar_color.b, bar_color.a);
    if(ratio > 1) SDL_RenderFillRect(renderer, &bar);

    //TODO: HORIZONTAL

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

void customsdl::UI::button::renderButton(SDL_Renderer *renderer, SDL_Event &evt, SDL_Rect rect, void (*onClick)(void*), void* data, SDL_Texture *texture){
    CoreButton(evt, rect, onClick, data);

    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_SetRenderDrawColor(renderer, outline_color.r, outline_color.g, outline_color.b, outline_color.a);
    if(onRect(rect)){
        if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK) SDL_SetRenderDrawColor(renderer, hovered_color.r, hovered_color.g, hovered_color.b, hovered_color.a-hovered_color.a/3);
        else SDL_SetRenderDrawColor(renderer, hovered_color.r, hovered_color.g, hovered_color.b, hovered_color.a);

        SDL_RenderFillRect(renderer, &rect);
    }
}

void customsdl::UI::button::CoreButton(SDL_Event &evt, SDL_Rect rect, void (*onClick)(void*), void* data){
    if(evt.type == SDL_MOUSEBUTTONUP && evt.button.button == SDL_BUTTON_LEFT)
        if(onRect(rect)) onClick(data);
    return;
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

bool customsdl::compareRects(SDL_Rect &rect1, SDL_Rect &rect2){
    if(rect1.x == rect2.x && rect1.y == rect2.y && rect1.h == rect2.h && rect1.w == rect2.w) return true;
    return false;
}

customsdl::color customsdl::getSurfaceColors(pixel coords, SDL_Surface *surface, bool freeSurface){
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