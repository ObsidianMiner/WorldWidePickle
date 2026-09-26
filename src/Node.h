#pragma once  
#include <string>  
#include <tuple>
#include <vector>
#include <unordered_map>
#include <functional>
#include <map>
#include <variant>
#include <imgui/imgui.h>
#include <memory>

struct ImDrawList;
class Website;

struct Percentage
{
public:
	float percent = 0;
	Percentage(float percentVal) { this->percent = percentVal; }
};

#define RuleValue std::variant<std::string, int, float, ImU32, Percentage>

class NodeLayout
{
public:
	ImVec2 localPos{ 0.0f, 0.0f };
	ImVec2 contentSize{ 0.0f, 0.0f };
	ImVec2 minContentSize{ 0.0f, 0.0f };
	ImVec2 maxContentSize{ 0.0f, 0.0f };
	ImVec2 perferedSize{ 0.0f,0.0f };
	ImVec2 topLeftMargin{ 0.0f, 0.0f };
	ImVec2 bottomRightMargin{ 0.0f, 0.0f };
	ImVec2 border{ 0.0f, 0.0f };
	ImVec2 padding{ 0.0f, 0.0f };
	float posInLine = 0.0f;

	NodeLayout() = default;
};
struct NodeMutableState
{

};
struct TextInputMutableState : NodeMutableState
{
	std::string inputText;
};
struct CheckboxInputMutableState : NodeMutableState
{
	bool value;
};
struct SelectMutableState : NodeMutableState
{
	std::string selectedName;
};
struct Node
{
public:
	//Core Node
	std::string type;
	int id = -1;
	std::unordered_map<std::string, std::string> attributes;
	std::unordered_map<std::string, RuleValue> style;

	bool HasAttribute(const std::string attribute) const;
	bool HasStyle(const std::string s) const;
	float GetStyleFloat(const std::string& key, float defaultValue = 0.0f) const;
	float GetStyleFloatAllowPercent(const std::string& key, float defaultValue, float whole) const;
	

	//Hierarchy
	Node* parent = nullptr;
	std::vector<std::unique_ptr<Node>> children;
	std::vector<Node*> GetParents();
	Node* GetParentNodeOfType(std::string type);
	Node* TryGetChildOfType(std::string type);
	int GetIndexInParent() const;

	NodeLayout layout;

	Node(Node* parent, std::string type, int id);
	Node() = default;

	std::unique_ptr<Node> CrClone() const;

	//Reference Loading
	void LoadReferences(Website& website);
	void UpdateStyle(const Website& website, const std::unordered_map<std::string, RuleValue>& inheritedStyle);

	bool operator==(const Node& other)
	{
		return this == &other;
	}
};
enum class PrintNodeInfo : uint8_t
{
	None = 0,
	ContentSize = 1,
	MinMaxContentSize = 2,
	LocalPos = 4,
	GlobalPos = 8,
	Margins = 16,
	Everything = 255
};
inline PrintNodeInfo operator|(PrintNodeInfo lhs, PrintNodeInfo rhs)
{
	using T = std::underlying_type_t<PrintNodeInfo>;
	return static_cast<PrintNodeInfo>(static_cast<T>(lhs) | static_cast<T>(rhs));
}
inline PrintNodeInfo operator&(PrintNodeInfo lhs, PrintNodeInfo rhs)
{
	using T = std::underlying_type_t<PrintNodeInfo>;
	return static_cast<PrintNodeInfo>(static_cast<T>(lhs) & static_cast<T>(rhs));
}

std::string PrintNode(Node* node, int indent, int maxDetph = 999, PrintNodeInfo infoFlags = PrintNodeInfo::None);

std::string PrintNodeAttributes(Node* node);

std::vector<std::string> GetSelectOptions(Node& node);