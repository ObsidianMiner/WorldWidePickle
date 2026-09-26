/**
* @file Website.cpp
* @brief Holds a website including the url, content, source, and other data a tab might want.
*/

#include "Website.h"
#include <thread>
#include "Browser/BrowserConsole.h"
#include <regex>
#include "UI/PickleGUI.h"
#include "CSS/CSSBuilder.h"
#include <tidy/tidybuffio.h>
#include "Browser/Tab.h"
#include "HTML/DOMBuilder.h"


std::unique_ptr<Node> Website::CloneTree(const Node& node, Node* newParent) const
{
    auto copy = node.CrClone();
    copy->parent = newParent;
    copy->children.clear();
    for (size_t i = 0; i < node.children.size(); i++)
    {
        copy->children.push_back(CloneTree(*node.children[i], copy.get()));
    }
    return copy;
}

Node* Website::GetNodeFromIdInChildren(Node* node, int id) const
{
    if (node->id == id) return node;
    for (size_t i = 0; i < node->children.size(); i++)
    {
        Node* possibleNodeInChild = GetNodeFromIdInChildren(node->children[i].get(), id);
        if (possibleNodeInChild != nullptr) return possibleNodeInChild;
    }

    return nullptr;
}

std::string Website::ConvertRelativeURLs(std::string url) const
{
    if (url.size() > 0 && url[0] == '/')
    {
        url.insert(0, this->url);
    }
    return url;
}

Website::Website(std::string url, Tab* tab, std::string cachedContents)
{
    fullyLoaded = false;
    wasLoaded = false;
	this->url = url;
    this->tab = tab;
    name = url;
    content = "<h1>Loading...</h1>";
	websiteSource = "<h1>Loading...</h1>";
    styles = globalStyles;
    nextNodeID = 0;
    rootNode = std::make_shared<Node>(nullptr, "rootHTML", nextNodeID++);
	std::thread(&Website::FetchContent, this, cachedContents).detach();
    layout = Layout();
    debugDOMViewer = DebugDOMViewer();
}

Website::~Website()
{
    for (auto image : images)
    {
		if (image.second.img == nullptr) continue;
        image.second.img->Release();
		image.second.img = nullptr;
    }
}

void FixParentPointers(Node& node, Node* parent)
{
    node.parent = parent;
    for (size_t i = 0; i < node.children.size(); i++)
    {
        FixParentPointers(*node.children[i], &node);
    }
}
void Website::InitalizeMutableData(Node& node, bool recursive)
{
    if (node.type == "input" && node.attributes["type"] == "text")
    {
        mutableState.try_emplace(node.id, std::make_unique<TextInputMutableState>(TextInputMutableState()));
    }
    if (node.type == "select")
    {
        mutableState.try_emplace(node.id, std::make_unique<SelectMutableState>(SelectMutableState()));
    }
    if (recursive)
    {
        for (size_t i = 0; i < node.children.size(); i++)
        {
            InitalizeMutableData(*node.children[i], true);
        }
    }
}
/// <summary>
/// Ran in a seperate thread, to fetch, source -> nodes -> references (img/css/js/ect.) -> Styles Page
/// </summary>
void Website::FetchContent(std::string cachedContent)
{
    //Source
    if (!cachedContent.empty())
    {
        BrowserConsole::GetInstance().Log("Fetch", "HTML Content aquired from cache.", true);
        content = cachedContent;
        CreateRawTextVersion();
    }
    else
    {
        BrowserConsole::GetInstance().Log("Fetch", "Fetching URL: " + url, true);
        content = GetTextFromFileOrURL(url, true, this);
        CreateRawTextVersion();
        BrowserConsole::GetInstance().Log("Fetch", "Fetched HTML from: " + url, true);
    }
    //Nodes
    std::shared_ptr<Node> parsedTreeRoot = ParseNodes(content);
    InitalizeMutableData(*parsedTreeRoot, true);
    layout.FullLayoutPass(*parsedTreeRoot, tab->contentSize.x, *this);
    rootNode.store(CloneTree(*parsedTreeRoot, nullptr));
    
    //References (img/css/js/ect.)
    (*parsedTreeRoot).LoadReferences(*this);
    //Style Page
    StyleAndLayoutNodes(parsedTreeRoot);
    layout.FullLayoutPass(*parsedTreeRoot, tab->contentSize.x, *this);
    rootNode.store(CloneTree(*parsedTreeRoot, nullptr));
    interpretedDOM = PrintNode((parsedTreeRoot.get()), 0);
    interpretedCSS = CSSBuilder::CSSString(styles);
    BrowserConsole::GetInstance().Log("Fetch", "Finished fetching URL:" + url, true);
    fullyLoaded = true;
}
/// <summary>
/// Applys style changes, intended to be used on a seperate building tree, not the rendering tree.
/// </summary>
void Website::StyleAndLayoutNodes(std::shared_ptr<Node> tree)
{
    std::unordered_map<std::string, RuleValue> emptyStyle;

    (*tree).UpdateStyle(*this, emptyStyle);
    layout.FullLayoutPass(*tree, tab->contentSize.x, *this);
}

