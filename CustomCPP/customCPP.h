/* Custom functions and data types that are useful for various c++ applications. */

#ifndef CUSTOMCPP_H
#define CUSTOMCPP_H

/* Standard libraries */
#include<fstream>
#include<vector>
#include<string.h>
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
#include<cstdio>
#include<sstream>
#include<atomic>
#include<csignal>
#include<map>

#ifndef UNICODE 
    #define UNICODE 
#endif

/* Windows*/
#ifdef _WIN32
/* non-c++ standard libraries */
#ifndef NOMINMAX 
    #define NOMINMAX 
#endif
#include<Windows.h>
#include<memoryapi.h>
#include<shlobj.h>
#include<conio.h>
#include<lmcons.h> 
#include<shlwapi.h>
#include<shellapi.h>

/* UNIX */
#else
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/select.h>
#include<signal.h> 
#include<sys/ioctl.h>
#include<pwd.h>
#include<termios.h>
#include<dirent.h>
#include<fcntl.h>
#include<libgen.h>
#include<sys/stat.h>
#include<sys/prctl.h>
#endif
/////////////////

namespace customcpp{
    const float pi = 3.14159;
    struct pixel{
        int x, y;
    };
    using point = pixel;
    struct pointf{
        float x, y;
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
        std::string ORANGE = "\033[38;5;214m";
        std::string TEAL = "\033[38;5;37m";
        std::string NAVY = "\033[38;5;17m";
        std::string MAROON = "\033[38;5;1m";
        std::string TURQUOISE = "\033[38;5;80m";
        std::string BROWN = "\033[38;5;130m";

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
     * Clears a line (up to \n) in a string starting from the end.
     * @param numb_of_lines how many lines to clear (text until \n).
     */
    void clearStrLine(std::string &str, int numb_of_lines);

    /**
     * Splits a string that's seperated by spaces: word1  word2     word3
     * @param in your whole sentence.
     */
    std::vector<std::string> stringSeperation(std::string in);

    /**
     * Turns all alphabetical ASCII characters to their lowercase form.
     */
    std::string strtolower(std::string in);

    /**
    * Function to check if two customcpp points/pixels are equal.
    */
    bool commparePoints(point point1, point point2);

    //////////////

    /**
     * Returns the -almost- exact number of updates per second.
     * 
     * + This function should be called only once per main loop update.
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
     * 
     * Fractional part can be seperated either by a comma or a dot.
     * @param in a string holding a number.
     * @return a number of type `double`.
    */
    double stringToDouble(std::string in);

    /**
     * Returns a number of `float` type represented as a string.
     * @param in a `float` type number.
     * @param precision number of fractional values.
     * @return a `float` type numbers as a string.
    */
    std::string floatToString(float in, int precision);

    /**
     * Returns a number of `float` type from a given string.
     * 
     * Fractional part can be seperated either by a comma or a dot.
     * @param in a string holding a number.
     * @return a number of type `float`.
    */
    float stringToFloat(std::string in);

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
    #ifdef _WIN32
        /* Required by 'setAutoStart'*/
        const std::wstring REGISTRY_PATH = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";

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
     * Returns true if 'driveLetter' is already used by a disk.
     */
    bool isDriveOccupied(char &driveLetter);

    #else
        /* Unix-OS specific functions */
    /**
     * Returns a vector string of all directory names within the specified path.
     */
    std::vector<std::string> listDirectories(const char* pth);

    /**
     * A template to launch a funciton that runs in the background.
     * All data passed is copied and NOT shared.
     */
    void childFunction(void (*foo)(void*), void *data);
    
    #endif

    ////////////////////////////////
        /* Unix-windows cross platform functions*/

    /**
     * Pass a list of commands. Each string is considered as an individual command.
     * @param commands a vector of strings 
     * @param waitInterval time to wait before sending each command in milliseconds.
     */
    void cmd(std::vector<std::string> commands, int waitInterval);

    /**
     * Pass a list of commands. Each string is considered as an individual command.
     * 
     * @return Returns commands' output line by line.
     * @param commands a vector of strings 
     * @param waitInterval time to wait before sending each command in milliseconds.
     */
    std::vector<std::string> getcmd(std::vector<std::string> commands, int waitInterval);
    
    /**
     * Clears the console by calling a `CLS` if on windows and `CLEAR` if on Unix.
     */
    void CLEAR();

    /**
     * Calls on windows Sleep() and on Unix usleep().
     * @param milliseconds Time to sleep in milliseconds.
     */
    void SLEEP(int milliseconds);

    /**
     * Executes a command in the background. Using the same ID for multiple processes can lead to undefined behaviour!
     * @param command the command.
     * @param waitForQuit awaits for the background process to finish before proceeding.
     * @param ID a number (but not -1) to be able to terminate the process later. Use 'ctrlC_backgroundProcess() to terminate'.
     */
    bool backgroundProcess(std::string command, bool waitForQuit, int ID);

    /**
     * *More like insta kill, the same as closing the parent application *
     *  After calling 'backgroundProcess' with a given positive ID, call this function to send CTRL+C command to the running process.
     *  @param ID a number given to 'backgroundProcess' when a background application is still running. If it's -1, will terminate all existing processes.
     */
    void ctrlC_backgroundProcess(int ID);

    /**
     * Returns true if server responds.
     * @param server the address.
     */
    bool ping(std::string server);

