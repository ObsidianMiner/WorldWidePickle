#pragma once

#include <string>
#include <mutex>


class BrowserConsole
{
	BrowserConsole() {}
	bool m_verboseMode = true;
	mutable std::mutex m_logMutex;
	void BaseLog(const std::string_view source, const std::string_view log, const std::string_view color, bool verboseOnly = false);
public:
	BrowserConsole(const BrowserConsole&) = delete;
	BrowserConsole& operator=(const BrowserConsole&) = delete;
	BrowserConsole(const BrowserConsole&&) = delete;
	BrowserConsole& operator=(const BrowserConsole&&) = delete;

	static BrowserConsole& GetInstance()
	{
		static BrowserConsole instance;
		return instance;
	}

	void Error(const std::string_view source, const std::string_view error, bool verboseOnly = false);
	void Log(const std::string_view source, const std::string_view log, bool verboseOnly = false);
};