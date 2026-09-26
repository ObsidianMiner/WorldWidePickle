/**
* @file FullLayoutPass.cpp
* @brief Determines the layout of children
*/

#include "Layout.h"
#include "Util/StringUtils.h"
#include "Browser/BrowserConsole.h"
#include "Util/Fonts.h"
using namespace std;

inline void MoveCursorToNextLine(float& currentLineHeight, float& cursorY)
{
    float lineHeight = currentLineHeight == 0 ? defaultFontSize : currentLineHeight;

    cursorY += lineHeight;
    currentLineHeight = 0;
}
/// <summary>
/// Helper to check if a node has inline display
/// </summary>
inline bool IsInlineDisplay(Node& node)
{
    return node.HasStyle("display") &&
        std::get<std::string>(node.style["display"]) == "inline";
}
/// <summary>
/// Recursively look up the tree to determine if the last rendered element has a space after it, or it has a space before it.
/// </summary>
inline bool ShouldRenderSpaceBefore(Node& childNode, int childIndex)
{
    // Check if this node explicitly requests a space before
    
    // Walk up the tree to find the previously rendered element
    Node* currentNode = &childNode;
    int currentIndex = childIndex;
    
    while (currentNode != nullptr) {
        Node* parent = currentNode->parent;
        if (parent == nullptr) {
            break; // Reached root
        }
        
        // If we're not the first child, check the previous sibling
        if (currentIndex > 0)
        {
            Node& previousSibling = *(parent->children[currentIndex - 1]);

            //If we pass by a block, it must mean there is a new line inbetween them, and a space should not be inserted
            if (!IsInlineDisplay(previousSibling)) return false;
            
            // Check if previous sibling has spaceAfter
            if ((previousSibling.HasAttribute("sa") || childNode.HasAttribute("sb"))) {
                return true;
            }

            
            // Find the last descendant of the previous sibling
            Node* lastDescendant = &previousSibling;
            while (!lastDescendant->children.empty()) {
                lastDescendant = lastDescendant->children.back().get();
                //If we pass by a block, it must mean there is a new line inbetween them, and a space should not be inserted
                if (!IsInlineDisplay(*lastDescendant)) return false;

                if ((lastDescendant->HasAttribute("sa") || childNode.HasAttribute("sb"))) {
                    return true;
                }
            }
            
            // We found the previously rendered element and it has no spaceAfter
            return false;
        }
        if (!IsInlineDisplay(*parent)) return false;
        
        // We're the first child, so move up to parent
        currentIndex = parent->GetIndexInParent(); // You'll need this method
        currentNode = parent;
    }
    
    return false;
}


/// <summary>
/// Sets the layout of its children, determening their size and position, and then setting it's own size.
/// </summary>
/// <param name="node"></param>
/// <param name="parentWidth"></param>
/// <param name="website"></param>
void Layout::FullLayoutPass(Node& node, float parentWidth, Website& website)
{
    //All nodes use this

    GetMinMaxSize(node, website);

    node.layout.contentSize = ImVec2(parentWidth, 0);
    if (node.HasStyle("padding"))
    {
        float paddingAmount = node.GetStyleFloat("padding");
        node.layout.padding = ImVec2(paddingAmount, paddingAmount);
    }
    SetMargins(node);

    //Exceptions

    if (node.type == "table")
    {
        LayoutTable(node, parentWidth, website);
        return;
    }

    //Normal FullLayoutPass

    float cursorY = 0;
    float cursorX = 0;
    float farthestRight = 0;
    float currentLineHeight = 0;

    for (size_t i = 0; i < node.children.size(); i++)
    {
        Node& child = *node.children[i];

        // If this child starts a new block.
        bool blockDisplay = true;
        if (node.children[i]->HasStyle("display") && std::get<string>(child.style["display"]) == "inline") blockDisplay = false;

        if (node.type == "br")
        {
            MoveCursorToNextLine(currentLineHeight, cursorY);
        }
        else if (blockDisplay)
        {
            if(currentLineHeight > 0) MoveCursorToNextLine(currentLineHeight, cursorY);

            //Determines the childs size, as well as sets all of the sizes and positions of its children.
            FullLayoutPass(child, parentWidth, website);

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
            // Non block display
            if (child.type == "#text")
            {
                cursorX += child.layout.topLeftMargin.x + child.layout.border.x;

                if (ShouldRenderSpaceBefore(child, i))
                {
                    child.attributes.emplace(std::pair<std::string, std::string>{ "spR", "1"});
                    cursorX += 5;
                }
                child.layout.posInLine = cursorX;
                child.layout.localPos = ImVec2(cursorX, cursorY);

                
                ImFont* font = GetNodeFont(child);
                WrapNode(child, parentWidth, cursorX, cursorY);

                cursorX += child.layout.bottomRightMargin.x + child.layout.border.x;
                currentLineHeight = std::max(currentLineHeight, child.layout.contentSize.y);
            }
            else
            {
                FullLayoutPass(child, parentWidth, website);
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
    CalculateSelfSize(node, parentWidth, website);
}