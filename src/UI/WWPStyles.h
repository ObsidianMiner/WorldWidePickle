#pragma once
#include "UI/PickleGUI.h"

inline const char* browserStyles[]{ "Purple","Cherry","Light","DarkBlue","Everfrost","ClasicSteam" };
void ApplyStyle(int theme);
ImGuiStyle SetupEverfrostStyle();
ImGuiStyle SetupClasicSteamStyle();
ImGuiStyle SetupCherryStyle();
ImGuiStyle SetupDarkBlueStyle();
ImGuiStyle SetupLightStyle();
ImGuiStyle SetPurpleStyle();