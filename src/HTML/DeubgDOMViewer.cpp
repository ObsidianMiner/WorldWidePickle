#include "Browser/Tab.h"
#include <UI/PickleGUI.h>
#include "Util/ImMath.h"
#include <format>
#include "CSS/CSSBuilder.h"

DebugDOMViewer::DebugDOMViewer()
{
    expanded.resize(10);
    expanded[0] = true;
    inspectedNodeId = -2;
}

void DebugDOMViewer::Render(Tab* tab)
{
    if (tab == nullptr || tab->currentPage == nullptr) return;

    RenderDOMTree(tab);
    RenderInspect(tab);
}
void DebugDOMViewer::RenderDOMTree(Tab* tab)
{
    char name[256];
    std::snprintf(name, sizeof(name), "%s DOM###DOM %d", tab->currentPage->name.c_str(), tab->id);
    ImGui::Begin(name);
    ImVec2 size = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
    char title[256];
    std::snprintf(name, sizeof(title), "##LoadedPageDOM");

    if (tab->currentPage)
    {
        std::shared_ptr<Node> domSnapshot = tab->currentPage->rootNode.load();
        RenderNode(*domSnapshot, 0);
    }

    ImGui::End();
}

const ImVec2 layoutBoxInset = ImVec2(20.0f, 20.0f);
const ImVec2 layoutBoxSize = ImVec2(300.0f, 150.0f);

std::string FormatFloatToString(float value)
{
    std::string labelText;
    if (value == std::trunc(value)) labelText = std::to_string((int)value);
    else labelText = std::format("{:.2f}", value);
    return labelText;
}

void CenteredLayoutLabel(ImDrawList* dl, ImVec2 pos, ImU32 color, std::string labelText)
{
    float width = PGUI::defaultFont.regular->CalcTextSizeA(20.0f, FLT_MAX, FLT_MAX, labelText.c_str()).x;

    dl->AddText(pos - ImVec2(width / 2.0f, 0.0f), IM_COL32(255, 255, 255, 255), labelText.c_str());
}
void CenteredLayoutLabel(ImDrawList* dl, ImVec2 pos, ImU32 color, float value)
{
    CenteredLayoutLabel(dl, pos, color, FormatFloatToString(value));
}
void DrawLayoutBox(const char* name, ImDrawList* dl, ImU32 color, int indentsIn, ImVec2 posiveDirectionValues, ImVec2 negitiveDirectionValues, ImVec2 startPos, ImVec2 bottomRight)
{
    startPos += layoutBoxInset * indentsIn;
    bottomRight -= layoutBoxInset * indentsIn;
    ImVec2 boxSize = layoutBoxSize - layoutBoxInset * indentsIn * 2;
    dl->AddRectFilled(
        startPos,
        bottomRight,
        color, 4.0f);

    CenteredLayoutLabel(dl, startPos + ImVec2(boxSize.x / 2.0f, 0.0f), color, posiveDirectionValues.y);
    CenteredLayoutLabel(dl, startPos + ImVec2(boxSize.x / 2.0f, boxSize.y - 20.0f), color, negitiveDirectionValues.y);
    CenteredLayoutLabel(dl, startPos + ImVec2(10.0f, boxSize.y / 2.0f - 10.0f), color, posiveDirectionValues.x);
    CenteredLayoutLabel(dl, startPos + ImVec2(boxSize.x - 10.0f, boxSize.y / 2.0f - 10.0f), color, negitiveDirectionValues.x);
}
void DrawLayoutBoxContent(const char* name, ImDrawList* dl, ImU32 color, int indentsIn, ImVec2 contentSize, ImVec2 startPos, ImVec2 bottomRight)
{
    startPos += layoutBoxInset * indentsIn;
    bottomRight -= layoutBoxInset * indentsIn;
    ImVec2 boxSize = layoutBoxSize - layoutBoxInset * indentsIn * 2;
    dl->AddRectFilled(
        startPos,
        bottomRight,
        color, 4.0f);

    CenteredLayoutLabel(dl, startPos + ImVec2(boxSize.x / 2.0f, 0.0f), color, FormatFloatToString(contentSize.x) + "x" + FormatFloatToString(contentSize.y));
}

