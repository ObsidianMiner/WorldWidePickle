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

// Main code
int main(int, char**)
{

    PGUI::PGUI();

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;

    std::vector<Tab> tabs;
    
    tabs.emplace_back("http://example.com", 1);

    // Main loop
    bool done = false;
    while (!done)
    {
        done = PGUI::HandleWindow();
        if (done) break;

        PGUI::StartFrame();


        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
        
        ImGui::Begin("TopBar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiDockNodeFlags_HiddenTabBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        if (ImGui::Button("Create New Tab"))
        {
            tabs.emplace_back("http://example.com");
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f /  (*PGUI::io).Framerate, (*PGUI::io).Framerate);
        ImGui::End();

        for (size_t i = 0; i < tabs.size(); i++)
        {
            tabs[i].Render();
        }
        tabs.erase(std::remove_if(tabs.begin(), tabs.end(), [](const Tab& w) { return !w.waitingToClose; }), tabs.end());

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        ImGui::End();
        // Rendering
        PGUI::Render();
        
    }
	std::cout << "Cleaning up and exiting..." << std::endl;
    PGUI::Cleanup();

    return 0;
}