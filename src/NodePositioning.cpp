#include "Tab.h"
#include "StringUtils.h"
#include "ImMath.h"


void Node::CalculatePosition(float parentWidth, Website& website)
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
        if (HasStyle("display") && std::get<string>(style["display"]) == "flex") CalculateChildrenPositionsFlex(website);
        else
        {
            CalculateChildrenAutoMargins(parentWidth, website);
            CalculateChildrenPositions(parentWidth, website);
        }
    }


    CaculateContentSize(parentWidth, website);
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
                currentX = 0;
                wasBlock = false;
            }
            children[i].CalculatePosition(0, website);
            children[i].localPos = ImVec2(currentX + children[i].topLeftMargin.x + children[i].border.x + children[i].padding.x, currentY);
            currentX += children[i].contentSize.x + children[i].topLeftMargin.x + children[i].bottomRightMargin.x;
            maxLineHeight = std::max(maxLineHeight, children[i].contentSize.y);
        }
    }
    currentY += maxLineHeight;

    //Set our current size now that we have figured out how big our display of our children is.
    if (HasStyle("display") && std::get<string>(style["display"]) == "inline")
    {
        if (contentSize.x == 0) contentSize.x = currentX;
    }
    contentSize.y = currentY;
    if (HasAutoMargin())
    {
        contentSize.x = currentX;
    }
}
void Node::CalculateChildrenPositionsFlex(Website& website)
{
    
    for (size_t i = 0; i < children.size(); i++)
    {

    }
}
void Node::CalculateChildrenAutoMargins(float parentWidth, Website& website)
{

    float currentY = 0;
    float currentX = 0;
    float maxLineHeight = 0;
    bool wasBlock = false;
    vector<Node*> autoMarginedChildren;

    for (size_t i = 0; i < children.size(); i++)
    {
        children[i].CalculatePosition(parentWidth, website);

        bool blockDisplay = true;
        if (children[i].HasStyle("display") && std::get<string>(children[i].style["display"]) == "inline") blockDisplay = false;

        if (blockDisplay)
        {
            wasBlock = true;
            if (children[i].HasAutoMargin())
            {
                autoMarginedChildren.emplace_back(&children[i]);
            }
            if (maxLineHeight > 0)
            {
                currentY += maxLineHeight;
                maxLineHeight = 0;
            }
            ApplyAutoMargins(autoMarginedChildren, currentX, parentWidth);
        }
        else
        {
            if (wasBlock)
            {
                currentX = 0;
                wasBlock = false;
            }
            if (children[i].HasAutoMargin())
            {
                autoMarginedChildren.emplace_back(&children[i]);
            }
            children[i].CalculatePosition(0, website);
        }
    }
    ApplyAutoMargins(autoMarginedChildren, currentX, parentWidth);
}
void Node::CaculateContentSize(float parentWidth, Website& website)
{
    if (type == "img" && attributes.find("src") != attributes.end())
    {
        auto it = website.images.find(attributes["src"]);
        if (it != website.images.end())
        {
            PImage* image = &it->second;
            contentSize.y += ImVec2((float)image->width, (float)image->height).y;
        }
    }
    if (type == "#text" && attributes.find("text") != attributes.end())
    {
        const char* text = attributes.at("text").c_str();
        float fontSize = GetFontSize(style);
        ImVec2 textSize = (ImGui::CalcTextSize(text) * (fontSize / 20));
        contentSize.y += textSize.y;
        contentSize.x = textSize.x;
    }
    if (type == "header" || type == "title")
    {
        contentSize = ImVec2(0, 0);
    }
}