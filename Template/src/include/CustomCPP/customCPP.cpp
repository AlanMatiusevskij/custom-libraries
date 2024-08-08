/* Custom functions and data types that are useful for various c++ applications. */

#include<CustomCPP/customCPP.h>

///////////////////////////////

void customcpp::clearLine(int numb_of_lines){
    for(int i = 0; i < numb_of_lines; i++){
        std::cout << "\r\033[K" << std::flush; /* Clear a line */
        if(i + 1 < numb_of_lines) std::cout << "\033[A"; /* Moves up a line */
    }
    std::cout << std::flush;
    return;
}

std::vector<std::string> customcpp::stringSeperation(std::string in){
    std::vector<std::string> split{};
    std::string word = "";
    for(int i = 0; i < in.size(); i++){
        if(in[i] == ' ' && in[i] !='\n'){ if(i + 1 < in.size()) if(in[i+1] != ' ') {split.push_back(word); word = "";}}
        else if(in[i]=='\n' && word.size() > 0){split.push_back(word); word = "";}
        else word+=in[i];
    }
    /*trailing one*/
    if(word.size()>0) split.push_back(word);
    return split;
}
///////////////////////////////

int customcpp::upsAverage(bool _cout){
    /* Keep track of passed time, number of updates tracked in `this` second and number of updates in the last second. */
    static time_t then = std::time(nullptr);
    static int updates = 0;
    static int lastValue = 0;

    /* Check if a second or more has passed to 'reset the clock.' */
    if(std::time(nullptr) - then >= 1){
        lastValue = updates;
        if(_cout) std::cout << "UPS: " << lastValue << "\n";
        then = std::time(nullptr);
        updates = 0;
    }
    /* keep track of updates */
    updates++;
    return lastValue;
}

/////////////////////

std::string customcpp::intToString(int numb){
    std::string _return_backwards = "";

    if(numb == 0) _return_backwards = "0";

    /* Conver the number backwards into a string. */
    while(numb != 0){
        _return_backwards += char(numb%10+48);
        numb/=10;
    }

    /* Reverse the number. */
    std::string _return = "";
    for(int i = _return_backwards.size()-1; i>=0; i--)
        _return+=_return_backwards[i];

    return _return;
}

int customcpp::stringToInt(std::string string){
    /* Empty string shall be nothing, which is 0. */
    if(string.size() == 0) return 0;

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

    /* Seperate integer and fractional parts. */
    int whole = int(in);
    double fraction = in - whole;

    /* Save the integer part. */
    _return = intToString(whole);
    _return += ".";

    /* Save fractional part to the provided precision. */
    _return += intToString(int(fraction*std::pow(10, precision)));

    return _return;
}

double customcpp::stringToDouble(std::string in){
    /* Empty string shall be nothing, which is 0. */
    if(in.size() == 0) return 0;
    
    double _return = 0;
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
    /* Get the total length of the appended-char-array */
    size_t length = strlen(array) + strlen(to_add);
    /* Create a new char array with that length + \0 (null-terminator)*/
    char* _return = new char[length + 1];

    /* Copy the given array to a new array*/
    strcpy(_return, array);
    /* Push the characters to the new array */
    for(int i = 0; i < strlen(to_add); i++)
        _return[i + strlen(array)] = to_add[i];
    /* Add the null-terminator */
    _return[length] = '\0';

    return _return;        
}

///////////////////

