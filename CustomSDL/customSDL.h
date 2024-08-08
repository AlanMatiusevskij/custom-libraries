/* Custom functions and data types that are useful for various SDL applications. */

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
   ////////////////

   /**
    * Function to check if a cursor is within given rect's area.
    * 
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
    * Function to check if two customsdl::color's are equal.
    */  
   bool compareColors(color clr1, color clr2);

   /**
    * Gets RGBA values from a specified surface's given pixel.
    * @param coords pixel coordinates relative to the surface.
    * @param surface surface's colors
    */
   color getSurfaceColors(pixel coords, SDL_Surface *surface);

   /**
    * Blits a surface (`from`) to another surface (`to`).
    * 
    * Can be understood as: copies a surface (`from`) and pastes (`to`) a surface.
    * 
    * This function blits the whole `from` surface.
    * @param coords the top-left corner of `to` surface from where the pixels will be edited.
    * @param to the surface which will be edited.
    * @param from the surface which will be copied. 
    */
   void blitSurface(pixel coords, SDL_Surface *to, SDL_Surface *from);

   /**
    * Blits a surface (`from`) to another surface (`to`).
    * 
    * Can be understood as: copies a surface (`from`) and pastes (`to`) a surface.
    * @param coords the top-left corner of `to` surface from where the pixels will be edited.
    * @param to the surface which will be edited.
    * @param areaFrom the x,y,w,h values of the `from` surface to be copied.
    * @param from the surface which will be copied. 
    */
   void blitSurface(pixel coords, SDL_Surface *to, SDL_Rect areaFrom, SDL_Surface *from);

   /**
    * Creates an RGBA surface with SDL_BLENDMODE_BLEND blend mode. Alias for `SDL_CreateRGBSurfaceWithFormat`
    * @param width the width of the surface.
    * @param height the height of the surface.
    */
   SDL_Surface* createSurface(int width, int height);

   /**
    * Draws a rectangle on a surface. Must be a 32bit surface.
    * @param rect The rectangle values to use.
    * @param surface The surface to which to draw.
    * @param clr the RGBA values to use.
    * @param blendMode tells how to blend colors. 0 will overwrite completely.
    */
   void drawSurfaceRect(SDL_Rect rect, SDL_Surface *surface, color clr, bool blend);

   /**
    * Draws a line connecting `point1` and `point2` on a surface. Must be a 32bit surface. TODO: half-works
    * @param point1 {x1, y1} coordinates.
    * @param point2 {x2, y2} coordinates.
    * @param surface The surface to which to draw.
    * @param clr the RGBA values to use.
    * @param blendMode tells how to blend colors. 0 will overwrite completely.
    */
   void drawSurfaceLine(pixel point1, pixel point2, SDL_Surface *surface, color clr, bool blend);
    
   /**
    * Draws given color to a specific pixel.
    * @param surf32 an RGBA 32bit surface
    * @param coords the coordinates of a pixel.
    * @param clr the color struct to use (passed by reference).
    * @param blendMode tells how to blend colors. 0 will overwrite completely.
    */
   void drawToSurface(SDL_Surface *surf32, pixel coords, color &clr, bool blend);

   /**
    * Fill a rect on a surface.
    * @param rect The rectangle values to use.
    * @param surface a 32bit RGBA surface to which to draw.
    * @param clr the RGBA values to use.
    * @param blendMode tells how to blend colors. 0 will overwrite completely.
    */
   void fillSurfaceRect(SDL_Rect rect, SDL_Surface *surface, color clr, bool blend);
    
   #ifdef _WIN32
      /**
       * Windows OS
       * Returns a handle to SDL_Window.
       * @param window a pointer to SDL_Window.
       */
      HWND getWindowHandle(SDL_Window *window);
   #endif

