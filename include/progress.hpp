#ifndef PROGRESS_HPP
#define PROGRESS_HPP

#include <string>

bool downloadFileWithProgress(
    const std::string& url,
    const std::string& localFile
);

#endif