static int CALLBACK __usedByBrowseFolder__BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM LParam, LPARAM lpData){
    if(uMsg == BFFM_INITIALIZED){
        std::string tmp = (const char*)lpData;
        SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
    }
    return 0;
}
std::string customcpp::browseFolder(std::string saved_path, std::string title){
    /* welcome to what I call `windows-magic` */
    TCHAR path[MAX_PATH];

    BROWSEINFO bi = {0};

    WCHAR wchr[title.size()];
    MultiByteToWideChar(0, 0, title.c_str(), -1, wchr, title.size()+1);
    bi.lpszTitle = wchr;
    
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    bi.lpfn = __usedByBrowseFolder__BrowseCallbackProc;

    std::wstring wsaved_path(saved_path.begin(),saved_path.end());
    const wchar_t * path_param = wsaved_path.c_str();
    bi.lParam = (LPARAM)path_param;

    /* Creates that window and the program freezes on this point. */
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

    /* If a directory was chosen and choice was confirmed, `pidl` won't be 0. */
    if(pidl != 0){
        SHGetPathFromIDList(pidl, path);

        IMalloc * imalloc = 0;
        if(SUCCEEDED(SHGetMalloc(&imalloc))){
            imalloc->Free(pidl);
            imalloc->Release();
        }
        std::wstring wStr = path;
        std::string str(wStr.begin(), wStr.end());
        return str;
    }
    
    //Window closed without confirming a selection.
    return saved_path;
}

customcpp::color* customcpp::getScreenPixels(HWND *hWnd, int wind_Width, int wind_Height){
    HDC hdc, hdcMemory;
    DWORD purposeIsToRemoveWarning;
    BITMAPINFO bitmap;
    
    /**
     * An array containing all pixel rgb values.
     * BUT! micorosft is a place with a bunch of weirdos. 
     * The origin is on the bottom left corner instead of the top left
     * and color values are saved in ABGR order instead of RGBA.
     * ...honestly, why?
     */
    BYTE *data;

    hdc = GetDC(HWND_DESKTOP);
    hdcMemory = CreateCompatibleDC(hdc);

    bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
    bitmap.bmiHeader.biWidth = wind_Width;
    bitmap.bmiHeader.biHeight = wind_Height;
    bitmap.bmiHeader.biPlanes = 1;
    bitmap.bmiHeader.biBitCount = 32;
    bitmap.bmiHeader.biCompression = BI_RGB;
    bitmap.bmiHeader.biSizeImage = wind_Width * 4 * wind_Height;
    bitmap.bmiHeader.biClrUsed = 0;
    bitmap.bmiHeader.biClrImportant = 0;
    HBITMAP hbitmap = CreateDIBSection(hdcMemory, &bitmap, DIB_RGB_COLORS, (void**)(&data), NULL, purposeIsToRemoveWarning);
    SelectObject(hdcMemory, hbitmap);
    BitBlt(hdcMemory, 0, 0, wind_Width, wind_Height, hdc, 0, 0, SRCCOPY);

    color *pixels = new color[wind_Width * wind_Height];

    /* Save pixel RGB values in a more intuitive format - struct with rgba values (alfa always 255) and origin is the top left corner. */
    for(int x = 0; x < wind_Width; x++){
        for(int y = 0; y < wind_Height; y++){
            pixels[(wind_Height-y-1)*wind_Width+x] = {(uint8_t)data[4*(y*wind_Width+x)+2], (uint8_t)data[4*(y*wind_Width+x)+1], (uint8_t)data[4*(y*wind_Width+x)], 255};
        }
    }

    DeleteObject(hbitmap);
    DeleteDC(hdcMemory);
    ReleaseDC(*hWnd, hdc);

    return pixels;
}

bool customcpp::windowIgnoreColor(HWND *windowHandle, color ingored_rgb){
    COLORREF clr = RGB(ingored_rgb.r, ingored_rgb.g, ingored_rgb.b);
    SetWindowLong(*windowHandle, GWL_EXSTYLE, GetWindowLong(*windowHandle, GWL_EXSTYLE) | WS_EX_LAYERED);
    return SetLayeredWindowAttributes(*windowHandle, clr, 0, LWA_COLORKEY);
}

void customcpp::passCommands(std::vector<std::string> commands, int waitInterval){
    if(commands.size() == 0) return;
    FILE *f = popen(commands[0].c_str(), "w");

    for(int i = 1; i < commands.size(); i++){
        Sleep(waitInterval);

        std::string cmd = commands[i] + "\n";
        fprintf(f, cmd.c_str());
        fflush(f);
    }

    pclose(f);
    return;
}

