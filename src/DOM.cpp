#include "DOMBuilder.h"
#include <regex>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include "DOM.h"
#include "ImMath.h"

using namespace std;



bool Node::HasAttribute(std::string attribute)
{
    return attributes.find(attribute) != attributes.end();
}
bool Node::HasStyle(std::string s)
{
    return style.find(s) != style.end();
}


ImVec2 Node::GlobalPos()
{
    uintptr_t pval = reinterpret_cast<uintptr_t>(parent);
    if (pval < 0x1000 || pval > 0xFFFFFFFFFFFF)
    {
        cout << "Uh oh, parent invalid." << endl;
        return localPos;
    }
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
    string output;
    output = std::string(indent, '\t') + node->type + ' ' + AttributesToString(node->attributes) + "\n";
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