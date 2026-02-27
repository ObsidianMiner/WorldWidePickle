/**
* @file Node.cpp
* @brief Basic node functions
*/

#include "HTML/HTMLBuilder.h"
#include <regex>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include "Node.h"
#include "Util/ImMath.h"
#include <Layout/Layout.h>
#include <Browser/BrowserConsole.h>

using namespace std;

#define DebugMargins
#define DebugPos

bool Node::HasAttribute(const std::string attribute) const
{
    return attributes.find(attribute) != attributes.end();
}
bool Node::HasStyle(const std::string s) const
{
    return style.find(s) != style.end();
}

float Node::GetStyleFloat(const std::string& key, float defaultValue) const
{
    if (!HasStyle(key)) return defaultValue;

    const auto& val = style.at(key);
    if (std::holds_alternative<int>(val)) return static_cast<float>(std::get<int>(val));
    if (std::holds_alternative<float>(val)) return std::get<float>(val);
    return defaultValue;
}



/// <summary>
/// Gets all of the node's parents from bottom (most directly) to top parent.
/// </summary>
/// <returns></returns>
std::vector<Node*> Node::GetParents()
{
    std::vector<Node*> parents;
    Node* currentParent = parent;
    while (currentParent != nullptr)
    {
        parents.push_back(currentParent);
        currentParent = currentParent->parent;
    }
    return parents;
}
int Node::GetIndexInParent() const
{
    auto it = std::find_if(parent->children.begin(), parent->children.end(),
        [&](const Node& elem) {
            return elem.id == id;
        });

    if (it != parent->children.end()) {
        return std::ranges::distance(parent->children.begin(), it);
    }
    BrowserConsole::Error("Layout", "Could not find index in parent, parent is likely invalid");
    return -1;
}
string AttributesToString(const unordered_map<string, string> attributes)
{
    //if (attributes.size() == 0) return "No Attributes";
    string result;
    for(const pair<string, string>& attribute : attributes)
    {
        string value = attribute.second;
        if (attribute.second.size() > 40) value = attribute.second.substr(0, 37) + "...";
        result += attribute.first + "=" + value + " ";
    }
    return result;
}
string PrintNode(Node* node, int indent)
{
    string output = std::string(indent, '\t') + node->type + ' ' + AttributesToString(node->attributes);
#ifdef DebugPos
    string globalPos = " | GPos: " + to_string(int(Layout::GetGlobalPos(*node).x)) + ' ' + to_string(int(Layout::GetGlobalPos(*node).y));
    string localPos = "         | LPos: " + to_string(int(node->layout.localPos.x)) + ' ' + to_string(int(node->layout.localPos.y));
    output += localPos + globalPos;
#endif
#ifdef DebugMargins
    string margins = " | Margins L: " + to_string(int(node->layout.topLeftMargin.x)) + " R: " + to_string(int(node->layout.bottomRightMargin.x))
        + " T: " + to_string(int(node->layout.topLeftMargin.y)) + " B: " + to_string(int(node->layout.bottomRightMargin.y));
    output += margins;
#endif
    string contentSize = " | CS: " + to_string(int(node->layout.contentSize.x)) + ' ' + to_string(int(node->layout.contentSize.y));
    output += contentSize + "\n";
    for (size_t i = 0; i < node->children.size(); i++)
    {
        output += PrintNode(&node->children[i], indent + 1);
    }
    return output;
}

std::string PrintNodeAttributes(Node* node)
{
    std::string result;
    result.reserve(node->attributes.size() * 5);
    for (const auto& attribute : node->attributes)
    {
        result += attribute.first + "=\"" + attribute.second + "\"";
    }
    return result;
}
Node* Node::TryGetChildOfType(std::string type)
{
    for (size_t i = 0; i < children.size(); i++)
    {
        if (children[i].type == type) return &children[i];
    }
    return nullptr;
}
bool operator==(const Node& node1, const Node& node2)
{
    return node1.type == node2.type && node1.attributes == node2.attributes && node1.children.size() == node2.children.size();
}