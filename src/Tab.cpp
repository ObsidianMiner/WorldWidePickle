#include "Tab.h"
#include "PickleGUI.h"
#include <chrono>
#include "../imgui/imgui.h"
#include <iostream>


void validateURL(char* urlBuffer, size_t bufferSize)
{
    std::string urlBufferStr(urlBuffer, bufferSize);
    if (urlBufferStr.find("http", 0) != 0)
    {
        urlBufferStr.insert(0, "https://");
        strncpy_s(urlBuffer, bufferSize, urlBufferStr.c_str(), bufferSize - 1);
        urlBuffer[bufferSize - 1] = '\0'; // Ensure null-termination
    }
}
Tab::Tab(std::string url, unsigned int id)
{
    strncpy_s(urlBuffer, url.c_str(), sizeof(urlBuffer) - 1);
    currentPage = std::make_unique<Website>(urlBuffer);
    auto current_time = std::chrono::high_resolution_clock::now();
    auto elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(current_time.time_since_epoch()).count();
    if (id == 0) this->id = elapsed_milliseconds;
    else this->id = id;
    char name[256];
    std::snprintf(name, sizeof(name), "Tab##%d", this->id);
    viewingSource = false;
    waitingToClose = false;
}
void Tab::Render()
{
    char name[256];
    std::snprintf(name, sizeof(name), "Tab##%d", id);
    ImGui::Begin(name);                          // Create a window called "Hello, world!" and append into it.

    //ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    //ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

    //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    //ImGui::ColorEdit3("clear color", (float*)&PGUI::clear_color); // Edit 3 floats representing a color
    if (ImGui::InputText("URL", urlBuffer, IM_ARRAYSIZE(urlBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
    {
        validateURL(urlBuffer, sizeof(urlBuffer));
        currentPage = std::make_unique<Website>(urlBuffer);
    }
    ImGui::SameLine();
    if (ImGui::Button("Get Page"))
    {
        validateURL(urlBuffer, sizeof(urlBuffer));
        currentPage = std::make_unique<Website>(urlBuffer);
    }
    ImGui::SameLine();
    if (ImGui::Button("Close Tab"))
    {
        waitingToClose = true;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_S, false) && ImGui::IsWindowFocused()) viewingSource = !viewingSource;
    if (viewingSource)
    {
        std::string* currentDisplayText = PGUI::io->KeyAlt ? &currentPage->content : &currentPage->rawTextVersion;
        ImVec2 size = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetTextLineHeight() * 20);
        ImGui::InputTextMultiline(
            "##LoadedPage",
            (char*)(*currentDisplayText).c_str(),
            (*currentDisplayText).size() + 1,
            size,
            ImGuiInputTextFlags_ReadOnly
        );
    }
    else
    {
        for (size_t i = 0; i < currentPage->nodes.size(); i++)
        {
            currentPage->nodes[i].Render();
        }
    }
    ImGui::End();
}