//////////////////////////////////////////////////////////////////////////////////////////

   /**
   * To create an object, call `init` function: `customsdl::UI *ui = customsdl::UI::init(...);`
   * 
   * Currently supports only a single window.
   */
   class UI{
   public:

      /**
      * Initialize UI object.
      * 
      * When this object exists, to retrieve a pointer to it, use `customsdl::UI::get_pointer()`.
      * 
      * Calling `UI::init` function a second time will just overwrite the given parameters in the same way as modifying `customsdl::UI::data` struct.
      * @param renderer a pointer to the main window's renderer target.
      * @param event_struct a pointer to the main SDL event structure.
      * @param wind_width the width of the main window.
      * @param wind_height the height of the main window.
      * @param UPS the time in milliseconds before each update. Recommended value is 1000/30 (30= updates per second). Entering 0 will disable UPS limit. (UPS = Updates Per Second).
      * @param outline_color default rect's outline(border) color.
      * @param background_color the background color behind UI objects, like buttons, scrollboxes.
      * @param hovered_color button-hovered effect color. Best effect is to use `background_color` values with lower `alfa` value.
      * @param bar_color horizontal and vertical scroll bar's colors.
      * @param defaultFontPath The default .ttf font path, which is used when calling a render_text() function without specifying a font path.
      */
      static UI* init(SDL_Renderer *renderer, SDL_Event *event_struct, int wind_width, int wind_height,int UPS, color outline_color, color background_color, color hovered_color, color bar_color, std::string defaultFontPath){
         if(ui == nullptr) ui = new UI();

         if( UPS < 0 ) UPS = 30; /* Default value */
         ui->ui_ups = UPS;
         ui->data.renderer = renderer;
         ui->data.event = event_struct;
         ui->data.wind_width = wind_width;
         ui->data.wind_height = wind_height;
         ui->data.outline_color = outline_color;
         ui->data.background_color = background_color;
         ui->data.hovered_color = hovered_color;
         ui->data.bar_color = bar_color;
         ui->data.default_font = defaultFontPath;

         /* Fix segmentation fault */
         ui->sDisplay = createSurface(100,100);
         ui->tDisplay = SDL_CreateTextureFromSurface(renderer, ui->sDisplay);

         return ui;
      }

      /* Get active UI object's pointer after it was initialized. */
      static UI* get_pointer(){
         if(ui == nullptr) { std::cout << "customcpp::UI::init(...) first! Read the description above UI class definition in customSDL.h!"; return nullptr; }
         return ui;
      }

      /**
         * Note the descriptions above each flag.
         * 
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
          * 
          * If this flag is used, the given names of buttons in `entries` parameter of `renderButtonScrollBox(...)` function will not be displayed.
          * 
          * This is useful when the text passed to a function when clicked on a button should have a different value to what is displayed (because of this bad design).
          * 
          * For example, you can overlap `renderTextureScrollBox` with a text texture containing the last folder name of each directory and `renderButtonScrollBox` with a vector of directory paths.
          * 
          * With this flag, the full directories will not be displayed and a user will only see the folder names, while clicking on a button will pass the full directory path to a function.
          * 
          * Note that for this case to work font and rect values should be identical in both functions.
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
         /* Makes it so the button `onClick` function is called only when button is unpressed on its area. `onHold` function will not be called when this flag is used.
            Used by `button`, `buttonScrollBox` */
         BUTTON_CLICK_ON_UP = 0x00001000,
         /* Removes the rendered pane (background) with its 'click blocking feature' (when pressing a button A, a button B that is behind button A will also be pressed.).
            Used by `buttonScrollBox`, `textureScrollBox`, `button` */
         NO_PANE = 0x00002000,
         /* Button appears to be always pressed / held. 
            Affects only the visual aspect of a button.
            Used by `button` */
         BUTTON_STATE_PRESSED = 0x00004000,
         /* Button is rendered but presses or hovers are not registered 
            Used by `button` */
         BUTTON_STATE_INACTIVE = 0x00008000,
      };

      ///////////////

      struct pane{
         /**
          * Create a pane that blocks raycasts and draws a rect.
          * @param rect the pane's area.
          * @param clr RGBA color values. Don't forget to set renderer's alfa mode for transparency.
          * @param orderInLayer a number which determines the order in which objects are rendered. A button with `orderInLayer` of 5 will be behind a pane with `orderInLayer` of 6. When the numbers are equal, the object called the last will be rendered on top.
          * @param flags `0` or use `NO_VISUALS` flag to disable background and have an invisible 'wall'.
          */
         void renderPane(SDL_Rect rect, color clr, int orderInLayer, Uint32 flags);
      }pane;

      ////////////////

      struct button{
         /**
          * Renders the button and applies colors when hovered. Functions as a button.
          * @param rect button area.
          * @param onClick the function to call on click. Passing `nullptr` will ignore button clicks.
          * @param click_data the void-pointer data to pass to the function when clicked.
          * @param onHold the function call when button is held. The interval to wait before function call is defined in `customsdl::UI::vars`. Passing `nullptr` will ignore button holds.
          * @param hold_data the void-pointer data to pass to the function when held. 
          * @param orderInLayer a number which determines the order in which objects are rendered. A button with `orderInLayer` of 5 will be behind a pane with `orderInLayer` of 6. When the numbers are equal, the object called the last will be rendered on top.
          * @param flags `0` or one or more flags. See `UIFLAGS` enum for available flags. Combine multiple of them with bitwise operator OR `|`.
          */
         void renderButton(SDL_Rect rect, void (*onClick)(void*), void* click_data, void (*onHold)(void*), void* hold_data, int orderInLayer, Uint32 flags);
      }button;

      ////////////////

      struct text{
         /**
          * ...
          */
         void renderText(std::string text, SDL_Rect rect, color clr, int orderInLayer, int fontSize, Uint32 flags);
         void renderText(std::string text, SDL_Rect rect, color clr, int orderInLayer, int fontSize, std::string fontPath, Uint32 flags);
      }text;

      ////////////////

      struct scrollbox{
         /**
          * 
          */
         void renderSurfaceScrollBox(SDL_Surface *surface, SDL_Rect rect, int orderInLayer, Uint32 flags);
         
         /**
          * 
          */
         void renderTextScrollBox(std::string text, SDL_Rect rect, color clr, int orderInLayer, int fontSize, std::string fontPath, Uint32 flags);
         void renderTextScrollBox(std::string text, SDL_Rect rect, color clr, int orderInLayer, int fontSize, Uint32 flags);

         /**
          * ...
          */
         void renderButtonScrollBox(std::vector<std::string> entries, SDL_Rect rect, color clr, int orderInLayer, int fontSize, std::string fontPath, void (*onClick)(std::string), Uint32 flags);
         void renderButtonScrollBox(std::vector<std::string> entries, SDL_Rect rect, color clr, int orderInLayer, int fontSize, void (*onClick)(std::string), Uint32 flags);
      }scrollbox;

      ////////////////////

      struct slider{/*TODO*/}slider;

      //////////////////////////////

      /* Functions: draw a line, draw a rect, display an image, text input ... */

      //////////////////////

      /**
       * ...
       */
      void present();
      
      /**
       * 
       */
      void update();

      /////////////////////

      struct __data_struct{
         /* a pointer to the main window's renderer target. */
         SDL_Renderer* renderer;
         /* a pointer to the main SDL event structure. */
         SDL_Event *event;
         /* the width of the main window. */
         int wind_width;
         /* the height of the main window. */
         int wind_height;
         /* default rect's outline(border) color. */
         color outline_color;
         /* the background color behind UI objects, like buttons, scrollboxes. */
         color background_color;
         /* button-hovered effect color. Best effect is to use `background_color` values with lower `alfa` value. */
         color hovered_color;
         /* horizontal and vertical scroll bar's colors. */
         color bar_color;
         /* The time (in millisec.) to wait before function calls when a button with a given `TODO (Rephrase): button_held` function is held. */
         int hold_wait = 500;
         /* The default .ttf font path, which is used when calling a render_text() function without specifying a font path. */
         std::string default_font;
      };

      /* Stores information that is, most of the time, the same across many function calls */
      __data_struct data;
      /*  the time in milliseconds before each update. Recommended value is 1000/30 (30= updates per second). Entering 0 will disable UPS limit. (UPS = Updates Per Second). */
      int ui_ups;

      //////////

   private:
      /* There can only exist a single UI instance. `init` function will create an object if it does not exist and, if it does exist, returns this pointer. */
      static UI *ui;
      /* Tells a function to re-render all surfaces, because something was updated. */
      bool UPDATE = true;
      /* Tells to check if UIs were updated */
      bool CHECK = true;
      /* A texture that displays all UI objects */
      SDL_Texture *tDisplay;
      /* A surface which will be converted into a texture */
      SDL_Surface *sDisplay;

      SDL_Color _filled_pallete[256]; /*To convert an 8bit white surface to a 32bit one*/
      UI(){
         for(int i = 0; i < 256; i++) _filled_pallete[i].r=_filled_pallete[i].g=_filled_pallete[i].b=_filled_pallete[i].a=i;
      }

      /* Returns true if current mouse position is "directly" touching/hitting a provided target */
      bool layerCheck(void *ptr);

      /* Fonts */
         /* Returns an already loaded font or initializes a new one and stores inside a vector held by UI object */
         FT_FaceRec_* getFont(std::string &fontPath, int &fontSize);
         /* Struct containing information about each initialized font for quick identification */
         struct FONTDATA_STRUCT{
            std::string fontPath;
            int fontSize;
            FT_FaceRec_ *font = nullptr;
            FT_Library lib = nullptr;
         };
         std::vector<FONTDATA_STRUCT> fontData;

      /* For a white glyphs' surface conversion in CORE_TEXT */
      SDL_Surface* surf8bitTo32bit(SDL_Surface* _8bit);


      /* core pane*/
         /* Internal pane functions and data types. */
         struct PANE_STRUCT{
            SDL_Rect rect;
            color clr;
            Uint32 flags;
         };
         /* ... */
         void __rend_pane(PANE_STRUCT *str);
            /* Pane doesn't do anything */
            //int __CORE_PANE();


      /* core button */
         /* Internal button functions and data types. */
         struct BUTTON_STRUCT{
            SDL_Rect rect;
            void (*onClick)(void*);
            void *click_data;
            void (*onHold)(void*);
            void *hold_data;
            int state = 0;
            Uint32 flags;
         };
         /* ... */
         void __rend_button(BUTTON_STRUCT *str);
         /* ... */
         void __CORE_BUTTON(BUTTON_STRUCT *str);


      /* core Text*/
         /* Internal text data */
         struct TEXT_STRUCT{
            /* Unique parameters */
               std::string text;
               color clr;
               SDL_Rect rect; /* The area the text will TRY to occupy */
               int fontSize;
               std::string fontPath; /*Path to a .ttf file*/
               Uint32 flags;

            /* Data used internally */
               /* A surface containing bitmapped text */
               SDL_Surface *surface = nullptr;
               /* An array holding the width-in-pixels of each word */
               std::vector<int> wordLenght = {};
               /* To control surfaces */
               bool toDiscard = true;
               /* Used by scroll box */
               bool render = true;
               /* used by _CENTER flags */
               pixel pivot;
         };
         /* ... */
         void __rend_text(TEXT_STRUCT *str);
         /* ... */
         void __CORE_TEXT(TEXT_STRUCT *str);

      
      /* core scrollbox */
         /* Internal scrollbox data */
         struct SCROLLBOX_STRUCT{
            /* Params */
               SDL_Rect rect;
               std::vector<std::string> entries;
               Uint32 flags;

            /* Internally used */
               int whichScrollbox; /* 0->surface, 1->text, 2->button */
               SDL_Surface *surface = nullptr;
               int clickedX, clickedY;
               bool state_clickedx = false, state_clickedy = false;
               bool stickX = true, stickY = true; /* As in "stick to a surface" */
               int shiftx = 0, shifty = 0;
               SDL_Rect vert_bar, horiz_bar;
               double ratioy, ratiox;
               int fontSize;
               int whichHovered = -1;
               SDL_Rect hover_area;
               TEXT_STRUCT *text_ptr = nullptr;
               void (*onClick)(std::string);
         };
         /* ... */
         void __rend_surface_scrollbox(SCROLLBOX_STRUCT *str);
         /* ... */
         void __rend_text_scrollbox(SCROLLBOX_STRUCT *str);
         /* ... */
         void __rend_button_scrollbox(SCROLLBOX_STRUCT *str);
         /* ... */
         void __CORE_SCROLLBOX(SCROLLBOX_STRUCT *str);


      /* Object lists*/
         /* A struct for convenience to hold an object and its order in layer.*/
         struct __object_layer_pos_struct{
            PANE_STRUCT *PANE = nullptr;
            BUTTON_STRUCT *BUTTON = nullptr;
            TEXT_STRUCT *TEXT = nullptr;
            SCROLLBOX_STRUCT *SCROLLBOX = nullptr;
            int order_in_layer;
         };
         /* A vector array that contains information about all active objects. */
         std::vector<__object_layer_pos_struct> layers;
         /* Previous list of objects and layers. If these don't match, something changed and everything must be re-rendered. */
         std::vector<__object_layer_pos_struct> layers_old;

      bool pressed_qm; /* One time check if a button was pressed*/
      bool unpressed_qm; /* One time check if a button was unpressed*/
      int mx, my; /* Mouse positions */
   };
}

#endif
   /**
   * The "render_xxx" functions can return nothing.
   * The "get_xxx" functions are not a part of UI::rendering process.
   */
   /**
   * Elements as global structs :
   * Pane
   * Button
   * Text
   * Scrollbox
   * Slider (empty)
   */