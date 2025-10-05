#pragma once  
#include <string>  
#include <tuple>
#include <vector>
#include <unordered_map>


struct Attribute
{
	std::string name;
	std::string value;
};
struct Node
{  
public:
	std::string type;
	std::unordered_map<std::string, std::string> attributes;
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

/// <summary>
/// Compares two nodes. Does not check parent
/// </summary>
/// <param name="node1"></param>
/// <param name="node2"></param>
/// <returns></returns>
bool operator==(const Node& node1, const Node& node2);
bool operator==(const Attribute& att1, const Attribute& att2);