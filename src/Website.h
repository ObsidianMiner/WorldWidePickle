#pragma once
#include <string>
#include "Node.h"
#include "stb/stb_image.h"
#include <d3d10_1.h>
#include <d3d10.h>
#include <variant>
#include <atomic>
#include <memory>
#include <Layout/Layout.h>

struct PImage
{
	int width;
	int height;
	ID3D10ShaderResourceView* img;
};
class Tab;
class Website
{
public:
	std::string name;
	std::string url;
	std::string content;
	/// <summary>
	/// Only ever used to diplay it in the debug popout.
	/// </summary>
	std::string websiteSource;
	std::atomic<std::shared_ptr<Node>> rootNode;
	std::unordered_map<std::string, PImage> images;
	std::unordered_map<std::string, std::unordered_map<std::string, RuleValue>> styles;
	bool loaded;
	bool wasLoaded;
	int highestID;
	Tab* tab;

	Layout layout;

	std::string interpretedDOM;
	std::string interpretedCSS;

	/// <summary>
	/// Getsa a copy of the nodes. Parent pointers are broken though and still point to the original tree!
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<Node> ReadTree() { return std::make_shared<Node>(*rootNode.load()); }

	Website(std::string url, Tab* tab);
	~Website();
	void FetchContent();
	void CreateRawTextVersion();
	const char* getContent() const;

private:
	void StyleAndLayoutNodes();
};
PImage LoadImgFromWebsiteURL(const std::string& url);

/// <summary>The URL can be modified if a redirect is nessasary.</summary>/// <param name="url"></param>/// <param name="isHTMLWebsite"></param>/// <returns></returns>
std::string GetTextFromURL(std::string& url, bool isHTMLWebsite = false);

std::string GetTextFromFileOrURL(std::string& url, bool isHTMLWebsite = false);