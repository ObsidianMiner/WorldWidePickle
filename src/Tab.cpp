#include "Tab.h"
#include "PickleGUI.h"
#include <chrono>
#include "../imgui/imgui.h"
#include <iostream>

std::vector<Tab> tabs;
int maxTabID = 0;
char tryNewTabBuffer[256];
Tab* renderingTab;

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
Tab::Tab(std::string url)
{
    strncpy_s(urlBuffer, url.c_str(), sizeof(urlBuffer) - 1);
    currentURL = urlBuffer;
    currentPage = std::make_unique<Website>(urlBuffer);
    auto current_time = std::chrono::high_resolution_clock::now();
    maxTabID++;
    this->id = maxTabID;
    char name[256];
    std::snprintf(name, sizeof(name), "Tab##%d", this->id);
    viewingSource = false;
    waitingToClose = false;
    attemptingToLoadNewPage = false;
}
void Tab::Render()
{
    renderingTab = this;
    char name[256];
    std::snprintf(name, sizeof(name), "Tab##%d", id);
    ImGui::Begin(name);

    if (urlHistory.size() > 0 && ImGui::Button("Back"))
    {
        currentURL = urlHistory[urlHistory.size() - 1];
        urlHistory.pop_back();
        strncpy_s(urlBuffer, currentURL.c_str(), sizeof(urlBuffer) - 1);
        currentPage = std::make_unique<Website>(urlBuffer);
    }
    ImGui::SameLine();
    if (ImGui::InputText("URL", urlBuffer, IM_ARRAYSIZE(urlBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
    {
        validateURL(urlBuffer, sizeof(urlBuffer));
        attemptingToLoadNewPage = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Get Page"))
    {
        validateURL(urlBuffer, sizeof(urlBuffer));
        attemptingToLoadNewPage = true;
    }
    if (attemptingToLoadNewPage)
    {
        urlHistory.push_back(currentURL);
        currentURL = urlBuffer;
        currentPage = std::make_unique<Website>(urlBuffer);
        attemptingToLoadNewPage = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Close Tab")) waitingToClose = true;

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
    renderingTab = nullptr;
}

void Tab::SwitchPage(std::string url)
{
    strncpy_s(urlBuffer, url.c_str(), sizeof(urlBuffer) - 1);
    //validateURL(urlBuffer, sizeof(urlBuffer));
    attemptingToLoadNewPage = true;
}
