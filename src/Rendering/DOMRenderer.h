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
	inline void RenderContents(Node& node, std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer);

	void DrawInput(Node& node);
};