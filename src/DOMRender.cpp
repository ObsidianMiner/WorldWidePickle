#include "DOM.h"
#include "PickleGUI.h"
#include <iostream>
#include "WWP.h"
#include "Tab.h"
#include "ImMath.h"
#include <curl/curl.h>
#include "StringCasting.h"

void Node::Render(std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer)
{
    if (type == "head") return;


	if (HasStyle("padding")) padding = ImVec2(std::get<int>(style["padding"]), std::get<int>(style["padding"]));
    ImVec2 totalImbed = padding + border + topLeftMargin;
    ImVec2 bg_min = ImGui::GetCursorScreenPos() - totalImbed;
    ImVec2 cursor = ImGui::GetCursorScreenPos();

    ImGui::BeginGroup();


    RenderContents(queue, layer);
    RenderChildren(queue, layer + 1);

    ImGui::EndGroup();
    if (type == "div")
    {
        ImVec2 bg_max = ImGui::GetItemRectMax() + totalImbed;

        ImU32 bg_color = IM_COL32(60, 60, 60, 180);
        float rounding = 6.0f;
        ImU32 border_color = IM_COL32(255, 255, 255, 40);
        queue.insert({ layer, [=](ImDrawList* dl) { dl->AddRectFilled(bg_min, bg_max, bg_color, rounding); } });
        queue.insert({ layer, [=](ImDrawList* dl) { dl->AddRect(bg_min, bg_max, border_color, rounding); } });
    }
    
    if (HasAttribute("href"))
    {
        ImVec2 bg_max = ImGui::GetItemRectMax() + totalImbed;
        ImVec2 bg_size = bg_max - bg_min;
        if (bg_size.x > 0 && bg_size.y > 0)
        {
            ImVec2 cursor = ImGui::GetCursorScreenPos();
            ImVec2 localPos = bg_min - ImGui::GetWindowPos();

            ImVec2 savedCursor = ImGui::GetCursorPos();
            ImGui::SetCursorPos(localPos);

            if (ImGui::InvisibleButton(std::to_string(id).c_str(), bg_size)) {
                std::string url = attributes["href"];
                if (url[0] == '/') url = renderingTab->currentURL + url;

                std::cout << "Opening URL: " << url << std::endl;
                renderingTab->SwitchPage(url);
            }
            ImGui::SetCursorPos(savedCursor);
        }
        
    }

    if (type == "input")
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
    ImGui::Dummy(padding);
}
inline float GetFontSize(std::unordered_map<std::string, RuleValue> style)
{
    float fS = 20.0f;
    if (style.find("font-size") != style.end())
    {
        if (std::holds_alternative<int>(style["font-size"]))fS = std::get<int>(style["font-size"]);
        else if (std::holds_alternative<float>(style["font-size"])) fS = std::get<float>(style["font-size"]);
    }
    return fS;
}
inline void RenderText(std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer, std::string text, ImU32 color, float fontSize = 13.0f)
{
    ImVec2 text_pos = ImGui::GetCursorScreenPos();
    ImVec2 text_size = ImGui::CalcTextSize(text.c_str());
    ImGui::Dummy(text_size * (fontSize / 20));
    queue.insert({ layer, [=](ImDrawList* dl) { dl->AddText(PGUI::io->FontDefault, fontSize, text_pos, color, text.c_str()); } });
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
        RenderText(queue, layer, attributes["alt"], IM_COL32(255, 255, 255, 255));
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
	RenderText(queue, layer, text, color, fontSize);
}
inline void Node::RenderContents(std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer)
{
    ImGui::SetCursorPos(GlobalPos() + renderingTab->contentPos);
    if (type == "img" && attributes.find("src") != attributes.end())
    {
        RenderImage(queue, layer);
    }
    if (type == "#text")
    {
        RenderTextElement(queue, layer);
    }
}

void Node::CaculatePosition(float parentWidth, Website&  website)
{
    return;
    contentSize = ImVec2(parentWidth, 0);

    float currentY = 0;
    if (children.size() > 0)
    {
        for (size_t i = 0; i < children.size(); i++)
        {
            children[i].CaculatePosition(parentWidth, website);

            //Caculate childrens marigns
			
            
            children[i].localPos = ImVec2(children[i].topLeftMargin.x + children[i].border.x + children[i].padding.x, currentY);
            currentY = children[i].localPos.y + children[i].contentSize.y + children[i].padding.y;
        }
    }
    contentSize.y = currentY;

    if (type == "img" && attributes.find("src") != attributes.end())
    {
        auto it = website.images.find(attributes["src"]);
        if (it != website.images.end())
        {
            PImage* image = &it->second;
            contentSize.y += ImVec2((float)image->width, (float)image->height).y;
            return;
        }
    }
    if (type == "#text" && attributes.find("text") != attributes.end())
    {
        const char* text = attributes.at("text").c_str();
        float fontSize = GetFontSize(style);
        contentSize.y += (ImGui::CalcTextSize(text) * (fontSize / 20)).y;
    }
}
void Node::CaculateContentSize()
{

}