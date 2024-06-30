#ifndef CUSTOMSDL_H
#define CUSTOMSDL_H

//Include standard libraries and custom c++ functions and classes.
#include <CustomCPP/customCPP.h>

//Include SDL itself.
#include<SDL2/SDL.h>

//Freetype library
#include<ft2build.h>
#include FT_FREETYPE_H

//////////////////////

namespace customsdl{
    //Useful functions and data types.

    struct pixel{
        int x, y;
    };
    struct color{
        Uint8 r, g, b, a;
    };

    /////////////////////////

    /**
     * Function to check if a cursor is within given rect's area.
     * This function gets the current mouse position and calls `bool onRect(SDL_Rect rect, int x, int y)`.
     * @param rect and SDL_Rect to check if cursor is within its area.
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
    bool compareRects(SDL_Rect &rect1, SDL_Rect &rect2);

    /**
     * Gets RGBA values from a specified surface's given pixel.
     * @param coords pixel coordinates relative to the surface.
     * @param surface surface's colors
     */
    color getSurfaceColors(pixel coords, SDL_Surface *surface, bool freeSurface);
    /**
     * Blits a surface (`from`) to another surface (`to`).
     * Can be understood as: copies a surface (`from`) and pastes (`to`) a surface.
     * This function blits the whole `from` surface.
     * @param coords the top-left corner of `to` surface from where the pixels will be edited.
     * @param to the surface which will be edited.
     * @param from the surface whill be copied. 
     */
    void blitSurface(pixel coords, SDL_Surface *to, SDL_Surface *from);
    /**
     * Blits a surface (`from`) to another surface (`to`).
     * Can be understood as: copies a surface (`from`) and pastes (`to`) a surface.
     * @param coords the top-left corner of `to` surface from where the pixels will be edited.
     * @param to the surface which will be edited.
     * @param areaFrom the x,y,w,h values of the `from` surface to be copied.
     * @param from the surface whill be copied. 
     */
    void blitSurface(pixel coords, SDL_Surface *to, SDL_Rect areaFrom, SDL_Surface *from);

//////////////////////////////////////////////////////////////////////////////////////////

    /**
     * A class to manipulate a single active (object's) surface.
     * It is designed so that a single surface instance is created, drawn to and converted to a texture.
     * Use this when you need to draw to a surface multiple times.
     */
    class surfaceManipulation{
    public:
        SDL_Surface *active_surface = nullptr;

        /**
         * Prepares a surface for a batch of changes.
         * @param width the width of the surface.
         * @param height the height of the surface.
         * @param depth the depth (bits per pixel) of the surface.
         * @param format the format of the surface. Use `SDL_PIXELFORMAT_RGBA32` for an RGBA surface.
         */
        void createSurface(int width, int height, int depth, SDL_PixelFormatEnum format);
        /**
         * Creates a texture pointer and deletes the surface.
         * @param renderer the rendering target.
         */
        SDL_Texture* createTextureFromSurface(SDL_Renderer *renderer);
        /**
         * Draws given color to a specific pixel.
         * @param coords the coordinates of a pixel.
         * @param RGBA the color struct to use.
         */
        void drawToSurface(pixel coords, color RGBA);

    private:
        Uint8 *data;
    };

////////////////////

    /** 
     * A class that has all UI object classes.
     * Should only be created once!
     */
    class UI{
    private:
        //Data types and functions.
        color _8bitpalletecolors[256];

        struct activeFaces_struct{
            std::string path;
            int fontSize;
            FT_FaceRec_ *face;
            FT_Library ft;
        };
        std::vector<activeFaces_struct> active_faces;
        
        /**
         * Converts an 8bit surface to a 32bit one.
         * @param _8bit an 8bit surface pointer.
         * @return returns a 32bit surface pointer.
         */
        SDL_Surface* surf8bitTo32bit(SDL_Surface* _8bit);

        /**
         * Returns an already loaded font or initializes a new one.
         */
        FT_FaceRec* useFont(std::string path, int fontsize);

