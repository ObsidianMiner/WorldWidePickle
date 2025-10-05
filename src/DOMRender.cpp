#include "DOM.h"
#include "PickleGUI.h"
#include <iostream>
#include "WWP.h"
#include "Tab.h"

void Node::Render()
{
    for (size_t i = 0; i < children.size(); i++)
    {
        children[i].Render();
    }
    if (innerText.size() > 0)
    {
        if (attributes.find("href") != attributes.end()) {
            if (ImGui::Button(innerText.c_str())) {
                std::string url = attributes["href"];
                if (url[0] == '/') url = renderingTab->currentURL + url;

                std::cout << "Opening URL: " << url << std::endl;
                renderingTab->SwitchPage(url);
            }
        }
        else
        {
            ImGui::Text(innerText.c_str());
        }
    }
}