std::string StripHiddenTags(const std::string& html)
{
    std::regex multiTagRegex(R"(<(header|head|nav|footer|head|style|script)\b[^>]*?>[\s\S]*?<\/\1>)", std::regex::icase);
    std::string result = html;
    std::string old;

    // Loop until no more changes (handles multiple occurrences)
    do {
        old = result;
        result = std::regex_replace(result, multiTagRegex, "");
    } while (result != old);

    return result;
}
std::string RemoveDoctype(const std::string& html)
{
    std::regex doctypeRegex(R"(<!DOCTYPE[^>]*>)", std::regex::icase);
    return std::regex_replace(html, doctypeRegex, "");
}
std::string RemoveOuterHtmlTag(const std::string& html)
{
    // Matches opening <html ...> tag (with optional attributes) at start, and closing </html> tag at the end
    std::regex htmlTagRegex(R"(<html\b[^>]*>([\s\S]*?)<\/html>)", std::regex::icase);
    std::smatch match;

    if (std::regex_match(html, match, htmlTagRegex) && match.size() > 1) {
        return match[1].str();  // Return inner content between <html> and </html>
    }

    // If no outer html tags, just return original
    return html;
}
std::string RemoveHtmlComments(const std::string& input) {
    std::regex commentRegex(R"(<!--.*?-->)"); // non-greedy
    return std::regex_replace(input, commentRegex, "");
}
std::string TidyFormat(const std::string& html)
{
    TidyDoc tdoc = tidyCreate();
    TidyBuffer output;
    TidyBuffer errbuf;

    tidyBufInit(&output);
    tidyBufInit(&errbuf);

    tidyOptSetBool(tdoc, TidyXhtmlOut, yes);
    tidyOptSetBool(tdoc, TidyIndentContent, yes);
    tidyOptSetInt(tdoc, TidyIndentSpaces, 2);
    tidyOptSetInt(tdoc, TidyOptionId::TidyWrapLen, 0);
    tidyOptSetBool(tdoc, TidyQuiet, yes);
    tidyOptSetBool(tdoc, TidyShowWarnings, no);
    tidyOptSetBool(tdoc, TidyShowInfo, no);


    if (tidyParseString(tdoc, html.c_str()) >= 0 &&
        tidyCleanAndRepair(tdoc) >= 0 &&
        tidyRunDiagnostics(tdoc) >= 0)
    {
        tidySaveBuffer(tdoc, &output);
        std::string prettyHtml((char*)output.bp, output.size);

        tidyBufFree(&output);
        tidyBufFree(&errbuf);
        tidyRelease(tdoc);

        /*
        std::regex headRegex(R"(<head\b[^>]*>[\s\S]*?<\/head>\s*)", std::regex::icase);
        prettyHtml = std::regex_replace(prettyHtml, headRegex, "");*/
        return prettyHtml;
    }

    tidyBufFree(&output);
    tidyBufFree(&errbuf);
    tidyRelease(tdoc);
    return html;
}
void Website::CreateRawTextVersion()
{
	content = TidyFormat(content);
    content = RemoveHtmlComments(content);
    content = RemoveDoctype(content);
    websiteSource = content;
    //rawTextVersion = stripHiddenTags(rawTextVersion);
    //rawTextVersion = removeOuterHtmlTag(rawTextVersion);
    //rawTextVersion = tidyFormat(rawTextVersion);
    //rawTextVersion = removeDoctype(rawTextVersion);
}

const char* Website::getContent() const
{
	return nullptr;
}
