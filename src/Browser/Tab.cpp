/**
* @file Tab.cpp
* @brief The tab that is rendered to the user.
*/

#include "Tab.h"
#include "UI/PickleGUI.h"
#include <chrono>
#include "../imgui/imgui.h"
#include "CSS/CSSBuilder.h"
#include <imgui/imgui_internal.h>
#include "Util/ImMath.h"
#include "BrowserConsole.h"

std::vector<Tab> tabs;
int maxTabID = 0;
char tryNewTabBuffer[256];
Tab* renderingTab;


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

    BrowserConsole::Log("Tab", "Creating tab with Name: " + std::string(name) + " ID: " + std::to_string(id));
    viewingSource = false;
    viewingDOM = false;
    viewingCSS = false;
    waitingToClose = false;
    attemptingToLoadNewPage = false;
    firstFrame = true;
    attemptingToLoadFromHistory = false;
    displayingRightPannel = false;
    dockID = -1;
}
void ValidateURL(char* urlBuffer, size_t bufferSize)
{
    std::string urlBufferStr(urlBuffer);
    if (urlBufferStr[0] == '\"' && urlBufferStr[urlBufferStr.size() - 1] == '\"')
    {
        urlBufferStr = urlBufferStr.substr(1, urlBufferStr.size() - 2);
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
    justResized = true;
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

    //Update Tab Window
	winPos = ImGui::GetWindowPos();
	ImVec2 newWinSize = ImGui::GetWindowSize();
    if (newWinSize.x != winSize.x || newWinSize.y != winSize.y)
    {
        winSize = newWinSize;
        OnResize();
	}
    contentSize = ImGui::GetContentRegionAvail();

    if (currentPage->loaded)
    {
        std::shared_ptr<std::vector<Node>> domSnapshot = currentPage->nodes.load();
        for (size_t i = 0; i < (*domSnapshot).size(); i++)
        {
            currentPage->layout.LayoutChildren((*domSnapshot)[i], contentSize.x, *currentPage);
        }
        currentPage->nodes.store(domSnapshot);
        justResized = false;
    }

    //URL Bar
    RenderURLBar();

    if (attemptingToLoadNewPage && currentPage->loaded)
    {
        if(!attemptingToLoadFromHistory) urlHistory.push_back(currentURL);
        currentURL = urlBuffer;
        currentPage = std::make_unique<Website>(urlBuffer, this);
        attemptingToLoadNewPage = false;
        attemptingToLoadFromHistory = false;
    }

    if (currentPage->loaded && !currentPage->wasLoaded) OnPageLoaded();

    //Display Page
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
    if (PGUI::io->KeyAlt && ImGui::IsKeyPressed(ImGuiKey_L, false) && ImGui::IsWindowFocused()) Layout::debuggingLayout = !Layout::debuggingLayout;
    ImGui::End();

    if (viewingSource) DisplayDebugMenu("Source", currentPage->websiteSource);
    if (viewingDOM) DisplayDebugMenu("DOM", currentPage->interpretedDOM);
    if (viewingCSS) DisplayDebugMenu("CSS", currentPage->interpretedCSS);
    renderingTab = nullptr;
    UpdateDocking();
}
void Tab::OnPageLoaded()
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
    bool smallTopBar = winSize.x < 550;
    float backButtonSize = (urlHistory.size() > 0 ? 45 : 0);
    
    if (urlHistory.size() > 0 && ImGui::Button("Back"))
    {
        currentURL = urlHistory[urlHistory.size() - 1];
		strncpy_s(urlBuffer, currentURL.c_str(), sizeof(urlBuffer) - 1);
        urlHistory.pop_back();
        attemptingToLoadNewPage = true;
        attemptingToLoadFromHistory = true;
    }
    ImGui::SameLine();
    ImGui::PushItemWidth(winSize.x - (smallTopBar ? 110 : 210) - backButtonSize);
    if (ImGui::InputText("URL", urlBuffer, IM_ARRAYSIZE(urlBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
    {
        ValidateURL(urlBuffer, sizeof(urlBuffer));
        attemptingToLoadNewPage = true;
    }
    ImGui::SameLine();
    /*
    string getPageButtonText = "Get Page";
    if (smallTopBar) getPageButtonText = "->";
    if (ImGui::Button(getPageButtonText.c_str()))
    {
        ValidateURL(urlBuffer, sizeof(urlBuffer));
        attemptingToLoadNewPage = true;
    }*/
    ImGui::SameLine();
    string refreshButtonText = "Refresh";
    if (smallTopBar) refreshButtonText = "O";
    if (ImGui::Button(refreshButtonText.c_str()))
    {
        attemptingToLoadNewPage = true;
    }
    ImGui::SameLine();
    string closeButtonText = "Close Tab";
    if (smallTopBar) closeButtonText = "X";
    if (ImGui::Button(closeButtonText.c_str())) waitingToClose = true;
}
/// <summary>
/// Takes a snapshot of the current DOM and renders it to the screen.
/// </summary>
void Tab::DisplayPage()
{
    std::multimap<int, std::function<void(ImDrawList*)>> draw_queue;
    std::shared_ptr<std::vector<Node>> domSnapshot = currentPage->nodes.load();

    contentPos = ImGui::GetCursorPos() + ImVec2(0, 20);
    contentScreenPos = ImGui::GetCursorScreenPos() + ImVec2(0, 20);

    //Generate Draw Queue
    for (size_t i = 0; i < (*domSnapshot).size(); i++)
    {
        renderer.Render((*domSnapshot)[i], draw_queue, 0);
    }
    //Draw Everything in the draw queue
    ImDrawList* dl = ImGui::GetWindowDrawList();
    for (auto& pair : draw_queue) {
        pair.second(dl);
    }
}
void Tab::DisplayDebugMenu(string type, string content)
{
    if (currentPage == nullptr) return;
    char name[256];
    std::snprintf(name, sizeof(name), "%s %s###%s %d", currentPage->name.c_str(), type.c_str(), type.c_str(), id);
    ImGui::Begin(name);
    ImVec2 size = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
    char title[256];
    std::snprintf(name, sizeof(title), "##LoadedPage%s", type.c_str());
    
    ImGui::InputTextMultiline(
        title,
        (char*)(content).c_str(),
        content.size() + 1,
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