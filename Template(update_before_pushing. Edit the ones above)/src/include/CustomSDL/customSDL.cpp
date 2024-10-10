/* Custom functions and data types that are useful for various SDL applications. */

#include<CustomSDL/customSDL.h>

//////////////////////////
//TODO
//The heck is this?
customsdl::UI *customsdl::UI::ui = nullptr;

///////////////////////////////////////////
/* Uncategorised */
    bool customsdl::onRect(SDL_Rect rect){
        /* Get current mouse position and call the variant of this function that {x,y} values. */
        int x, y;
        SDL_GetMouseState(&x, &y);
        return onRect(rect, x, y);
    }

    bool customsdl::onRect(SDL_Rect rect, int x, int y){
        /* Determines if a point is inside/on a rectangle. */
        if(x >= rect.x && x < rect.x+rect.w && y >= rect.y && y < rect.y+rect.h) return true;
        return false;
    }

    bool customsdl::compareRects(SDL_Rect rect1, SDL_Rect rect2){
        /* Compares two rects' values because you can't just (rect1==rect2). */
        if(rect1.x == rect2.x && rect1.y == rect2.y && rect1.h == rect2.h && rect1.w == rect2.w) return true;
        return false;
    }

    bool customsdl::compareColors(color clr1, color clr2){
        /* Compares two colors' values because you can't just (clr1==clr2) */
        if(clr1.r == clr2.r && clr1.g == clr2.g && clr1.b == clr2.b && clr1.a == clr2.a) return true;
        return false;
    }

    color customsdl::getSurfaceColors(pixel coords, SDL_Surface *surface){
        /* Color key values */
        Uint8 red{}, green{}, blue{}, alfa{};
        /* Gets the Uin32 integer that stores the color values from a surface. */
        Uint32 values = *(Uint32*)((Uint8*)((Uint8*)surface->pixels + coords.y*surface->pitch + coords.x*surface->format->BytesPerPixel));
        /* Takes the 8bit values (each represents a color) from a 32bit integer. */
        SDL_GetRGBA(values, surface->format, (Uint8*)(&red), (Uint8*)(&green), (Uint8*)(&blue), (Uint8*)(&alfa));
        /* Returns in a struct data type of customcpp::color{...} */
        return {red, green, blue, alfa};
    }

    void customsdl::blitSurface(pixel coords, SDL_Surface *to, SDL_Rect areaFrom, SDL_Surface *from){
        /* the purpose is to just make this a bit more intuitive. */
        SDL_Rect areaTo = {coords.x, coords.y, to->w, to->h};
        SDL_BlitSurface(from, &areaFrom, from, &areaTo);
    }

    void customsdl::blitSurface(pixel coords, SDL_Surface *to, SDL_Surface *from){
        /* the purpose is to just make this a bit more intuitive. */
        SDL_Rect areaTo = {coords.x, coords.y, to->w, to->h};
        SDL_BlitSurface(from, NULL, to, &areaTo);
    }

    SDL_Surface* customsdl::createSurface(int width, int height){
        /* Just an alias for this long function name to create an RGBA surface with _ADD blendmode*/
        auto _return = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32);
        SDL_SetSurfaceBlendMode(_return, SDL_BlendMode::SDL_BLENDMODE_ADD);

        return _return;
    }

    void customsdl::drawSurfaceRect(SDL_Rect rect, SDL_Surface *surface, color clr, bool blend){
        /* Make sure to not draw outside the bounds. */
        if(rect.x < 0) rect.x = 0;
        if(rect.y < 0) rect.y = 0;
        if(rect.x + rect.w >= surface->w) rect.w = surface->w-1-rect.x;
        if(rect.y + rect.h >= surface->h) rect.h = surface->h-1-rect.y;

        /* Check if a surface is 32bit one. `customsdl::drawToSurface` only supports 32bit ones. */
        if(surface->format->format != SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32){
            std::cout << "Surface provided to `customsdl::drawSurfaceRect` must be a 32bit one!\n";
            return;
        }

        //upper horizontal
        for(int x = rect.x; x < rect.x+rect.w; x++) drawToSurface(surface, {x, rect.y}, clr, blend);
        //lower horizontal
        for(int x = rect.x; x < rect.x+rect.w; x++) drawToSurface(surface, {x, rect.y+rect.h}, clr, blend);
        //Left vertical
        for(int y = rect.y; y < rect.y+rect.h; y++) drawToSurface(surface, {rect.x, y}, clr, blend);    
        //Right vertical
        for(int y = rect.y; y < rect.y+rect.h; y++) drawToSurface(surface, {rect.x+rect.w, y}, clr, blend);
    }

    void customsdl::drawSurfaceLine(pixel point0, pixel point1, SDL_Surface *surface, color clr, bool blend){  
        /* source: https://www.geeksforgeeks.org/dda-line-generation-algorithm-computer-graphics/ */ 

        /* A value-rounding lambda function */
        auto __round = [](float n){
            if (n - (int)n < 0.5) return (int)n; 
            return (int)(n + 1); 
        };

        /* Check if a surface is 32bit one. `customsdl::drawToSurface` only supports 32bit ones. */
        if(surface->format->format != SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32){
            std::cout << "Surface provided to `customsdl::drawSurfaceLine` must be a 32bit one!\n";
            return;
        }

        int dx = point1.x - point0.x; 
        int dy = point1.y - point0.y;   
        int step; 

        if (std::abs(dx) > std::abs(dy)) 
            step = std::abs(dx); 
        else
            step = std::abs(dy); 
    
        float x_incr = (float)dx / step; 
        float y_incr = (float)dy / step; 
    
        float x = point0.x; 
        float y = point0.y; 
    
        for (int i = 0; i < step; i++) { 
            drawToSurface(surface, {__round(x), __round(y)},clr, blend);
            x += x_incr; 
            y += y_incr; 
        } 
    } 

     void customsdl::drawToSurface(SDL_Surface *surf32, pixel coords, color &clr, bool blend){
        if(clr.a != 255 && blend){
            float srcAlpha = clr.a/255.0f;

            /* Get given point's color value */
            color dst = getSurfaceColors(coords, surf32);

            clr.r = static_cast<Uint8>(srcAlpha * clr.r+(1-srcAlpha)*dst.r);
            clr.g = static_cast<Uint8>(srcAlpha * clr.g+(1-srcAlpha)*dst.g);
            clr.b = static_cast<Uint8>(srcAlpha * clr.b+(1-srcAlpha)*dst.b);
        }

        /* Get a pointer to a pixel within a surface. */
        Uint8 *data = (Uint8*)surf32->pixels + coords.y*surf32->pitch + coords.x * surf32->format->BytesPerPixel;
        /* Push the color values to a 32bit integer. */
        *((Uint32*)data) = (Uint32)(clr.r << 0 | clr.g << 8 | clr.b << 16 | clr.a << 24);
    }

    void customsdl::fillSurfaceRect(SDL_Rect rect, SDL_Surface *surface, color clr, bool blend){
        /* Make sure to not draw outside the bounds. */
        if(rect.x < 0) rect.x = 0;
        if(rect.y < 0) rect.y = 0;
        if(rect.x + rect.w >= surface->w) rect.w = surface->w-1-rect.x;
        if(rect.y + rect.h >= surface->h) rect.h = surface->h-1-rect.y;

        /* Check if a surface is 32bit one. `customsdl::drawToSurface` only supports 32bit ones. */
        if(surface->format->format != SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32){
            std::cout << "Surface provided to `customsdl::fillSurfaceRect` must be a 32bit one!\n";
            return;
        }

        /* Fill the rectangle */
        for(int y = rect.y; y < rect.y+rect.h; y++)
            for(int x = rect.x; x < rect.x+rect.w; x++)
                drawToSurface(surface, {x, y}, clr, blend);
    }

    #ifdef _WIN32
        HWND customsdl::getWindowHandle(SDL_Window *window){
            SDL_SysWMinfo wmInfo;
            SDL_VERSION(&wmInfo.version);
            SDL_GetWindowWMInfo(window, &wmInfo);
            return wmInfo.info.win.window;
        }
    #endif

