/**
* @file Layout.cpp
* @brief Everything nessasary for LayoutChildren
*/

#include "Browser/Tab.h"
#include "Util/StringUtils.h"
#include "Util/ImMath.h"
#include "UI/PickleGUI.h"
#include "Node.h"
#include "Layout.h"
#include <iostream>

bool Layout::debuggingLayout = false;

void Layout::SetMargins(Node& node)
{
    if (node.HasStyle("margin"))
    {
        float margin = node.GetStyleFloat("margin");
        node.layout.topLeftMargin = node.layout.bottomRightMargin = ImVec2(margin, margin);
    }

    node.layout.topLeftMargin.x = node.GetStyleFloat("margin-left", node.layout.topLeftMargin.x);
    node.layout.bottomRightMargin.x = node.GetStyleFloat("margin-right", node.layout.bottomRightMargin.x);
    node.layout.topLeftMargin.y = node.GetStyleFloat("margin-top", node.layout.topLeftMargin.y);
    node.layout.bottomRightMargin.y = node.GetStyleFloat("margin-bottom", node.layout.bottomRightMargin.y);
}
inline void ApplyAutoMargins(vector<Node*>& autoMarginedChildren, float currentX, float parentWidth)
{
    if (autoMarginedChildren.size() > 0)
    {
        if (currentX > parentWidth) return;
        float autoMarginAmount = (parentWidth - currentX) / (autoMarginedChildren.size() * 2); //*2 for left and right margins
        for (size_t i = 0; i < autoMarginedChildren.size(); i++)
        {
            autoMarginedChildren[i]->layout.bottomRightMargin.x = autoMarginAmount;
            autoMarginedChildren[i]->layout.topLeftMargin.x = autoMarginAmount;
        }
        autoMarginedChildren.clear();
    }
}
bool Layout::ShouldGiveAutoMargin(Node& node)
{
    return node.HasStyle("margin") && std::holds_alternative<string>(node.style["margin"]) && std::get<string>(node.style["margin"]) == "auto";
}
ImVec2 Layout::GetGlobalPos(const Node& node)
{
    if (node.parent != nullptr) return node.layout.localPos + GetGlobalPos(*node.parent);
    return node.layout.localPos;
}
/// <summary>
/// Sets the proper contentSize and new cursorPos from wrapping the text. Make sure to set node.lineStart first.
/// Will error if the node does not contain "text" attribute.
/// </summary>
/// <param name="parentWidth"></param>
/// <param name="currentX"></param>
/// <param name="currentY"></param>
void Layout::WrapNode(Node& node, float parentWidth, float& cursorX, float& cursorY)
{
    node.layout.contentSize = ImVec2(parentWidth, 0);
    const char* text = node.attributes.at("text").c_str();
    const char* textEnd = text + node.attributes.at("text").size();
    float fontSize = GetFontSize(node.style, defaultFontSize);
    float lineHeight = fontSize;
    float startX = cursorX;
    float startY = cursorY;

    if (text == textEnd)
    {
        std::cout << "[ERROR] INVALID TEXT IN WRAP NODE";
        return;
    }

    ImFont* font = PGUI::io->FontDefault;
    if (font == nullptr)
    {
        std::cout << "[ERROR] INVALID FONT IN WRAP NODE";
        return;
    }

    while (text < textEnd)
    {
        float remainingLineWidth = std::fmax(parentWidth - cursorX, 0);

        const char* lineEnd = font->CalcWordWrapPositionA(font->Scale, text, textEnd, remainingLineWidth);
        ImVec2 lineSize = font->CalcTextSizeA(fontSize, FLT_MAX, parentWidth, text, lineEnd); //Abort here for some reason
        node.layout.contentSize.x = std::fmax(node.layout.contentSize.x, lineSize.x);
        if (lineEnd != textEnd)
        {
            //Wrapping is happening
            cursorY += lineHeight;
            cursorX = 0;
        }
        else
        {
            cursorX += lineSize.x;
        }
        text = lineEnd;
    }
    //If wraped correctly set content size accordingly
    if (cursorY != startY)
    {
        node.layout.localPos.x = 0;
        node.layout.contentSize.y = lineHeight + (cursorY - startY);
    }
    else
    {
        node.layout.contentSize.x = (ImGui::CalcTextSize(node.attributes.at("text").c_str(), nullptr, false, parentWidth) * (fontSize / 20)).x;
        node.layout.contentSize.y = lineHeight;
    }
}
//First pass on laying out children
void Layout::CalculateChildrenAutoMargins(Node& node, float parentWidth, Website& website)
{
    float currentY = 0;
    float currentX = 0;
    float maxLineHeight = 0;
    bool wasBlock = false;
    bool textAlignCenter = false;

    if (node.HasStyle("text-align") && std::get<string>(node.style["text-align"]) == "center") textAlignCenter = true;

    vector<Node*> autoMarginedChildren;

    for (size_t i = 0; i < node.children.size(); i++)
    {
        Node& child = node.children[i];
        LayoutChildren(node, parentWidth, website);

        bool blockDisplay = true;
        if (child.HasStyle("display") && std::get<string>(child.style["display"]) == "inline") blockDisplay = false;

        bool shouldAutoMargin = ShouldGiveAutoMargin(child) || (child.type != "div" && textAlignCenter);
        if (blockDisplay)
        {
            wasBlock = true;
            if (shouldAutoMargin)
            {
                autoMarginedChildren.emplace_back(&node.children[i]);
            }
            if (maxLineHeight > 0)
            {
                currentY += maxLineHeight;
                maxLineHeight = 0;
            }
            currentX = child.layout.contentSize.x;
            ApplyAutoMargins(autoMarginedChildren, currentX, parentWidth);
            currentX = 0;
        }
        else
        {
            if (wasBlock)
            {
                currentX = 0;
                wasBlock = false;
            }
            if (shouldAutoMargin)
            {
                autoMarginedChildren.emplace_back(&node.children[i]);
            }
        }
    }
    ApplyAutoMargins(autoMarginedChildren, currentX, parentWidth);
}
/// <summary>
/// Gets the size of the element, not including that of the children. div's size is set to their parentsWidth.
/// Currently gets last say on the size of an object.
/// </summary>
/// <param name="node"></param>
/// <param name="parentWidth"></param>
/// <param name="website"></param>
void Layout::CalculateContentSize(Node& node, float parentWidth, Website& website)
{
    if (node.type == "div")
    {
        node.layout.contentSize.x = parentWidth;
    }
    if (node.type == "img" && node.HasAttribute("src"))
    {
        auto it = website.images.find(node.attributes["src"]);
        if (it != website.images.end())
        {
            PImage* image = &it->second;
            node.layout.contentSize.x = (float)image->width;
            node.layout.contentSize.y += (float)image->height;
        }
    }
    if (node.type == "#text" && node.HasAttribute("text"))
    {
        const char* text = node.attributes.at("text").c_str();
        float fontSize = GetFontSize(node.style, defaultFontSize);
        if (node.HasStyle("display") && std::get<string>(node.style["display"]) == "inline")
        {
            ImVec2 textSize = (ImGui::CalcTextSize(text, nullptr, false, parentWidth) * (fontSize / 20));
            node.layout.contentSize.y += textSize.y;
            node.layout.contentSize.x = textSize.x;
        }
        else
        {
            float currentY = node.layout.localPos.y;
            float currentX = node.layout.localPos.x;
            WrapNode(node, parentWidth, currentX, currentY);
        }
    }
    if (node.type == "title")
    {
        node.layout.contentSize = ImVec2(0, 0);
    }
}