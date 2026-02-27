#include "Layout.h"
#include <numeric>

void Layout::LayoutTable(Node& node, float parentWidth, Website& website)
{
	//Step 0: Have children determine their size
	
	
	for (size_t i = 0; i < node.children.size(); i++)
	{
		LayoutChildren(node.children[i], 22, website);
	}

	//Step 1: Get important sub-nodes.

	Node* header = node.TryGetChildOfType("thead");
	Node* footer = node.TryGetChildOfType("tfoot");

	//tbody can be ommited if a footer or head is not present. In that case treat the entire table like the body.
	Node* body = node.TryGetChildOfType("tbody");
	if (!body) body = &node;

	//Step 2: Get conceptual rows and columns by combining info

	std::vector<Node*> rows;

	int extraRows = 0;
	if (header != nullptr) extraRows += header->children.size();
	if (footer != nullptr) extraRows += footer->children.size();

	rows.reserve(body->children.size() + extraRows);
	if (header != nullptr)
		for (auto& child : header->children) rows.push_back(&child);

	for (auto& child : body->children) rows.push_back(&child);


	if (footer != nullptr)
		for (auto& child : footer->children) rows.push_back(&child);

	//Step 3: Get Size of each Collumn and row of the body

	int columns = 0;
	std::vector<float> rowHeights;
	std::vector<float> columnWidths;
	float sizeWidth = 0;

	for (size_t i = 0; i < rows.size(); i++)
	{
		Node* row = rows[i];
		sizeWidth = std::fmax(sizeWidth, row->layout.contentSize.x);
		rowHeights.push_back(row->layout.contentSize.y);
		columns = std::max<int>(row->children.size(), columns);
		for (size_t j = 0; j < row->children.size(); j++)
		{
			Node* element = &row->children[j];
			if (j >= columnWidths.size()) columnWidths.push_back(element->layout.contentSize.x);
			else columnWidths[j] = std::fmax(element->layout.contentSize.x, columnWidths[j]);
		}
	}
	std::vector<float> columnPositions;
	columnPositions.resize(columnWidths.size());
	if (columnWidths.size() > 0)
	{
		columnPositions[0] = 0;
		if (columnWidths.size() > 1)
		{
			for (size_t i = 1; i < columnPositions.size(); i++)
			{
				columnPositions[i] = columnWidths[i - 1] + columnPositions[i - 1];
			}
		}
	}
	std::vector<float> rowPositions;
	rowPositions.resize(rowHeights.size());
	if (rowHeights.size() > 0)
	{
		rowPositions[0] = 0;
		if (rowHeights.size() > 1)
		{
			for (size_t i = 1; i < rowPositions.size(); i++)
			{
				rowPositions[i] = rowHeights[i - 1] + rowPositions[i - 1];
			}
		}
	}

	//Step 4: Position children

	for (size_t i = 0; i < rows.size(); i++)
	{
		Node* row = rows[i];
		for (size_t j = 0; j < row->children.size(); j++)
		{
			Node* element = &row->children[j];
			element->layout.localPos.x = columnPositions[j];
			element->layout.localPos.y = rowPositions[i];
			element->layout.contentSize.x = columnWidths[j];
			element->layout.contentSize.y = rowHeights[i];
		}
	}
	if(!columnWidths.empty() && !rowHeights.empty())node.layout.contentSize =
		{ columnPositions[columnPositions.size() - 1] + columnWidths[columnWidths.size() - 1],
		  rowPositions[rowPositions.size() - 1] + rowHeights[rowHeights.size() - 1] };

}