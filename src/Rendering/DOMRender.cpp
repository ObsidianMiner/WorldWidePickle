/**
* @file DOMRender.cpp
* @brief Renders nodes to the screen, TextRenderer is seperated.
*/


#include "UI/PickleGUI.h"
#include <iostream>
#include "Browser/Tab.h"
#include "Util/ImMath.h"
#include <curl/curl.h>
#include "Util/StringUtils.h"
#include "Node.h"
#include "DOMRenderer.h"
#include "Browser/BrowserConsole.h"
#include <Dynamic/Forms.h>

inline bool NodeCantRender(Node& node)
{
    return node.type == "option" || node.type == "head";
}

/// <summary>
/// Renders a node, and all of it's children. The main inlit to all site rendering.
/// IMPORTANT: Layout must be done before hand.
/// </summary>
/// <param name="queue">Will be drawn after all nodes are rendered.</param>
/// <param name="layer">Which layer should this node be rendered on.</param>
void DOMRenderer::Render(Node& node, std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer)
{
    if (NodeCantRender(node)) return;

	//if (HasStyle("padding")) padding = ImVec2(std::get<int>(style["padding"]), std::get<int>(style["padding"]));
    ImVec2 totalImbed = node.layout.padding + node.layout.border;
    ImGui::SetCursorPos(Layout::GetGlobalPos(node) + renderingTab->contentPos);
    ImVec2 bg_min = ImGui::GetCursorScreenPos() - totalImbed;

    ImGui::BeginGroup();

    RenderContents(node, queue, layer, bg_min, totalImbed);
    RenderChildren(node, queue, layer + 1);

    ImGui::EndGroup();
}
void DOMRenderer::DrawDiv(const ImVec2& bg_min, const Node& node, const ImVec2& totalImbed, std::multimap<int, std::function<void(ImDrawList*)>>& queue, int& layer)
{
    ImVec2 bg_max = bg_min + node.layout.contentSize + totalImbed;

    ImU32 bg_color = IM_COL32(60, 60, 60, 180);
    float rounding = 6.0f;
    ImU32 border_color = IM_COL32(255, 255, 255, 40);
    queue.insert({ layer, [=](ImDrawList* dl) { dl->AddRectFilled(bg_min, bg_max, bg_color, rounding); } });
    queue.insert({ layer, [=](ImDrawList* dl) { dl->AddRect(bg_min, bg_max, border_color, rounding); } });
}
void DOMRenderer::ProcessLink(const ImVec2& bg_min, Node& node, const ImVec2& totalImbed)
{
    ImVec2 bg_max = bg_min + node.layout.contentSize + totalImbed;
    ImVec2 bg_size = bg_max - bg_min;
    if (bg_size.x > 0 && bg_size.y > 0)
    {
        if (ImGui::InvisibleButton(std::to_string(node.id).c_str(), bg_size)) {
            std::cout << "Clicked";
            std::string url = node.attributes["href"];
            if (url[0] == '/') url = renderingTab->currentURL + url;

            std::cout << "Opening URL: " << url << std::endl;
            renderingTab->SwitchPage(url);
        }
    }
}
void DOMRenderer::DrawSelect(Node& node)
{
    std::vector<std::string> options = GetSelectOptions(node);
    SelectMutableState* mutState = static_cast<SelectMutableState*>(renderingTab->currentPage->mutableState.at(node.id).get());

    if (ImGui::BeginCombo(("###" + std::to_string(node.id)).c_str(), mutState->selectedName.c_str()))
    {
        for (size_t i = 0; i < options.size(); i++)
        {
            if (ImGui::Selectable(options[i].c_str(), options[i] == mutState->selectedName))
            {
                mutState->selectedName = options[i];
            }
            if (mutState->selectedName == options[i]) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}
void DOMRenderer::DrawInput(Node& node)
{
    if (node.HasAttribute("type") && (node.attributes["type"] == "button" || node.attributes["type"] == "submit"))
    {
        ImVec2 btn_size = node.layout.contentSize;
        ImVec2 btn_pos = ImGui::GetItemRectMin();
        std::string btn_label = "Button";
        if (node.HasAttribute("value")) btn_label = node.attributes["value"];
        if (ImGui::Button((btn_label + "##" + std::to_string(node.id)).c_str(), btn_size))
        {
            if (node.HasAttribute("onclick"))
            {
                std::cout << "Doin an onclick: " << node.attributes["onclick"] << std::endl;
                /*
                std::string js = attributes["onclick"];
                JSEngine::ExecuteJS(js, renderingTab);*/
            }
            Node* possibleFormParent = node.GetParentNodeOfType("form");
            if (possibleFormParent != nullptr)
            {
                SubmitForm(possibleFormParent, *renderingTab->currentPage);
            }
        }
    }
    else if (node.HasAttribute("type") && node.attributes["type"] == "hidden")
    {
        
    }
    else if(node.HasAttribute("type") && node.attributes["type"] == "text")
    {
        int maxValueLength = 256;
        if (node.HasAttribute("maxlength"))
        {
            TryCastToInt(node.attributes["maxlength"], maxValueLength);
        }
        static char buffer[256] = "";
        ImVec2 input_size = node.layout.contentSize;
        ImVec2 input_pos = ImGui::GetItemRectMin();
        input_pos += ImVec2(0.0f, 8.0f);
        ImGui::SetCursorScreenPos(input_pos);
        ImGui::InputTextMultiline(("##" + std::to_string(node.id)).c_str(), buffer, sizeof(buffer), node.layout.contentSize);
        static_cast<TextInputMutableState*>(renderingTab->currentPage->mutableState[node.id].get())->inputText = std::string(buffer);
    }
}

inline void RenderText(std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer, std::string text, ImU32 color, ImVec2 contentSize, float fontSize = 13.0f)
{
    ImVec2 text_pos = ImGui::GetCursorScreenPos();
    ImGui::Dummy(contentSize);
    queue.insert({ layer, [=](ImDrawList* dl) { dl->AddText(PGUI::io->FontDefault, fontSize, text_pos, color, text.c_str()); }});
}
inline void DOMRenderer::RenderChildren(Node& node, std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer)
{
    for (size_t i = 0; i < node.children.size(); i++)
    {
        Render(*node.children[i], queue, layer);
    }
}
void DOMRenderer::RenderImage(Node& node, std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer)
{
    auto it = renderingTab->currentPage->images.find(node.attributes["src"]);
    if (it != renderingTab->currentPage->images.end() && it->second.img != nullptr)
    {
        PImage* image = &it->second;
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 img_size = ImVec2((float)image->width, (float)image->height);
        queue.insert({ layer, [=](ImDrawList* dl) {
            dl->AddImage((void*)(image->img), pos, pos + img_size);
        } });
        ImGui::Dummy(img_size);
    }
    else if (node.HasAttribute("alt"))
    {
        RenderText(queue, layer, node.attributes["alt"], IM_COL32(255, 255, 255, 255), node.layout.contentSize);
    }
}
void DrawDebugBox(const ImVec2& bg_min, const Node& node, const ImVec2& totalImbed, std::multimap<int, std::function<void(ImDrawList*)>>& queue, int& layer)
{
    ImVec2 bg_max = bg_min + node.layout.contentSize + totalImbed;

    ImU32 color = IM_COL32(255, 0, 255, 20);
    if (node.type == "#text") color = IM_COL32(255, 255, 0, 140);
    if (node.type == "body") return;
    if (node.type == "html") return;

    queue.insert({ layer, [=](ImDrawList* dl) {
        dl->AddRectFilled(bg_min, bg_max, color);
    } });
}
void DisplayDebugText(const char* text)
{
    ImVec2 textSize = ImGui::CalcTextSize(text, nullptr);
    ImVec2 availSize = ImGui::GetContentRegionAvail();
    ImGuiStyle style = ImGui::GetStyle();
    float x = ImGui::GetCursorPosX() + availSize.x - textSize.x - style.WindowPadding.x;
    float y = ImGui::GetCursorPosY() + availSize.y - textSize.y - style.WindowPadding.y;

    ImGui::SetCursorPos(ImVec2(x, y));
    ImGui::Text("%s", text);
}
inline void DOMRenderer::RenderContents(Node& node, std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer, const ImVec2& bg_min, const ImVec2& totalImbed)
{
    if (node.type == "img" && node.attributes.find("src") != node.attributes.end())
    {
        RenderImage(node, queue, layer);
    }
    if (node.type == "#text")
    {
        RenderTextElement(node, queue, layer);
    }

    if (node.HasAttribute("href")) ProcessLink(bg_min, node, totalImbed);
    if (node.type == "input") DrawInput(node);
    if (node.type == "select") DrawSelect(node);

    if (Layout::debuggingLayout && renderingTab->hoveringNode == &node)
    {
        DrawDebugBox(bg_min, node, totalImbed, queue, layer);
        DisplayDebugText(PrintNode(&node, 0, 2).c_str());
    }
    //if (node.type == "div") DrawDiv(bg_min, node, totalImbed, queue, layer);
}