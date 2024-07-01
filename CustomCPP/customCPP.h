#ifndef CUSTOMCPP_H
#define CUSTOMCPP_H

//Standard libraries
#include<fstream>
#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<chrono>
#include<bitset>
#include<filesystem>
#include<algorithm>

//OS specific libraries
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include<Windows.h>
#include<memoryapi.h>
#include<shlobj.h>
#include<sstream>

///////////////////

namespace customcpp{
    /**
     * Returns the -almost- exact number of updates per second.
     * This function should be called once per update in your main loop.
     * @param _cout `true` if you want to get a console message: `UPS: upsValue`.
    */
    int upsAverage(bool _cout);

///////////////////////////////

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
     * TODO: a memory leak?
     * Converts a char* to LPWSTR (wchar_t*) type.
     * @param charArr the pointer to a char array to convert.
     * @return returns converted array. 
    */
    wchar_t* charToLPWSTR(const char* charArr);

///////////////////////////////////////////////

    /** 
     * TODO: a memory leak?
     * Append char* to a given character array.
     * @param array the char* object to which `to_add` is appended.
     * @param to_add the data to append.
     * @return Returns a new char* with the appended characters.
    */
    char* append_char_p(const char* array, const char* to_add);

///////////////////////////////////////////////////////////////
    //Windows specific

    /**
     * Creates a window to select or create a folder.
     * @param saved_path path to a folder which will be revealed on start.
     * @return Returns a path to a selected folder. If value is `""`, that means user closed the window without selecting a folder.
     */
    std::string browseFolder(std::string saved_path);

}

#endif