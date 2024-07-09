#ifndef CUSTOMSDL_H
#define CUSTOMSDL_H

//Include standard libraries and custom c++ functions and classes.
#include <CustomCPP/customCPP.h>
using namespace customcpp;

//Include SDL itself.
#include<SDL2/SDL.h>
#include<SDL2/SDL_syswm.h>

//Freetype library
#include<ft2build.h>
#include FT_FREETYPE_H

//////////////////////

namespace customsdl{
    ///////////////

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
    bool compareRects(SDL_Rect rect1, SDL_Rect rect2);

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

    /**
     * Returns a handle to a window created by SDL.
     * @param window a pointer to a window created by SDL.
     * @param indx an index for the window. Use 0,1,2... for your first, second, third and so on, windows.
     */
    HWND *getWindowHandle(SDL_Window *window, int indx);

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
     * An object should be created once like this: `customsdl::UI *obj = customsdl::UI::INITIALIZE(...);`.
     * A class that has all UI object classes.
     * Can be created as many times as possible, but all pointers will point to a the same UI object, as only one can exist.
     * So feel free to get a pointer to this object in multiple functions.
     * Do note the comments for `INITIALIZE` function.
     */
    class UI{
    public:
        struct font{
            //The font size. The value appears to be equal to the height of symbols.
            int fontSize{14};
            //The path to a true type font (`.ttf`) file.
            std::string fontPath;
        };
    
        ////////////////

        /**
         * Note the descriptions above each flag.
         * `Used by` means an object who does not use that flag will ignore it.
         */
        enum UIFLAGS{
            /**
             * A `text` object's flag. Set it to automatically shift words to a new line if the word should be too long to fit the given text box.
             * Will not work if a single word is longer than the width of the textbox.
             */
            AUTO_NEW_LINES = 0x00000001,
            /* Centers text VERTICALLY within given rect's area. For buttons, screen size values must be provided first to UI::baseValues;
               Used by `text`, `button`. */
            YCENTERED = 0x00000002,
            /* Centers text HORIZONTALLY within given rect's area. For buttons, screen size values must be provided first to UI::baseValues;
               Used by `text`, `button`. */
            XCENTERED = 0x00000004,
            /* Disables background and border outlines for UI objects. (for button hover effect, use `NO_BUTTON_EFFECT`)
               Used by `button`, `scrollBox`, `buttonScrollBox`, `pane`. */
            NO_VISUALS = 0x00000008,
            /* Disables the applied color effect when hovering and pressing a button.
               Used by `button`, `buttonScrollBox`. */
            NO_BUTTON_EFFECT = 0x00000010,
            /**
             * Used by `buttonScrollBox`.
             * If this flag is used, the given names of buttons in `entries` parameter of `renderButtonScrollBox(...)` function will not be displayed.
             * This is useful when the text passed to a function when clicked on a button should have a different value to what is displayed (because of this bad design).
             * 
             * For example, you can overlap `renderTextureScrollBox` with a text texture containing the last folder name of each directory and `renderButtonScrollBox` with a vector of directory paths.
             * With this flag, the full directories will not be displayed and a user will only see the folder names, while clicking on a button will pass the full directory path to a function.
             * Note that for this case to work font and rect values should be the same for both functions.
             */
            NO_BUTTON_TEXT = 0x00000020,
            /* Disables the basic vertical bar rect rendering.
               Used by `scrollBox`, `buttonScrollBox`. */
            NO_VERTICAL_SCROLLBAR = 0x00000040,
            /* Makes it so the vertical scroll bar will "stick" to the bottom when it is close to it and the vertical size of the scroll box is increased.
               Used by `scrollBox`, `buttonScrollBox`. */
            VERTICAL_SCROLLBAR_STICK = 0x00000080,
            /* Disables the basic horizontal bar rect rendering.
               Used by `scrollBox`, `buttonScrollBox`. */
            NO_HORIZONTAL_SCROLLBAR = 0x00000100,
            /* Makes it so the horizontal scroll bar will "stick" to the right side when it is close to it and the horizontal size of the scroll box is increased.
               Used by `scrollBox`, `buttonScrollBox`. */
            HORIZONTAL_SCROLLBAR_STICK = 0x00000200,
            /* Make vertical scroll bar not interactable.
               Used by `scrollBox`, `buttonScrollBox`. */
            LOCK_VERTICAL_SCROLLBAR = 0x00000400,
            /* Make horizontal scroll bar not interactable.
               Used by `scrollBox`, `buttonScrollBox`. */
            LOCK_HORIZONTAL_SCROLLBAR = 0x00000800,
            /* Works only when paired with `UIFLAGS::BUTTON_HOLD`. Makes it so when the cursor leaves button's area, the button is not `unpressed` after being held.
               Used by `button`, `buttonScrollBox` */
            BUTTON_HOLD_NOT_RELATIVE = 0x00001000,
            /* Makes it so the button `onClick` function is called only when button is unpressed on its area. `onHold` function will not be called when this flag is used.
               Used by `button`, `buttonScrollBox` */
            BUTTON_CLICK_ON_UP = 0x00002000,
        };

