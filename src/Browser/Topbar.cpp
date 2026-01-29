/**
* @file Topbar.cpp
* @brief Renders the TopBar, that allows you create a new tab, and get the fps
*/


#include "Topbar.h"
#include "UI/PickleGUI.h"
#include "Tab.h"
#include <imgui/imgui_internal.h>

void TopBar::RenderTopBar()
{
    ImGui::Begin("TopBar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiDockNodeFlags_HiddenTabBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    if (ImGui::Button("Create New Tab"))
    {
        tabs.emplace_back("http://example.com");
    }

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / (*PGUI::io).Framerate, (*PGUI::io).Framerate);
    ImGui::End();
}