    public:
        color backgroundColor = {0,0,0,0};

        UI(){
            for(int i = 0; i < 256; i++) _8bitpalletecolors[i].r = _8bitpalletecolors[i].g = _8bitpalletecolors[i].b = _8bitpalletecolors[i].a = i;
        }

        //UI elements
        class button{
        public:
            color outline_color = {255,255,255,255};
            color hovered_color = {0,0,0,100};

            /**
             * The core functionallity of a button.
             * @param evt sdl event structure.
             * @param rect button area.
             * @param onClick the function to call on click.
             * @param data the void-pointer data to pass to the function.
             */
            void CoreButton(SDL_Event &evt, SDL_Rect rect, void (*onClick)(void*), void* data);

            /**
             * Renders the button and applies colors when hovered. Calls CoreButton function.
             * @param renderer the rendering target.
             * @param evt sdl event structure.
             * @param rect button area.
             * @param onClick the function to call on click.
             * @param data the void-pointer data to pass to the function.
             * @param texture to render in the button.
             */
            void renderButton(SDL_Renderer *renderer, SDL_Event &evt, SDL_Rect rect, void (*onClick)(void*), void* data, SDL_Texture* texture);
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        class text{
        private:
            UI& parent_class;
            std::vector<int> wordLength;

        public:
            struct{
                SDL_Texture *texture = nullptr;
                SDL_Surface *surface = nullptr;

                std::string text;
                SDL_Rect textDimensions;
                SDL_Rect TextBox;
                int fontsize;
                std::string fontpath;
                bool autonewlines;
            }last;

            text(UI& instance) : parent_class(instance){}
            ~text(){
                SDL_DestroyTexture(last.texture);
                SDL_FreeSurface(last.surface);
            }

            SDL_Surface *getSurface(SDL_Renderer *renderer, std::string text, SDL_Rect textBox, int fontsize, std::string fontpath, bool autoNewLines);
            SDL_Texture *getTexture(SDL_Renderer *renderer, std::string text, SDL_Rect textBox, int fontsize, std::string fontpath, bool autoNewLines);
            /**
             * Creates a text texture from given parameters and renders it.
             * @param renderer the rendering target.
             * @param text the text to use.
             * @param textBox the box in which the text will be displayed. (Note on `autoNewLines`)
             * @param fontsize font size.
             * @param fontpath the path to the .ttf (true type font) file. Should be relative to the application.
             * @param autoNewLines `true` if function should automatically write words in a new line instead of clipping. Note that, if it is `false`, the texture will scale to fit the whole text, but it will not automatically create a new line.
             * @return A pointer to the SDL_Texture structure.
            */
            void renderText(SDL_Renderer *renderer, std::string text, SDL_Rect textBox, int fontsize, std::string fontpath, bool autoNewLines, bool centered);
        };

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        class scrollBox{
        private:
            int x, y;
            int shiftx=0, shifty=0;
            int clickedx, clickedy;
            bool clicked = false;

            void scrollBoxCore(SDL_Renderer *renderer, SDL_Event &evt, SDL_Rect boxToRenderIn, int textureWidth, int textureHeight, SDL_Texture *texture);
        public:
            int bar_width = 8;
            color bar_color = {255,255,255,100};
            color outline_color = {255, 255, 255, 255};
            color button_hovered = {0,0,0,200};
            double ratio{};

            /**
             * TODO
             */
            void renderTextureScrollBox(SDL_Renderer *renderer, SDL_Event &evt, SDL_Rect boxToRenderIn, int textureWidth, int textureHeight, SDL_Texture *texture);
            /**
             * TODO
             */
            void renderButtonScrollBox(SDL_Renderer *renderer, SDL_Event &evt, SDL_Rect boxToRenderIn, int textureWidth, int textureHeight, SDL_Texture *texture, void (*onClick)(std::string), std::vector<std::string> entries, int fontSize, std::string fontpath);
        };

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
        class slider{
            //TODO
        };
    };
}

#endif