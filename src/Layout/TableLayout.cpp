#include "Layout.h"
#include <numeric>

void Layout::LayoutTable(Node& node, float parentWidth, Website& website)
{
	ImVec2 targetSize = node.layout.minContentSize;

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
		for (auto& child : header->children) rows.push_back(child.get());

	for (auto& child : body->children) rows.push_back(child.get());


	if (footer != nullptr)
		for (auto& child : footer->children) rows.push_back(child.get());

	//Step 3: Get Size of each Collumn and row of the body

	int columns = 0;
	std::vector<float> rowHeights;
	std::vector<float> columnWidths;
	float sizeWidth = 0;

	// Get the minimum size of each collumn before actualy calcualting the size of each cell.
	for (size_t i = 0; i < rows.size(); i++)
	{
		// Get the min max size of each full row as a block.
		Node* row = rows[i];
		for (size_t j = 0; j < row->children.size(); j++)
		{
			Node* element = row->children[j].get();
			GetBlockedElementMinMaxSize(*row->children[j], website);
			if (j >= columnWidths.size()) columnWidths.push_back(element->layout.perferedSize.x);
			else columnWidths[j] = std::fmax(element->layout.perferedSize.x, columnWidths[j]);
		}
	}
	for (size_t i = 0; i < columnWidths.size(); i++)
	{
		sizeWidth += columnWidths[i];
	}

	// Calculate the size of each cell.

	for (size_t i = 0; i < rows.size(); i++)
	{
		Node* row = rows[i];
		columns = std::max<int>(row->children.size(), columns);
		float height = 0;
		for (size_t j = 0; j < row->children.size(); j++)
		{
			Node* element = row->children[j].get();
			FullLayoutPass(*element, columnWidths[j], website);
			height = std::fmax(element->layout.contentSize.y, height);
		}
		row->layout.contentSize.x = sizeWidth;
		row->layout.contentSize.y = height;
		rowHeights.push_back(row->layout.contentSize.y);
	}

	// Calculate the position of all cells.
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
		row->layout.localPos.y = rowPositions[i];
		row->layout.contentSize.y = rowHeights[i];
		for (size_t j = 0; j < row->children.size(); j++)
		{
			Node* element = row->children[j].get();
			element->layout.localPos.x = columnPositions[j];
			element->layout.contentSize.y = rowHeights[i];
		}
	}
	if(!columnWidths.empty() && !rowHeights.empty())node.layout.contentSize =
		{ columnPositions[columnPositions.size() - 1] + columnWidths[columnWidths.size() - 1],
		  rowPositions[rowPositions.size() - 1] + rowHeights[rowHeights.size() - 1] };

}

/// <summary>
/// 
/// </summary>
void Layout::GetTableMinMaxSize(Node& node, Website& website)
{
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
		for (auto& child : header->children) rows.push_back(child.get());

	for (auto& child : body->children) rows.push_back(child.get());


	if (footer != nullptr)
		for (auto& child : footer->children) rows.push_back(child.get());

	// Step 3: Calculate Row MinMax Sizes

	std::vector<float> columnWidths;
	float sizeWidth = 0;
	float perferedWidth = 0;
	
	float minHeight = 0;
	float perferedHeight = 0;
	for (size_t i = 0; i < rows.size(); i++)
	{
		// Get the min max size of each full row as a block.
		Node* row = rows[i];
		for (size_t j = 0; j < row->children.size(); j++)
		{
			Node* element = row->children[j].get();
			GetBlockedElementMinMaxSize(*row->children[j], website);
			if (j >= columnWidths.size()) columnWidths.push_back(element->layout.minContentSize.x);
			else columnWidths[j] = std::fmax(element->layout.minContentSize.x, columnWidths[j]);
			row->layout.minContentSize.y = std::fmax(element->layout.minContentSize.y, row->layout.minContentSize.y);
			row->layout.perferedSize.x = std::fmax(element->layout.perferedSize.x, row->layout.perferedSize.x);
			row->layout.perferedSize.y = std::fmax(element->layout.perferedSize.y, row->layout.perferedSize.y);
		}
		minHeight += row->layout.minContentSize.y;
		perferedWidth += row->layout.perferedSize.x;
		perferedHeight += row->layout.perferedSize.y;
	}
	
	float minWidth = 0;
	for (size_t i = 0; i < columnWidths.size(); i++)
	{
		minWidth += columnWidths[i];
	}
	node.layout.minContentSize = { minWidth, minHeight };
	node.layout.perferedSize = {perferedWidth, perferedHeight };
}