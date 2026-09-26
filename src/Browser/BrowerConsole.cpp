/**
* @file BrowserConsole.cpp
* @brief An extra layer ontop of the normal console for color, and multi-threaded capabilities.
*/


#include "BrowserConsole.h"
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#include <mutex>


void SetColorHelper(int textColor, int bgColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,
        (bgColor << 4) | textColor);
}
inline std::string SetColor(std::string_view source)
{
    if (source == "Browser") SetColorHelper(11, 0);
    else if (source == "Layout") SetColorHelper(6, 0);
    else if (source == "CSS") SetColorHelper(9, 0);
    else if (source == "Rendering") SetColorHelper(10, 0);
    else if (source == "Error") SetColorHelper(12, 0);
    else if (source == "DOM Builder") SetColorHelper(6, 0);
    else if (source == "Fetch") SetColorHelper(8, 0);
    return "";
}
inline void ResetColor()
{
    SetColorHelper(7, 0);
}
#else

inline std::string GetColor(std::string_view source)
{
    if (source == "Browser") color = R"(\033["36m)";
    if (source == "Layout") color = R"(\033["33m)";
    if (source == "DOM Builder") color = R"(\033["33m)";
    if (source == "CSS") color = R"(\033["96m)";
    if (source == "Rendering") color = R"(\033["92m)";
    if (source == "Fetch") color = R"(\033["92m)";
    if (source == "Error") color = R"(\033["31m)";
    return color;
}
inline void ResetColor()
{

}
#endif

void BrowserConsole::BaseLog(const std::string_view source, const std::string_view log, const std::string_view color, bool verboseOnly)
{
    if (!m_verboseMode && verboseOnly) return;

    std::string formatted = "[" + std::string(source) + "] " + std::string(log);

    std::lock_guard<std::mutex> lock(m_logMutex);
    std::string colorPrefix = SetColor(color);
    std::cout << colorPrefix << formatted << std::endl;
    ResetColor();
}
void BrowserConsole::Error(const std::string_view source, const std::string_view error, bool verboseOnly)
{
    BaseLog(source, error, error, verboseOnly);
}
void BrowserConsole::Log(const std::string_view source, const std::string_view log, bool verboseOnly)
{
    BaseLog(source, log, source, verboseOnly);
}