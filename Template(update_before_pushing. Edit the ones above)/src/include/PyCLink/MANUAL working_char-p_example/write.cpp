//this is an example of how you could do this by yourself

//Essential
#include<Windows.h>
#include<memoryapi.h>

//Optional
#include<iostream>

LPCWSTR fileMapName = L"fileMapTest";
int MAXcharNUMB = 1000;

int main(){
    //create a file map
    HANDLE fileMappingObject = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(char)*MAXcharNUMB, fileMapName);
    char *mapData = (char*)MapViewOfFile(fileMappingObject, FILE_MAP_WRITE, 0, 0, 0);

    std::string INPUT;
    while(true){
        std::cin >> INPUT;
        if(INPUT == "0") break;
        strcpy_s(mapData, MAXcharNUMB, INPUT.c_str());
    }
    //Close
    UnmapViewOfFile(mapData);
    CloseHandle(fileMappingObject);
    return 0;
}
