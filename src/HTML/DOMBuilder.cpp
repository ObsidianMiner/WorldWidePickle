/**
* @file DOMBuilder.cpp
* @brief Builds a DOM from an HTML file
*/

#include "DOMBuilder.h"
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
std::string TextTrim(const std::string& str, bool& spacesBefore, bool& spacesAfter) {
    auto is_not_space = [](unsigned char ch) {
        return !std::isspace(ch);
        };
    auto is_not_newline = [](unsigned char ch) {
        return !std::isspace(ch);
        };

    auto begin = std::find_if(str.begin(), str.end(), is_not_space);
    if (begin == str.end()) return ""; // All whitespace

    auto end = std::find_if(str.rbegin(), str.rend(), is_not_newline).base();

    spacesBefore = begin != str.begin();
    spacesAfter = end != str.end();

    return std::string(begin, end);
}
inline void CreateInnerText(Node* parent, std::string textInbetweenTags, int& nextNodeID)
{
    
    bool spacesBefore = false;
    bool spacesAfter = false;
    std::string innerText = TextTrim(textInbetweenTags, spacesBefore, spacesAfter);
    if (innerText.length() > 0)
    {
        auto& node = *parent->children.emplace_back(std::make_unique<Node>(parent, "#text", nextNodeID++));
        node.attributes.emplace(std::pair<std::string, std::string>{ "text", innerText });
        if (spacesBefore) node.attributes.emplace(std::pair<std::string, std::string>{ "sb", "1"});
        if (spacesAfter) node.attributes.emplace(std::pair<std::string, std::string>{ "sa", "1"});
        nextNodeID++;
    }
}
inline void HTMLTreeBuilder::StartTag()
{
    insideTag = true;
    readingAttribute = false;
    buildingNodeStart = pointer;
    buildingPropertyStart = buildingNodeStart;
    if (currentContext == nullptr)
    {
        rootNodes.emplace_back(std::make_unique<Node>());
        buildingNode = rootNodes[rootNodes.size() - 1].get();
    }
    else
    {
        //Handle innerText
        CreateInnerText(currentContext, htmlContent.substr(lastEndTagPos + 1, pointer - (lastEndTagPos + 1)), nextNodeID);
        //Add the node to the current context
        currentContext->children.emplace_back(std::make_unique<Node>());
        buildingNode = currentContext->children[currentContext->children.size() - 1].get();
        buildingNode->id = nextNodeID;
        nextNodeID++;


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
                Node* target = buildingNode;
                parents[i]->children.erase(
                    std::remove_if(parents[i]->children.begin(), parents[i]->children.end(),
                        [target](const std::unique_ptr<Node>& obj) {
                            return obj.get() == target;
                        }),
                    parents[i]->children.end()
                );


                if (parents[i]->children.size() == 0)
                {
                    CreateInnerText(parents[i], htmlContent.substr(lastEndTagPos + 1, lastStartTagPos - (lastEndTagPos + 1)), nextNodeID);
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
        if (node->children[i]->parent != node)
        {
            std::cout << "Invalid parent reference in node: " << node->children[i]->type << std::endl;
            node->children[i]->parent = node;
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
std::unique_ptr<Node> HTMLTreeBuilder::ParseNodes()
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
            else if (htmlContent[pointer] == ' ' && (!readingAttribute || htmlContent[pointer - 1] == '"' || htmlContent[pointer - 1] == '\'') && insideTag)
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

    BrowserConsole::GetInstance().Log("DOM Builder", "Finished parsing nodes, starting validation parent child validation.", true);
    for (size_t i = 0; i < rootNodes.size(); i++)
    {
        ValidateNode(rootNodes[i].get());
    }
    BrowserConsole::GetInstance().Log("DOM Builder", "Validated", true);

    if (rootNodes.size() != 1)
    {
        std::unique_ptr<Node> rootNode = std::make_unique<Node>(nullptr, "html"s, nextNodeID++);
        rootNode->children = std::move(rootNodes);
        return rootNode;
    }


    return std::move(rootNodes[0]);
}

std::unique_ptr<Node> ParseNodes(const string& htmlContent)
{
    HTMLTreeBuilder builder = HTMLTreeBuilder(htmlContent);
    return builder.ParseNodes();
}