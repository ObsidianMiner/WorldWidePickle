/**
* @file Browser.cpp
* @brief Performs the main loop, and loads up the first tab, topbar, globalCSS, and PickleGUI
*/

#include "UI/PickleGUI.h"
#include <iostream>
#include "Tab.h"
#include "Topbar.h"
#include "CSS/CSSBuilder.h"
#include "UI/WWPStyles.h"
#include "Browser.h"
#include "BrowserConsole.h"

using namespace std::string_literals;

TopBar topbar;


void Browser::SetupUI()
{
    PGUI::PGUI();
    SetPurpleStyle();
    BrowserConsole::Log("Browser", "Setup UI");
}
void Browser::LoadGlobalStyles()
{
    std::string globalPath = "C:\\Users\\JGEle\\source\\repos\\WorldWidePickle\\x64\\Debug\\global.css"s;
    std::string globalCSS = GetTextFromFileOrURL(globalPath, false);
    CSSBuilder globalStylesBuilder(globalCSS);
    globalStyles = globalStylesBuilder.ParseCSS();
    BrowserConsole::Log("Browser", "Loaded Global Styles");
}
void Browser::LoadHomePage()
{
    tabs.emplace_back("https://example.com");
}
void Browser::RenderTabs()
{
    for (size_t i = 0; i < tabs.size(); i++)
    {
        tabs[i].Render();
    }
    tabs.erase(std::remove_if(tabs.begin(), tabs.end(), [](const Tab& w) { return w.waitingToClose; }), tabs.end());
}

void Browser::MainLoop()
{
    bool done = false;
    while (!done)
    {
        done = PGUI::HandleWindow();
        if (done) break;
        if (PGUI::minimized) continue;

        PGUI::StartFrame();

        topbar.RenderTopBar();
        RenderTabs();

        ImGui::End();

        PGUI::Render();
    }
}
void Browser::Cleanup()
{
    std::cout << "Cleaning up and exiting..." << std::endl;
    PGUI::Cleanup();
}