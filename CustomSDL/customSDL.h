#pragma once

//Include standard libraries and custom c++ functions and classes.
#include "customCPP.h"

//Include SDL itself.
#include<SDL2/SDL.h>

//Freetype library
#include<ft2build.h>
#include FT_FREETYPE_H

//////////////////////

namespace customsdl{
    /**
     * Function to check if a cursor is within given rect's area.
     * This function gets the current mouse position and calls another `onRect()` function.
     * @param rect an SDL_Rect to check if cursor is within its area.
    */
    bool onRect(SDL_Rect rect);
    /**
     * Function to check if given coordinates are within given rect's area.
     * @param rect and SDL_Rect to check if coordinates are within its area.
     * @param x `x` coordinate. (from left to right).
     * @param y `y` coordinate. (from top to bottom).
    */
    bool onRect(SDL_Rect rect, int x, int y);
    /**
     * Function to check if two rects are equal.
    */
    bool checkIfRectsEqual(SDL_Rect &rect1, SDL_Rect &rect2);

////////////////////////////////////////////

   /**
    * A class to manipulate a single active surface.
    * It is designed so that a single surface instance is created, drawn to and converted to a texture.
    */
    class surfaceManipulation{
    public:
        /** 
         * Prepares surface for a high batch of changes.
         * Format for RGBA surface: `SDL_PIXELFORMAT_RGBA32`
        */
        void createSurface(int width, int height, int depth, SDL_PixelFormatEnum format);
        /**
         * Creates a texture and deletes the surface.
        */
        SDL_Texture* createTextureAndDeleteSurface(SDL_Renderer *renderer);
        /**
         * Draws to a specifc pixel provided colors.
        */
        void drawToSurface(__pixel coords, __color RGBA);
        /**
         * Gets RGBA values from a specified surface's given pixel.
        */
        __color getSurfaceColors(__pixel coords, SDL_Surface *surface, bool freeSurface);
        /**
         * Blits surfaces using `SDL_BlitSurface()`.
         * @param areaTO the area of the Currently Working Surface `(CWS)` to draw to. (Width and Height are ignored).
         * @param from the surface from which pixels will be `blitted` to the CWS.
         * @param areaFROM the area of the `from` surface to copy. If all values are `{-1,-1,-1,-1}`, the whole surface is used.
        */
        void blitSurface(SDL_Rect areaTO, SDL_Surface* from, SDL_Rect areaFROM);
        /**
         * Blits surfaces using `SDL_BlitSurface()`.
         * @param areaTO the area of the Currently Working Surface `(CWS)` to draw to. (Width and Height are ignored).
         * @param from the surface from which pixels will be `blitted` to the CWS.
        */
        void blitSurface(SDL_Rect areaTO, SDL_Surface* from);

    private:
        SDL_Surface* surface = nullptr;
        Uint8 *data;
    };

////////////////////

    class UI{
    private:
        SDL_Rect *destination;
        std::vector<int> wordLength;
        static SDL_Color _8bitpalleteColors[256];

        struct _activeTextFields_struct{
            SDL_Texture* texture;
            std::string entry = "";
            int lastAccessed = 0; //-frames/updates ago.
            SDL_Rect dimensions;
            //For scroll boxes
            int shiftX = 0, shiftY = 0;
        };
        struct _activeFaces_struct{
            std::string path;
            int fontSize;
            FT_FaceRec_ *face;
            FT_Library ft;
        };     
        struct _activeScrollBoxesStruct{
            SDL_Texture *texture;
            SDL_Rect renderBox;
            SDL_Rect dimensions;
            int lastAccessed = 0;
            int shiftx = 0, shifty = 0;
        };

        //add comments here
        FT_FaceRec* useFont(std::string path, int fontsize);
        _activeTextFields_struct* findExistingText(std::string &entry, SDL_Rect &textBox);
        _activeScrollBoxesStruct* findExistingScrollBox(SDL_Texture *texture, SDL_Rect &rect);
        SDL_Surface* surf8bitTo32bit(SDL_Surface* _8bit);

        //Information about the last created text field.  
        static _activeTextFields_struct &lastTextFieldData;
        //Contains information about the active UI class's text fields.
        static std::vector<_activeTextFields_struct> __activeTexts;
        //Information about active faces (-fonts).
        static std::vector<_activeFaces_struct> __activeFaces;
        //Information about active scroll boxes.
        static std::vector<_activeScrollBoxesStruct> __activeScrollBoxes;
    
    public:
        UI(){
            for(int i = 0; i < 256; i++)
                _8bitpalleteColors[i].r = _8bitpalleteColors[i].g = _8bitpalleteColors[i].b = _8bitpalleteColors[i].a = i;
            __activeTexts.clear();
            __activeFaces.clear();
            __activeScrollBoxes.clear();
        }

        /**
         * Creates a text field texture from given parameters.
         * @param sentence the text to use.
         * @param textBox the box in which the text will be displayed. (Note on `autoNewLines`)
         * @param fontsize font size.
         * @param fontpath the path to the .ttf (true type font) file. Should be relative to the application.
         * @param autoNewLines `true` if function should automatically write words in a new line instead of clipping. Note that, if it is `false`, the texture's size will still fit all given text, but in a single line (unless there's a new line).
         * @return A pointer to the SDL_Texture structure.
         * @note The given rect should be used to display the text within desired boundries by using `renderText` function from this class IMMEDIATELLY after calling this function.
        */
        SDL_Texture* textFieldTexture(SDL_Renderer *renderer, std::string sentence, SDL_Rect textBox, int fontsize, std::string fontpath, bool autoNewLines);
        //Button
        //todo comment
        SDL_Texture* button(SDL_Renderer* renderer, SDL_Event &evt, std::string label, SDL_Rect buttonbox, int fontSize, std::string fontpath, void(*onClick)(void*), void* param);
        //Slider - scroll for a simple texture.
        //todo comment
        void TextureScrollBox(SDL_Renderer *renderer, SDL_Event &evt, SDL_Rect boxToRenderIn, SDL_Rect texturesDimensions, SDL_Texture* texture);
        //Slider - scroll box for buttons. If there's enough space, will start in a new line.
        //todo (whats above ) and add comment.
        void ButtonScrollBox(SDL_Renderer *renderer, SDL_Event &evt, SDL_Rect box, std::vector<std::string> entries, int fontSize, std::string fontpath, void (*onClick)(std::string));

        //more...

        /**
         * Renders the specified part of the text field.
         * Use a function without `textToRenderArea` for the whole texture.
         * @param renderer the rendering target.
         * @param text the SDL_Texture pointer, which should be a returned pointer from `textFieldTexture()` function.
         * @param textToRenderArea the area of the text texture to render.
        */
        void renderText(SDL_Renderer* renderer, SDL_Texture *text, SDL_Rect textToRenderArea);
        /**
         * Renders the whole text field.
         * @param renderer the rendering target.
         * @param text the SDL_Texture pointer, which should be a returned pointer from `textFieldTexture()` function.
        */
        void renderText(SDL_Renderer* renderer, SDL_Texture *text);          
    };

///////////////////
    
    //For storing or classifying two points.
    struct __pixel{
        int x;
        int y;
    };
    //Contains 4 8bit color values.
    struct __color{
        Uint8 r;
        Uint8 g;
        Uint8 b;
        Uint8 a;
    };

//////////////////////

}