        struct vars_struct{
            //Rect's outline color. This value applies to all UI elements when they are created.
            color outline_color = {255,255,255,255};
            //Filled rect's button-hovered effect color. This value applies to all UI elements when they are created.
            color hovered_color = {255,255,255,30};
            //Horizontal and vertical bar's colors. This value applies to all UI scroll box elements when they are created.
            color bar_color = {255,255,255,100};
            //The background color behind buttons, scrollboxes and the color of `pane`.
            color background_color = {0,0,0,255};
            /* Time to wait for button with `UIFLAGS::BUTTON_HOLD` after pressed to execute secondary function.
               In milliseconds. */
            int button_hold_wait = 500;
            //Main screen's
            int WIDTH = 0, HEIGHT = 0;
            SDL_Renderer *renderer;
            SDL_Event *event;
        };

        // / / / / / / / / **

        /**
         * Holds essential information and pointers to the main renderer, default colors and more.
         * Each UI object (text, button, etc) will have a copy of this struct, so you can manually change all variables and pointers individually for each object.
         * The variables will not be updated or overwritten if UI's `vars` state is changed.
         */
        vars_struct vars;

        // / / / / / / / / **

        /**
         * Initialize UI class. This function is REQUIRED when creating UI class object for the first time.
         * For any consecutive times to get a pointer to this object, use `customsdl::UI::GET_UI_POINTER()`.
         * Calling this (`INITIALIZE()`) function a second time will just overwrite the given parameters in the same way as modifying `customsdl::UI::vars` struct.
         * @param renderer a pointer to the main window's renderer target.
         * @param event a pointer to the main SDL event structure.
         * @param width the width of the main window.
         * @param height the height of the main window.
         * @param outline_color default rect's outline color.
         * @param hovered_color filled rect's button-hovered effect color.
         * @param bar_color horizontal and vertical bar's colors.
         * @param background_color the background color behind some objects.
         */
        static UI* INITIALIZE(SDL_Renderer *renderer, SDL_Event *event, int width, int height, color outline_color, color hovered_color, color bar_color, color background_color){
            if(ui == nullptr) ui = new UI();
            ui->vars.bar_color = bar_color;
            ui->vars.event = event;
            ui->vars.HEIGHT = height;
            ui->vars.hovered_color = hovered_color;
            ui->vars.outline_color = outline_color;
            ui->vars.renderer = renderer;
            ui->vars.WIDTH = width;
            ui->vars.background_color = background_color;
            return ui;
        }

        static UI* GET_UI_POINTER(){
            if(ui == nullptr) {std::cout << "`INITIALIZE()` FUNCTION MUST BE CALLED FIRST! (customsdl::UI::INITIALIZE_PARAMETERS(..). See the `template` example)."; return nullptr; }
            return ui;
        }

        //UI elements

        /**
         * Pane can be used as a background and, most importantly, a wall (a 'raycast' blocking area).
         */
        class pane{
        public:
            //note ui's vars.
            UI::vars_struct vars;
            pane() : ui(UI::GET_UI_POINTER()){
                vars = ui->vars;
            }
            ~pane(){
                ui->layers.erase(ui->layers.begin() + index);
                ui->layers.shrink_to_fit();
                delete this;
            }

            /**
             * Create a pane that blocks raycasts and draws a rect.
             * @param rect the pane's area.
             * @param clr RGBA color values. Don't forget to set renderer's alfa mode for transparency.
             * @param flags `0` or use `NO_VISUALS` flag to disable background and have an invisible 'wall'.
             */
            void renderPane(SDL_Rect rect, color clr, int flags);

        private:
            UI *ui;
            bool initialized = true;
            int index;
        };
        
        //////////

        class button{
        public:
            //Note ui's vars.
            UI::vars_struct vars;

            button() : ui(UI::GET_UI_POINTER()){
                vars = ui->vars;
                background = new pane;
            }

            /**
            * Renders the button and applies colors when hovered. Functions as a button.
             * @param rect button area.
             * @param onClick the function to call on click. Passing `nullptr` will ignore button clicks.
             * @param click_data the void-pointer data to pass to the function when clicked.
             * @param onHold the function call when button is held. The interval to wait before function call is defined in `customsdl::UI::vars`. Passing `nullptr` will ignore button holds.
             * @param hold_data the void-pointer data to pass to the function when held. 
             * @param flags `0` or one or more flags. See `UIFLAGS` enum for available flags. Combine multiple of them with bitwise operator OR `|`.
             */
            void renderButton(SDL_Rect rect, void (*onClick)(void*), void* click_data, void (*onHold)(void*), void* hold_data, Uint32 flags);

        private:
            UI *ui;
            pane *background = nullptr;
            bool initialized = true;
            bool held = false;
        };

        ///////////////////////////////////////////////////////////////////////////////////////

        class text{
        public:
            //Some information and pointers of the last rendered `text` for this object.
            struct{
                SDL_Texture *texture = nullptr;
                SDL_Surface *surface = nullptr;
                
                std::string text;
                SDL_Rect textDimensions;
                SDL_Rect TextBox;
                font fontData;
                color rgba;
            }last;

