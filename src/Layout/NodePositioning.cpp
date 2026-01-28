/**
* @file NodePositioning.cpp
* @brief Everything nessasary for Node::CalculateChildPositions
*/

#include "Browser/Tab.h"
#include "Util/StringUtils.h"
#include "Util/ImMath.h"
#include "UI/PickleGUI.h"
#include "Node.h"

/// <summary>
/// Should be used to calculate the position of this nodes children.
/// </summary>
/// <param name="parentWidth"></param>
/// <param name="website"></param>
void Node::CalculateChildPositions(float parentWidth, Website& website)
{
    contentSize = ImVec2(parentWidth, 0);
    if (HasStyle("padding"))
    {
        float paddingAmount = GetStyleFloat("padding");
        padding = ImVec2(paddingAmount, paddingAmount);
    }
    SetMargins();


    //Caculate relative positions of children (The main part)
    if (children.size() > 0)
    {
        CalculateChildrenAutoMargins(parentWidth, website);
        CalculateChildrenPositions(parentWidth, website);

		//TODO: Flexbox support
        /*
        if (HasStyle("display") && std::get<string>(style["display"]) == "flex") CalculateChildrenPositionsFlex(website);
        else
        {
        }*/
    }


    CaculateContentSize(parentWidth, website); //Overrides in applicable cases after the value set by CalculateChildrenPositions
}


