#pragma once
#include <string>
#include "DOM.h"

class Website
{
public:
	std::string url;
	std::string content;
	std::string rawTextVersion;
	std::vector<Node> nodes;
	bool loaded;

	Website(std::string url);
	~Website();
	void fetchContent();
	void CreateRawTextVersion();
	const char* getContent() const;
};