/* UI functions */
    bool customsdl::UI::layerCheck(void *ptr){
        /* Check if the first rect in mx,my coords is 'this' pointer */
        for(int i=ui->layers.size()-1; i>=0; i--){

            if(ui->layers[i].BUTTON != nullptr){ 
                if(onRect(ui->layers[i].BUTTON->rect, mx, my)){
                    if(ui->layers[i].BUTTON == ptr) return true;
                    return false;
                }
            }
            else if(ui->layers[i].PANE != nullptr){ 
                if(onRect(ui->layers[i].PANE->rect, mx, my)){
                    if(ui->layers[i].PANE == ptr) return true;
                    return false;
                }
            }
        }

        /* Shouldn't reach this point */
        return true;
    }

    FT_FaceRec_* customsdl::UI::getFont(std::string &fontPath, int &fontSize){
        /* Go through already initialized fonts in memory and return an existing one. */
        for(UI::FONTDATA_STRUCT &obj : ui->fontData)
            if(obj.fontPath == fontPath && obj.fontSize == fontSize) return obj.font;

        /* Initialize a new font */
            /* Provide path arguments */
            FT_Open_Args args;
            args.flags = FT_OPEN_PATHNAME;
            args.pathname = fontPath.data();

            /* Push back the vector with data*/
            ui->fontData.push_back({fontPath, fontSize, nullptr, nullptr});
            UI::FONTDATA_STRUCT *obj = &ui->fontData[ui->fontData.size()-1];

            /* Initialize FreeType library*/
            FT_Init_FreeType(&obj->lib);

            /* Try to initialize the font*/
            if(FT_Open_Face(obj->lib, &args, 0, &obj->font))
                /* Returns true on failure */
                /* TODO: try catching an exception and loading a default font or something else that won't crash the program? */
                std::cout << "Failed to initialize font face (fontpath: \"" << fontPath <<"\") of size \"" << fontSize << "\".\n";

            /* Set the required parameters */
            obj->font->glyph->format = FT_GLYPH_FORMAT_BITMAP;
            FT_Set_Pixel_Sizes(obj->font, fontSize, 0);
        
            return obj->font;
    }

    SDL_Surface* customsdl::UI::surf8bitTo32bit(SDL_Surface* _8bit){
        SDL_SetPaletteColors(_8bit->format->palette, _filled_pallete, 0, 256);
        SDL_Surface* _return = SDL_ConvertSurfaceFormat(_8bit, SDL_PIXELFORMAT_RGBA32,0);

        SDL_FreeSurface(_8bit);
        return _return;
    }

    void customsdl::UI::pane::renderPane(SDL_Rect rect, color clr, int orderInLayer, Uint32 flags){
        if(ui->CHECK){
            /* Store the pane in the layers list */
            ui->layers.push_back({new PANE_STRUCT({rect, clr, flags}), nullptr, nullptr, nullptr, orderInLayer});
        }
    }

    void customsdl::UI::button::renderButton(SDL_Rect rect, void (*onClick)(void*), void* click_data, void (*onHold)(void*), void* hold_data, int orderInLayer, Uint32 flags){
        if(ui->CHECK){
            /* Modify position if a flag to center is used */
            if(flags & UIFLAGS::XCENTERED) rect.x = ui->data.wind_width/2 - rect.w/2; 
            if(flags & UIFLAGS::YCENTERED) rect.y = ui->data.wind_height/2 - rect.h/2;

            /* The click-blocking pane */
            if(!(flags & UIFLAGS::NO_PANE)) ui->pane.renderPane(rect, ui->data.background_color, orderInLayer, flags); 

            /* Store the button in the layers list */
            ui->layers.push_back({nullptr, new BUTTON_STRUCT({rect, onClick, click_data, onHold, hold_data, 0, flags}), nullptr, nullptr, orderInLayer});  
        }
   }

   void customsdl::UI::text::renderText(std::string text, SDL_Rect rect, color clr, int orderInLayer, int fontSize, Uint32 flags){
        if(ui->CHECK)  ui->layers.push_back({nullptr,nullptr, new TEXT_STRUCT({text, clr, rect, fontSize, ui->data.default_font, flags}), nullptr, orderInLayer});
    }

    void customsdl::UI::text::renderText(std::string text, SDL_Rect rect, color clr, int orderInLayer, int fontSize, std::string fontPath, Uint32 flags){
        if(ui->CHECK) ui->layers.push_back({nullptr,nullptr, new TEXT_STRUCT({text, clr, rect, fontSize, fontPath, flags}), nullptr, orderInLayer});
    }

    void customsdl::UI::scrollbox::renderSurfaceScrollBox(SDL_Surface *surface, SDL_Rect rect, int orderInLayer, Uint32 flags){
        if(ui->CHECK) ui->layers.push_back({nullptr,nullptr,nullptr, new SCROLLBOX_STRUCT({rect, {}, flags, 0, surface}), orderInLayer});
    }

    void customsdl::UI::scrollbox::renderTextScrollBox(std::string text, SDL_Rect rect, color clr, int orderInLayer, int fontSize, std::string fontPath, Uint32 flags){
        if(ui->CHECK){
            if(!(flags & UIFLAGS::NO_PANE)) ui->pane.renderPane(rect, ui->data.background_color, orderInLayer, flags);
            ui->text.renderText(text, rect, clr, orderInLayer, fontSize, fontPath, flags);
            UI::TEXT_STRUCT *ptr = ui->layers[ui->layers.size()-1].TEXT;
            ptr->render = false;
            ui->layers.push_back({nullptr,nullptr,nullptr, new SCROLLBOX_STRUCT({rect, {}, flags, 1}), orderInLayer});
            ui->layers[ui->layers.size()-1].SCROLLBOX->text_ptr = ptr;
        }
    }

    void customsdl::UI::scrollbox::renderTextScrollBox(std::string text, SDL_Rect rect, color clr, int orderInLayer, int fontSize, Uint32 flags){
        if(ui->CHECK) ui->scrollbox.renderTextScrollBox(text, rect, clr, orderInLayer, fontSize, ui->data.default_font, flags);
    }

    void customsdl::UI::scrollbox::renderButtonScrollBox(std::vector<std::string> entries, SDL_Rect rect, color clr, int orderInLayer, int fontSize, std::string fontPath, void (*onClick)(std::string), Uint32 flags){
        if(ui->CHECK){
            static std::string in;
            in = "";
            for(std::string &line : entries)
                in+=line + "\n";
            in.pop_back(); /*Get rid of trailing \n*/

            if(!(flags & UIFLAGS::NO_PANE)) ui->pane.renderPane(rect, ui->data.background_color, orderInLayer, flags);
            ui->text.renderText(in, rect, clr, orderInLayer, fontSize, fontPath, flags);
            UI::TEXT_STRUCT *ptr = ui->layers[ui->layers.size()-1].TEXT;
            ptr->render = false;
            ui->layers.push_back({nullptr, nullptr, nullptr, new SCROLLBOX_STRUCT({rect, entries, flags, 2}), orderInLayer});
            ui->layers[ui->layers.size()-1].SCROLLBOX->text_ptr = ptr;
            ui->layers[ui->layers.size()-1].SCROLLBOX->onClick = onClick;
            ui->layers[ui->layers.size()-1].SCROLLBOX->fontSize = fontSize;
        }
    }

    void customsdl::UI::scrollbox::renderButtonScrollBox(std::vector<std::string> entries, SDL_Rect rect, color clr, int orderInLayer, int fontSize, void (*onClick)(std::string), Uint32 flags){
        if(ui->CHECK) ui->scrollbox.renderButtonScrollBox(entries, rect, clr, orderInLayer, fontSize, ui->data.default_font, onClick, flags);
    }

    void customsdl::UI::__rend_pane(PANE_STRUCT *str){
        /* If no NO_VISUAL flag, draw the pane/rectangle */
        if(!(str->flags & UI::UIFLAGS::NO_VISUALS)){
            fillSurfaceRect(str->rect, sDisplay, str->clr, true);
        }
    }

    void customsdl::UI::__rend_button(BUTTON_STRUCT *str){
        /* Lambda function to draw the pressed button (to not repeat twice) 
           Pressed state -> user pressed a button. Apply the hovered_color with a lower alfa value */
        auto pressedButton = [&](){
            int alfa = ui->data.hovered_color.a/2;
            /* Fill surface rect with the appropriate color */
            fillSurfaceRect(str->rect, sDisplay, {ui->data.hovered_color.r, ui->data.hovered_color.g, ui->data.hovered_color.b, (uint8_t)alfa}, true);
        };

        /* Determine button state */
            //__CORE_BUTTON(str); /*Already determined in ui::present function */
        int core_status = str->state;
        /**
         * 0 -> base state, which is always active
         * 1 -> hovered state
         * 2 -> pressed state
         */

        /* If this flag was used, overwrite the visual changes */
        if(str->flags & UIFLAGS::BUTTON_STATE_PRESSED) pressedButton();
        else /* Else proceed with proper visuals */
        switch(core_status){
            case 1:
                /* Hovered state->user hovers over a button */
                fillSurfaceRect(str->rect, sDisplay, ui->data.hovered_color, true);
                break;
            case 2:
                pressedButton();
                break;  
        }

        /*Base state->no user interaction.*/
        /*Pane has already been drawn*/
        drawSurfaceRect(str->rect, sDisplay, ui->data.outline_color, true);
    }

    void customsdl::UI::__CORE_BUTTON(BUTTON_STRUCT *str){
        static int time_held = 0; /*Track the time a button was held. Only 1 button can be held at a time*/
        static std::chrono::steady_clock::time_point then; /*to get the time difference*/
        
        /*Button Unpressed*/
        if(unpressed_qm){
            /* Base state, may be overwriten to "hover" and or "clicked" further down the lines*/
            str->state = 0;

            /* If flags and conditions return to true, call a "pressed" function */
            /* Check if a "onClick" function was even passed */
            if(str->onClick != nullptr) 
            /* Check if a flag to trigger onClick on LMB up is used and whether button is active */
            if((str->flags & UIFLAGS::BUTTON_CLICK_ON_UP) && !(str->flags & UIFLAGS::BUTTON_STATE_INACTIVE)) 
            /* Check if a button press was made on 'this' button */
            if(onRect(str->rect, mx, my)) 
            /* Check if another UI element did not block a button click */
            if(layerCheck((void*)str)){
                /*IF ALL TRUE: */
                str->onClick(str->click_data);
            }
        }

        /*Button Pressed, if it is active* */
        if(pressed_qm  && !(str->flags & UIFLAGS::BUTTON_STATE_INACTIVE)){
            /* Check if a button press was made on 'this' button */
            if(onRect(str->rect, mx, my))
            /* Check if another UI element did not block a button click */
            if(layerCheck((void*)str)){
                /* Check if a button click is only made on LMB up, if not, and there's a passed onClick function, call it. */
                if(!(str->flags & UIFLAGS::BUTTON_CLICK_ON_UP) && str->onClick != nullptr) str->onClick(str->click_data);
                /* Button is pressed and effectivelly held */
                str->state = 2;
                /*Time vars*/
                time_held = 0;
                then = std::chrono::steady_clock::now();
            }
        }

        /* Button held */
        if(str->state == 2)
        /* Check if there's a onHold function passed, for which we count the time held*/
        if(str->onHold != nullptr){
            /* Call onHold function if enough time passed */
            if(time_held >= ui->data.hold_wait) str->onHold(str->hold_data);
            /* Track passed time if not */
            else{
                time_held += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-then).count();
                then = std::chrono::steady_clock::now();
            }
        }

        /* Check if mouse hovered, while button is not held */
        if(str->state != 2){
            /* Check if hovering over "this" button */
            if(onRect(str->rect, mx, my)){
                /* ...while another button on top does not interfere */
                if(layerCheck((void*)str))
                /* If this flag is not used */
                if(!(str->flags & UIFLAGS::NO_BUTTON_EFFECT))
                /* Hovered state */
                str->state = 1;
            }
            /* If not within the rect, means button is untouched */
            else str->state = 0;
        }
    }

    void customsdl::UI::__rend_text(TEXT_STRUCT *str){
        /* Get surface */  
        ui->__CORE_TEXT(str);

        /* Blit text surface to the main surface */
        if(str->render) blitSurface({str->pivot.x, str->pivot.y}, ui->sDisplay, str->surface);
    }

    void customsdl::UI::__CORE_TEXT(TEXT_STRUCT *str){
        /* Text has already been rendered, return. */
        if(str->surface != nullptr) return;
        str->pivot = {str->rect.x, str->rect.y};  

        /* An array that holds each each word */
        std::vector<std::string> words;
        std::string word{""}; /* Temp string to hold each word width */

        /* Pointer to loaded font face */
        FT_FaceRec_ *font = ui->getFont(str->fontPath, str->fontSize);

        /* Get whole text's dimensions, by finding appropriate position for words and their width & height */
            /* Save each word, and whitespaces, in a vector array */
                
            auto new_word = [&](std::string a){
                /* do not repeat yourself */
                /* Pushes back a word and resets the string*/
                words.push_back(word);
                word = a;
            };

            for(char symb : str->text){
                if(symb == '\n') new_word("\n");
                else if(symb == ' ') new_word("");
                else word += symb;
            }
            /* Push back the trailing word*/
            words.push_back(word);

            /* Get the total width and height of the area the text will occupy */
            int cWidth=1, cHeight=1; /* 'c' stands for CURRENT */
            int wordWidth=0;
            int maxWidth=0, maxHeight=0;

            /* Width of whitespace */
            int whitespaceWidth = str->fontSize/4;
            /* fontSize happens to be characters height */
            int &charHeight = str->fontSize;

            for(std::string word : words){
                /* Loop through each symbol */
                for(char symb : word){ 
                    /* Load a specific character metrics */
                    FT_Load_Char(font, symb, FT_LOAD_BITMAP_METRICS_ONLY);
                    /* Add character's width */
                    cWidth+=font->glyph->bitmap.width;
                    wordWidth+=font->glyph->bitmap.width;
                    
                    /* Check if it is a new line OR current width is outside text box's area and, if a flag was used, automatically put the text in a new line */
                    if(((str->flags & UIFLAGS::AUTO_NEW_LINES) && cWidth>=str->rect.w) || symb == '\n'){
                        /* Increase height */
                        cHeight+=charHeight+1;
                        /* Reset width to word's width (since the whole word goes to a new line)*/
                        cWidth=wordWidth+1;
                    }
                }
                /* Track each word's length*/
                str->wordLenght.push_back(wordWidth);
                wordWidth = 0; /* reset */
                cWidth+=whitespaceWidth;
                if(cWidth > maxWidth) maxWidth = cWidth;
            }
            /* Max height is current height (last row character's very top position) + character's height */
            maxHeight = cHeight+charHeight;

        /* Create a surface where the text will be stored and bitmap everything to it */
            str->surface = createSurface(maxWidth, maxHeight);

            /* Tells how many pixels from the top left corner should the origin point be (when 0, naturally str->rect) */
            if(str->flags & UIFLAGS::XCENTERED && str->rect.w > maxWidth) str->pivot.x = str->rect.x+(str->rect.w-maxWidth)/2+1;
            if(str->flags & UIFLAGS::YCENTERED && str->rect.h > maxHeight) str->pivot.y = str->rect.y+(str->rect.h-maxHeight)/2+1;

            /* Keep track of char positions */
            cWidth=1; cHeight=1;
            
            /* Get each characters's bitmap, convert to and SDL_Surface and blit character to the main surface */
            for(int i = 0; i < words.size(); i++){
                /* New line if: flag is used, current row is not the first row and current width + current word's width is wider than rect OR new line was used */
                if(((str->flags & UIFLAGS::AUTO_NEW_LINES) && i != 0 && cWidth+str->wordLenght[i]>=str->rect.w) || words[i][0] == '\n'){
                    cHeight+=charHeight+1;
                    cWidth=1;
                }
                
                for(char symb : words[i]){
                    FT_Load_Char(font, symb, FT_LOAD_RENDER);
                    /* calculate how many pixels to add to the current height to level certain characters, that extend "below ground" (notice that 'qajoy' are not exactly on the the same level)*/
                    int belowBaseLine = str->fontSize+(font->glyph->metrics.height - font->glyph->metrics.horiBearingY)/55-str->fontSize/5; /* this is a terrible approach, but this gets just about the right height, somehow*/                
                    
                    /* Convert face's bitmap to an sdl_surface*/
                    SDL_Surface *glyph;
                    glyph = ui->surf8bitTo32bit(SDL_CreateRGBSurfaceFrom(font->glyph->bitmap.buffer, font->glyph->bitmap.width, font->glyph->bitmap.rows, 8, font->glyph->bitmap.pitch, 0,0,0,0xFF));
                    
                    /* Blit glyph to the main surface and clean up*/
                    blitSurface({cWidth, cHeight + (belowBaseLine-glyph->h)}, str->surface, glyph);
                    SDL_FreeSurface(glyph);

                    cWidth+=font->glyph->bitmap.width;
                }
                cWidth+=whitespaceWidth;
            }

        /* Modulate colors */
        for(int x = 0; x < maxWidth; x++)
            for(int y = 0; y < maxHeight; y++){
                /* Get surface colors */
                color src = getSurfaceColors({x,y}, str->surface);
                /* Because this surface is a mixture of black and white, change the values to just white */
                src.r = src.g = src.b = 255;

                /* Modulate colors */
                /* TODO: when clr.x is 0, result's 255*/
                src.r = (Uint8)(int(src.r*(float)str->clr.r/255) << 0);
                src.g = (Uint8)(int(src.g*(float)str->clr.g/255) << 0);
                src.b = (Uint8)(int(src.b*(float)str->clr.b/255) << 0);
                src.a = (Uint8)(int(src.a*(float)str->clr.a/255) << 0);

                /* Draw the modulated colors to the surface */
                drawToSurface(str->surface, {x,y}, src, 0);
            }

        /* Honestly, i don't know bitmaps well enough to understand why i have to do convert here. Isn't str->surface already an RGBA surface? Converting to a texture yields correct results but nothing shows up if i were to blit to another RGBA32 surface (sDisplay)*/
        SDL_Surface *converted = SDL_ConvertSurface(str->surface, sDisplay->format, 0);
        SDL_FreeSurface(str->surface);
        str->surface = converted;

        return;
    }

    void customsdl::UI::__rend_surface_scrollbox(SCROLLBOX_STRUCT *str){
        __CORE_SCROLLBOX(str);
        if(!(str->flags & UIFLAGS::NO_BUTTON_TEXT)){
            SDL_Rect renderArea = {int(str->shiftx*str->ratiox), int(str->shifty*str->ratioy), std::min(str->rect.w, str->surface->w), std::min(str->rect.h, str->surface->h)};
            SDL_Rect destArea = {str->rect.x, str->rect.y, std::min(str->rect.w, str->surface->w), std::min(str->rect.h, str->surface->h)};
            SDL_BlitSurface(str->surface, &renderArea, ui->sDisplay, &destArea);
        }
    }

    void customsdl::UI::__rend_text_scrollbox(SCROLLBOX_STRUCT *str){
        ui->__rend_surface_scrollbox(str);
        if(!(str->flags & UIFLAGS::NO_VISUALS)){
            drawSurfaceRect(str->rect, ui->sDisplay, ui->data.outline_color, true);
        }
    }

    void customsdl::UI::__rend_button_scrollbox(SCROLLBOX_STRUCT *str){
        ui->__rend_surface_scrollbox(str);
        if(!(str->flags & UIFLAGS::NO_VISUALS)){
            drawSurfaceRect(str->rect, ui->sDisplay, ui->data.outline_color, true);
        }

        if(str->whichHovered != -1)
            fillSurfaceRect(str->hover_area, sDisplay, ui->data.hovered_color, true);
    }

    void customsdl::UI::__CORE_SCROLLBOX(SCROLLBOX_STRUCT *str){
        /* assign surface pointer */
        if(str->text_ptr != nullptr && str->surface == nullptr) str->surface = str->text_ptr->surface;
        if(str->surface == nullptr) return;

        /* Vertical */
            /* Determine scroll bar's height */
            str->ratioy = (float)(str->surface->h)/str->rect.h;
            int bar_height = str->rect.h/str->ratioy;

            str->vert_bar = {str->rect.x + str->rect.w, str->rect.y+str->shifty, 8, bar_height};

            /* Move the scroll bar if it is held*/
            if(ui->pressed_qm) if(onRect(str->vert_bar) && !(str->flags & UIFLAGS::LOCK_VERTICAL_SCROLLBAR) && str->ratioy > 1){
                str->state_clickedy = true;
                str->clickedY = ui->my;
            }
            if(ui->unpressed_qm) str->state_clickedy = false;

            if(str->state_clickedy){
                /* move the surface and the bar*/
                str->shifty+=ui->my-str->clickedY;
                str->clickedY=ui->my;

                /* Fix out of bound movements and determine sticked state */
                str->stickY = false;
                if(str->shifty < 0) str->shifty = 0;
                else if(str->shifty+bar_height >= str->rect.h){
                    str->shifty = str->rect.h-bar_height-1;
                    str->stickY = true;
                }
            }

            if(str->stickY)
                if(str->flags & UIFLAGS::VERTICAL_SCROLLBAR_STICK)
                    str->shifty = str->rect.h-bar_height-1;

        /* Horizontal */
            /* Determine scroll bar's width */
            str->ratiox = (float)(str->surface->w)/str->rect.w;
            int bar_width = str->rect.w/str->ratiox;

            str->horiz_bar = {str->rect.x + str->shiftx, str->rect.y+str->rect.h, bar_width, 8};

            /* Move the scroll bar if it is held*/
            if(ui->pressed_qm) if(onRect(str->horiz_bar) && !(str->flags & UIFLAGS::LOCK_HORIZONTAL_SCROLLBAR) && str->ratiox > 1){
                str->state_clickedx = true;
                str->clickedX = ui->mx;
            }
            if(ui->unpressed_qm) str->state_clickedx = false;

            if(str->state_clickedx){
                /* move the surface and the bar*/
                str->shiftx+=ui->mx-str->clickedX;
                str->clickedX=ui->mx;

                /* Fix out of bound movements and determine sticked state */
                str->stickX = false;
                if(str->shiftx < 0) str->shiftx = 0;
                else if(str->shiftx+bar_width >= str->rect.w){
                    str->shiftx = str->rect.w-bar_width-1;
                    str->stickX = true;
                }
            }

            if(str->stickX)
                if(str->flags & UIFLAGS::HORIZONTAL_SCROLLBAR_STICK)
                    str->shiftx = str->rect.w-bar_width-1;
        if(!(str->flags & UIFLAGS::NO_VERTICAL_SCROLLBAR) && str->ratioy > 1)
            fillSurfaceRect(str->vert_bar, ui->sDisplay, ui->data.bar_color, false);
        if(!(str->flags & UIFLAGS::NO_HORIZONTAL_SCROLLBAR) && str->ratiox > 1)
            fillSurfaceRect(str->horiz_bar, ui->sDisplay, ui->data.bar_color, false);

        /* If it's button scrollbox, determine which hovered and pressed */
            if(str->whichScrollbox == 2){
                if(onRect(str->rect, mx, my)){
                    /* Hover effect */
                        /* Find the button in the list*/
                        int n = (my-str->rect.y+str->shifty*str->ratioy)/(str->fontSize+1);
                        str->whichHovered = n;
                    if(n < str->entries.size() && !(str->flags & NO_BUTTON_EFFECT)){
                        int y_hover = std::max(int(str->rect.y+n*(str->fontSize+1)-str->shifty*str->ratioy), str->rect.y);
                        int h_hover = str->fontSize+1;
                        int offsetTop = int(str->shifty*str->ratioy)%h_hover;
                        int offsetBottom = str->rect.h%h_hover;

                        /* Lower the area on clipped top button */
                        if(my < str->rect.y + h_hover - offsetTop) h_hover -= offsetTop;
                        /* lower the are on clipped bottom button */
                        if(y_hover + h_hover > str->rect.y+str->rect.h) h_hover = str->rect.y+str->rect.h-y_hover;
                        
                        str->hover_area = {str->rect.x, y_hover, str->rect.w, h_hover};
                    }

                    /* Clicked */
                    if(ui->pressed_qm && n < str->entries.size())
                        str->onClick(str->entries[n]);
                }
                else str->whichHovered = -1;
            }

        return;
    }

    void customsdl::UI::update(){
        /*to calculate how much time passed*/
        static std::chrono::steady_clock::time_point then, now; 
        now = std::chrono::steady_clock::now();

        if(std::chrono::duration_cast<std::chrono::milliseconds>(now-then).count() >= ui->ui_ups){
            /* update time variables */
            then = now;
            CHECK = true;

            //customcpp::upsAverage(true);

            /* Update mouse positions, poll events */
            SDL_PollEvent(ui->data.event);
            SDL_GetMouseState(&mx, &my);
        }
    }

    void customsdl::UI::present(){
        if(CHECK){
            /* Mouse clicks */
            if(ui->data.event->type == SDL_MOUSEBUTTONDOWN && ui->data.event->button.button == SDL_BUTTON_LEFT) pressed_qm = true;
            else pressed_qm = false;
            if(ui->data.event->type == SDL_MOUSEBUTTONUP && ui->data.event->button.button == SDL_BUTTON_LEFT) unpressed_qm = true;
            else unpressed_qm = false;

            /* Check if there's a need to update */
                /* Use merge sort to sort based on `order in layer` (ascending order), then by the last function call (from left to right): {{1,obj4}, {2,obj2}, {2,obj3}=>{{2,obj3},{2,obj2},{1,obj4}} */
            layers = customcpp::mergeSort<UI::__object_layer_pos_struct>(layers,[](UI::__object_layer_pos_struct &a, UI::__object_layer_pos_struct&b){return a.order_in_layer <= b.order_in_layer;}); /* primitive '<=', see the gist. */

            /* PARAM checks:  checks for different parameters. When these changed, it is fine to reset UI states */
                    /* If number of GUI elements changed.. */
                if(layers.size() != layers_old.size()) UPDATE = true; 

                if(!UPDATE){
                    for(int i = 0; i < layers.size(); i++){
                        /* Button checks */
                            /* Check if both pointers exist, then compare values*/ 
                            if(layers[i].BUTTON != nullptr && layers_old[i].BUTTON != nullptr){
                                UI::BUTTON_STRUCT *old = layers_old[i].BUTTON;
                                UI::BUTTON_STRUCT *new_ = layers[i].BUTTON;

                                if(!compareRects(new_->rect, old->rect) || new_->flags != old->flags){
                                    UPDATE = true;
                                    break;
                                }
                            }
                            /* One of the pointers is not a button nullptr, check if one of them is a different element, if so, update, if not, skip. */
                            else if(layers[i].BUTTON != layers_old[i].BUTTON){
                                UPDATE = true; 
                                break;
                            }
                        /* Pane checks */
                            /* Check if both pointers exist, then compare values*/ 
                            if(layers[i].PANE != nullptr && layers_old[i].PANE != nullptr){
                                UI::PANE_STRUCT *old = layers_old[i].PANE;
                                UI::PANE_STRUCT *new_ = layers[i].PANE;

                                if(!compareRects(new_->rect, old->rect) || !compareColors(new_->clr, old->clr) || new_->flags!=old->flags){
                                    UPDATE = true;
                                    break;
                                }
                            }
                            /* One of the pointers is not a pane nullptr, check if one of them is a different element, if so, update, if not, skip. */
                            else if(layers[i].PANE != layers_old[i].PANE){
                                UPDATE = true;
                                break;
                            } 
                        /* Text checks */
                            /* Check if both pointers exist, then compare values*/ 
                            if(layers[i].TEXT != nullptr && layers_old[i].TEXT != nullptr){
                                UI::TEXT_STRUCT *old = layers_old[i].TEXT;
                                UI::TEXT_STRUCT *new_ = layers[i].TEXT;

                                if(!compareRects(new_->rect, old->rect) || !compareColors(new_->clr, old->clr) || new_->flags!=old->flags || new_->text != old->text || new_->fontSize != old->fontSize || new_->fontPath != old->fontPath){
                                    UPDATE = true;
                                    break;
                                }
                            }
                            /* One of the pointers is not a pane nullptr, check if one of them is a different element, if so, update, if not, skip. */
                            else if(layers[i].TEXT != layers_old[i].TEXT){
                                UPDATE = true;
                                break;
                            }
                        /* Scroll box checks */
                            /* Check if both pointers exist, then compare values*/ 
                            if(layers[i].SCROLLBOX != nullptr && layers_old[i].SCROLLBOX != nullptr){
                                UI::SCROLLBOX_STRUCT *old = layers_old[i].SCROLLBOX;
                                UI::SCROLLBOX_STRUCT *new_ = layers[i].SCROLLBOX;

                                if(!compareRects(new_->rect, old->rect) || new_->flags!=old->flags /*|| new_->clickedX != old->clickedX || new_->clickedY != old->clickedY || new_->state_clickedx != old->state_clickedx ||
                                        new_->state_clickedy != old->state_clickedy || new_->stickX != old->stickX || new_->stickY != old->stickY ||
                                        new_->shiftx != old->shiftx || new_->shifty != old->shifty*/){
                                    UPDATE = true;
                                    break;
                                }
                            }
                            /* One of the pointers is not a pane nullptr, check if one of them is a different element, if so, update, if not, skip. */
                            else if(layers[i].SCROLLBOX != layers_old[i].SCROLLBOX){
                                UPDATE = true;
                                break;
                            }
                        /* .... */

                        /* No need to check if order in layer changed, because if it did, struct data wouldn't match */
                    }
                }

            /* Call some CORE functions that depending on user input change states. 
               Transfer a state from the (most likely) the same previous frame button. */ 
                for(int i = 0; i < layers_old.size(); i++){
                    for(int x = 0; x < layers.size(); x++){
                        /* Button states */
                            if(layers_old[i].BUTTON != nullptr && layers[x].BUTTON != nullptr){
                                UI::BUTTON_STRUCT *old = layers_old[i].BUTTON;
                                UI::BUTTON_STRUCT *new_ = layers[x].BUTTON;
                                    /* If these changed, no one would complain if button was unpressed, would they? */
                                if(compareRects(old->rect, new_->rect) && new_->flags == old->flags){
                                    /* Transfer state */
                                    new_->state = old->state;
                                    /* Update states */
                                    __CORE_BUTTON(new_);
                                    /* Update if states changed */
                                    if(new_->state != old->state)
                                        UPDATE = true;
                                }
                            }
                        /* Scroll box states */
                            if(layers_old[i].SCROLLBOX != nullptr && layers[x].SCROLLBOX != nullptr){
                                UI::SCROLLBOX_STRUCT *old = layers_old[i].SCROLLBOX;
                                UI::SCROLLBOX_STRUCT *new_ = layers[x].SCROLLBOX;
                                    /* If these changed, no one would complain if button was unpressed, would they? */
                                if(compareRects(old->rect, new_->rect) && new_->flags == old->flags){
                                    /* Transfer state */
                                    new_->clickedX = old->clickedX;
                                    new_->clickedY = old->clickedY;
                                    new_->state_clickedx = old->state_clickedx;
                                    new_->state_clickedy = old->state_clickedy;
                                    new_->stickX = old->stickX;
                                    new_->stickY = old->stickY;
                                    new_->shiftx = old->shiftx;
                                    new_->shifty = old->shifty;
                                    new_->surface = old->surface;
                                    new_->whichHovered = old->whichHovered;

                                    /* Update states */
                                    __CORE_SCROLLBOX(new_);
                                    /* Update if states changed */
                                    if(new_->clickedX != old->clickedX || new_->clickedY != old->clickedY || new_->state_clickedx != old->state_clickedx ||
                                        new_->state_clickedy != old->state_clickedy || new_->stickX != old->stickX || new_->stickY != old->stickY ||
                                        new_->shiftx != old->shiftx || new_->shifty != old->shifty || new_->whichHovered != old->whichHovered) UPDATE = true;
                                }
                            }
                        /* OTHER ... */
                    }
                }

            /*Re-render everything. */
                if(UPDATE == true){
                    //std::cout << "UPDATING\n";
                    /* Transfer text surfaces (No point in rerendering the same thing) */
                        for(int i = 0; i < layers_old.size(); i++){
                            for(int x = 0; x < layers.size(); x++){
                                if(layers_old[i].TEXT != nullptr && layers[x].TEXT != nullptr){
                                    UI::TEXT_STRUCT *old = layers_old[i].TEXT;
                                    UI::TEXT_STRUCT *new_ = layers[x].TEXT;
                                    /* If these match, transfer data */
                                    if(compareRects(old->rect, new_->rect) && old->text == new_->text && compareColors(old->clr, new_->clr) && old->fontSize == new_->fontSize && old->fontPath == new_->fontPath){
                                        new_->surface = old->surface;
                                        new_->wordLenght = old->wordLenght;
                                        new_->pivot = old->pivot;
                                        /* By default toDisarc is set to true. If in the next frame text is unused, this if condition will not be true and the old pointer's toDiscard will be set to true. */
                                        old->toDiscard = false;
                                    }
                    }}}

                    /* Update the layer vectors*/;
                    for(int i = 0; i < layers_old.size(); i++){
                        /* Delete old struct pointers. */
                        delete layers_old[i].BUTTON;
                        delete layers_old[i].PANE;
                        if(layers_old[i].TEXT != nullptr) if(layers_old[i].TEXT->toDiscard) SDL_FreeSurface(layers_old[i].TEXT->surface);
                        delete layers_old[i].TEXT;
                        delete layers_old[i].SCROLLBOX;
                        /* OTHERS ... */
                    }
                    layers_old.clear();
                    layers_old = layers;

                    /* Clear the surface */ 
                    SDL_FreeSurface(sDisplay);
                    sDisplay = createSurface(ui->data.wind_width, ui->data.wind_height);

                    /* Render everything in a proper order */
                    for(int i = 0; i < layers.size(); i++){
                            if(layers[i].BUTTON     != nullptr) __rend_button(layers[i].BUTTON);
                        else if(layers[i].PANE      != nullptr) __rend_pane(layers[i].PANE);
                        else if(layers[i].TEXT      != nullptr) __rend_text(layers[i].TEXT);
                        else if(layers[i].SCROLLBOX != nullptr){
                            if(layers[i].SCROLLBOX     ->whichScrollbox == 0) __rend_surface_scrollbox(layers[i].SCROLLBOX);
                            else if(layers[i].SCROLLBOX->whichScrollbox == 1) __rend_text_scrollbox(layers[i].SCROLLBOX);
                            else if(layers[i].SCROLLBOX->whichScrollbox == 2) __rend_button_scrollbox(layers[i].SCROLLBOX);
                        }
                        /* Others...*/
                    }

                    /* Convert surface to a texture */
                    SDL_DestroyTexture(tDisplay);
                    tDisplay = SDL_CreateTextureFromSurface(ui->data.renderer, sDisplay);

                    UPDATE = false;
                }
                else
                    /* Delete allocated pointers (when no update is made, because new memory is allocated every update) */
                    for(int i = 0; i < layers.size();i++){
                        delete layers[i].BUTTON;
                        delete layers[i].TEXT;
                        delete layers[i].PANE;
                        delete layers[i].SCROLLBOX;
                        /* others...*/
                    }

            CHECK = false;

            /* Must be reset since data is pushed back every update */
            layers.clear();
        }

        
        /* Render all GUI */
        SDL_RenderCopy(data.renderer, tDisplay, NULL, NULL);
        
        return;
    }
