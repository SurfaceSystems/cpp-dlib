#include "progress.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>

#ifdef _WIN32

#include <windows.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

void displayProgressBar(double progress)
{
    const int width = 40;

    std::cout << "[";

    int pos = width * progress;

    for (int i = 0; i < width; ++i)
    {
        if (i < pos)
            std::cout << "=";
        else if (i == pos)
            std::cout << ">";
        else
            std::cout << " ";
    }

    std::cout << "] "
              << int(progress * 100.0)
              << "%";
}

bool downloadFileWithProgress(
    const std::string& url,
    const std::string& localFile)
{
    HINTERNET hInternet =
        InternetOpen(
            "Downloader",
            INTERNET_OPEN_TYPE_DIRECT,
            NULL,
            NULL,
            0
        );

    if (!hInternet)
    {
        std::cerr << "InternetOpen failed\n";
        return false;
    }

    HINTERNET hUrl =
        InternetOpenUrl(
            hInternet,
            url.c_str(),
            NULL,
            0,
            INTERNET_FLAG_RELOAD,
            0
        );

    if (!hUrl)
    {
        std::cerr << "InternetOpenUrl failed\n";

        InternetCloseHandle(hInternet);

        return false;
    }

    std::ofstream file(
        localFile,
        std::ios::binary
    );

    if (!file)
    {
        std::cerr << "Cannot create file\n";

        InternetCloseHandle(hUrl);
        InternetCloseHandle(hInternet);

        return false;
    }

    DWORD fileSize = 0;
    DWORD sizeSize = sizeof(fileSize);

    HttpQueryInfo(
        hUrl,
        HTTP_QUERY_CONTENT_LENGTH |
        HTTP_QUERY_FLAG_NUMBER,
        &fileSize,
        &sizeSize,
        NULL
    );

    char buffer[8192];
    DWORD bytesRead;

    double totalDownloaded = 0;

    auto start =
        std::chrono::steady_clock::now();

    while (
        InternetReadFile(
            hUrl,
            buffer,
            sizeof(buffer),
            &bytesRead
        ) &&
        bytesRead > 0)
    {
        file.write(buffer, bytesRead);

        totalDownloaded += bytesRead;

        auto now =
            std::chrono::steady_clock::now();

        double elapsed =
            std::chrono::duration<double>(
                now - start
            ).count();

        double speed =
            elapsed > 0
                ? totalDownloaded / elapsed
                : 0;

        double remaining =
            speed > 0
                ? (fileSize - totalDownloaded)
                    / speed
                : 0;

        double progress =
            fileSize > 0
                ? totalDownloaded / fileSize
                : 0;

        std::cout << "\r";

        std::cout
            << std::fixed
            << std::setprecision(2)
            << "Downloaded: "
            << totalDownloaded /
                (1024.0 * 1024.0)
            << " MB / "
            << fileSize /
                (1024.0 * 1024.0)
            << " MB ";

        std::cout
            << "Speed: "
            << speed / 1024.0
            << " KB/s ";

        std::cout
            << "ETA: "
            << int(remaining)
            << " sec ";

        displayProgressBar(progress);

        std::cout << std::flush;
    }

    file.close();

    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);

    std::cout << "\nDownload complete!\n";

    return true;
}

#else

#include <curl/curl.h>

void displayProgressBar(double progress)
{
    const int width = 40;

    std::cout << "[";

    int pos = width * progress;

    for (int i = 0; i < width; ++i)
    {
        if (i < pos)
            std::cout << "=";
        else if (i == pos)
            std::cout << ">";
        else
            std::cout << " ";
    }

    std::cout << "] "
              << int(progress * 100.0)
              << "%";
}

size_t writeData(
    void* ptr,
    size_t size,
    size_t nmemb,
    void* stream)
{
    std::ofstream* file =
        static_cast<std::ofstream*>(stream);

    file->write(
        static_cast<char*>(ptr),
        size * nmemb
    );

    return size * nmemb;
}

struct ProgressData
{
    std::chrono::steady_clock::time_point start;
};

int progressCallback(
    void* clientp,
    curl_off_t total,
    curl_off_t now,
    curl_off_t,
    curl_off_t)
{
    ProgressData* data =
        static_cast<ProgressData*>(clientp);

    double elapsed =
        std::chrono::duration<double>(
            std::chrono::steady_clock::now()
            - data->start
        ).count();

    double speed =
        elapsed > 0
            ? now / elapsed
            : 0;

    double remaining =
        speed > 0
            ? (total - now) / speed
            : 0;

    double progress =
        total > 0
            ? (double)now / total
            : 0;

    std::cout << "\r";

    std::cout
        << std::fixed
        << std::setprecision(2)
        << "Downloaded: "
        << now / (1024.0 * 1024.0)
        << " MB / "
        << total / (1024.0 * 1024.0)
        << " MB ";

    std::cout
        << "Speed: "
        << speed / 1024.0
        << " KB/s ";

    std::cout
        << "ETA: "
        << int(remaining)
        << " sec ";

    displayProgressBar(progress);

    std::cout << std::flush;

    return 0;
}

bool downloadFileWithProgress(
    const std::string& url,
    const std::string& localFile)
{
    CURL* curl = curl_easy_init();

    if (!curl)
    {
        std::cerr << "curl init failed\n";
        return false;
    }

    std::ofstream file(
        localFile,
        std::ios::binary
    );

    if (!file)
    {
        std::cerr << "Cannot create file\n";

        curl_easy_cleanup(curl);

        return false;
    }

    ProgressData data;
    data.start =
        std::chrono::steady_clock::now();

    curl_easy_setopt(
        curl,
        CURLOPT_URL,
        url.c_str());

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEFUNCTION,
        writeData);

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEDATA,
        &file);

    curl_easy_setopt(
        curl,
        CURLOPT_NOPROGRESS,
        0L);

    curl_easy_setopt(
        curl,
        CURLOPT_XFERINFOFUNCTION,
        progressCallback);

    curl_easy_setopt(
        curl,
        CURLOPT_XFERINFODATA,
        &data);

    curl_easy_setopt(
        curl,
        CURLOPT_FOLLOWLOCATION,
        1L);

    CURLcode res =
        curl_easy_perform(curl);

    file.close();

    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
    {
        std::cerr
            << "\nDownload failed: "
            << curl_easy_strerror(res)
            << "\n";

        return false;
    }

    std::cout << "\nDownload complete!\n";

    return true;
}

#endif
