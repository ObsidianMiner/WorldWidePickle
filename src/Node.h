#pragma once  
#include <string>  
#include <tuple>
#include <vector>
#include <unordered_map>
#include <functional>
#include <map>
#include <variant>
#include <imgui/imgui.h>

struct ImDrawList;
class Website;

#define RuleValue std::variant<std::string, int, float, ImU32>

class NodeLayout
{
public:
	ImVec2 localPos{ 0.0f, 0.0f };
	ImVec2 contentSize{ 0.0f, 0.0f };
	ImVec2 topLeftMargin{ 0.0f, 0.0f };
	ImVec2 bottomRightMargin{ 0.0f, 0.0f };
	ImVec2 border{ 0.0f, 0.0f };
	ImVec2 padding{ 0.0f, 0.0f };
	float posInLine = 0.0f;

	NodeLayout() = default;
};

struct Node
{
public:
	//Core Node
	int id = -1;
	std::string type;
	std::unordered_map<std::string, std::string> attributes;
	std::unordered_map<std::string, RuleValue> style;

	bool HasAttribute(const std::string attribute) const;
	bool HasStyle(const std::string s) const;
	float GetStyleFloat(const std::string& key, float defaultValue = 0.0f) const;
	

	//Hierarchy
	Node* parent = nullptr;
	std::vector<Node> children;
	std::vector<Node*> GetParents();
	Node* TryGetChildOfType(std::string type);
	int GetIndexInParent() const;

	NodeLayout layout;

	Node(Node* parent, std::string type, int id) { this->parent = parent; this->type = type; this->id = id; };
	Node() = default;

	//Reference Loading
	void LoadReferences(Website& website);
	void UpdateStyle(const Website& website, const std::unordered_map<std::string, RuleValue>& inheritedStyle);

	bool operator==(const Node& other)
	{
		return this == &other;
	}
};

std::string PrintNode(Node* node, int indent);

std::string PrintNodeAttributes(Node* node);
