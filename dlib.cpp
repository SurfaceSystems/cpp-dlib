/*
    @name dlib
    @author Blas Fernández
    @version prerelease 0.1
    @copyright GNU GPL v3
*/

// Include necessary functions
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>  // Make sure this is included for stringstream
#include <chrono>
#include <thread>
#include <iomanip>
#include <conio.h>
#ifdef _WIN32
    #include <windows.h>
    #include <wininet.h>
    #pragma comment(lib, "wininet.lib")  // Link WinINet library
#else
    #include <cstdlib>  // For system() call on Linux
#endif

using namespace std;

// ASCII colors 
// Define ASCII escape sequences for text modifications
const std::string ASCII_BOLD = "\x1b[1m";        // Bold text
const std::string ASCII_UNDERLINE = "\x1b[4m";   // Underlined text
const std::string ASCII_REVERSE = "\x1b[7m";     // Reverse (swap foreground and background colors)

// Define colors (optional)
const std::string ASCII_BLACK = "\x1b[30m";
const std::string ASCII_RED = "\x1b[31m";
const std::string ASCII_GREEN = "\x1b[32m";
const std::string ASCII_YELLOW = "\x1b[33m";
const std::string ASCII_BLUE = "\x1b[34m";
const std::string ASCII_MAGENTA = "\x1b[35m";
const std::string ASCII_CYAN = "\x1b[36m";
const std::string ASCII_WHITE = "\x1b[37m";

// Additional text modifications (optional)
const std::string ASCII_DIM = "\x1b[2m";         // Dim text
const std::string ASCII_BLINK = "\x1b[5m";       // Blinking text
const std::string ASCII_HIDDEN = "\x1b[8m";      // Hidden text
 
// Background colors (optional)
const std::string ASCII_BG_BLACK = "\x1b[40m";
const std::string ASCII_BG_RED = "\x1b[41m";
const std::string ASCII_BG_GREEN = "\x1b[42m";
const std::string ASCII_BG_YELLOW = "\x1b[43m";
const std::string ASCII_BG_BLUE = "\x1b[44m";
const std::string ASCII_BG_MAGENTA = "\x1b[45m";
const std::string ASCII_BG_CYAN = "\x1b[46m";
const std::string ASCII_BG_WHITE = "\x1b[47m";
const std::string ASCII_RESET = "\x1b[0m" + ASCII_WHITE;       // Reset text to default

// Define bright color variables
const std::string BRIGHT_RED = "\033[91m";
const std::string BRIGHT_GREEN = "\033[92m";
const std::string BRIGHT_YELLOW = "\033[93m";
const std::string BRIGHT_BLUE = "\033[94m";
const std::string BRIGHT_MAGENTA = "\033[95m";
const std::string BRIGHT_CYAN = "\033[96m";
const std::string BRIGHT_WHITE = "\033[97m";
const std::string RESET_COLOR = "\033[0m";

// Util functions
bool fileExists(string filename) {
    fstream f(filename);
    return f.good();
}


bool fileExistsOnInternet(const std::string& url) {
#ifdef _WIN32
    // Windows-specific code using WinINet API
    HINTERNET hInternet = InternetOpen("HTTP Checker", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!hInternet) return false;

    HINTERNET hConnect = InternetOpenUrl(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hConnect) {
        InternetCloseHandle(hInternet);
        return false;
    }

    DWORD statusCode = 0;
    DWORD length = sizeof(DWORD);

    HttpQueryInfo(hConnect, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &statusCode, &length, NULL);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return statusCode == 200;

#else
    // Linux-specific code using system call
    std::string command = "wget --spider -q " + url;
    int result = system(command.c_str());

    return result == 0;  // If wget returns 0, it means the file exists
#endif
}
std::string readFileFromInternet(const std::string& url) {
    // Initialize an internet session
    HINTERNET hInternet = InternetOpen("File Reader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        std::cerr << "Failed to open internet connection." << std::endl;
        return "";
    }

    // Open the URL
    HINTERNET hUrl = InternetOpenUrl(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hUrl) {
        std::cerr << "Failed to open URL: " << url << std::endl;
        InternetCloseHandle(hInternet);
        return "";
    }

    // Buffer for reading data
    char buffer[4096];
    DWORD bytesRead;
    std::stringstream fileContent;

    // Read data from the URL
    while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        fileContent.write(buffer, bytesRead);
    }

    // Clean up
    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);

    return fileContent.str();  // Return the full content of the file as a string
}
std::vector<std::string> splitString(const std::string& input, char delimiter) {
    std::vector<std::string> result;
    std::istringstream stream(input);
    std::string token;
    
    while (std::getline(stream, token, delimiter)) {
        if (!token.empty()) {
            result.push_back(token);
        }
    }

    return result;
}

void displayProgressBar(double percentage) {
    int barWidth = 35;  // Width of the progress bar
    std::cout << "[";
    int pos = static_cast<int>(barWidth * percentage);
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(percentage * 100.0) << " %";
}