void Node::SetMargins()
{
    if (HasStyle("margin"))
    {
        float margin = GetStyleFloat("margin");
        topLeftMargin = bottomRightMargin = ImVec2(margin, margin);
    }

    topLeftMargin.x = GetStyleFloat("margin-left", topLeftMargin.x);
    bottomRightMargin.x = GetStyleFloat("margin-right", bottomRightMargin.x);
    topLeftMargin.y = GetStyleFloat("margin-top", topLeftMargin.y);
    bottomRightMargin.y = GetStyleFloat("margin-bottom", bottomRightMargin.y);
}
inline void ApplyAutoMargins(vector<Node*>& autoMarginedChildren, float currentX, float parentWidth)
{
    if (autoMarginedChildren.size() > 0)
    {
        if (currentX > parentWidth) return;
        float autoMarginAmount = (parentWidth - currentX) / (autoMarginedChildren.size() * 2); //*2 for left and right margins
        for (size_t i = 0; i < autoMarginedChildren.size(); i++)
        {
            autoMarginedChildren[i]->bottomRightMargin.x = autoMarginAmount;
            autoMarginedChildren[i]->topLeftMargin.x = autoMarginAmount;
        }
        autoMarginedChildren.clear();
    }
}
void Node::CalculateChildrenPositions(float parentWidth, Website& website)
{
    float currentY = 0;
    float currentX = 0;
    float highestX = 0;
    float maxLineHeight = 0;
    bool wasBlock = false;

    for (size_t i = 0; i < children.size(); i++)
    {

        bool blockDisplay = true;
        if (children[i].HasStyle("display") && std::get<string>(children[i].style["display"]) == "inline") blockDisplay = false;

        if (blockDisplay)
        {
            wasBlock = true;
            if (maxLineHeight > 0)
            {
                currentY += maxLineHeight;
                maxLineHeight = 0;
            }

            children[i].localPos = ImVec2(children[i].topLeftMargin.x + children[i].border.x + children[i].padding.x, currentY);
            children[i].localPos.y = currentY;
            currentX = children[i].contentSize.x + children[i].topLeftMargin.x + children[i].bottomRightMargin.x;
            currentY = children[i].localPos.y + children[i].contentSize.y + children[i].padding.y + children[i].bottomRightMargin.y + children[i].topLeftMargin.y;
        }
        else
        {
            if (wasBlock)
            {
                highestX = std::max(highestX, currentX);
                currentX = 0;
                wasBlock = false;
            }
            if (children[i].type == "#text")
            {
                currentX += children[i].topLeftMargin.x + children[i].border.x;
                children[i].posInLine = currentX;
                children[i].localPos = ImVec2(currentX, currentY);
                children[i].CalculateChildPositions(parentWidth, website);
                children[i].WrapSelf(parentWidth, currentX, currentY);
                currentX += children[i].bottomRightMargin.x + children[i].border.x;
                maxLineHeight = std::max(maxLineHeight, children[i].contentSize.y);
            }
            else
            {
                children[i].CalculateChildPositions(parentWidth, website);
                children[i].localPos = ImVec2(currentX + children[i].topLeftMargin.x + children[i].border.x + children[i].padding.x, currentY);
                currentX += children[i].contentSize.x + children[i].topLeftMargin.x + children[i].bottomRightMargin.x;
                maxLineHeight = std::max(maxLineHeight, children[i].contentSize.y);
            }
        }
    }
    highestX = std::max(highestX, currentX);
    currentY += maxLineHeight;

    //Set our current size now that we have figured out how big our display of our children is.
    if (HasStyle("display") && std::get<string>(style["display"]) == "inline")
    {
        contentSize.x = highestX;
    }
    if(children.size() > 0) contentSize.y = currentY;
    if (HasAutoMargin() && children.size() > 0)
    {
        contentSize.x = highestX;
    }
}
void Node::CalculateChildrenPositionsFlex(Website& website)
{
    for (size_t i = 0; i < children.size(); i++)
    {

    }
}
/// <summary>
/// Sets the proper contentSize and new cursorPos from wrapping the text. Make sure to set node.lineStart first.
/// </summary>
/// <param name="parentWidth"></param>
/// <param name="currentX"></param>
/// <param name="currentY"></param>
void Node::WrapSelf(float parentWidth, float& currentX, float& currentY)
{
    contentSize = ImVec2(parentWidth, 0);
    const char* text = attributes.at("text").c_str();
    const char* textEnd = text + attributes.at("text").size();
    float fontSize = GetFontSize(style);
    float lineHeight = fontSize;
    float startY = currentY;
    while (text < textEnd)
    {
        ImFont* font = PGUI::io->FontDefault;
        const char* lineEnd = font->CalcWordWrapPositionA(font->Scale, text, textEnd, parentWidth - currentX);
        ImVec2 lineSize = font->CalcTextSizeA(fontSize, FLT_MAX, parentWidth, text, lineEnd);
        contentSize.x = std::fmax(contentSize.x, lineSize.x);
        if (lineEnd != textEnd)
        {
            //Wrapping is happening
            currentY += lineHeight;
            currentX = 0;
        }
        else
        {
            currentX = lineSize.x;
        }
        text = lineEnd;
    }
    //If wraped correctly set content size accordingly
    if (currentY != startY)
    {
        localPos.x = 0;
        contentSize.x = parentWidth;
        contentSize.y = lineHeight + (currentY - startY);
    }
    else
    {
        contentSize.y = lineHeight;
    }
}
//First pass on laying out children
void Node::CalculateChildrenAutoMargins(float parentWidth, Website& website)
{

    float currentY = 0;
    float currentX = 0;
    float maxLineHeight = 0;
    bool wasBlock = false;
    bool textAlignCenter = false;

    if (HasStyle("text-align") && std::get<string>(style["text-align"]) == "center") textAlignCenter = true;

    vector<Node*> autoMarginedChildren;

    for (size_t i = 0; i < children.size(); i++)
    {
        children[i].CalculateChildPositions(parentWidth, website);

        bool blockDisplay = true;
        if (children[i].HasStyle("display") && std::get<string>(children[i].style["display"]) == "inline") blockDisplay = false;

        if (blockDisplay)
        {
            wasBlock = true;
            if (children[i].HasAutoMargin() || (children[i].type != "div" && textAlignCenter))
            {
                autoMarginedChildren.emplace_back(&children[i]);
            }
            if (maxLineHeight > 0)
            {
                currentY += maxLineHeight;
                maxLineHeight = 0;
            }
            currentX = children[i].contentSize.x;
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
            if (children[i].HasAutoMargin() || (children[i].type != "div" && textAlignCenter))
            {
                autoMarginedChildren.emplace_back(&children[i]);
            }
        }
    }
    ApplyAutoMargins(autoMarginedChildren, currentX, parentWidth);
}
void Node::CaculateContentSize(float parentWidth, Website& website)
{
    if (type == "div")
    {
        contentSize.x = parentWidth;
    }
    if (type == "img" && attributes.find("src") != attributes.end())
    {
        auto it = website.images.find(attributes["src"]);
        if (it != website.images.end())
        {
            PImage* image = &it->second;
            contentSize.x = (float)image->width;
            contentSize.y += (float)image->height;
        }
    }
    if (type == "#text" && attributes.find("text") != attributes.end())
    {
        const char* text = attributes.at("text").c_str();
        float fontSize = GetFontSize(style);
        if (HasStyle("display") && std::get<string>(style["display"]) == "inline")
        {
            ImVec2 textSize = (ImGui::CalcTextSize(text, nullptr, false, parentWidth) * (fontSize / 20));
            contentSize.y += textSize.y;
            contentSize.x = textSize.x;
        }
        else
        {
            float currentY = localPos.y;
            float currentX = localPos.x;
            WrapSelf(parentWidth, currentX, currentY);
        }
    }
    if (type == "title")
    {
        contentSize = ImVec2(0, 0);
    }
}