#pragma once

#include "Topbar.h"

class Browser
{
public:
	TopBar topbar;
	void RenderTabs();
	void MainLoop();
	void Cleanup();
	void LoadGlobalStyles();
	void LoadHomePage();
	void SetupUI();
};