// Function to download the file and display progress in one line
bool downloadFileWithProgress(const std::string& url, const std::string& localFile) {
    HINTERNET hInternet = InternetOpen("File Downloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        std::cerr << "Failed to open internet connection." << std::endl;
        return false;
    }

    HINTERNET hUrl = InternetOpenUrl(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hUrl) {
        std::cerr << "Failed to open URL: " << url << std::endl;
        InternetCloseHandle(hInternet);
        return false;
    }

    // Buffer and progress tracking variables
    char buffer[8192];
    DWORD bytesRead;
    std::ofstream outputFile(localFile, std::ios::binary);

    if (!outputFile) {
        std::cerr << "Failed to create file: " << localFile << std::endl;
        InternetCloseHandle(hUrl);
        InternetCloseHandle(hInternet);
        return false;
    }

    // Get file size (if available from server)
    DWORD fileSize = 0;
    DWORD fileSizeLength = sizeof(fileSize);
    HttpQueryInfo(hUrl, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &fileSize, &fileSizeLength, NULL);

    double totalDownloaded = 0;
    double lastDownloaded = 0;
    double downloadSpeed = 0;
    double lastDownloadSpeed = 0;
    auto startTime = std::chrono::steady_clock::now();
    bool speedIncreased = false;

    // Read the file in chunks and track progress
    while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        totalDownloaded += bytesRead;
        outputFile.write(buffer, bytesRead);

        // Calculate time and speed
        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsedTime = currentTime - startTime;
        downloadSpeed = totalDownloaded / elapsedTime.count();  // Bytes per second

        // Check if speed increased or decreased
        speedIncreased = downloadSpeed >= lastDownloadSpeed;

        // Estimate remaining time
        double remainingTime = (fileSize - totalDownloaded) / downloadSpeed;

        // Display progress in one line
        std::cout << "\r";  // Carriage return to overwrite the line


        // Print progress line
        std::cout << "Downloading " << url << " to " << localFile << ":"
                  << " Size: " << fileSize / (1024 * 1024) << " MB"
                  << " D. Speed: ";
        if(speedIncreased) {
            cout << ASCII_GREEN << ASCII_BOLD;
        } else {
            cout << ASCII_RED << ASCII_BOLD;
        }
        cout << std::fixed << std::setprecision(2) << downloadSpeed / (1024) << " KB/s";
        cout << ASCII_RESET
                  << " Time left: " << int(remainingTime) << " sec"
                  << " Remaining: " << (fileSize - totalDownloaded) / (1024 * 1024) << " MB ";
        displayProgressBar(totalDownloaded / fileSize);  // Display progress bar


        lastDownloadSpeed = downloadSpeed;

        // Simulate real-time progress update
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    outputFile.close();
    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);

    std::cout << "\nDownload completed successfully!" << std::endl;
    return true;
}


void runLine(string line) {
    if(line.substr(0,4) == "msg ") {
        cout << line.substr(4) << endl;
    } else if(line.substr(0,7) == "downli ") {
        string url = line.substr(7, line.substr(7).find(";"));
        string saveto = line.substr(line.substr(7).find(";") + 8);

        downloadFileWithProgress(url, saveto);
    } else if(line.substr(0,6) == "mkdir ") {
        string com = "mkdir " + line.substr(6);

        system(com.c_str());
    } else if(line == "check") {
        cout << "Do you want to continue? (y=yes)";

        if(_getch() != 'y') {
            exit(0);
        }
        cout << "\n";
    } else {
        cout << ASCII_BOLD << ASCII_RED << "Error: " << ASCII_RESET << ASCII_RED << "UnknownCommand: " << line << ASCII_RESET << endl;
    }
}

void compileFullFile(string text) {
    for(const auto& str : splitString(text, '\n')) {
        runLine(str);
    }
}

// Main method
int main(int argc, char** argv) {
    if(argc > 1) {
        string n(argv[1]);

        if(n == "//version") {
            cout << ASCII_BOLD << "dlib prerelease v_0.3" << ASCII_RESET << endl;
            cout << "Checking for updates...\n";
            if(readFileFromInternet("https://elitees.github.io/dlib-index/version") == "prerelease v_0.3\n") {
                cout << ASCII_BOLD << "All is up-to-date.\n" << ASCII_RESET;
            } else {
                cout << ASCII_UNDERLINE << ASCII_RED << "New update avaiable, consider updating to get new security and new functions." << ASCII_RESET << "\n";
            }
            return 0;
        }

        if(fileExistsOnInternet("https://elitees.github.io/dlib-index/scripts/" + n + ".ds")) {
            cout << ASCII_BLINK << ASCII_BOLD << ASCII_WHITE << "Reading " << n << ".ds script...\n" << ASCII_RESET;

            compileFullFile(readFileFromInternet("https://elitees.github.io/dlib-index/scripts/" + n + ".ds"));
        } else if(fileExistsOnInternet(n)) {
            cout << ASCII_BLINK << ASCII_BOLD << ASCII_WHITE << "Reading " << n << ".ds script...\n" << ASCII_RESET;

            compileFullFile(readFileFromInternet(n));
        } else if(fileExists(n)) {
            cout << ASCII_BLINK << ASCII_BOLD << ASCII_WHITE << "Reading " << n << " script...\n" << ASCII_RESET;

            string t, l;

            fstream f(n);
            while(getline(cin, l)) {
                t += l + "\n";
            }
            f.close();

            cout << t;

            compileFullFile(t);
        } else {
            cout << ASCII_BOLD << ASCII_RED << "Error:\nThe script couldn't be found on 'dlib' index repo, either on internet, either on your computer.\nPlease consider to check for spelling errors.\n";
            cout << ASCII_RESET;
            return -1;
        }
    } else {
        cout << ASCII_BOLD << ASCII_WHITE << "No Argument Found\n" << ASCII_RESET;
        cout << "Usage: dlib [repo]\n";
        cout << "     : dlib [script_url]\n";
        cout << "     : dlib [script_path]\n";
        cout << "     : dlib //version\n";
        //cout << "More info at " << ASCII_UNDERLINE << ASCII_BLUE << "https://e--dlib.web.app/help/" << ASCII_RESET << "\n";
    }

    cout << ASCII_RESET;
    return 0;
}