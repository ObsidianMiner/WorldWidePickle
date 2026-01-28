#pragma once  
#include <string>  
#include <tuple>
#include <vector>
#include <unordered_map>
#include <functional>
#include <map>
#include <variant>
#include <imgui/imgui.h>

class ImDrawList;
class Website;

#define RuleValue std::variant<std::string, int, float, ImU32>

struct Node
{  
public:
	//Core Node
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

	//Layout properties
	ImVec2 localPos;
	ImVec2 GlobalPos() const;
	ImVec2 contentSize;
	ImVec2 topLeftMargin;
	ImVec2 bottomRightMargin;
	ImVec2 border;
	ImVec2 padding;
	float posInLine;


	int id;

public:
	//Reference Loading
	void LoadReferences(Website& website);
	void UpdateStyle(const Website& website, const std::unordered_map<std::string, RuleValue>& inheritedStyle);

	bool operator==(const Node& other)
	{
		return this == &other;
	}

	
public:
	//Layout
	void CalculateChildPositions(float parentWidth, Website& website);
private:
	void CaculateContentSize(float parentWidth, Website& website);
	void SetMargins();
	bool HasAutoMargin();
	void CalculateChildrenPositions(float parentWidth, Website& website);
	void CalculateChildrenPositionsFlex(Website& website);
	void WrapSelf(float parentWidth, float& currentX, float& currentY);
	void CalculateChildrenAutoMargins(float parentWidth, Website& website);
};
std::vector<Node> ParseNodes(const std::string& htmlContent);

std::string PrintNode(Node* node, int indent);

std::string PrintDOM(std::vector<Node> children);