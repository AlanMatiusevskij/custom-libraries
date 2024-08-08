/* Custom functions and data types that are useful for various c++ applications. */

#ifndef CUSTOMCPP_H
#define CUSTOMCPP_H

/* Standard libraries */
#include<fstream>
#include<vector>
#include<string>
#include<cmath>
#include<ctime>
#include<chrono>
#include<bitset>
#include<functional>
#include<filesystem>
#include<algorithm>
#include<memory>
#include<iostream>
#include<locale>
#include<codecvt>

/* non-c++ standard libraries */
#ifndef NOMINMAX 
    #define NOMINMAX 
#endif

#ifndef UNICODE 
    #define UNICODE 
#endif

#include<Windows.h>
#include<memoryapi.h>
#include<shlobj.h>
#include<sstream>
#include<lmcons.h> 

/////////////////

namespace customcpp{
    /* Required by 'setAutoStart'*/
    const std::wstring REGISTRY_PATH = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";

    struct pixel{
        int x, y;
    };
    struct color{
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };

    //////////////

    struct{
        const color &RED = {255, 0, 0, 255},
            &ORANGE = {255, 165, 0, 255},
            &YELLOW = {255, 255, 0, 255},
            &GREEN = {0, 128, 0, 255},
            &BLUE = {0, 0, 255, 255},
            &INDIGO = {75, 0, 130, 255},
            &VIOLET = {238, 130, 238, 255},
            &CYAN = {0, 255, 255, 255},
            &MAGENTA = {255, 0, 255, 255},
            &BLACK = {1, 1, 1, 255},
            &WHITE = {255, 255, 255, 255},
            &BROWN = {165, 42, 42, 255},
            &GRAY = {128, 128, 128, 255},
            &PINK = {255, 192, 203, 255},
            &PURPLE = {128, 0, 128, 255},
            &TURQUOISE = {64, 224, 208, 255},
            &MAROON = {128, 0, 0, 255},
            &NAVY = {0, 0, 128, 255},
            &TEAL = {0, 128, 128, 255},
            &LIME = {0, 255, 0, 255};
    }CLR; /* abbreviation of CoLoR */

    struct{
        std::string BLACK = "\033[30m";
        std::string RED = "\033[31m";
        std::string GREEN = "\033[32m";
        std::string YELLOW = "\033[33m";
        std::string BLUE = "\033[34m";
        std::string MAGENTA = "\033[35m";
        std::string CYAN = "\033[36m";
        std::string WHITE = "\033[37m";

        std::string BRIGHT_BLACK = "\033[90m";
        std::string BRIGHT_RED = "\033[91m";
        std::string BRIGHT_GREEN = "\033[92m";
        std::string BRIGHT_YELLOW = "\033[93m";
        std::string BRIGHT_BLUE = "\033[94m";
        std::string BRIGHT_MAGENTA = "\033[95m";
        std::string BRIGHT_CYAN = "\033[96m";

        /* Reset to default */
        std::string RESET = "\033[0m";
    }ACLR; /* Ascii + CoLoR abbreviation. Prints to console colored letters. Use RESET or a different color to change. Should change the name? */

    /////////////////////////////////////

    /**
     * Clears console lines from the current one.
     * @param numb_of_lines How many lines to clear.
     */
    void clearLine(int numb_of_lines);

    /**
     * Splits a string that's seperated by spaces: word1  word2     word3
     * @param in your whole sentence.
     */
    std::vector<std::string> stringSeperation(std::string in);

    //////////////

    /**
     * Returns the -almost- exact number of updates per second.
     * This function should be called only once per main loop update.
     * @param _cout `true` to get a console message.
     */
    int upsAverage(bool _cout);

    ///////////////////////////
        /* Conversion functions */

    /**
     * Returns a number value represented as a string.
     * @param a number.
     * @return a number as a string.
    */
    std::string intToString(int numb);

    /**
     * Returns an integer value from a given string.
     * @param a string holding a number.
     * @return a number as an int.
    */
    int stringToInt(std::string string);

    /**
     * Returns a number of `double` type represented as a string.
     * @param in a `double` type number.
     * @param precision number of fractional values.
     * @return a `double` type numbers as a string.
    */
    std::string doubleToString(double in, int precision);

    /**
     * Returns a number of `double` type from a given string.
     * Fractional part can be seperated either by a comma or a dot.
     * @param in a string holding a number.
     * @return a number of type `double`.
    */
    double stringToDouble(std::string in);

