#pragma once
#include <string>
#include "nlohmann/json.hpp"

class BrowserConfig
{
public:
	nlohmann::json configDoc;

	BrowserConfig();

	void RenderConfig();
	void LoadConfigFromWorkingDir();
	void SaveConfigToWorkingDir();
	void ApplyChanges();
private:

	bool setupDocking;

	void SetupDocking(const char* windowName);
	void RenderKVP(const std::string& key, nlohmann::json& value);
};