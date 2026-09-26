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
	void FullLayoutPass(Node& node, float parentWidth, Website& website);
	void GetMinMaxSize(Node& node, Website& website);
	Node* GetHover(const ImVec2& cursorPos, Website& website);
	static bool IsNodeBoundsInPoint(Node& node, const ImVec2& globalPos);
	static ImVec2 GetGlobalPos(const Node& node);
	static bool debuggingLayout;
private:
	void CalculateSelfSize(Node& node, float parentWidth, Website& website);
	void SetMargins(Node& node);
	bool ShouldGiveAutoMargin(Node& node);
	//void CalculateChildrenPositionsFlex(Node& node, Website& website);
	void WrapNode(Node& node, float parentWidth, float& currentX, float& currentY);
	void CalculateChildrenAutoMargins(Node& node, float parentWidth, Website& website);
	void LayoutTable(Node& node, float parentWidth, Website& website);

	//Size
	void GetMinMaxSizeSelf(Node& node, Website& website);
	void GetTableMinMaxSize(Node& node, Website& website);
	void GetBlockedElementMinMaxSize(Node& node, Website& website);
};