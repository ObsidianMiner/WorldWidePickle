#include "DOM.h"
#include <regex>
#include <iostream>
#include <tuple>
#include <unordered_map>



std::vector<Node> ParseNodes(const std::string& htmlContent)
{
    int pointer = 0;
	int buildingNodeStart = 0;
	bool insideTag = false;
	bool inClosingTag = false;
	bool readingAttribute = false;
	std::vector<Node> rootNodes;
    Node* currentContext = nullptr;
	Node* buildingNode = nullptr;
    std::unordered_map<Node*, int> nodeStartPositions;
    std::unordered_map<Node*, int> nodeClosePositions;

    while (pointer < htmlContent.length())
    {
        pointer++;
        if (htmlContent[pointer] == '<')
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
                nodeStartPositions.emplace(buildingNode, pointer);
            }
            buildingNode->parent = currentContext;
            currentContext = buildingNode;
        }
        else if (htmlContent[pointer] == ' ' && insideTag)
        {
            if (readingAttribute)
            {
                //Attributes
            }
            else
            {
				buildingNode->type = htmlContent.substr(buildingNodeStart + 1, pointer - buildingNodeStart - 1);
                readingAttribute = true;
            }
        }
        else if (htmlContent[pointer] == '>' && buildingNode != nullptr)
        {
            //End tag
            nodeClosePositions.emplace(buildingNode, pointer);
            if (!readingAttribute)
            {
                buildingNode->type = htmlContent.substr(buildingNodeStart + 1, pointer - buildingNodeStart - 1);
                readingAttribute = true;
            }
            if(buildingNode->type[0] == '/')
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
                                [buildingNode](const Node& obj) {
                                    return &obj == buildingNode;
                                }),
                            parents[i]->children.end()
                        );
                        
                        
                        if (parents[i]->children.size() == 0)
                        {
                            parents[i]->innerText = htmlContent.substr(nodeClosePositions[parents[i]] + 1, nodeStartPositions[buildingNode] - (nodeClosePositions[parents[i]] + 1));
                            std::cout << parents[i]->innerText << std::endl;
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
#ifdef ParseDebug
            PrintDOM(rootNodes);
#endif
			insideTag = false;
        }
    }
	std::cout << "Finished parsing nodes." << std::endl;
    PrintDOM(rootNodes);
    return rootNodes;
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
void PrintNode(Node* node, int indent)
{
    std::cout << std::string(indent, ' ') << node->type << std::endl;
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