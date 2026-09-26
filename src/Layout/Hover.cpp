#include "Layout.h"
#include "Browser/Tab.h"
#include "Util/ImMath.h"

Node* GetChildHovered(Node* node, const ImVec2& globalPos)
{
	for (size_t i = 0; i < node->children.size(); i++)
	{
		if (Layout::IsNodeBoundsInPoint(*node->children[i], globalPos)) return GetChildHovered(node->children[i].get(), globalPos);
	}
	return node;
}

/// <summary>
/// Get the node that is being hovered over on the specified website.
/// </summary>
/// <param name="cursorPos"></param>
/// <param name="website"></param>
/// <returns></returns>
Node* Layout::GetHover(const ImVec2& cursorPos, Website& website)
{
	ImVec2 globalPos = cursorPos - website.tab->contentPos;
	return GetChildHovered((website.rootNode.load().get()), globalPos);
}

//TODO: Make work with margins, borders and everything else but content size.
bool Layout::IsNodeBoundsInPoint(Node& node, const ImVec2& globalPos)
{
	ImVec2 nodeGlobalPos = Layout::GetGlobalPos(node);
	return globalPos.x > nodeGlobalPos.x && globalPos.x < nodeGlobalPos.x + node.layout.contentSize.x && globalPos.y > nodeGlobalPos.y && globalPos.y < nodeGlobalPos.y + node.layout.contentSize.y;
}
