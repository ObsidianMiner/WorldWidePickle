/**
* @file HTMLTreeBuilder.cpp
* @brief Builds a DOM from an HTML file
*/

#include "HTMLBuilder.h"
#include <regex>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include "Node.h"
#include "Util/ImMath.h"
#include "Util/StringUtils.h"
#include <Browser/BrowserConsole.h>

using namespace std;

inline void HTMLTreeBuilder::AddAttributeOrType()
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
std::string Trim(const std::string& str) {
    auto is_not_space = [](unsigned char ch) {
        return !std::isspace(ch);
        };

    auto begin = std::find_if(str.begin(), str.end(), is_not_space);
    if (begin == str.end()) return ""; // All whitespace

    auto end = std::find_if(str.rbegin(), str.rend(), is_not_space).base();
    return std::string(begin, end);
}
inline void HTMLTreeBuilder::StartTag()
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
        std::string innerText = Trim(htmlContent.substr(lastEndTagPos + 1, pointer - (lastEndTagPos + 1)));
        if (innerText.length() > 0)
        {
            currentContext->children.emplace_back();
            currentContext->children[currentContext->children.size() - 1].parent = currentContext;
            currentContext->children[currentContext->children.size() - 1].type = "#text";
            currentContext->children[currentContext->children.size() - 1].attributes.emplace(std::pair<std::string, std::string>{ "text", innerText });
            currentContext->children[currentContext->children.size() - 1].id = highestID;
            highestID++;
        }

        currentContext->children.emplace_back();
        buildingNode = &currentContext->children[currentContext->children.size() - 1];
        buildingNode->id = highestID;
        highestID++;


        lastStartTagPos = pointer;
    }
    buildingNode->parent = currentContext;
    currentContext = buildingNode;
}
inline void HTMLTreeBuilder::EndTag()
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
                    std::string innerText = Trim(htmlContent.substr(lastEndTagPos + 1, lastStartTagPos - (lastEndTagPos + 1)));
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
void HTMLTreeBuilder::EndCDATA()
{
    readingCDATA = false;

    string CDATAContent = htmlContent.substr(lastStartTagPos, pointer - lastStartTagPos - 1);
    currentContext->attributes.emplace("CDATA", CDATAContent);
    lastEndTagPos = pointer;
}
std::vector<Node> HTMLTreeBuilder::ParseNodes()
{
    while (pointer < htmlContent.length())
    {
        if (readingCDATA)
        {
            if (htmlContent[pointer] == '>' && htmlContent[pointer - 1] == ']')
            {
                EndCDATA();
            }
        }
        else
        {
            if (htmlContent[pointer] == '<')
            {
                if (htmlContent[pointer + 1] == '!')
                {
                    readingCDATA = true;
                    lastStartTagPos = pointer + 14;
                }
                else StartTag();
            }
            else if (htmlContent[pointer] == ' ' && (!readingAttribute || htmlContent[pointer - 1] == '"') && insideTag)
            {
                AddAttributeOrType();
            }
            else if (htmlContent[pointer] == '>' && buildingNode != nullptr)
            {
                EndTag();
            }
        }

        pointer++;
    }

    BrowserConsole::Log("DOM Builder", "Finished parsing nodes, starting validation.");
    for (Node& root : rootNodes)
    {
        ValidateNode(&root);
	}

    return rootNodes;
}

std::vector<Node> ParseNodes(const string& htmlContent)
{
    HTMLTreeBuilder builder = HTMLTreeBuilder(htmlContent);
    return builder.ParseNodes();
}