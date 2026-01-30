#pragma once

#include <imgui/imgui.h>
#include <Node.h>

/// <summary>
/// Represents all the layout information of a website.
/// </summary>
class Layout
{
public:
	//Layout
	void LayoutChildren(Node& node, float parentWidth, Website& website);
	static ImVec2 GetGlobalPos(const Node& node);
	static bool debuggingLayout;
private:
	void CalculateContentSize(Node& node, float parentWidth, Website& website);
	void SetMargins(Node& node);
	bool ShouldGiveAutoMargin(Node& node);
	//void CalculateChildrenPositionsFlex(Node& node, Website& website);
	void WrapNode(Node& node, float parentWidth, float& currentX, float& currentY);
	void CalculateChildrenAutoMargins(Node& node, float parentWidth, Website& website);
};