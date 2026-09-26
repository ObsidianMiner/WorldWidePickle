#pragma once
#include "Node.h"

class Tab;
class Website;

class DebugDOMViewer
{
public:
	DebugDOMViewer();

	void Render(Tab* tab);
private:
	Website* website;
	int inspectedNodeId;
	std::vector<bool> expanded;

	void RecurisveExpand(Node& node, bool retract);
	void RenderNode(Node& node, int indent);

	void RenderDOMTree(Tab* tab);
	void RenderInspect(Tab* tab);
};