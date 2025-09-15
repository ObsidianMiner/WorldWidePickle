#pragma once
#include <memory>
#include "Website.h"

class Tab
{
public:
	bool waitingToClose;
	unsigned int id;
	char urlBuffer[256];
	std::unique_ptr<Website> currentPage;
	Tab(std::string url, unsigned int id = 0);
	void Render();
private:
	bool viewingSource;
};