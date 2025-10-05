#include "DOM.h"
#include <regex>
#include <iostream>
#include <tuple>
#include <unordered_map>

using namespace std;

inline void DOMBuilder::AddAttributeOrType()
{
    if (readingAttribute)
    {
        int startChar = buildingNodeStart + 1 + buildingNode->type.size();
        string attributePortion = htmlContent.substr(startChar, pointer - startChar - 1);
        int equalIndex = attributePortion.find('=');
        if (equalIndex != -1)
        {
            buildingNode->attributes.emplace(attributePortion.substr(1, equalIndex - 1), attributePortion.substr(equalIndex + 2));
        }
    }
    else
    {
        buildingNode->type = htmlContent.substr(buildingNodeStart + 1, pointer - buildingNodeStart - 1);
        readingAttribute = true;
    }
}
inline void DOMBuilder::StartTag()
{
    insideTag = true;
    readingAttribute = false;
    buildingNodeStart = pointer;
    if (currentContext == nullptr)
    {
        rootNodes.emplace_back();
        buildingNode = &rootNodes[rootNodes.size() - 1];
    }
    else
    {
        currentContext->children.emplace_back();
        buildingNode = &currentContext->children[currentContext->children.size() - 1];
        lastStartTagPos = pointer;
    }
    buildingNode->parent = currentContext;
    currentContext = buildingNode;
}
std::string trim(const std::string& str) {
    auto is_not_space = [](unsigned char ch) {
        return !std::isspace(ch);
        };

    auto begin = std::find_if(str.begin(), str.end(), is_not_space);
    if (begin == str.end()) return ""; // All whitespace

    auto end = std::find_if(str.rbegin(), str.rend(), is_not_space).base();
    return std::string(begin, end);
}
inline void DOMBuilder::EndTag()
{
    AddAttributeOrType();
    if (buildingNode->type[0] == '/')
    {
        buildingNode->type = buildingNode->type.substr(1);
        std::vector<Node*> parents = buildingNode->GetParents();
#ifdef ParseDebug
        std::cout << std::string(parents.size() - 1, ' ') << "Closed node: " << buildingNode->type << std::endl;
#endif
        bool completedNode = false;
        for (size_t i = 0; i < parents.size(); i++)
        {
            if (parents[i]->type == buildingNode->type)
            {
                //Remove the closing tag from becoming a child
                parents[i]->children.erase(
                    std::remove_if(parents[i]->children.begin(), parents[i]->children.end(),
                        [this](const Node& obj) {
                            return &obj == this->buildingNode;
                        }),
                    parents[i]->children.end()
                );


                if (parents[i]->children.size() == 0)
                {
                    parents[i]->innerText = htmlContent.substr(lastEndTagPos + 1, lastStartTagPos - (lastEndTagPos + 1));
					parents[i]->innerText = trim(parents[i]->innerText);
#ifdef ParseDebug
                    std::cout << parents[i]->innerText << std::endl;
#endif
                }

                currentContext = parents[i]->parent;
                completedNode = true;
                break;
            }
        }
        if (!completedNode)
        {
            std::cout << "Could not properly close node: " << buildingNode->type << std::endl;
        }
    }
    else
    {
        std::vector<Node*> parents = buildingNode->GetParents();
#ifdef ParseDebug
        std::cout << std::string(parents.size(), ' ') << "Parsed node: " << buildingNode->type << std::endl;
#endif
    }
    lastEndTagPos = pointer;
#ifdef ParseDebug
    PrintDOM(rootNodes);
#endif
    insideTag = false;
}
std::vector<Node> DOMBuilder::ParseNodes()
{
    while (pointer < htmlContent.length())
    {
        if (htmlContent[pointer] == '<')
        {
            StartTag();
        }
        else if (htmlContent[pointer] == ' ' && insideTag)
        {
            AddAttributeOrType();
        }
        else if (htmlContent[pointer] == '>' && buildingNode != nullptr)
        {
            EndTag();
        }

        pointer++;
    }
    std::cout << "Finished parsing nodes." << std::endl;
    PrintDOM(rootNodes);
    return rootNodes;
}

std::vector<Node> ParseNodes(const string& htmlContent)
{
    DOMBuilder builder = DOMBuilder(htmlContent);
    return builder.ParseNodes();
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
void PrintNode(Node* node, int indent)
{

    std::cout << std::string(indent, ' ') << node->type << AttributesToString(node->attributes) << std::endl;
    for (size_t i = 0; i < node->children.size(); i++)
    {
        PrintNode(&node->children[i], indent + 1);
    }
}
void PrintDOM(std::vector<Node> children)
{
    for (size_t i = 0; i < children.size(); i++)
    {
        PrintNode(&children[i], 0);
    }
}
bool operator==(const Node& node1, const Node& node2)
{
    return node1.type == node2.type && node1.attributes == node2.attributes && node1.innerText == node2.innerText && node1.children.size() == node2.children.size();
}
bool operator==(const Attribute& att1, const Attribute& att2)
{
	return att1.name == att2.name && att1.value == att2.value;
}