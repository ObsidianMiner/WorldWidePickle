#include "BrowserConfig.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include "Browser/BrowserConsole.h"
#include "imgui/imgui.h"
#include <UI/PickleGUI.h>
#include "UI/WWPStyles.h"

using json = nlohmann::json;

BrowserConfig::BrowserConfig()
{
	setupDocking = false;
}

void BrowserConfig::RenderConfig()
{
	char name[256];
	std::snprintf(name, sizeof(name), "Browser Config###idnstuff");
	bool isVisible = ImGui::Begin(name);

	if (!setupDocking) SetupDocking(name);

	json oldVersion = configDoc;

	ImGui::Text("World Wide Pickle Settings");
	for (auto& [key, value] : configDoc.items())
	{
		RenderKVP(key, value);
	}
	ImGui::End();

	if (oldVersion != configDoc) ApplyChanges();
}

void BrowserConfig::SetupDocking(const char* windowName)
{
	setupDocking = true;
	PGUI::DockWindowToTabBar(windowName);
}
void BrowserConfig::LoadConfigFromWorkingDir()
{
	if (std::filesystem::exists("config.json"))
	{
		std::ifstream inputFile("config.json");
		try
		{
			configDoc = json{ json::parse(inputFile) };
		}
		catch(json::parse_error e)
		{
			BrowserConsole::GetInstance().Error("Config (nlohmann JSON)", e.what());
		}
		ApplyChanges();
	}
	else
	{
		BrowserConsole::GetInstance().Error("Config", std::filesystem::current_path().string() + "/config.json");
	}
}

void BrowserConfig::SaveConfigToWorkingDir()
{
	std::ofstream saveFile("config.json");
	saveFile << configDoc;
}

void BrowserConfig::ApplyChanges()
{
	try
	{
		BrowserConsole::GetInstance().SetVerbose(configDoc["verbose"]);

		if(!PGUI::firstFrame) ApplyStyle(configDoc["theme"].get<int>());
	}
	catch (std::runtime_error e)
	{
		BrowserConsole::GetInstance().Error("Config", std::string("Error applying changes\n") + e.what());
	}
}

void BrowserConfig::RenderKVP(const std::string& key, nlohmann::json& value)
{
#define imGuiKey ("###" + key + "Config").c_str()
#define imGuiLabelKey (key).c_str()
	if (key == "theme")
	{
		static int currentItem = value.get<int>();
		ImGui::Text(imGuiLabelKey);
		ImGui::SameLine();
		if (ImGui::Combo(imGuiKey, &currentItem, browserStyles, IM_ARRAYSIZE(browserStyles)))
		{
			value = currentItem;
		}
	}
	else if (value.is_string())
	{
		std::string strValue = value.get<std::string>();
		ImGui::Text(imGuiLabelKey);
		ImGui::SameLine();
		static char buffer[512] = "";
		strncpy_s(buffer, strValue.c_str(), sizeof(strValue) - 1);
		ImGui::InputText(imGuiKey, buffer, sizeof(buffer));
		value = std::string(buffer);
	}
	else if (value.is_boolean())
	{
		bool outValue = value.get<bool>();
		ImGui::Text(imGuiLabelKey);
		ImGui::SameLine();
		ImGui::Checkbox(imGuiKey, &outValue);
		value = outValue;
	}
	else if (value.is_number_float())
	{
		float outValue = value.get<float>();
		ImGui::Text(imGuiLabelKey);
		ImGui::SameLine();
		ImGui::InputFloat(imGuiKey, &outValue);
		value = outValue;
	}
	else if (value.is_number_integer())
	{
		int outValue = value.get<int>();
		ImGui::Text(imGuiLabelKey);
		ImGui::SameLine();
		ImGui::InputInt(imGuiKey, &outValue);
		value = outValue;
	}
#undef imGuiKey, imGuiLabelKey
}
