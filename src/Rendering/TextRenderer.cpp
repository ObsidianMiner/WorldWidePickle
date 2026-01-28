#include "DOMRenderer.h"
#include "Util/StringUtils.h"
#include "UI/PickleGUI.h"

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
inline void DOMRenderer::RenderTextElement(Node& node, std::multimap<int, std::function<void(ImDrawList*)>>& queue, int layer)
{
    float fontSize = GetFontSize(node.style);
    ImU32 color = IM_COL32(255, 255, 255, 255);
    std::string text = node.attributes.at("text");
    if (node.style.find("color") != node.style.end() && std::holds_alternative<ImU32>(node.style["color"]))
    {
        color = std::get<ImU32>(node.style["color"]);
    }
    RenderTextWrapped(queue, layer, text, color, fontSize, node.contentSize.x, node.posInLine);
}