void DebugDOMViewer::RenderInspect(Tab* tab)
{
    char inspectName[256];
    std::snprintf(inspectName, sizeof(inspectName), "%s Inspect###Inspect %d", tab->currentPage->name.c_str(), tab->id);

    ImGui::Begin(inspectName);

    if (tab != nullptr && tab->currentPage != nullptr && tab->currentPage->fullyLoaded)
    {
        Node* rootNode = tab->currentPage->rootNode.load().get();

        Node* inspectedNode = tab->currentPage->GetNodeFromIdInChildren(rootNode, inspectedNodeId);
        
        if (inspectedNode != nullptr)
        {
            ImGui::Text(("Inspected Node: " + inspectedNode->type + " ID: " + std::to_string(inspectedNodeId)).c_str());

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 2.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));
            if (ImGui::CollapsingHeader("Layout:"))
            {
                ImGui::PopStyleVar(2);

                ImGui::Indent(20);
                ImDrawList* dl = ImGui::GetWindowDrawList();

                ImVec2 wholeLayoutInset = ImVec2(10.0f, 10.0f);

                ImVec2 startPos = ImGui::GetCursorScreenPos() + wholeLayoutInset;
                ImVec2 bottomRight = startPos + layoutBoxSize;

                DrawLayoutBox("Margin", dl, IM_COL32(70, 50, 30, 255), 0, inspectedNode->layout.topLeftMargin, inspectedNode->layout.bottomRightMargin, startPos, bottomRight);
                DrawLayoutBox("Border", dl, IM_COL32(135, 80, 40, 255), 1, inspectedNode->layout.border, inspectedNode->layout.border, startPos, bottomRight);
                DrawLayoutBox("Padding", dl, IM_COL32(40, 135, 40, 255), 2, inspectedNode->layout.padding, inspectedNode->layout.padding, startPos, bottomRight);
                DrawLayoutBoxContent("Content", dl, IM_COL32(50, 50, 135, 255), 3, inspectedNode->layout.contentSize, startPos, bottomRight);

                ImGui::Dummy(layoutBoxSize + wholeLayoutInset * 2);

                ImGui::Text(("Min Size: (" + FormatFloatToString(inspectedNode->layout.minContentSize.x) + "," + FormatFloatToString(inspectedNode->layout.minContentSize.y) + ")").c_str());
                ImGui::Text(("Max Size: (" + FormatFloatToString(inspectedNode->layout.maxContentSize.x) + "," + FormatFloatToString(inspectedNode->layout.minContentSize.y) + ")").c_str());
                ImGui::Text(("Perfered Size: (" + FormatFloatToString(inspectedNode->layout.perferedSize.x) + "," + FormatFloatToString(inspectedNode->layout.minContentSize.y) + ")").c_str());
                ImGui::Text(("Local Position: (" + FormatFloatToString(inspectedNode->layout.localPos.x) + "," + FormatFloatToString(inspectedNode->layout.localPos.y) + ")").c_str());
                ImGui::Text(("Global Position: (" + FormatFloatToString(Layout::GetGlobalPos(*inspectedNode).x) + "," + FormatFloatToString(Layout::GetGlobalPos(*inspectedNode).y) + ")").c_str());

                ImGui::Unindent(20);
            }
            else ImGui::PopStyleVar(2);


            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 2.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));
            if (ImGui::CollapsingHeader("CSS Styles:"))
            {
                ImGui::PopStyleVar(2);
                ImGui::Indent(20);
                for (const auto& pair : inspectedNode->style)
                {
                    ImGui::Text(CSSBuilder::CSSRuleString(pair).c_str());
                }
                ImGui::Unindent(20);
            }
            else ImGui::PopStyleVar(2);

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 2.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));
            if (ImGui::CollapsingHeader("Attributes"))
            {
                ImGui::PopStyleVar(2);
                for (const auto& pair : inspectedNode->attributes)
                {
                    ImGui::Text(CSSBuilder::CSSRuleString(pair).c_str());
                }
            }
            else ImGui::PopStyleVar(2);
            
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 2.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));
            if (ImGui::CollapsingHeader("Mutable State"))
            {
                ImGui::PopStyleVar(2);
                if (inspectedNode->type == "input")
                {
                    if (inspectedNode->HasAttribute("type") && inspectedNode->attributes["type"] == "text")
                    {
                        std::string inputText = static_cast<TextInputMutableState*>(renderingTab->currentPage->mutableState[inspectedNode->id].get())->inputText;
                        ImGui::Text(("Input Text: " + inputText).c_str());
                    }
                }
            }
            else ImGui::PopStyleVar(2);
        }
        else ImGui::Text("Could not find node");
        /*
        ImGui::Text(("Bottom Left Margin: " + std::to_string(inspectedNode->layout.bottomRightMargin.x)).c_str());
        ImGui::Text(("Top Margin: " + inspectedNode->layout.bottomRightMargin).c_str());
        ImGui::Text(("Border: " + inspectedNode->layout.border).c_str());
        ImGui::Text(("Padding: " + inspectedNode->layout.padding).c_str());
        */
    }

    ImGui::End();
}

