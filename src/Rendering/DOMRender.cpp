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



/// <summary>
/// Renders a node, and all of it's children. The main inlit to all site rendering.
/// IMPORTANT: Layout must be done before hand.
/// </summary>
/// <param name="queue">Will be drawn after all nodes are rendered.</param>
/// <param name="layer">Which layer should this node be rendered on.</param>
void DOMRenderer::Render(Node& node, std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer)
{
    if (node.type == "head") return;

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
void DOMRenderer::DrawInput(Node& node)
{
    if (node.HasAttribute("type") && (node.attributes["type"] == "button" || node.attributes["type"] == "submit"))
    {
        ImVec2 btn_size = ImGui::GetItemRectSize();
        ImVec2 btn_pos = ImGui::GetItemRectMin();
        std::string btn_label = "Button";
        if (node.HasAttribute("value")) btn_label = node.attributes["value"];
        if (ImGui::Button(btn_label.c_str(), btn_size))
        {
            if (node.HasAttribute("onclick"))
            {
                std::cout << "Doin an onclick: " << node.attributes["onclick"] << std::endl;
                /*
                std::string js = attributes["onclick"];
                WWP::ExecuteJS(js, renderingTab);*/
            }
        }
    }
    else if (node.HasAttribute("type") && node.attributes["type"] == "hidden")
    {

    }
    else
    {
        int maxValueLength = 256;
        if (node.HasAttribute("maxlength"))
        {
            TryCastToInt(node.attributes["maxlength"], maxValueLength);
        }
        static char buffer[256] = "";
        ImVec2 input_size = ImGui::GetItemRectSize();
        ImVec2 input_pos = ImGui::GetItemRectMin();
        ImGui::SetCursorScreenPos(input_pos);
        ImGui::PushItemWidth(input_size.x);
        ImGui::InputText(std::to_string(node.id).c_str(), buffer, sizeof(buffer));
        ImGui::PopItemWidth();
        node.attributes["value"] = std::string(buffer);
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
        Render(node.children[i], queue, layer);
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

    if(Layout::debuggingLayout && renderingTab->hoveringNode == &node) DrawDebugBox(bg_min, node, totalImbed, queue, layer);
    if (node.type == "div") DrawDiv(bg_min, node, totalImbed, queue, layer);
}