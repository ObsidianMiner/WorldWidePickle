#pragma once  
#include <string>  
#include <tuple>
#include <vector>
#include <unordered_map>
#include <functional>
#include <map>

class ImDrawList;

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

	void Render(std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer);
	bool operator==(const Node& other)
	{
		return this == &other;
	}
private:
	inline void RenderChildren(std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer);
	inline void RenderContents(std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer);
};
struct DOMBuilder
{
public:
	std::vector<Node> ParseNodes();
	DOMBuilder(const std::string& html) : htmlContent(html) {}
private:
	void StartTag();
	void AddAttributeOrType();
	void EndTag();
	const std::string& htmlContent;
	int pointer = 0;
	int buildingNodeStart = 0;
	bool insideTag = false;
	bool inClosingTag = false;
	bool readingAttribute = false;
	std::vector<Node> rootNodes;
	Node* currentContext = nullptr;
	Node* buildingNode = nullptr;
	int lastStartTagPos = 0;
	int lastEndTagPos = 0;
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