/**
* @file DOMBuilder.cpp
* @brief Basic node functions
*/

#include "DOMBuilder.h"
#include <regex>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include "Node.h"
#include "Util/ImMath.h"

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
bool Node::HasAutoMargin()
{
    return HasStyle("margin") && std::holds_alternative<string>(style["margin"]) && std::get<string>(style["margin"]) == "auto";
}

float Node::GetStyleFloat(const std::string& key, float defaultValue) const
{
    if (!HasStyle(key)) return defaultValue;

    const auto& val = style.at(key);
    if (std::holds_alternative<int>(val)) return static_cast<float>(std::get<int>(val));
    if (std::holds_alternative<float>(val)) return std::get<float>(val);
    return defaultValue;
}


ImVec2 Node::GlobalPos() const
{
    if (parent != nullptr) return localPos + parent->GlobalPos();
    return localPos;
}

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
    string globalPos = " | GPos: " + to_string(int(node->GlobalPos().x)) + ' ' + to_string(int(node->GlobalPos().y));
    string localPos = "         | LPos: " + to_string(int(node->localPos.x)) + ' ' + to_string(int(node->localPos.y));
    output += localPos + globalPos;
#endif
#ifdef DebugMargins
    string margins = " | Margins L: " + to_string(int(node->topLeftMargin.x)) + " R: " + to_string(int(node->bottomRightMargin.x))
        + " T: " + to_string(int(node->topLeftMargin.y)) + " B: " + to_string(int(node->bottomRightMargin.y));
    output += margins;
#endif
    string contentSize = " | CS: " + to_string(int(node->contentSize.x)) + ' ' + to_string(int(node->contentSize.y));
    output += contentSize + "\n";
    for (size_t i = 0; i < node->children.size(); i++)
    {
        output += PrintNode(&node->children[i], indent + 1);
    }
    return output;
}
string PrintDOM(std::vector<Node> children)
{
    string output;
    for (size_t i = 0; i < children.size(); i++)
    {
        output += PrintNode(&children[i], 0);
    }
    return output;
}
bool operator==(const Node& node1, const Node& node2)
{
    return node1.type == node2.type && node1.attributes == node2.attributes && node1.children.size() == node2.children.size();
}