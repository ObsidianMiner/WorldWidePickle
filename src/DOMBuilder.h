#pragma once
#include "Node.h"

struct DOMBuilder
{
public:
	std::vector<Node> ParseNodes();
	DOMBuilder(const std::string& html) : htmlContent(html) {}
private:
	void StartTag();
	void AddAttributeOrType();
	void EndTag();
	void EndCDATA();
	const std::string& htmlContent;
	int pointer = 0;
	int buildingNodeStart = 0;
	int buildingPropertyStart = 0;
	bool insideTag = false;
	bool inClosingTag = false;
	bool readingAttribute = false;
	bool readingCDATA = false;
	std::vector<Node> rootNodes;
	Node* currentContext = nullptr;
	Node* buildingNode = nullptr;
	int lastStartTagPos = 0;
	int lastEndTagPos = 0;
	int highestID = 0;
};
std::vector<Node> ParseNodes(const std::string& htmlContent);