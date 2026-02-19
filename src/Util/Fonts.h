#pragma once
#include "UI/PickleGUI.h"
#include "Node.h"

inline ImFont* GetNodeFont(Node& node)
{
    if (!node.HasStyle("font-style")) return PGUI::defaultFont.regular;
    else if (std::get<std::string>(node.style["font-style"]) == "bold")
    {
        if (std::get<std::string>(node.style["font-style"]) == "italic") return PGUI::defaultFont.boldItalic;
        else return PGUI::defaultFont.bold;
    }
    else if (std::get<std::string>(node.style["font-style"]) == "italic") return PGUI::defaultFont.italic;
}
inline float GetFontSize(Node& node, float defaultFontSize)
{
	float fS = defaultFontSize;
	if (node.HasStyle("font-size"))
	{
		if (std::holds_alternative<int>(node.style["font-size"]))fS = std::get<int>(node.style["font-size"]);
		else if (std::holds_alternative<float>(node.style["font-size"])) fS = std::get<float>(node.style["font-size"]);
	}
	return fS;
}
const static float defaultFontSize = 20.0f;