/* Custom functions and data types that are useful for various c++ applications. */

#include<CustomCPP/customCPP.h>

///////////////////////////////

#ifdef _WIN32
    struct BGPROC{
        int ID;
        PROCESS_INFORMATION pi;
        STARTUPINFO si;
    };
    std::vector<BGPROC> proc{};
#else
    /* UNIX */
    struct BGPROC_STR{
        int ID;
        pid_t pidt;
    };
    std::vector<BGPROC_STR> proc{};
#endif

///////////////////////////////

void customcpp::clearLine(int numb_of_lines){
    for(int i = 0; i < numb_of_lines; i++){
        std::cout << "\r\033[K" << std::flush; /* Clear a line */
        if(i + 1 < numb_of_lines) std::cout << "\033[A"; /* Moves up a line */
    }
    std::cout << std::flush;
    return;
}

void customcpp::clearStrLine(std::string &str, int numb_of_lines){
    int numbCleared = 0;
    for(int i = str.size()-1; i>=0; i--){
        if(str[i] == '\n')
            numbCleared++;
        if(numbCleared == numb_of_lines) break;
        str.pop_back();
    }
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

std::string customcpp::strtolower(std::string in){
    std::string _return = "";
    for(char &chr : in)
        _return += std::tolower(chr);
    return _return;
}

bool customcpp::commparePoints(point point1, point point2){
    if(point1.x == point2.x && point1.y == point2.y) return true;
    return false;
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

    bool negative = false;
    if(numb == 0) _return_backwards = "0";
    if(numb < 0) negative = true;

    /* Conver the number backwards into a string. */
    while(numb != 0){
        _return_backwards += char(std::abs(numb)%10+48);
        numb/=10;
    }
    if(negative)_return_backwards+='-';
    /* Reverse the number. */
    std::string _return = "";
    for(int i = _return_backwards.size()-1; i>=0; i--)
        _return+=_return_backwards[i];

    return _return;
}

int customcpp::stringToInt(std::string string){
    /* Empty string shall be nothing, which is 0. */
    if(string.size() == 0) return 0;
    int negative = 1;
    if(string[0] == '-') negative = -1;

    /* remove non-numbers */
    std::string str = "";
    for(char &chr : string)
        if(chr >= '0' && chr <= '9') str+=chr;
    string = str;

    int _return = 0;
    int multiplier = 1;
    
    for(int i = string.size()-1; i >= 0; i--){
        if(i == 0 && string[i] == '-') _return*=-1;
        else if(i!=0 && string[i] == '-') return 0;

        _return += ((int)string[i]-48)*multiplier;
        multiplier*=10;
    }

    return _return*negative;
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

std::string customcpp::floatToString(float in, int precision){
    std::string _return = "";
    if(in == 0) return "0";

    /* Seperate integer and fractional parts. */
    int whole = int(in);
    float fraction = in - whole;

    /* Save the integer part. */
    _return = intToString(whole);
    _return += ".";

    /* Save fractional part to the provided precision. */
    _return += intToString(int(fraction*std::pow(10, precision)));

    return _return;
}

float customcpp::stringToFloat(std::string in){
    /* Empty string shall be nothing, which is 0. */
    if(in.size() == 0) return 0;
    
    float _return = 0;
    int k = 0;
    if(in[0] == '-') k = 1;
    float multiplier = 0.1;
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

#ifdef _WIN32
wchar_t* customcpp::charToLPWSTR(const char* charArr){
    int length = MultiByteToWideChar(CP_UTF8, 0, charArr, -1, nullptr, 0);
    wchar_t* wideChar = new wchar_t[length];

    MultiByteToWideChar(CP_UTF8, 0, charArr, -1, wideChar, length);

    return wideChar;
}
#endif
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
#ifdef _WIN32

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
    const wchar_t *path_param = wsaved_path.c_str();
    bi.lParam = (LPARAM)path_param;

    /* Creates that window and the program freezes on this point. */
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

    /* If a directory was chosen and choice was confirmed, `pidl` won't be 0. */
    if(pidl != 0){
        SHGetPathFromIDList(pidl, path);

        IMalloc *imalloc = 0;
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

bool customcpp::isDriveOccupied(char &driveLetter) {
    // Ensure driveLetter is uppercase and in the valid range
    driveLetter = toupper(driveLetter);
    if (driveLetter < 'A' || driveLetter > 'Z') {
        std::cout << ACLR.BRIGHT_RED << "Invalid letter, must be between A and Z.\n" << ACLR.RESET;
        return true;
    }

    // Construct the drive string (e.g., "D:\\")
    std::string drivePath = std::string(1, driveLetter) + ":\\";
    
    // Check if the path is a valid directory
    DWORD attributes = GetFileAttributes(charToLPWSTR(drivePath.c_str()));
    
    // If attributes are valid and the path exists, the drive is occupied
    return (attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY));
}

#else

std::vector<std::string> customcpp::listDirectories(const char* pth){
    std::string path = std::string(pth);
    if(path[0] == '~'){
        path.erase(path.begin());
        path = "/home/" + getCurrentUserName() + path;
    }
    
    DIR* dir = opendir(path.c_str());
    struct dirent* entry;
    
    if(dir == nullptr) {
        std::cerr << "Error opening directory: " << path << std::endl;
        return {};
    }

    std::vector<std::string> list;
    while ((entry = readdir(dir)) != nullptr) {
        // Check if entry is a directory
        if (entry->d_type == DT_DIR) {
            // Skip the "." and ".." entries
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                list.push_back(entry->d_name);
            }
        }
    }

    closedir(dir);
    return list;
}

void customcpp::childFunction(void (*foo)(void*), void* data){
    pid_t pidt;
    pidt = fork();
    
    if(pidt == 0){
        /* Child*/
        foo(data);
        _exit(0);
    }
    else{
        /* Parent*/
        //waitpid(pidt, NULL, 0);
    }
    return;
}

#endif

//////////////////////
/*Cross platform*/

void customcpp::cmd(std::vector<std::string> commands, int waitInterval){
    if(commands.size() == 0) return;
    FILE *f = popen(commands[0].c_str(), "w");

    for(int i = 1; i < commands.size(); i++){
        SLEEP(waitInterval);

        std::string cmd = commands[i] + "\n";
        fprintf(f, cmd.c_str());
        fflush(f);
    }

    pclose(f);
    return;
}

std::vector<std::string> customcpp::getcmd(std::vector<std::string> commands, int waitInterval){
    if(commands.size() == 0) return {};
    FILE *f = popen(commands[0].c_str(), "r");
    for(int i = 1; i < commands.size(); i++){
        SLEEP(waitInterval);

        std::string cmd = commands[i] + "\n";
        fprintf(f, cmd.c_str());
        fflush(f);
    }  

    /* Read the output*/
    std::vector<std::string> _return;
    char buffer[1024];

    while(fgets(buffer, sizeof(buffer), f) != nullptr) _return.push_back(std::string(buffer));

    pclose(f);
    return _return;
}

void customcpp::CLEAR(){
    #ifdef _WIN32
        cmd({"cls"}, 0);
    #else
        cmd({"clear"}, 0);
    #endif
}

void customcpp::SLEEP(int milliseconds){
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds*1000);
    #endif
}

bool customcpp::backgroundProcess(std::string command, bool waitForQuit, int ID){
    static bool init = true;
    if(init){
        init = false;
        proc.push_back({});
        proc.clear();
    }

    #ifdef _WIN32
        /* Windows */
        proc.push_back({ID});
        int indx = proc.size()-1;

        ZeroMemory(&proc[indx].si, sizeof(proc[indx].si));
        proc[indx].si.cb = sizeof(proc[indx].si);
        ZeroMemory(&proc[indx].pi, sizeof(proc[indx].pi));

        LPWSTR cmd = charToLPWSTR(command.c_str());
        if(!CreateProcess(NULL, cmd, NULL, NULL, FALSE, CREATE_NEW_CONSOLE & CREATE_NO_WINDOW, NULL, NULL, &proc[indx].si, &proc[indx].pi)){
            std::cout <<  ACLR.BRIGHT_RED << "CreateProcess failed (" << GetLastError() << ").\n" << ACLR.RESET;
            return false;
        }

        if(waitForQuit){
            DWORD dwWaitResult = WaitForSingleObject(proc[indx].pi.hProcess, INFINITE);
            if(dwWaitResult != WAIT_OBJECT_0) std::cout << ACLR.BRIGHT_RED << "WaitForSingleObject failed (" << GetLastError() << ").\n" << ACLR.RESET;
            
            CloseHandle(proc[indx].pi.hProcess);
            CloseHandle(proc[indx].pi.hThread);
            proc.erase(proc.begin() + indx);
            proc.shrink_to_fit();
        }
        
        /* process will end when application quits */
    #else
        /* Unix */
        proc.push_back({ID});
        int indx = proc.size()-1;

        proc[indx].pidt = fork();
        if(proc[indx].pidt == -1){
            std::cout << ACLR.BRIGHT_RED << "fork() failed.\n" << ACLR.RESET;
            return false;
        }

        if(proc[indx].pidt == 0){
            /* Child */
            setsid();
            execlp(command.c_str(), command.c_str(), (char*)NULL);
            /*Failed*/
            std::cerr << ACLR.BRIGHT_RED << "execlp failed.\n" << ACLR.RESET;
            exit(EXIT_FAILURE);
        }
        else{
            /* Parent */
            if(waitForQuit){
                waitpid(proc[indx].pidt, NULL, 0);
                proc.erase(proc.begin() + indx);
                proc.shrink_to_fit();
                return true;
            }
        }

    #endif    
    return true;
}

void customcpp::ctrlC_backgroundProcess(int ID){
    if(ID == -1){
        for(int i = proc.size()-1; i>= 0; i--) ctrlC_backgroundProcess(proc[i].ID);
        return;
    }
    if(proc.size() == 0) return;
    int indx = 0;
    for(int i = 0; i < proc.size(); i++)
        if(proc[i].ID == ID){ indx = i; break; }

    #ifdef _WIN32
        if (!TerminateProcess(proc[indx].pi.hProcess, 1)) std::cerr  << ACLR.BRIGHT_RED<< "Failed to terminate process (" << GetLastError() << ").\n" + ACLR.RESET;

        CloseHandle(proc[indx].pi.hProcess);
        CloseHandle(proc[indx].pi.hThread);

    #else
        /* Unix */
        if(kill(proc[indx].pidt, SIGINT) == -1) std::cerr << ACLR.BRIGHT_RED << "Failed to send SIGINT to process (" << proc[indx].pidt << "): " << strerror(errno) << "\n" << ACLR.RESET;
    
    #endif  

    proc.erase(proc.begin() + indx);
    proc.shrink_to_fit();
}

bool customcpp::ping(std::string server){
    #ifdef _WIN32
        std::string command = "ping " + server + " -n 1 > NUL 2>&1";
    #else
        std::string command = "ping -c 1 " + server + " > /dev/null 2>&1";
    #endif

    if(std::system(command.c_str()) > 0) return false; /*Did not reach a server*/
    return true;
}

#ifndef _WIN32
    void setNonCanonicalMode(bool enable) {
    static struct termios oldt, newt;
    if (enable) {
        // Save old terminal settings
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;

        // Disable canonical mode and echo
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        // Restore old terminal settings
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}
#endif

bool customcpp::getConsoleINPUT(std::string &YourString, std::string restrict_, bool RestrictiveInput, bool printToConsole){
    #ifdef _WIN32
        auto lambda = [&](){
            char chr = _getch();
            if(chr == 13){ /* Enter*/
                std::cout << "\n";
                return true;
            }
            if(chr == 8){ /* BackSpace */
                if(YourString.size() != 0){
                    YourString.pop_back();
                    if(printToConsole) std::cout << "\b \b";
                }
            }
            else{
                bool pass = true;
                if(restrict_.size()>0){
                    if(restrict_[0] == '\b'){
                        /* WHITELIST */
                        pass = false;
                        for(char whitelist:restrict_){
                            if(whitelist==chr){
                                pass = true;
                                break;
                            }
                        }
                    }
                    else{
                        /* BLACKLIST*/
                        for(char blacklist : restrict_)
                            if(blacklist == chr){
                                pass = false;
                                break;
                            }
                    }
                }
                if(pass) {YourString+=chr;if(printToConsole)std::cout << chr;}
            }
            return false;
        };

        if(RestrictiveInput){ while(true) if(lambda()) break; }
        else if(_kbhit()) return lambda();

    #else
        auto lambda = [&](){
            char chr = getchar();
            if(chr == '\n'){ /* Enter*/
                std::cout << "\n";
                return true;
            }
            if(chr == 127){ /* BackSpace */
                if(YourString.size() != 0) YourString.pop_back();
            }
            else{
                bool pass = true;
                if(restrict_.size()>0){
                    if(restrict_[0] == '\b'){
                        /* WHITELIST */
                        pass = false;
                        for(char whitelist:restrict_){
                            if(whitelist==chr){
                                pass = true;
                                break;
                            }
                        }
                    }
                    else{
                        /* BLACKLIST*/
                        for(char blacklist : restrict_)
                            if(blacklist == chr){
                                pass = false;
                                break;
                            }
                    }
                }
                if(pass) YourString+=chr;
            }
            return false;
        };

        setNonCanonicalMode(true);
        if(RestrictiveInput){ while(true) if(lambda()) break; }
        else return lambda();
        setNonCanonicalMode(false);

    #endif
    return false;
}

void customcpp::setAutoStart(bool enable) {
    #ifdef _WIN32
        std::string path = getExecutablePath();
        std::wstring Wpath = std::wstring(path.begin(), path.end() + path.size());

        std::string appName = getCurrentApplicationName();
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
    #else
        /* Unix */
        std::string appName = getCurrentApplicationName();
        if(true){
            std::ofstream serviceFile("/etc/systemd/system/"+appName+".service");
            if(!write) std::cerr << ACLR.BRIGHT_RED << "Couldn't write to a file when enabling autostart for " + appName << "\n" + ACLR.RESET;

            serviceFile << "[Unit]\n";
            serviceFile << "Description=" + appName + " application, enable autostart.\n";
            serviceFile << "After=network.target\n\n";

            serviceFile << "[Service]\n";
            serviceFile << "ExecStart="+getExecutablePath()+"\n";
            serviceFile << "Restart=always\n";
            serviceFile << "User="+getCurrentUserName()+"\n";
            serviceFile << "Group="+getCurrentUserName() + "\n\n";

            serviceFile << "[Install]\n";
            serviceFile << "WantedBy=multi-user.target\n";

            serviceFile.close();
            cmd({"sudo systemctl enable " + appName + ".service", "sudo systemctl start " + appName + ".service", "sudo systemctl daemon-reload"}, 50);
            std::cout << "Auto-start enabled." << "\n";
        }
        else{
            cmd({ "sudo systemctl stop " + appName + ".service", "sudo systemctl disable " + appName + ".service", "sudo rm /etc/systemd/system/" + appName + ".service", "sudo systemctl daemon-reload"}, 50);
            std::cout << "Auto-start disabled." << "\n";
        }
    #endif
}

std::string customcpp::getCurrentUserName() {
    #ifdef _WIN32
        WCHAR username[UNLEN + 1]; // Buffer for the user name
        DWORD username_len = UNLEN + 1; // Length of the buffer

        // Get the user name
        if (GetUserNameW(username, &username_len)) {
            // Convert wide string to narrow string
            std::wstring wide_username(username, username_len - 1); // username_len includes null terminator, so subtract 1
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            return converter.to_bytes(wide_username);
        } else {
            std::cout << ACLR.BRIGHT_RED << "Failed to get user name. Error: " << GetLastError() << "\n" << ACLR.RESET;
            return "";
        }
    #else
        /* Unix */
        uid_t uid = getuid();
        struct passwd *pw = getpwuid(uid);
        if(pw) return std::string(pw->pw_name);
        
    #endif
    return "";
}

std::string customcpp::getCurrentApplicationName(){
    #ifdef _WIN32
        std::string reversed_appname = "";
        std::string path = getExecutablePath();
        std::string appname = "";

        for(int i = path.size()-1; i>=0; i--)
            if(path[i] == '\\' || path[i] == '/') break;
            else reversed_appname+=path[i];

        for(int i = reversed_appname.size()-1; i>=0; i--)
            appname+=reversed_appname[i];

        return appname;
    #else
        return basename(getExecutablePath().data()); 
    #endif
    return "";
}

std::string customcpp::getExecutablePath() {
    std::string path = "";
    #ifdef _WIN32
        std::vector<wchar_t> wpath(MAX_PATH);
        DWORD length = GetModuleFileName(nullptr, wpath.data(), static_cast<DWORD>(wpath.size()));

        if (length == 0) {
            std::cout << ACLR.BRIGHT_RED << "Failed to get executable path. Error: " << GetLastError() << "\n" << ACLR.RESET;
            return "";
        }

        // If the buffer was too small, allocate a larger buffer and try again
        if (length == wpath.size() - 1) {
            wpath.resize(length + 1);
            length = GetModuleFileName(nullptr, wpath.data(), static_cast<DWORD>(wpath.size()));
            if (length == 0) {
                std::cout << ACLR.BRIGHT_RED  << "Failed to get executable path on retry. Error: " << GetLastError() << "\n" << ACLR.RESET;
                return "";
            }
        }

        path = std::string(path.begin(), path.begin() + length);
    #else
        /* Unix */
        char cpath[1024];
        ssize_t len = readlink("/proc/self/exe", cpath, sizeof(cpath) - 1);
        if (len != -1) {
            cpath[len] = '\0'; // Null-terminate the string
            path = std::string(cpath);
        } else return "";
    #endif
    return path;
}

/* it is enough for now */
bool customcpp::checkGivenArgument(char *argument, std::string requiredArgument, bool singleLetter){
	if(singleLetter){
		char charArg;
		for(int i = 0; i < sizeof(argument)/sizeof(char); i++)
			if(argument[i] != '-'){
				charArg = argument[i];
				break;
			}
		char charWanted;
		for(int i = 0; i < requiredArgument.size(); i++)
			if(requiredArgument[i] != '-'){
				charWanted = requiredArgument[i];
				break;
			}

		if(charWanted == charArg) return true;
	}
	else{
		if(sizeof(argument)/sizeof(char) == requiredArgument.size()){
			for(int i = 0; i < requiredArgument.size(); i++){
				if(requiredArgument[i] != argument[i]) return false;
			}
			return true;
		}
	} 

	return false;
}

///////////////////////////////////////////////////////////////////////////

std::string customcpp::findLine(std::string keyword, point area, std::string pathToFile){
    if(area.x < 0) return "";

    std::ifstream file(pathToFile);
    if(!file){
        std::cerr << "Couldn't open file \"" << pathToFile << "\"\n";
        return {{}};
    }

    std::string line;
    int index = -1;
    bool findSpecificArea = false;

    auto check = [&keyword](std::string line)-> bool {
        if(line.size() != 0){
            int x = 0;
            while(line[x] == ' ') x++;
            if(line[x] == '#') return false;

            std::string key = "";
            for(int i = x; i < line.size(); i++)
                if(key == keyword) return true;
                else key+=line[i];
        }
        return false;
    };

    if(area.x != area.y) findSpecificArea = true; 
    /*kinda repeated myself three times. Fix this later*/
    if(!findSpecificArea){
        while(!file.eof()){
            std::getline(file, line);
            if(check(line)){
                file.close();
                return line;
            }
        }
    }
    else{
        /* Move to the nth line */
        int linenumb = 1;
        while(!file.eof() && linenumb < area.x){
            std::getline(file, line);
            linenumb++;
        }

        if(!file.eof()){
            /* Do checks until reached end of file or specified areas */
            bool allLeft = false;
            if(area.x > area.y) allLeft = true;

            if(allLeft){
                while(!file.eof()){
                    std::getline(file, line);
                    if(check(line)){
                        file.close();
                        return line;
                    }
                }
            }
            else{
                while(!file.eof() && linenumb <= area.y){
                    linenumb++;
                    std::getline(file, line);
                    if(check(line)){
                        file.close();
                        return line;
                    }
                }
            }
        }
    }

    file.close();
    return "";
}

bool customcpp::getBoolean(std::string line, bool &value){
    bool success = false;

    int i = 0;
    while(i<line.size()){
        if(line[i] == ':') break;
        i++;
    }
    i++;
    std::string var = "";
    while(i<line.size()){
        if(line[i] != ' ') break;
        i++;
    }
    while(i < line.size()){
        if(line[i] == ' ' || line[i] == '\n') break;
        var+=line[i];
        i++;
    }
    /* all characters to lowercase */
    std::transform(var.begin(), var.end(), var.begin(), [](unsigned char c){ return std::tolower(c); });

    if(var == "true"){
        success = true;
        value = true;
    }
    else if(var == "false"){
        success = true;
        value = false;
    }

    return success;
}

bool customcpp::getScalar(std::string line, double &value){
    bool success = false;
    int i = 0;
    while(i < line.size()){
        if(line[i] == ':') break;
        i++;
    }
    i++;
    
    std::string var = "";
    while(i < line.size()){
        if(line[i] != ' '){
            /* Number string has a character, */
            if(line[i] != ',' && line[i] != '.') return false;
            break;
        }
        i++;
    }
    while(i < line.size()){
        if(line[i] == ' ' || line[i] == '\n' || i+1 == line.size()){
            /* assume that reaching the end of point of possible number represented in a string, the extraction is successful. */
            success = true;
            value = stringToDouble(var);
            break;
        }
        var+=line[i];
        i++; 
    }

    return success;
}

bool customcpp::getString(std::string line, std::string &value){
    /* true if `:` is found */
    bool foundchar = false;
    for(int i = 0; i < value.size(); i++)
        if(value[i] == ':'){
            foundchar = true;
            break;
        }

    if(foundchar){
        int i = 0;
        std::string var = "";
        while(i < value.size())
            if(value[i] == ':') break;
            else i++;
        i++;

        while(i < value.size())
            if(value[i] != ' ') break;
            else i++;

        while(i < value.size())
            var+=value[i];

        /* Remove the trailing whitespaces*/
        for(int i = var.size() -1; i >=0; i--)
            if(var[i] == ' ') var.erase(var.begin() + i); 
            else break;

            value = var;
    }

    return foundchar;
}

customcpp::point customcpp::findHeader(std::string headerName, std::string pathToFile){
    point area = {-1, 0};

    auto removeTrailingAndLeadingWhitespaces = [](std::string in)->std::string {
        /* Clear possible whitespaces and new line at the end*/
        std::string _return = "";
        int indx = 0;
        for(indx; indx < in.size(); indx++)
            if(in[indx] == '['){
                indx++;
                break;
            }

        for(indx; indx < in.size(); indx++)
            if(in[indx] == ']') break;
            else _return+=in[indx];
        return _return;
    };

    headerName = removeTrailingAndLeadingWhitespaces(headerName);

    std::ifstream file(pathToFile);
    std::string str;
    int line = 1;

    while(!file.eof()){
        std::getline(file, str);
        int i = 0;
        while(str[i] == ' ') i++;

        if(str[i] != '#'){
            str = removeTrailingAndLeadingWhitespaces(str);
            if(str == headerName){
                line++;
                area.x = line;
                break;
            }
        }
        line++;
    }

    while(!file.eof()){
        std::getline(file, str);
        int i = 0;
        while(str[i] == ' ') i++;

        if(str[i] == '['){
            area.y = line - 1;
            break;
        }
        line++;
    }

    return area;
}

bool customcpp::FIND(std::string whatToFind, std::string &whereToFind, int &indx){
    bool itsPath = false;
    if(whatToFind.size() == 0) return false;

    /* determine if provided "whereToFind" is a path file. */
    #ifdef _WIN32
        if(whereToFind.size() >= 2)
        if(whereToFind[0] >= 'A' && whereToFind[0] <= 'Z' && (whereToFind[1] == '\\' || whereToFind[1] == '/')) itsPath = true;
    #else
        if(whereToFind[0] == '/') itsPath = true;
    #endif

    if(itsPath){
        std::ifstream file(whereToFind);
        if(!file.is_open()){
            std::cerr << ACLR.BRIGHT_RED << "Couldn't open file '" << whereToFind << "'.\n" << ACLR.RESET;
            return false;
        }

        std::string oneWholeFile = "";
        std::string line;
        while(!file.eof()){
            std::getline(file, line);
            oneWholeFile+=line;
        }
        file.close();

        /* What "whereToFind" to its contents */
        whereToFind = oneWholeFile;
    }

    if(whatToFind.size() > whereToFind.size()) return false;
    size_t pos = whereToFind.find(whatToFind);
    if(pos == std::string::npos) return false;
    
    if(indx != -1) indx = static_cast<int>(pos);
    return true;
}

void customcpp::TRUNCATEFILE(int line, std::string file){
    std::ifstream read(file);
    std::vector<std::string> lines;
    while(!read.eof()){
        lines.push_back("");
        std::getline(read, lines[lines.size()-1]);
    }
    read.close();

    std::ofstream write(file, std::fstream::trunc);
    for(int i = 0; i < lines.size(); i++){
        if(i != line) write << lines[i] + "\n";
    }
    write.close();
    return;
}

float customcpp::rad(float degrees){
    return degrees*pi/180;
}

float customcpp::deg(float rads){
    return rads*180/pi;
}

float customcpp::angleBetweenTwoPoints(pixel origin, pixel point){
    return atan2(origin.y - point.y, origin.x - point.x);
}

