#include "UI/PickleGUI.h"
#include <iostream>
#include "Browser/Tab.h"
#include "Util/ImMath.h"
#include <curl/curl.h>
#include "Util/StringUtils.h"
#include "DOM.h"

/// <summary>
/// Renders a node, and all of it's children. The main inlit to all site rendering.
/// IMPORTANT: Positioning is done in a seperate pass.
/// </summary>
/// <param name="queue">Will be drawn after all nodes are rendered.</param>
/// <param name="layer">Which layer should this node be rendered on.</param>
void Node::Render(std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer)
{
    if (type == "head") return;


	//if (HasStyle("padding")) padding = ImVec2(std::get<int>(style["padding"]), std::get<int>(style["padding"]));
    ImVec2 totalImbed = padding + border;
    ImGui::SetCursorPos(GlobalPos() + renderingTab->contentPos);
    ImVec2 bg_min = ImGui::GetCursorScreenPos() - totalImbed;

    ImGui::BeginGroup();


    RenderContents(queue, layer);
    RenderChildren(queue, layer + 1);
    ImGui::Dummy(ImVec2(0,0));

    ImGui::EndGroup();
    if (type == "div")
    {
        ImVec2 bg_max = bg_min + contentSize + totalImbed;

        ImU32 bg_color = IM_COL32(60, 60, 60, 180);
        float rounding = 6.0f;
        ImU32 border_color = IM_COL32(255, 255, 255, 40);
        queue.insert({ layer, [=](ImDrawList* dl) { dl->AddRectFilled(bg_min, bg_max, bg_color, rounding); } });
        queue.insert({ layer, [=](ImDrawList* dl) { dl->AddRect(bg_min, bg_max, border_color, rounding); } });
    }
    
    if (HasAttribute("href"))
    {
        ImVec2 bg_max = bg_min + contentSize + totalImbed;
        ImVec2 bg_size = bg_max - bg_min;
        if (bg_size.x > 0 && bg_size.y > 0)
        {

            if (ImGui::InvisibleButton(std::to_string(id).c_str(), bg_size)) {
                std::cout << "Clicked";
                std::string url = attributes["href"];
                if (url[0] == '/') url = renderingTab->currentURL + url;

                std::cout << "Opening URL: " << url << std::endl;
                renderingTab->SwitchPage(url);
            }
        }
        
    }

    if (type == "input") DrawInput();
    //ImGui::Dummy(padding);
}
void Node::DrawInput()
{
    if (HasAttribute("type") && (attributes["type"] == "button" || attributes["type"] == "submit"))
    {
        ImVec2 btn_size = ImGui::GetItemRectSize();
        ImVec2 btn_pos = ImGui::GetItemRectMin();
        std::string btn_label = "Button";
        if (HasAttribute("value")) btn_label = attributes["value"];
        if (ImGui::Button(btn_label.c_str(), btn_size))
        {
            if (HasAttribute("onclick"))
            {
                std::cout << "Doin an onclick: " << attributes["onclick"] << std::endl;
                /*
                std::string js = attributes["onclick"];
                WWP::ExecuteJS(js, renderingTab);*/
            }
        }
    }
    else if (HasAttribute("type") && attributes["type"] == "hidden")
    {

    }
    else
    {
        int maxValueLength = 256;
        if (HasAttribute("maxlength"))
        {
            TryCastToInt(attributes["maxlength"], maxValueLength);
        }
        static char buffer[256] = "";
        ImVec2 input_size = ImGui::GetItemRectSize();
        ImVec2 input_pos = ImGui::GetItemRectMin();
        ImGui::SetCursorScreenPos(input_pos);
        ImGui::PushItemWidth(input_size.x);
        ImGui::InputText(std::to_string(id).c_str(), buffer, sizeof(buffer));
        ImGui::PopItemWidth();
        attributes["value"] = std::string(buffer);
    }
}

