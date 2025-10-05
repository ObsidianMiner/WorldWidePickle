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
	Tab(string url);
	void Render();
	void SwitchPage(string url);
private:
	bool viewingSource;
	bool attemptingToLoadNewPage;
	vector<string> urlHistory;
};
extern vector<Tab> tabs;
extern int maxTabID;
extern Tab* renderingTab;