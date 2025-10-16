// Dear ImGui: standalone example application for DirectX 10

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "PickleGUI.h"
#include <iostream>
#include "Tab.h"
#include <imgui/imgui_internal.h>
#include "WWP.h"
#include "Topbar.h"
#include "CSSBuilder.h"

using namespace std::string_literals;

TopBar topbar;

void RenderTabs()
{
    for (size_t i = 0; i < tabs.size(); i++)
    {
        tabs[i].Render();
    }
    tabs.erase(std::remove_if(tabs.begin(), tabs.end(), [](const Tab& w) { return w.waitingToClose; }), tabs.end());
}

// Main code
int main(int, char**)
{
    PGUI::PGUI();
    ImGui::StyleColorsDark();

	std::string globalPath = "C:\\Users\\JGEle\\source\\repos\\WorldWidePickle\\x64\\Debug\\global.css"s;
    std::string globalCSS = GetTextFromFileOrURL(globalPath, false);
    CSSBuilder globalStylesBuilder(globalCSS);
    globalStyles = globalStylesBuilder.ParseCSS();
    
    tabs.emplace_back("https://example.com");

    // Main loop
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
	std::cout << "Cleaning up and exiting..." << std::endl;
    PGUI::Cleanup();

    return 0;
}