    /**
     * Get's user input and stores it constantly in the provided string and outputs to the console (unless disabled). 
     * @param YourString characters will be pushed into this string and removed from it.
     * @param restrict an array of characters that cannot be typed OR, if the string starts with '\b', the string is considred an array of characters that are allowed. In short: "abcd.."-blacklist, "\babcd.." -whitelist.
     * @param RestrictiveInput if true, program will stop until user presses ENTER key, however, if false, this function must be put in a loop to get input, while at the same time other function's can be executed.
     * @param printToConsole true to do.
     * @returns if `RestrictiveInput` == true, return value is irrelevant, but if it is false, this function will return true once "ENTER" was pressed.
     */
    bool getConsoleINPUT(std::string &YourString, std::string restrict_, bool RestrictiveInput, bool printToConsole);
    
    /* Enables/disables application's auto start. */
    void setAutoStart(bool enable);

    /* Returns the name of the user. */
    std::string getCurrentUserName();

    /* Returns the application's that is calling the function name*/
    std::string getCurrentApplicationName();

    /* Returns application's that called this function path*/
    std::string getExecutablePath();

	/*
	 * Returns true if given argument matches with requiredArgument.
	 * @param argument the argument given by a user which needs to be checked.
	 * @param requiredArgument the desired argument.
	 * @param singleLetter checks if argument's first letter matches with requiredArgument's first letter ('-' doesnt count as a letter). If false, will check arguments one to one.
	 */
	bool checkGivenArgument(char *argument, std::string requiredArgument, bool singleLetter);

    //////////////////////////////////////////////////////////////
        /* Algorithms */

    /**
     * Merge sort. A stable sorting function of O(n*log(n)) time complexity.
     * 
     * ---
     * 
     * + Check this for syntax: https://gist.github.com/AlanMatiusevskij/9853775e3649dbd66795465b933c82ec ).
     * 
     * ---
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
     * - Returns a line in a file which starts with the given keyword.
     * 
     * - The keyword may contain whitespaces. 
     * 
     * @param area although a struct with x.y variables, this is used to describe FROM(x) which line TO(y) which line to read. 
     * 
     * If both numbers are equal ({0,0}), then will read the whole file instead.
     * 
     * If the second (y) number is lesser than the first ({5,2}), will read till the end of file from line 5.
     * 
     * - However, if first(x) number is negative, will not read anything and return an empty string.
     * 
     * - If 'keyword' is not found, returns an empty string
     * 
     * ---
     * 
     * @param pathToFile path to any text based file.
     * @return empty string "" or the whole line, including the keyword.
     */
    std::string findLine(std::string keyword, point area, std::string pathToFile);

    /**
     * - Returns the `area` of a header:
     * 
     * 1| [this is a header]
     * 
     * 2| somedata: 
     * 
     * 3| a line of text to be used 
     * 
     * 4| alsoData:
     * 
     * 5| [another header or end of file]
     * 
     * - The area returned would be in a point variable, in this example, {2, 4} -> the lines the header defines, inclusively from line 2 to line 4. 
     * @return return `area` of lines. If the header name was not found, {-1, 0} instead.
     */
    point findHeader(std::string headerName, std::string pathToFile);

    /**
     * @param line the line defined as this => `myBoolean: true` or `my boolean:false` (however, generally whitespaces are not accepted).
     * 
     * ---
     * 
     * - Searches for the first `:` symbol inside a line and extracts 'true' or 'false' keywords that come after it. 
     * 
     * - The line must have only a single `:` character. There can be multiple leading or trailing whitespaces, but the `true` or `false` phrases must stay intact.
     * 
     * If fails, the original value is preserved.
     * 
     * @returns true on successful boolean extraction and false if fails.
     */
    bool getBoolean(std::string line, bool &value);
    
    /**
     * @param line the line defined as this => `myScalar: 0.5` or `my scalar:-5` (however, generally whitespaces are not accepted).
     * 
     * ---
     * 
     * - Searches for the first `:` symbol inside a line and extracts the numerical value that come after it, which can be positive or negative and decimal digits (seperator can be both commas and dots). 
     * 
     * - The line must have only a single `:` character. There can be multiple leading or trailing whitespaces, but the number itself must stay intact.
     * 
     * If fails, the original value is preserved.
     * 
     * @returns true on successful numerical extraction and false if fails. 
     */
    bool getScalar(std::string line, double &value);

    /**
     * - Simply erases all leading characters until the first, and including, the `:` character;
     * 
     * - Also removes all trailing and leading whitespaces.
     * 
     * If fails, the original value is preserved.
     * 
     * @returns false if string doesn't have `:` character.
     */
    bool getString(std::string line, std::string &value);


    /**
     * - Returns true if "whatToFind" is found withint "whereToFind"
     * 
     * If "whereToFind" starts with "LETTER:/" or "LETTER:\\" or "/", will search within a file in specified path. 
     * @param index returned value of -1 if not found, or a number, which will be an index of the first "whatToFind" char in "whereToFind" string.
     * @return returns `true` if found.
     */
    bool FIND(std::string whatToFind, std::string &whereToFind, int &index);

    /**
     * Removes an nth line (starting from 0) from a specified file and moves text by that one line.
     * @param line which line to remove, the first line is indexed as 0.
     * @param file path to a file.
     */
    void TRUNCATEFILE(int line, std::string file);

    ///////////////////////////////////////////////
        /* math */

    /* Returns an angle in radians between 2 points.*/
    float angleBetweenTwoPoints(pixel origin, pixel point);

    /* returns radians converted to degrees */
    float deg(float rads);

    /* Returns degrees converted to radians */
    float rad(float degrees);
}
#endif
