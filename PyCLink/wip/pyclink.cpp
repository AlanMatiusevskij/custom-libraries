#include"pyclink.h"

//Declaring private functions and variables
//Note that `map` is shortened for `File Map Object`.
namespace pyclink{
    HANDLE mainHandle;
    LPCWSTR mainMap = L"PyC-Link-v.01";
    int* mainData;

    //Variables for launching python
    PROCESS_INFORMATION py_info;
    STARTUPINFOW s_info{sizeof(STARTUPINFOW)};

    //Temporary variables. This one is needed to write an int pointer from enum.
    int tempInt;

    std::vector<LPCWSTR> mapNames;
    std::vector<HANDLE> mapHandles;

    struct {
        std::vector<char*> char_pointer;
    }data;

    std::thread mainThread;

    enum COMMANDS{
        LAUNCHED = 0, //starting command.
        PYRUNNING = 1 //python overwrites to this value, comfirming everything is running smoothly.
    };

    /**
     * A function that loops continiously and checks for any calls/updates.
     * @param thread_frequency is how many times per second the thread loops through or how many milliseconds it will sleep;
    */
    void loop(int thread_frequency);
}


void pyclink::loop(int thread_frequency){
    while(true){
        //wait for updates. Data has to end with /O?
        Sleep(int(1000/thread_frequency));
    }
    return;
}

bool pyclink::initialize(LPCWSTR path, int thread_frequency){
    //Create main filemap for exchanging crucial information, like "create/destroy a filemap".
    mainHandle = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(int), mainMap);
    //TODO:process must be new
    //TODO:check if failed

    //Create a write char* pointer. 
    mainData = (int*)MapViewOfFile(mainHandle, FILE_MAP_WRITE, 0, 0, 0);
    
    //check if failed to create a pointer.
    if(mainData == 0) return true;
    
    //Write launch command.
    tempInt = COMMANDS::LAUNCHED;
    //Note that the data copied to (tempInt) cant be longer than mainData allows and specified length (sizeof(Commands::launched))
    memmove(mainData, &tempInt, sizeof(COMMANDS::LAUNCHED));

    //Fill with 0s (clear) a memory address
    ZeroMemory(&s_info, sizeof(s_info));
    s_info.cb = sizeof(s_info);
    ZeroMemory(&py_info, sizeof(py_info));

    //Launch python as a child application and check if it failz`ed
    if(!CreateProcessW(path, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &s_info, &py_info)){
        return 1;
    }

    //Wait for python to load everything.
    while(*mainData != PYRUNNING){
        //timeout check?
        Sleep(100);
    }

    //Create the main loop thread and wait for further calls/commands.
    try{
        mainThread = std::thread(pyclink::loop, thread_frequency);
    }
    catch(const std::system_error &e){
        //Couldn't create a thread
        return true;
    }

    return false;
}

bool pyclink::write(LPCWSTR fileMapName, char* input){
    //check if string is nullTerminated - must be; What about nullTerminating it ourselves?. 

    HANDLE fileMapObject = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(char)*strlen(input), fileMapName);
    //check for failure
    char* objectData = (char*)MapViewOfFile(fileMapObject, FILE_MAP_WRITE, 0, 0, 0);
    //check for failure

    UnmapViewOfFile(objectData);
    CloseHandle(fileMapObject);
    return 0;
}

bool pyclink::createLink(LPCWSTR fileMapName, types type, int *maxNumb){
    if(type == pyclink::types::char_p){
        //check for correct name. Program should add a prefix.
        
        //Save file map name.
        mapNames.push_back(fileMapName);

        //Create a new file map object handle 
        mapHandles.push_back(CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(char)*(*maxNumb), fileMapName));
        //check for failure
        //must be new

        //Save the pointer to a vector
        data.char_pointer.push_back((char*)MapViewOfFile(mapHandles[mapHandles.size()-1], FILE_MAP_WRITE, 0, 0, 0));
        //Check for failure

        return false;
    }
    //other types
    return true;
}