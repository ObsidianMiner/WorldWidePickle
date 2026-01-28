#pragma once
#include <memory>
#include "Website.h"
using namespace std;

extern char tryNewTabBuffer[256];
class Tab
{
public:
	bool waitingToClose;
	unsigned int id;
	char urlBuffer[256];
	string currentURL;
	unique_ptr<Website> currentPage;

	ImVec2 contentSize;
	ImVec2 contentPos;
	ImVec2 contentScreenPos;

	Tab(string url);
	void Render();
	void RenderURLBar();
	void DisplayPage();
	void SwitchPage(string url);
private:
	ImVec2 winSize;
	ImVec2 winPos;
	bool viewingSource;
	bool viewingDOM;
	bool viewingCSS;
	bool firstFrame;
	bool displayingRightPannel;
	ImGuiID dockID;

	bool attemptingToLoadNewPage;
	bool attemptingToLoadFromHistory;
	vector<string> urlHistory;
	void SetupDocking();
	void OnResize();
	void DisplaySource();
	void DisplayDOM();
	void DisplayCSS();
	void UpdateDocking();
};
extern vector<Tab> tabs;
extern int maxTabID;
extern Tab* renderingTab;