    /** 
     * Must be deallocated manually!
     * Converts a char* to LPWSTR (wchar_t*) type.
     * @param charArr the pointer to a char array to convert.
     * @return returns converted array, which must be manually deallocated using delete[]. 
    */
    wchar_t* charToLPWSTR(const char* charArr);

    ///////////////////////////////////////////

    /** 
     * Must be deallocated manually!
     * Append char* to a given character array. 
     * @param array the char* object to which `to_add` is appended.
     * @param to_add the data to append.
     * @return Returns a new char* with the appended characters. The returned char* array must be manually deallocated using delete[].
    */
    char* append_char_p(const char* array, const char* to_add);

    ///////////////////////////////////////////////////////////
        /* Windows-OS specific functions */

    /**
     * Creates a window's "Browse For Folder" window.
     * @param saved_path path to a folder which will be selected on start.
     * @return Returns a path to a selected folder.
     */
    std::string browseFolder(std::string saved_path, std::string title);

    /**
     * Must be deallocated manually!
     * Get an array of screen pixel color values.
     * @param hWnd a handle to a window. With sdl, you can get it by calling `customsdl::getWindowHandle` function.
     * @param wind_Width the width in pixels of the main window.
     * @param wind_Height the height in pixels of the main window.
     * @return Returns a color struct of pixel colors, which must be manually deallocated using delete[]. 
     */
    color* getScreenPixels(HWND *hWnd, int wind_Width, int wind_Height);

    /**
     * Make it so a window will ignore -and effectivelly turn transparent- a specified color. One time function call.
     * @param windowHandle a handle to a window. With sdl, you can get it by calling `customsdl::getWindowHandle` function.
     * @param ignored_rgb is the color the window will completely ignore.
     */
    bool windowIgnoreColor(HWND *windowHandle, color ignored_rgb);

    /**
     * Pass a list of commands. Each string's end is considered as the end of command that has to be passed to command line.
     * @param commands a vector of strings 
     * @param waitInterval time to wait before sending each command in milliseconds.
     */
    void passCommands(std::vector<std::string> commands, int waitInterval);

    /* Returns application's that called this function path*/
    std::string getExecutablePath();

    /* Enables/disables application's auto start. */
    void setAutoStart(bool enable);

    //////////////////////////////////////////////////////////////
        /* Algorithms */

    /**
     * Merge sort. A stable sorting function of O(n*log(n)) time complexity.
     * 
     * Check this for syntax: https://gist.github.com/AlanMatiusevskij/9853775e3649dbd66795465b933c82ec ).
     * 
     * @param array an array to sort of any 'proper' type.
     * @param compare a lambda function of this format: `[]('your_type' &a, 'your_type' &b){return a <= b; }. Check the github gist, there's a comment about how primitives affect the sorting.
     * @return Returns a sorted vector array of any given data type.
     */
    template<typename T>
    std::vector<T> mergeSort(std::vector<T> &array, std::function<bool(T&, T&)> compare){
        /* Defining in a header file to avoid creating another file just for this function's definition */

        /* Already sorted */
        if(array.size() <= 1) return array;

        /* Split the array in 2 */
        std::vector<T> _left{array.begin(), array.begin() + array.size()/2};
        _left = mergeSort<T>(_left, compare);
        std::vector<T> _right{array.begin()+array.size()/2,array.end()};
        _right = mergeSort<T>(_right, compare);

        /* Combine everything into a single sorted array. */
        std::vector<T> _return = {};

        /* Keep track of positions */
        int from_left = 0, from_middle = 0;

        /* Compare and push numbers */
        while(from_left < _left.size() && from_middle < _right.size()){
            if(compare(_left[from_left], _right[from_middle])){
                _return.push_back(_left[from_left]);
                from_left++;
            }
            else{
                _return.push_back(_right[from_middle]);
                from_middle++;
            }
        }

        /* After the loop there can be a few trailing values from one of the arrays. */
        while(from_left < _left.size()){ _return.push_back(_left[from_left]); from_left++; }
        while(from_middle < _right.size()){ _return.push_back(_right[from_middle]); from_middle++; }

        return _return;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////
        /* File systems (?) */

    /**
     * Far from a decent approach, but I don't want to spend time searching for a good library and learn to use it.
     * Data file should be structured like this:
     * 
     * Firstkeyword All my data is here
     * 
     * Secondkeyword And here
     * 
     * Thirdkeyword and more here
     * 
     * @param keyword the first word in a list is always the keyword
     * @param pathToFile path to any text based file.
     */
    std::vector<std::string> findRow(std::string keyword, std::string pathToFile);
}
#endif