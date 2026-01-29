#pragma once

#include <functional>
#include <map>
#include <imgui/imgui.h>
#include <Node.h>

struct DOMRenderer
{
	//Rendering
	void Render(Node& node, std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer);
private:
	inline void RenderChildren(Node& node, std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer);
	void RenderImage(Node& node, std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer);
	void RenderTextElement(Node& node, std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer);
	inline void RenderContents(Node& node, std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer, const ImVec2& bg_min, const ImVec2& totalImbed);

	void ProcessLink(const ImVec2& bg_min, Node& node, const ImVec2& totalImbed);
	void DrawDiv(const ImVec2& bg_min, const Node& node, const ImVec2& totalImbed, std::multimap<int, std::function<void(ImDrawList*)>>& queue, int& layer);
	void DrawInput(Node& node);
};