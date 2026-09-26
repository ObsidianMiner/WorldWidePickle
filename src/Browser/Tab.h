#pragma once
#include <memory>
#include "Website.h"
#include <Rendering/DOMRenderer.h>
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

	Node* hoveringNode;

	Tab(string url);
	void Render();
	void RenderURLBar();
	void DisplayPage();
	void AddAndDisplayCachedPage(std::string name, std::string cachedPage);
	std::string GetPageFromCache(std::string name);
	std::string GetPageURLFromCacheURL(std::string name);
	void SwitchPage(string url);
private:
	ImVec2 winSize;
	ImVec2 winPos;
	bool viewingSource;
	bool viewingDOM;
	bool viewingCSS;
	bool debuggingHover;
	bool firstFrame;
	bool displayingRightPannel;
	ImGuiID dockID;

	DOMRenderer renderer;

	bool attemptingToLoadNewPage;
	bool attemptingToLoadFromHistory;
	bool justResized;
	vector<string> urlHistory;
	std::unordered_map<std::string, std::string> cachedPages;
	void SetupDocking();
	void OnResize();
	void OnPageLoaded();
	void DisplayDebugMenu(string type, string content);
	void DisplayHoverDebug();
	void UpdateDocking();
};
extern vector<Tab> tabs;
extern int maxTabID;
extern Tab* renderingTab;