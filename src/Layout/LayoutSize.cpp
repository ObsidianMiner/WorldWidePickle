/**
* @file Layout.cpp
* @brief Gets the min and max content size of any node
*/

#include "Browser/Tab.h"
#include "Browser/BrowserConsole.h"
#include "Util/StringUtils.h"
#include "Util/ImMath.h"
#include "UI/PickleGUI.h"
#include "Node.h"
#include "Layout.h"
#include <iostream>
#include <sstream>
#include "Util/Fonts.h"
#include "CSS/CSSBuilder.h"
#include "TableLayout.cpp"
#include "Util/Fonts.h"


/// <summary>
/// Expects that all parents have already called their <c>GetMinMaxSize</c>
/// </summary>
void Layout::GetMinMaxSize(Node& node, Website& website)
{
    node.layout.minContentSize = ImVec2(0, 0);
    GetMinMaxSizeSelf(node, website);
}

inline void GetMinMaxText(Node& node)
{
    float fontSize = GetFontSize(node, GetFontSize(node, defaultFontSize));
    const char* text = node.attributes.at("text").c_str();
    const char* textEnd = text + node.attributes.at("text").size();

    if (text == textEnd)
    {
        std::cerr << "[ERROR] INVALID TEXT IN WRAP NODE";
        return;
    }

    ImFont* font = GetNodeFont(node);
    if (font == nullptr)
    {
        std::cerr << "[ERROR] INVALID FONT IN WRAP NODE";
        return;
    }

    float longestWordSize = 0;
    std::istringstream wordStream(text);
    std::string word;
    while (wordStream >> word)
    {
        ImVec2 lineSize = font->CalcTextSizeA(fontSize, FLT_MAX, FLT_MAX, word.c_str());
        longestWordSize = max(longestWordSize, lineSize.x);
    }
    node.layout.minContentSize.x = longestWordSize;

    node.layout.minContentSize.y = font->CalcTextSizeA(fontSize, FLT_MAX, FLT_MAX, text).y;
    node.layout.maxContentSize.x = font->CalcTextSizeA(fontSize, FLT_MAX, FLT_MAX, text).x + 7.0f;
    node.layout.perferedSize.x = node.layout.maxContentSize.x;
}
void Layout::GetMinMaxSizeSelf(Node& node, Website& website)
{
    // node.layout.maxContentSize should never be this, but this is put there as a fallback in case it is not set
    node.layout.maxContentSize = ImVec2(20000, 20000);

    if (node.type == "#text")
    {
        GetMinMaxText(node);
    }
    if (node.type == "img" && node.HasAttribute("src"))
    {
        auto it = website.images.find(node.attributes["src"]);
        if (it != website.images.end())
        {
            PImage* image = &it->second;
            node.layout.minContentSize.x = (float)image->width;
            node.layout.minContentSize.y += (float)image->height;
            node.layout.maxContentSize = node.layout.minContentSize;
            node.layout.perferedSize = node.layout.minContentSize;
        }
    }
    if (node.type == "table")
    {
        GetTableMinMaxSize(node, website);
    }
}
void Layout::GetBlockedElementMinMaxSize(Node& node, Website& website)
{
    node.layout.minContentSize = ImVec2(0, 0);
    node.layout.perferedSize = ImVec2(0, 0);
    GetMinMaxSizeSelf(node, website);
    ImVec2 workingBlockMinSize = ImVec2(0, 0);
    ImVec2 workingBlockPerferedSize = ImVec2(0, 0);
    for (size_t i = 0; i < node.children.size(); i++)
    {
        Node& child = *node.children[i];

        bool blockDisplay = true;
        if (child.HasStyle("display") && std::get<string>(child.style["display"]) == "inline") blockDisplay = false;

        GetMinMaxSize(child, website);
        workingBlockMinSize.y += child.layout.minContentSize.y;
        workingBlockMinSize.x = std::fmax(workingBlockMinSize.x, child.layout.minContentSize.x);
        workingBlockPerferedSize.y += child.layout.perferedSize.y;
        workingBlockPerferedSize.x = std::fmax(workingBlockPerferedSize.x, child.layout.perferedSize.x);
        if (blockDisplay || i == node.children.size() - 1)
        {
            node.layout.minContentSize += workingBlockMinSize;
            node.layout.perferedSize += workingBlockPerferedSize;
            workingBlockPerferedSize = ImVec2(0, 0);
            workingBlockMinSize = ImVec2(0, 0);
        }
    }
}
