#include "Tab.h"
#include "PickleGUI.h"
#include <chrono>
#include "../imgui/imgui.h"
#include <iostream>
#include "CSSBuilder.h"
#include <imgui/imgui_internal.h>

std::vector<Tab> tabs;
int maxTabID = 0;
char tryNewTabBuffer[256];
Tab* renderingTab;

void ValidateURL(char* urlBuffer, size_t bufferSize)
{
    std::string urlBufferStr(urlBuffer);
    if (urlBufferStr[0] == '\"' && urlBufferStr[urlBufferStr.size() - 1] == '\"')
    {
        urlBufferStr = urlBufferStr.substr(1, urlBufferStr.size() - 2);
        std::cout << urlBufferStr;
        strncpy_s(urlBuffer, bufferSize, urlBufferStr.c_str(), bufferSize - 1);
        urlBuffer[bufferSize - 1] = '\0'; // Ensure null-termination
    }
    if (urlBufferStr.find("http", 0) != 0 && urlBufferStr[1] != ':')
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
    currentPage = std::make_unique<Website>(urlBuffer, this);
    auto current_time = std::chrono::high_resolution_clock::now();
    maxTabID++;
    this->id = maxTabID;
    char name[256];
    std::snprintf(name, sizeof(name), "%s###%d", currentPage->name.c_str(), id);
    std::cout << "Creating tab with name: " << name << " ID: " << id << std::endl;
    viewingSource = false;
    viewingDOM = false;
    viewingCSS = false;
    waitingToClose = false;
    attemptingToLoadNewPage = false;
    firstFrame = true;
}
void Tab::SetupDocking()
{
    char tabWindowName[256];
    std::snprintf(tabWindowName, sizeof(tabWindowName), "%s###%d", currentPage->name.c_str(), id);
    if (firstFrame)
    {
        PGUI::DockWindowToTabBar(tabWindowName);
        firstFrame = false;
    }
    dockID = ImGui::FindWindowByName(tabWindowName)->DockId;
    bool floating = dockID == 0;
    if (floating)
    {
        ImGuiID floatingDockNodeID = ImGui::DockBuilderAddNode(0, ImGuiDockNodeFlags_None);
        ImGuiDockNode* node = ImGui::DockBuilderGetNode(floatingDockNodeID);
        node->Pos = winPos;
        node->Size = winSize;
        node->SizeRef = winSize;
        dockID = floatingDockNodeID;
    }

    ImGuiID tabDock;
    ImGuiID rightPannelDock;
    ImGui::DockBuilderSplitNode(dockID, ImGuiDir_Right, 0.3f, &rightPannelDock, &tabDock);
    char cssID[256], domID[256], sourceID[256];
    std::snprintf(cssID, sizeof(cssID), "%s CSS###CSS %d", currentPage->name.c_str(), id);
    std::snprintf(domID, sizeof(domID), "%s DOM###DOM %d", currentPage->name.c_str(), id);
    std::snprintf(sourceID, sizeof(sourceID), "%s Source###Source %d", currentPage->name.c_str(), id);

    ImGui::Begin(cssID); ImGui::End();
    ImGui::Begin(domID); ImGui::End();
    ImGui::Begin(sourceID); ImGui::End();


    ImGui::DockBuilderDockWindow(cssID, rightPannelDock);
    ImGui::DockBuilderDockWindow(domID, rightPannelDock);
    ImGui::DockBuilderDockWindow(sourceID, rightPannelDock);
    ImGui::DockBuilderDockWindow(tabWindowName, tabDock);
    if(floating) ImGui::DockBuilderFinish(dockID);
}
void Tab::OnResize()
{
    if (currentPage->loaded)
    {
        std::shared_ptr<std::vector<Node>> domSnapshot = currentPage->nodes.load();
        for (size_t i = 0; i < (*domSnapshot).size(); i++)
        {
            (*domSnapshot)[i].CaculatePosition(contentSize.x, *currentPage);
		}
        currentPage->nodes.store(domSnapshot);
    }
}
void Tab::Render()
{
    renderingTab = this;
    char name[256];
    std::snprintf(name, sizeof(name), "%s###%d", currentPage->name.c_str(), id);
    bool isVisible = ImGui::Begin(name);
    if (!isVisible)
    {
        ImGui::End();
        renderingTab = nullptr;
        return;
    }
	winPos = ImGui::GetWindowPos();
	ImVec2 newWinSize = ImGui::GetWindowSize();
    if (newWinSize.x != winSize.x || newWinSize.y != winSize.y)
    {
        winSize = newWinSize;
        OnResize();
	}
    contentSize = ImGui::GetContentRegionAvail();

    RenderURLBar();

    if (attemptingToLoadNewPage && currentPage->loaded)
    {
        urlHistory.push_back(currentURL);
        currentURL = urlBuffer;
        currentPage = std::make_unique<Website>(urlBuffer, this);
        attemptingToLoadNewPage = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Close Tab")) waitingToClose = true;

    if (currentPage->loaded && !currentPage->wasLoaded)
    {
        currentPage->wasLoaded = true;
        if (currentPage->name.find("301") != string::npos && false)
        {
            size_t urlStart = currentPage->content.find("The document has moved <a href=\"");
            size_t urlEnd = currentPage->content.find('"', urlStart);
            std::string url = currentPage->content.substr(urlStart, urlEnd);
            currentURL = url;
            currentPage = std::make_unique<Website>(url, this);
        }
    }

    
    try
    {
        DisplayPage();
    }
    catch (const std::exception& e)
    {
        ImGui::Text("Error rendering page: %s", e.what());
	}


    if (PGUI::io->KeyAlt && ImGui::IsKeyPressed(ImGuiKey_S, false) && ImGui::IsWindowFocused()) viewingSource = !viewingSource;
    if (PGUI::io->KeyAlt && ImGui::IsKeyPressed(ImGuiKey_D, false) && ImGui::IsWindowFocused()) viewingDOM = !viewingDOM;
    if (PGUI::io->KeyAlt && ImGui::IsKeyPressed(ImGuiKey_C, false) && ImGui::IsWindowFocused()) viewingCSS = !viewingCSS;
    ImGui::End();

    if (viewingSource) DisplaySource();
    if (viewingDOM) DisplayDOM();
    if (viewingCSS) DisplayCSS();
    renderingTab = nullptr;
    UpdateDocking();
}
void Tab::UpdateDocking()
{
    if (firstFrame) SetupDocking();
    if (!displayingRightPannel && (viewingCSS || viewingDOM || viewingSource))
    {
        displayingRightPannel = true;
        SetupDocking();
    }
    if (displayingRightPannel && !(viewingCSS || viewingDOM || viewingSource))
    {
        displayingRightPannel = false;
        SetupDocking();
    }
}
void Tab::RenderURLBar()
{
    if (urlHistory.size() > 0 && ImGui::Button("Back"))
    {
        currentURL = urlHistory[urlHistory.size() - 1];
		strncpy_s(urlBuffer, currentURL.c_str(), sizeof(urlBuffer) - 1);
        urlHistory.pop_back();
        attemptingToLoadNewPage = true;
    }
    ImGui::SameLine();
    if (ImGui::InputText("URL", urlBuffer, IM_ARRAYSIZE(urlBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
    {
        ValidateURL(urlBuffer, sizeof(urlBuffer));
        attemptingToLoadNewPage = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Get Page"))
    {
        ValidateURL(urlBuffer, sizeof(urlBuffer));
        attemptingToLoadNewPage = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Refresh"))
    {
        attemptingToLoadNewPage = true;
    }
}
void Tab::DisplayPage()
{
    std::multimap<int, std::function<void(ImDrawList*)>> draw_queue;
    std::shared_ptr<std::vector<Node>> domSnapshot = currentPage->nodes.load();

    contentPos = ImGui::GetCursorPos();

    /*
    for (size_t i = 0; i < (*domSnapshot).size(); i++)
    {
        (*domSnapshot)[i].CaculatePosition(contentSize.x, *currentPage);
    }*/
    for (size_t i = 0; i < (*domSnapshot).size(); i++)
    {
        (*domSnapshot)[i].Render(draw_queue, 0);
    }
    ImDrawList* dl = ImGui::GetWindowDrawList();
    for (auto& pair : draw_queue) {
        pair.second(dl);
    }
}
void Tab::DisplayCSS()
{
    if (currentPage == nullptr) return;
    char name[256];
    std::snprintf(name, sizeof(name), "%s CSS###CSS %d", currentPage->name.c_str(), id);
    ImGui::Begin(name);
    ImVec2 size = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
    ImGui::InputTextMultiline(
        "##LoadedPageCSS",
        (char*)(currentPage->interpretedCSS).c_str(),
        (currentPage->interpretedCSS).size() + 1,
        size,
        ImGuiInputTextFlags_ReadOnly
    );
    ImGui::End();
}
void Tab::DisplayDOM()
{
    char name[256];
    std::snprintf(name, sizeof(name), "%s DOM###DOM %d", currentPage->name.c_str(), id);
    currentPage->interpretedDOM = PrintDOM(*currentPage->ReadNodes());
    ImGui::Begin(name);
    ImVec2 size = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
    ImGui::InputTextMultiline(
        "##LoadedPageDOM",
        (char*)(currentPage->interpretedDOM).c_str(),
        (currentPage->interpretedDOM).size() + 1,
        size,
        ImGuiInputTextFlags_ReadOnly
    );
    ImGui::End();
}
void Tab::DisplaySource()
{
    char name[256];
    std::snprintf(name, sizeof(name), "%s Source###Source %d", currentPage->name.c_str(), id);
    ImGui::Begin(name);
    std::string* currentDisplayText = PGUI::io->KeyAlt ? &currentPage->content : &currentPage->rawTextVersion;
    ImVec2 size = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
    ImGui::InputTextMultiline(
        "##LoadedPageSource",
        (char*)(*currentDisplayText).c_str(),
        (*currentDisplayText).size() + 1,
        size,
        ImGuiInputTextFlags_ReadOnly
    );
    ImGui::End();
}

void Tab::SwitchPage(std::string url)
{
    strncpy_s(urlBuffer, url.c_str(), sizeof(urlBuffer) - 1);
    attemptingToLoadNewPage = true;
}
