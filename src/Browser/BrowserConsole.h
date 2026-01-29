#pragma once

#include <string>

class BrowserConsole
{
public:
	static void Error(const std::string& source, const std::string& error);
	static void Log(const std::string& source, const std::string& log);
};