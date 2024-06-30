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
        SDL_Color _8bitpalletecolors[256];

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
         * Returns an already loaded true type font or initializes a new one.
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
            color hovered_color = {0,0,0,60};

            /**
             * The core functionallity of a button.
             * @param evt sdl event structure.
             * @param rect button area.
             * @param onClick the function to call on click.
             * @param data the void-pointer data to pass to the function.
             */
            void CoreButton(SDL_Event &evt, SDL_Rect rect, void (*onClick)(void*), void* data);

            /**
            * Renders the button and applies colors when hovered. Functions as a button.
             * @param renderer the rendering target.
             * @param evt sdl event structure.
             * @param rect button area.
             * @param onClick the function to call on click.
             * @param data the void-pointer data to pass to the function.
             * @param texture a texture stretched to fill the space inside the button.
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
             * @param fontSize the size of the font (the value appears to be equal to the height of the symbols in pixels).
             * @param fontPath the path to the true type font (.ttf).
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
            double ratioy{}, ratiox{};
            
            //For button function
            std::vector<std::string> prev_in{};
            std::string prev_compact_in{};

            void scrollBoxCore(SDL_Renderer *renderer, SDL_Event &evt, SDL_Rect boxToRenderIn, int *textureWidth, int *textureHeight, SDL_Texture *texture);
        public:
            /**
             * `True` if you want to render the outlines and `false` if you don't.
             */
            bool renderOutlines = true;

            /**
             * If `false`, the given names of buttons in `entries` parameter of `renderButtonScrollBox(...)` function will not be displayed.
             * This is useful when the names of the buttons, should have a different value to what is displayed (because of this bad design).
             * 
             * For example, you can overlap `renderTextureScrollBox` with a text texture containing the last folder name of each directory and `renderButtonScrollBox` with a vector of directory names.
             * When `renderButtonText` is false, the full directories will not be displayed and a user will only see the folder names, while cliking on a button will pass the full directory path to a function.
             * Note that font and rect values should be the same for both functions.
             */
            bool renderButtonText = true;

            int bar_width = 8;
            color bar_color = {255,255,255,100};
            color outline_color = {255, 255, 255, 255};
            color button_hovered = {0,0,0,50};

            /**
             * Allows to scroll vertically and horizontally through a provided texture, on a condition, that the texture's size exceeds the given `boxToRenderIn` dimensions. 
             * Useful for large blocks of texts, images.
             * @param renderer the rendering target.
             * @param evt the sdl event structure.
             * @param boxToRenderIn the largest area the texture can fill.
             * @param textureWidth a pointer width of the texture.
             * @param textureHeight a pointer to the height of the texture.
             * @param texture the texture pointer you want to display.
             */
            void renderTextureScrollBox(SDL_Renderer *renderer, SDL_Event &evt, SDL_Rect boxToRenderIn, int *textureWidth, int *textureHeight, SDL_Texture *texture);
            /**
             * Creates a vertically scrollable list of buttons.
             * @param renderer the rendering target.
             * @param evt the sdl event structure.
             * @param uiObj a pointer to an exsiting UI class object.
             * @param boxToRenderIn the largest area the texture can fill.
             * @param entries a vector array of all `entries`. Each entry is a single row and a single button. `\n`s within strings are ignored.
             * @param fontSize the size of the font (the value appears to be equal to the height of the symbols in pixels).
             * @param fontPath the path to the true type font (.ttf).
             * @param onClick() a function passed by reference to call on button click. The button's string value will be passed to the funcition.
             */
            void renderButtonScrollBox(SDL_Renderer *renderer, SDL_Event &evt, customsdl::UI *uiObj, SDL_Rect boxToRenderIn, std::vector<std::string> entries, int fontSize, std::string fontpath, void (*onClick)(std::string));
        };

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
        class slider{
            //TODO
        };
    };
}

#endif