std::string customcpp::getExecutablePath() {
    std::vector<wchar_t> path(MAX_PATH);
    DWORD length = GetModuleFileName(nullptr, path.data(), static_cast<DWORD>(path.size()));

    if (length == 0) {
        std::cout << ACLR.BRIGHT_RED << L"Failed to get executable path. Error: " << GetLastError() << "\n" << ACLR.RESET;
        return "";
    }

    // If the buffer was too small, allocate a larger buffer and try again
    if (length == path.size() - 1) {
        path.resize(length + 1);
        length = GetModuleFileName(nullptr, path.data(), static_cast<DWORD>(path.size()));
        if (length == 0) {
            std::cout << ACLR.BRIGHT_RED  << L"Failed to get executable path on retry. Error: " << GetLastError() << "\n" << ACLR.RESET;
            return "";
        }
    }

    return std::string(path.begin(), path.begin() + length);
}

void customcpp::setAutoStart(bool enable) {
    std::string path = getExecutablePath();
    std::wstring Wpath = std::wstring(path.begin(), path.end() + path.size());

    std::string appName = "";
    for(int i = path.size()-4-1; i>=0; i--){
        if(path[i] == '\\') break;
        else appName+=path[i];
    }
    std::string tmp = appName; appName="";
    for(int i = tmp.size()-1; i>=0; i--)
        appName+=tmp[i];
    
    std::wstring Wappname = std::wstring(appName.begin(), appName.end()+appName.size());

    HKEY hKey;
    LONG result;

    // Open the registry key
    result = RegOpenKeyEx(HKEY_CURRENT_USER, REGISTRY_PATH.c_str(), 0, KEY_ALL_ACCESS, &hKey);
    if (result != ERROR_SUCCESS) {
        std::cout << ACLR.BRIGHT_RED << "Failed to open registry key. Error: " << result << "\n" << ACLR.RESET;
        return;
    }

    if (enable) {
        // Set the registry value to enable auto-start
        result = RegSetValueEx(hKey, Wappname.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE*>(Wpath.c_str()), (Wpath.length() + 1) * sizeof(wchar_t));
        if (result != ERROR_SUCCESS) {
            std::cout << ACLR.BRIGHT_RED << "Failed to set registry value. Error: " << result << "\n" << ACLR.RESET;
        } else {
            std::cout << "Auto-start enabled." << "\n";
        }
    } else {
        // Delete the registry value to disable auto-start
        result = RegDeleteValue(hKey, Wappname.c_str());
        if (result != ERROR_SUCCESS) {
            std::cout << ACLR.BRIGHT_RED << L"Failed to delete registry value. Error: " << result << "\n" << ACLR.RESET;
        } else {
            std::cout << "Auto-start disabled." << "\n";
        }
    }

    // Close the registry key
    RegCloseKey(hKey);
}

///////////////////////////////////////////////////////////////////////////

std::vector<std::string> customcpp::findRow(std::string keyword, std::string pathToFile){
    std::ifstream file(pathToFile);
    if(!file){
        std::cout << "Couldn't open file \"" << pathToFile << "\"\n";
        return {{}};
    }

    std::string line;
    int index = -1;
    while(!file.eof()){
        std::getline(file, line);

        /* Get the keyword */
        std::string key = "";
        for(int i = 0; i < line.size(); i++)
            if(line[i] == ' '){
                if(key == keyword){
                    index = i+1;
                    break;
                }
            }
            else key+=line[i];

        if(index != -1) break;
    }

    /* Get the data */
    std::vector<std::string> data {};
    if(index != -1){
        data.push_back("");
        for(int i = index; i < line.size(); i++){
            if(line[i] == ' ') data.push_back("");
            else data[data.size()-1] += line[i];
        }
    }

    file.close();
    return data;
}