void DebugDOMViewer::RenderNode(Node& node, int indent)
{
    int generated = 0;
    int expId = node.id + 1;

    // Dropdown
    while (generated < 10000 && ((int)expanded.size() <= expId))
    {
        generated++;
        expanded.push_back(false);
    }
    if (node.children.size() > 0)
    {
        // Foldout
        if (expanded[expId])
        {
            if (ImGui::Button(("-##" + std::to_string(expId)).c_str()))
            {
                if (PGUI::io->KeyAlt) RecurisveExpand(node, true);
                else expanded[expId] = false;
            }
        }
        else if (ImGui::Button((">##" + std::to_string(expId)).c_str()))
        {
            if (PGUI::io->KeyAlt) RecurisveExpand(node, false);
            else expanded[expId] = true;
        }
        ImGui::SameLine();
    }

    // Inspect Button
    if (ImGui::Button(("I##" + std::to_string(node.id)).c_str())) inspectedNodeId = node.id;
    ImGui::SameLine();

    // Label
    std::string label;
    if (node.type == "#text") label = node.type + " " + PrintNodeAttributes(&node);
    else label = "<" + node.type + PrintNodeAttributes(&node) + ">" + ((node.children.size() > 0 && !expanded[expId]) ? "...< /" + node.type + ">" : "");

    ImGui::InputText(("##" + std::to_string(node.id)).c_str(), (char*)(label.c_str()), label.size() + 1, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
    if (expanded[expId] && node.type != "#text")
    {
        ImGui::Indent(20);
        for (size_t i = 0; i < node.children.size(); i++)
        {
            RenderNode(*node.children[i], indent + 1);
        }
        ImGui::Unindent(20);
        std::string endLabel = "</" + node.type + ">";
        ImGui::InputText(("##Closing" + std::to_string(node.id)).c_str(), (char*)(endLabel.c_str()), endLabel.size() + 1, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
    }
}
void DebugDOMViewer::RecurisveExpand(Node& node, bool retract)
{
    int generated = 0;
    int expId = node.id + 1;
    while (generated < 10000 && ((int)expanded.size() <= expId))
    {
        generated++;
        expanded.push_back(false);
    }
    expanded[expId] = !retract;
    for (size_t i = 0; i < node.children.size(); i++)
    {
        RecurisveExpand(*node.children[i], retract);
    }
}
