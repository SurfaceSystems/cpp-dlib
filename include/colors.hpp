#include <iostream>

#pragma once

// ASCII colors 
// Define ASCII escape sequences for text modifications
const std::string ASCII_BOLD = "\x1b[1m";      // Bold text
const std::string ASCII_UNDERLINE = "\x1b[4m"; // Underlined text
const std::string ASCII_REVERSE = "\x1b[7m";   // Reverse (swap foreground and background colors)

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
const std::string ASCII_DIM = "\x1b[2m";    // Dim text
const std::string ASCII_BLINK = "\x1b[5m";  // Blinking text
const std::string ASCII_HIDDEN = "\x1b[8m"; // Hidden text
 
// Background colors (optional)
const std::string ASCII_BG_BLACK = "\x1b[40m";
const std::string ASCII_BG_RED = "\x1b[41m";
const std::string ASCII_BG_GREEN = "\x1b[42m";
const std::string ASCII_BG_YELLOW = "\x1b[43m";
const std::string ASCII_BG_BLUE = "\x1b[44m";
const std::string ASCII_BG_MAGENTA = "\x1b[45m";
const std::string ASCII_BG_CYAN = "\x1b[46m";
const std::string ASCII_BG_WHITE = "\x1b[47m";
const std::string ASCII_RESET = "\x1b[0m"; // Reset text to default

// Define bright color variables
const std::string BRIGHT_RED = "\033[91m";
const std::string BRIGHT_GREEN = "\033[92m";
const std::string BRIGHT_YELLOW = "\033[93m";
const std::string BRIGHT_BLUE = "\033[94m";
const std::string BRIGHT_MAGENTA = "\033[95m";
const std::string BRIGHT_CYAN = "\033[96m";
const std::string BRIGHT_WHITE = "\033[97m";
const std::string RESET_COLOR = "\033[0m";

