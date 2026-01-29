#pragma once  
#include <string>  
#include <tuple>
#include <vector>
#include <unordered_map>
#include <functional>
#include <map>
#include <variant>
#include <imgui/imgui.h>

class NodeLayout;
class ImDrawList;
class Website;

#define RuleValue std::variant<std::string, int, float, ImU32>

class NodeLayout
{
public:
	ImVec2 localPos;
	ImVec2 contentSize;
	ImVec2 topLeftMargin;
	ImVec2 bottomRightMargin;
	ImVec2 border;
	ImVec2 padding;
	float posInLine;
};

struct Node
{
public:
	//Core Node
	int id;
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

	NodeLayout layout;

public:
	//Reference Loading
	void LoadReferences(Website& website);
	void UpdateStyle(const Website& website, const std::unordered_map<std::string, RuleValue>& inheritedStyle);

	bool operator==(const Node& other)
	{
		return this == &other;
	}
};

std::string PrintNode(Node* node, int indent);

std::string PrintDOM(std::vector<Node> children);