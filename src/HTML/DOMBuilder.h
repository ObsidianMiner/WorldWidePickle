#pragma once
#include "Node.h"

struct HTMLTreeBuilder
{
public:
	std::unique_ptr<Node> ParseNodes();
	HTMLTreeBuilder(std::string& html) : htmlContent(html) {}
private:
	void StartTag();
	void AddAttributeOrType();
	void EndTag();
	void EndCDATA();
	std::string& htmlContent;
	int pointer = 0;
	int buildingNodeStart = 0;
	int buildingPropertyStart = 0;
	bool insideTag = false;
	bool inClosingTag = false;
	bool readingAttribute = false;
	bool readingCDATA = false;
	std::vector<std::unique_ptr<Node>> rootNodes;
	Node* currentContext = nullptr;
	Node* buildingNode = nullptr;
	int lastStartTagPos = 0;
	int lastEndTagPos = 0;
	int nextNodeID = 0;
};
std::unique_ptr<Node> ParseNodes(std::string& htmlContent);