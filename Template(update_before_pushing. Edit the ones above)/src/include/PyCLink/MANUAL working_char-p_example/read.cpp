//this is an example of how you could do this by yourself
//Essential
#include<Windows.h>
#include<memoryapi.h>

//Optional
#include<iostream>

LPCWSTR fileMapName = L"fileMapTest";
int MAXcharNUMB = 1000;

int main(){
    HANDLE fileMappingObject = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(char)*MAXcharNUMB, fileMapName);
    char *mapData;
    std::string previous;
    while(true){
        mapData = (char*)MapViewOfFile(fileMappingObject, FILE_MAP_READ, 0, 0, 0);
        std::string current(mapData);
        if(current != previous){
            std::cout<<current;
            previous = current;
        }
        //if keypress '0', quit
        if(GetAsyncKeyState(0x30) != 0) break;
        Sleep(100);
    }                                                                   
    //Close
    UnmapViewOfFile(mapData);
    CloseHandle(fileMappingObject);
    return 0;
}   
