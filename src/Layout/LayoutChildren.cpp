/**
* @file LayoutChildren.cpp
* @brief Determines the layout of children
*/

#include "Layout.h"
#include "Util/StringUtils.h"
using namespace std;

/// <summary>
/// Should be used on the root nodes.
/// </summary>
/// <param name="parentWidth"></param>
/// <param name="website"></param>
/*
void Layout::LayoutDOM(Node& root, float parentWidth, Website& website)
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
        
        if (HasStyle("display") && std::get<string>(style["display"]) == "flex") CalculateChildrenPositionsFlex(website);
        else
        {
        }
    }


    CaculateContentSize(parentWidth, website); //Overrides in applicable cases after the value set by CalculateChildrenPositions
}*/

inline void MoveCursorToNextLine(float& currentLineHeight, float& cursorY)
{
    float lineHeight = currentLineHeight == 0 ? defaultFontSize : currentLineHeight;

    cursorY += lineHeight;
    currentLineHeight = 0;
}
/// <summary>
/// Sets the layout of its children, determening their size and position, and then setting it's own size.
/// </summary>
/// <param name="node"></param>
/// <param name="parentWidth"></param>
/// <param name="website"></param>
void Layout::LayoutChildren(Node& node, float parentWidth, Website& website)
{
    node.layout.contentSize = ImVec2(parentWidth, 0);
    if (node.HasStyle("padding"))
    {
        float paddingAmount = node.GetStyleFloat("padding");
        node.layout.padding = ImVec2(paddingAmount, paddingAmount);
    }
    SetMargins(node);

    float cursorY = 0;
    float cursorX = 0;
    float farthestRight = 0;
    float currentLineHeight = 0;

    for (size_t i = 0; i < node.children.size(); i++)
    {
        Node& child = node.children[i];

        bool blockDisplay = true;
        if (node.children[i].HasStyle("display") && std::get<string>(child.style["display"]) == "inline") blockDisplay = false;

        if (node.type == "br")
        {
            MoveCursorToNextLine(currentLineHeight, cursorY);
        }
        else if (blockDisplay)
        {
            if(currentLineHeight > 0) MoveCursorToNextLine(currentLineHeight, cursorY);

            //Determines the childs size, as well as sets all of the sizes and positions of its children.
            LayoutChildren(child, parentWidth, website);

            float blockWidth = child.layout.contentSize.x + child.layout.topLeftMargin.x + child.layout.bottomRightMargin.x;
            farthestRight = std::max(blockWidth, farthestRight);

            //Set Child LocalPos
            child.layout.localPos = ImVec2(child.layout.topLeftMargin.x + child.layout.border.x + child.layout.padding.x, cursorY);
            //Move the cursor to the next line
            cursorX = 0;
            cursorY = child.layout.localPos.y + child.layout.contentSize.y + child.layout.padding.y + child.layout.bottomRightMargin.y + child.layout.topLeftMargin.y;
        }
        else
        {
            if (child.type == "#text")
            {
                cursorX += child.layout.topLeftMargin.x + child.layout.border.x;
                child.layout.posInLine = cursorX;
                child.layout.localPos = ImVec2(cursorX, cursorY);

                //Text cannot have children, so this does not exactly make sense
                //LayoutChildren(child, parentWidth, website);
                //Instead
                WrapNode(child, parentWidth, cursorX, cursorY);
                cursorX += child.layout.bottomRightMargin.x + child.layout.border.x;
                currentLineHeight = std::max(currentLineHeight, child.layout.contentSize.y);
            }
            else
            {
                LayoutChildren(child, parentWidth, website);
                child.layout.localPos = ImVec2(cursorX + child.layout.topLeftMargin.x + child.layout.border.x + child.layout.padding.x, cursorY);
                cursorX += child.layout.contentSize.x + child.layout.topLeftMargin.x + child.layout.bottomRightMargin.x;
                currentLineHeight = std::max(currentLineHeight, child.layout.contentSize.y);
            }
        }
    }
    farthestRight = std::max(farthestRight, cursorX);
    cursorY += currentLineHeight;

    //Set our current size now that we have figured out how big our display of our children is.
    if (node.HasStyle("display") && std::get<string>(node.style["display"]) == "inline")
    {
        node.layout.contentSize.x = farthestRight;
    }
    if (node.children.size() > 0) node.layout.contentSize.y = cursorY;
    if (ShouldGiveAutoMargin(node) && node.children.size() > 0)
    {
        node.layout.contentSize.x = farthestRight;
    }
    CalculateContentSize(node, parentWidth, website);
}