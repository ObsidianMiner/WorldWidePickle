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
	std::string type;
	std::unordered_map<std::string, std::string> attributes;
	std::unordered_map<std::string, RuleValue> style;

	ImVec2 localPos;
	ImVec2 GlobalPos();
	ImVec2 contentSize;
	ImVec2 topLeftMargin;
	ImVec2 bottomRightMargin;
	ImVec2 border;
	ImVec2 padding;

	Node* parent = nullptr;
	std::vector<Node> children;
	std::vector<Node*> GetParents();

	int id;

	void Render(std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer);
	void LoadReferences(Website& website);
	void CaculatePosition(float parentWidth, Website& website);
	void CaculateContentSize();
	void UpdateStyle(const Website& website, const std::unordered_map<std::string, RuleValue>& inheritedStyle);
	bool operator==(const Node& other)
	{
		return this == &other;
	}

	bool HasAttribute(std::string attribute);
	bool HasStyle(std::string s);
private:
	inline void RenderChildren(std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer);
	void RenderImage(std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer);
	void RenderTextElement(std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer);
	inline void RenderContents(std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer);
};
std::vector<Node> ParseNodes(const std::string& htmlContent);

std::string PrintNode(Node* node, int indent);

std::string PrintDOM(std::vector<Node> children);