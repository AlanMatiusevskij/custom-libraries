#pragma once
#include<Windows.h>
#include<memoryapi.h>
#include<string.h>
#include<vector>
#include<system_error>
#include<thread>

//Public variables
namespace pyclink{
    enum types{
        char_p,
    };

    /**
     * Initializes PyC-Link library. Creates a new thread. Starts a python application.
     * @param path path to the python application.
     * @param thread_frequency is how many times per second the thread loops through or how many milliseconds it will sleep;
     * 
     * @return returns `false` if succeeded and `true` if failed. 
    */
    bool initialize(LPCWSTR path, int thread_frequency);

    /**
     * Writes to a specified file map.
     * @param fileMapName the name of a file mapping object.
     * @param in_string the string data you want to send to python.
     * 
     * @return `false` if succeeded and `true` if failed.
    */
    bool write(LPCWSTR fileMapName, char* in_string);
    
    /**
     * Creates a file map object handle at a specified address.
     * @param fileMapName is the name of the file map object. It must be unique!
     * @param type a pyclink::types enum of a data type the file map will read and write.
     * @param maxNumb maximum length of specific type, such as char*. If it's a type not requiring length, pass any value or nullptr.
     * 
     * @return `false` if succeeded and `true` if failed.
    */
    bool createLink(LPCWSTR fileMapName, pyclink::types type, int *maxNumb);
}