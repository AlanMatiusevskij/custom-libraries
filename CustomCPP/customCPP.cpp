/**
 * Custom functions and classes that are useful for various c++ application.
*/
#include<CustomCPP/customCPP.h>

/////////////////////
//Function to get updates per second:

int customcpp::upsAverage(bool _cout){
    static std::chrono::steady_clock::time_point then;
    static int updates = 0;
    static int lastValue = 0;

    if(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now()-then).count() >= 1){
        lastValue = updates;
        if(_cout) std::cout << "UPS: " << lastValue << "\n";
        then = std::chrono::steady_clock::now();
        updates = 0;
    }
    updates++;
    return lastValue;
}

/////////////////////

std::string customcpp::intToString(int numb){
    std::string _return_backwards = "";

    if(numb == 0)
        _return_backwards += "0";
    while(numb != 0){
        _return_backwards += char(numb%10+48);
        numb/=10;
    }

    std::string _return = "";
    for(int i = _return_backwards.size()-1; i>=0; i--)
        _return+=_return_backwards[i];

    return _return;
}
int customcpp::stringToInt(std::string string){
    if(string.size() == 0) return 1;

    int _return = 0;
    int multiplier = 1;
    for(int i = string.size()-1; i >= 0; i--){
        if(i == 0 && string[i] == '-') _return*=-1;
        else if(i!=0 && string[i] == '-') return 0;

        _return += ((int)string[i]-48)*multiplier;
        multiplier*=10;
    }

    return _return;
}
std::string customcpp::doubleToString(double in, int precision){
    std::string _return = "";
    if(in == 0) return "0";

    //Seperate integer and fractional parts.
    int whole = int(in);
    double fraction = in - whole;

    //Save the integer part.
    _return = intToString(whole);
    _return += ".";

    //Save fractional part to the provided precision.
    _return += intToString(int(fraction*std::pow(10, precision)));

    return _return;
}
double stringToDouble(std::string in){
    double _return = 0;
    if(in.size() == 0) return 1;

    int k = 0;
    if(in[0] == '-') k = 1;
    double multiplier = 0.1;
    while(true){
        if(in.size() == k) break;
        if(in[k] == ',' || in[k] == '.')
            break;
        k++;
        multiplier*=10;
    }
    
    for(int i = 0; i<in.size(); i++){
        if(in[i] != '-' && in[i] != ',' && in[i] != '.'){
            _return += ((int)in[i]-48)*multiplier;
            multiplier/=10;
        }
    }

    if(in[0] == '-') _return*=-1;
    return _return;
}

wchar_t* customcpp::charToLPWSTR(const char* charArr){
    int length = MultiByteToWideChar(CP_UTF8, 0, charArr, -1, nullptr, 0);
    wchar_t* wideChar = new wchar_t[length];

    MultiByteToWideChar(CP_UTF8, 0, charArr, -1, wideChar, length);

    return wideChar;
}

////////////////////

char* customcpp::append_char_p(const char* array, const char* to_add){
    size_t length = strlen(array) + strlen(to_add);
    char* _return = new char[length + 1];

    strcpy(_return, array);
    for(int i = 0; i < strlen(to_add); i++)
        _return[i + strlen(array)] = to_add[i];
    _return[length] = '\0';

    return _return;        
}

//Used by `browseFolder` function.
static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM LParam, LPARAM lpData){
    if(uMsg == BFFM_INITIALIZED){
        std::string tmp = (const char*)lpData;
        SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
    }
    return 0;
}

std::string customcpp::browseFolder(std::string saved_path){
    TCHAR path[MAX_PATH];
    const char *path_param = saved_path.c_str();

    BROWSEINFO bi = {0};
    bi.lpszTitle = (L"Files will be saved in a selected folder.");
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    bi.lpfn = BrowseCallbackProc;
    bi.lParam = (LPARAM)path_param;

    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

    if(pidl != 0){
        SHGetPathFromIDList(pidl, path);

        IMalloc * imalloc = 0;
        if(SUCCEEDED(SHGetMalloc(&imalloc))){
            imalloc->Free(pidl);
            imalloc->Release();
        }
        std::wstring wStr = path;
        std::string str = std::string(wStr.begin(), wStr.end());
        return str;
    }
    
    //Window closed without selecting.
    return "";
}
