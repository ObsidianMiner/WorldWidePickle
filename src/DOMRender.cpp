#include "DOM.h"
#include "PickleGUI.h"
#include <iostream>
#include "WWP.h"
#include "Tab.h"
#include "ImMath.h"


void Node::Render(std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer)
{
    if (type == "div")
    {
        ImVec2 padding = ImVec2(8, 8);
        ImVec2 bg_min = ImGui::GetCursorScreenPos() - padding;
        ImVec2 cursor = ImGui::GetCursorScreenPos();

        ImGui::BeginGroup();


        RenderContents(queue, layer);
        RenderChildren(queue, layer + 1);

        ImGui::EndGroup();
        ImVec2 bg_max = ImGui::GetItemRectMax() + padding;

        ImU32 bg_color = IM_COL32(60, 60, 60, 180);
        float rounding = 6.0f;
        ImU32 border_color = IM_COL32(255, 255, 255, 40);
        queue.insert({ layer, [=](ImDrawList* dl) { dl->AddRectFilled(bg_min, bg_max, bg_color, rounding); } });
        queue.insert({layer, [=](ImDrawList* dl) { dl->AddRect(bg_min, bg_max, border_color, rounding); }});
        ImGui::Dummy(padding);
    }
    else
    {
        RenderContents(queue, layer);
        RenderChildren(queue, layer);
    }
}
inline void Node::RenderChildren(std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer)
{
    for (size_t i = 0; i < children.size(); i++)
    {
        children[i].Render(queue, layer);
    }
}
inline void Node::RenderContents(std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer)
{
    if (innerText.size() > 0)
    {
        if (attributes.find("href") != attributes.end()) {
            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImVec2 text_size = ImGui::CalcTextSize(innerText.c_str());
            if (ImGui::InvisibleButton(innerText.c_str(), text_size)) {
                std::string url = attributes["href"];
                if (url[0] == '/') url = renderingTab->currentURL + url;

                std::cout << "Opening URL: " << url << std::endl;
                renderingTab->SwitchPage(url);
            }
            ImU32 fill_color = IM_COL32(70, 70, 200, 255);
            ImU32 border_color = IM_COL32(255, 255, 255, 255);

            queue.insert({ layer, [=](ImDrawList* dl) {
                //dl->AddRectFilled(pos, pos + text_size, fill_color, 5.0f);
                //dl->AddRect(pos, pos + text_size, border_color, 5.0f);
                dl->AddText(pos, IM_COL32(180, 180, 255, 255), innerText.c_str());
            } });
        }
        else
        {
            ImVec2 text_pos = ImGui::GetCursorScreenPos();
            ImVec2 text_size = ImGui::CalcTextSize(innerText.c_str());
            ImGui::Dummy(text_size);
            queue.insert({ layer, [=](ImDrawList* dl) { dl->AddText(text_pos, IM_COL32(255, 255, 255, 255), innerText.c_str()); } });
        }
    }
}