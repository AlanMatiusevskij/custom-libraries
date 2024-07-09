### Custom-Libraries
This repo contains my custom libraries and links to some that I use.

These are for my personal use, but feel free to try them out as well.

#### Short descriptions.
> Do note the parent-child relations of libraries. For example, `CustomSDL` requires `CustomCPP` and `freetype`.
- CustomSDL ` - has a UI class with some GUI elements. A surfaceManipulation class for quick surface editing and color retrieving.
   - CustomCPP - has a few conversion functions, windows-specific functions and a few algorithms.
   - [Freetype](https://freetype.org/).
- PyCLink - has functions to send data from a c++ application to python and vice-versa.

#### In detail.
- CustomSDL has:
   - a `UI` class. It uses [freetype](https://freetype.org/) library to display a text field, has has a `button` class, a `scrollBox` class for textures and a list of buttons, a `pane` class for a simple background that 'blocks clicks'. UIFLAGS allow quite a bit of customizability.
   - a `surfaceManipulation` class. It is useful if you want to draw to a surface multiple times, as the class creates a surface with specified parameters and you can easily call a function to draw to a specific pixel.
   - Outside-class functions:
      - `onRect` function to check if cursor or given x,y coordinates are within an `SDL_Rect`'s area.
      - `compareRects`, self explanatory, returns `true` if two `SDL_Rect`s are equal and `false` if not.
      - `getSurfaceColors` returns RGBA colors of a given `SDL_Surface`'s specificied pixel.
      - `blitSurface` more or less user-friendly explanation on what arguments to provide to an `SDL_BlitSurface` function to merge 2 `SDL_Surface`s.
      - `getWindowHandle` function to get HWND to SDL created window.
- CustomCPP has:
   - a `upsAverage` function, which returns the number of updates in the last second.
   - A few conversion fuctions:
      1) `intToString`
      2) `stringToInt`
      3) `doubleToString`
      4) `stringToDouble`
      5) `charToLPWSTR`
   - Miscellaneous
      1) `append_char_p` - Append a character array (char*) to a given character array (char*).
   - Windows specific functions:
      1) `browseFolder` function opens a `Browse for folder` window to select a path to a folder on the system.
      2) `getScreenPixels` function returns a pointer array to `color` struct of the specified area's on the screen pixel color values.
      3) `windowIgnoreColor` function makes a window completely ignore a specified color, effectively making that area transparent.
   - Algorithms
      1) `Mergesort`.
- PyCLink:
   - It is not finished. `direct_cpp-py_link` is currently the latest version and can only exchange `char*` type info. 
   
     In short how to use it (parameters explained in header/module files): Write a python script that calls `receiveInput` to receive input from c++ ( not required ). Before python closes, to send a string back, call `send` function. The C++ application shoud call `communicate` function to start the python executable and wait for it to close.
#### Libraries I use
> Links to useful libraries I use. 
- [freetype](https://freetype.org/)
- [SDL](https://www.libsdl.org/)