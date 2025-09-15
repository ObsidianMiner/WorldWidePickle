#include "DOM.h"
#include "PickleGUI.h"
#include <iostream>
void Node::Render()
{
    for (size_t i = 0; i < children.size(); i++)
    {
        children[i].Render();
    }
    if (innerText.size() > 0)
    {
        ImGui::Text(innerText.c_str());
    }
}