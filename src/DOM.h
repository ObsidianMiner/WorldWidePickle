#pragma once  
#include <string>  
#include <tuple>
#include <vector>


struct Node  
{  
public:
	std::string type;
	std::tuple<std::string, std::string> attributes;
	Node* parent = nullptr;
	std::vector<Node> children;
	std::string innerText;
	std::vector<Node*> GetParents();

	void Render();
	bool operator==(const Node& other)
	{
		return this == &other;
	}
};
std::vector<Node> ParseNodes(const std::string& htmlContent);

void PrintNode(Node* node, int indent);

void PrintDOM(std::vector<Node> children);