            //Note ui's vars.
            UI::vars_struct vars;

            text() : ui(UI::GET_UI_POINTER()){
                vars = ui->vars;
            }
            ~text(){
                SDL_DestroyTexture(last.texture);
                SDL_FreeSurface(last.surface);
                delete this;
            }

            SDL_Surface *getSurface(std::string text, color rgba, SDL_Rect textBox, font *fontData, Uint32 flags);
            SDL_Texture *getTexture(std::string text, color rgba, SDL_Rect textBox, font *fontData, Uint32 flags);
            /**
             * Creates a text texture from given parameters and renders it.
             * Call `getTexture` or `getSurface` to get pointers to them.
             * @param text the text to use.
             * @param rgba the color to apply to the text.
             * @param textBox the box in which the text will be displayed. (Note on `autoNewLines`)
             * @param fontData `font` struct with specified path to a true type font (.ttf) and font size value (appears to be equal to the height of symbols).
             * @param flags `0` or one or more flags. See `UIFLAGS` enum for available flags. Combine multiple of them with bitwise operator OR `|`.
            */
            void renderText(std::string text, color rgba, SDL_Rect textBox, font *fontData, Uint32 flags);

        private:
            UI *ui;
            std::vector<int> wordLength;
        };

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        class scrollBox{
        public:
            //Vertical bar's rect.
            SDL_Rect y_bar;
            //Horizontal bar's rect.
            SDL_Rect x_bar;
            //Note ui's vars.
            UI::vars_struct vars;

            scrollBox() : ui(UI::GET_UI_POINTER()){
                vars = ui->vars;
                background = new pane;
                text_ = new text;
            }

            /**
             * Allows to scroll vertically and horizontally through a provided texture, on a condition, that the texture's size exceeds the given `boxToRenderIn` dimensions. 
             * Useful for large blocks of texts, images.
             * @param boxToRenderIn the largest area the texture can fill.
             * @param textureWidth a pointer width of the texture.
             * @param textureHeight a pointer to the height of the texture.
             * @param texture the texture pointer you want to display.
             * @param flags `0` or one or more flags. See `UIFLAGS` enum for available flags. Combine multiple of them with bitwise operator OR `|`.
             */
            void renderTextureScrollBox(SDL_Rect boxToRenderIn, int *textureWidth, int *textureHeight, SDL_Texture *texture, Uint32 flags);
            /**
             * Creates a vertically scrollable list of buttons.
             * @param boxToRenderIn the largest area the texture can fill.
             * @param entries a vector array of all `entries`. Each entry is a single row and a single button. `\n`s within strings are ignored.
             * @param fontData `font` struct with specified path to a true type font (.ttf) and font size value (appears to be equal to the height of symbols).
             * @param onClick() a function passed by reference to call on button click. The button's string value will be passed to the funcition.
             * @param flags `0` or one or more flags. See `UIFLAGS` enum for available flags. Combine multiple of them with bitwise operator OR `|`.
             */
            void renderButtonScrollBox(SDL_Rect boxToRenderIn, std::vector<std::string> entries, color rgba, font *fontData, void (*onClick)(std::string), Uint32 flags);
        
        private:
            UI *ui;
            UI::pane *background = nullptr;
            int x, y;
            int shiftx=0, shifty=0;
            int clickedx, clickedy;
            bool state_clickedX = false, state_clickedY = false;
            double ratioy{}, ratiox{};
            bool stickX = true, stickY = true; //as in "stick to a surface"
            customsdl::UI::text *text_ = nullptr;
            
            //For button function
            std::vector<std::string> prev_in{};
            std::string prev_compact_in{};

            void scrollBoxCore(SDL_Rect boxToRenderIn, int *textureWidth, int *textureHeight, SDL_Texture *texture, Uint32 flags);
        
        };

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
        //TODO: add slider.
        class slider{
        public:
            //Note ui's vars.
            UI::vars_struct vars;

            slider() : ui(UI::GET_UI_POINTER()){
                vars = ui->vars;
            }
        
        private:
            UI *ui;
        };

    /////////////////

    private:
        SDL_Color _8bitpalletecolors[256];

        //Private initialization function prevents creation of instances: customSDL::UI obj; <- error, inaccessible.
        UI(){
            for(int i = 0; i < 256; i++) _8bitpalletecolors[i].r = _8bitpalletecolors[i].g = _8bitpalletecolors[i].b = _8bitpalletecolors[i].a = i;
        } 
        //Static pointer to the class will hold a single instance.
        static UI *ui;

        struct activeFaces_struct{
            std::string path;
            int fontSize;
            FT_FaceRec_ *face;
            FT_Library ft;
        };
        std::vector<activeFaces_struct> active_faces;
        
        struct layer_Struct{
            //to check if it is the same object.
            void *pointer;
            //to check the object's area.
            SDL_Rect rect;
        };
        std::vector<layer_Struct> layers; 
        
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

        /**
         * Returns true if the cursor is right above an object (there's no other object in the way). It's like raycasting, but not. TODO: update this desc.
         */
        bool checkLayers(void *obj_pointer, SDL_Rect &obj_area);
    };
}

#endif