inline void RenderText(std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer, std::string text, ImU32 color, ImVec2 contentSize, float fontSize = 13.0f)
{
    ImVec2 text_pos = ImGui::GetCursorScreenPos();
    ImGui::Dummy(contentSize);
    queue.insert({ layer, [=](ImDrawList* dl) { dl->AddText(PGUI::io->FontDefault, fontSize, text_pos, color, text.c_str()); }});
}
/// <summary>
/// Renders wrapped text into a specified drawing queue at a given layer.
/// </summary>
/// <param name="queue"></param>
/// <param name="layer"></param>
/// <param name="text">The text to be rendered.</param>
/// <param name="color"></param>
/// <param name="fontSize"></param>
/// <param name="wrapWidth">The maximum width of a line before wrapping occurs. This should include the wrapStart.</param>
/// <param name="wrapStart">The starting x-coordinate for the wrapped text. Defaults to the current cursor screen position.</param>
inline void RenderTextWrapped(std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer, std::string text, ImU32 color,
    float fontSize = 13.0f, float wrapWidth = 0.0f, float wrapStart = 0.0f)
{
    ImVec2 text_pos = ImGui::GetCursorScreenPos();
    //if (wrapStart == 0.0f) wrapStart = text_pos.x;
    //wrapStart += renderingTab->contentScreenPos.x;

    ImFont* font = PGUI::io->FontDefault;
    IM_ASSERT(font && font->IsLoaded());

    if (fontSize <= 0.0f)
        fontSize = ImGui::GetFontSize();

    if (wrapWidth <= 0.0f)
        wrapWidth = ImGui::GetContentRegionAvail().x;

    std::vector<std::string> wrappedLines;

    const char* textStart = text.c_str();
    const char* textEnd = textStart + text.size();
    const char* line = textStart;
    float lineHeight = fontSize;


    while (line < textEnd)
    {
        const char* lineEnd = font->CalcWordWrapPositionA(font->Scale, line, textEnd, wrapWidth);
        wrappedLines.emplace_back(line, lineEnd);
        while (lineEnd < textEnd && (*lineEnd == ' ' || *lineEnd == '\n' || *lineEnd == '\r')) lineEnd++;
        line = lineEnd;
    }
    queue.insert({ layer, [=](ImDrawList* dl) {
        ImVec2 cursor = text_pos;
        float startX = text_pos.x;
        cursor.x += wrapStart;
        for (auto& line : wrappedLines)
        {
            dl->AddText(PGUI::io->FontDefault, fontSize, cursor, color, line.c_str());
            cursor.y += lineHeight;
            cursor.x = startX;
        }
    } });
}
inline void Node::RenderChildren(std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer)
{
    for (size_t i = 0; i < children.size(); i++)
    {
        children[i].Render(queue, layer);
    }
}
inline void Node::RenderImage(std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer)
{
    auto it = renderingTab->currentPage->images.find(attributes["src"]);
    if (it != renderingTab->currentPage->images.end())
    {
        PImage* image = &it->second;
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 img_size = ImVec2((float)image->width, (float)image->height);
        queue.insert({ layer, [=](ImDrawList* dl) {
            dl->AddImage((void*)(image->img), pos, pos + img_size);
        } });
        ImGui::Dummy(img_size);
    }
    else if (attributes.find("alt") != attributes.end())
    {
        RenderText(queue, layer, attributes["alt"], IM_COL32(255, 255, 255, 255), contentSize);
    }
}
inline void Node::RenderTextElement(std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer)
{
    float fontSize = GetFontSize(style);
    ImU32 color = IM_COL32(255, 255, 255, 255);
    std::string text = attributes.at("text");
    if (style.find("color") != style.end() && std::holds_alternative<ImU32>(style["color"]))
    {
        color = std::get<ImU32>(style["color"]);
    }
    RenderTextWrapped(queue, layer, text, color, fontSize, contentSize.x, posInLine);
}
inline void Node::RenderContents(std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer)
{
    if (type == "img" && attributes.find("src") != attributes.end())
    {
        RenderImage(queue, layer);
    }
    if (type == "#text")
    {
        RenderTextElement(queue, layer);
    }
}
