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
#include <HTML/DebugDOMViewer.h>

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
	std::unordered_map<int, std::unique_ptr<NodeMutableState>> mutableState;
	bool fullyLoaded;
	bool wasLoaded;
	int nextNodeID;
	Tab* tab;

	Layout layout;

	DebugDOMViewer debugDOMViewer;
	std::string interpretedDOM;
	std::string interpretedCSS;

	/// <summary>
	/// Used in building passes to work on a clone of the tree before swaping it.
	/// </summary>
	std::unique_ptr<Node> CloneTree(const Node& node, Node* newParent);

	Node* GetNodeFromIdInChildren(Node* node, int id);

	Website(std::string url, Tab* tab);
	~Website();
	void InitalizeMutableData(Node& node, bool recursive);
	void FetchContent();
	void CreateRawTextVersion();
	const char* getContent() const;

private:
	void StyleAndLayoutNodes(std::shared_ptr<Node> tree);
};
PImage LoadImgFromWebsiteURL(const std::string& url);

/// <summary>The URL can be modified if a redirect is nessasary.</summary>/// <param name="url"></param>/// <param name="isHTMLWebsite"></param>/// <returns></returns>
std::string GetTextFromURL(std::string& url, bool isHTMLWebsite = false);

std::string GetTextFromFileOrURL(std::string& url, bool isHTMLWebsite = false);