#pragma once

#include "Topbar.h"
#include "BrowserConfig.h"

class Browser
{
public:
	bool configOpen;
	BrowserConfig config;

	TopBar topbar;

	Browser();

	void RenderTabs();
	void MainLoop();
	void Cleanup();
	void LoadGlobalStyles();
	void LoadHomePage();
	void SetupUI();
};