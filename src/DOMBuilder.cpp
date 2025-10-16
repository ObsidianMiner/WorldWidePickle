#include "DOMBuilder.h"
#include <regex>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include "DOM.h"
#include "ImMath.h"

using namespace std;

inline void DOMBuilder::AddAttributeOrType()
{
    if (readingAttribute)
    {
        int startChar = buildingPropertyStart + 1;
        string attributePortion = htmlContent.substr(startChar, pointer - startChar - 1);
        int equalIndex = attributePortion.find('=');
        if (equalIndex != -1)
        {
            buildingNode->attributes.emplace(attributePortion.substr(1, equalIndex - 1), attributePortion.substr(equalIndex + 2));
        }
        buildingPropertyStart = pointer - 1;
    }
    else
    {
        buildingNode->type = htmlContent.substr(buildingNodeStart + 1, pointer - buildingNodeStart - 1);
        buildingPropertyStart = pointer - 1;
        readingAttribute = true;
    }
}
inline void DOMBuilder::StartTag()
{
    insideTag = true;
    readingAttribute = false;
    buildingNodeStart = pointer;
    buildingPropertyStart = buildingNodeStart;
    if (currentContext == nullptr)
    {
        rootNodes.emplace_back();
        buildingNode = &rootNodes[rootNodes.size() - 1];
    }
    else
    {
        currentContext->children.emplace_back();
        buildingNode = &currentContext->children[currentContext->children.size() - 1];
        buildingNode->id = highestID;
        highestID++;
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
    if (htmlContent[pointer - 1] == '/') //Auto Closing Tag
    {
        currentContext = buildingNode->parent;
    }
    else if (buildingNode->type[0] == '/')
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
                    std::string innerText = trim(htmlContent.substr(lastEndTagPos + 1, lastStartTagPos - (lastEndTagPos + 1)));
                    if (innerText.length() > 0)
                    {
                        parents[i]->children.emplace_back();
                        parents[i]->children[0].parent = parents[i];
                        parents[i]->children[0].type = "#text";
                        parents[i]->children[0].attributes.emplace(std::pair<std::string, std::string>{ "text", innerText });
                        parents[i]->children[0].id = highestID;
                        highestID++;
                    }
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
#ifdef ParseDebug
        std::vector<Node*> parents = buildingNode->GetParents();
        std::cout << std::string(parents.size(), ' ') << "Parsed node: " << buildingNode->type << std::endl;
#endif
    }
    lastEndTagPos = pointer;
#ifdef ParseDebug
    PrintDOM(rootNodes);
#endif
    insideTag = false;
}

void ValidateNode(Node* node)
{
    for (size_t i = 0; i < node->children.size(); i++)
    {
        if (node->children[i].parent != node)
        {
            std::cout << "Invalid parent reference in node: " << node->children[i].type << std::endl;
            node->children[i].parent = node;
        }
    }
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

    std::cout << "Validating DOM..." << std::endl;
    for (Node& root : rootNodes)
    {
        ValidateNode(&root);
	}

    return rootNodes;
}

std::vector<Node> ParseNodes(const string& htmlContent)
{
    DOMBuilder builder = DOMBuilder(htmlContent);
    return builder.ParseNodes();
}