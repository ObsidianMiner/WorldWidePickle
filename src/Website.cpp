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
#include "HTML/HTMLBuilder.h"


Website::Website(std::string url, Tab* tab)
{
    loaded = false;
    wasLoaded = false;
	this->url = url;
    this->tab = tab;
    name = url;
    content = "<h1>Loading...</h1>";
	websiteSource = "<h1>Loading...</h1>";
    styles = globalStyles;
    highestID = 0;
    rootNode = std::make_shared<Node>(nullptr, "rootHTML", highestID++);
	std::thread(&Website::FetchContent, this).detach();
    layout = Layout();
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
        FixParentPointers(node.children[i], &node);
    }
}
/// <summary>
/// Ran in a seperate thread, to fetch, source -> nodes -> references (img/css/js/ect.) -> Styles Page
/// </summary>
void Website::FetchContent()
{
    //Source
    BrowserConsole::Log("Fetch", "Fetching URL: " + url);
    content = GetTextFromFileOrURL(url, true);
	CreateRawTextVersion();
    BrowserConsole::Log("Fetch", "Fetched HTML from: " + url);
    //Nodes
    auto parsedTreeRoot = std::make_shared<Node>(ParseNodes(content));
	FixParentPointers(*parsedTreeRoot, nullptr);
    
	rootNode.store(parsedTreeRoot);
    
    //References (img/css/js/ect.)
    (*parsedTreeRoot).LoadReferences(*this);
    //Style Page
    StyleAndLayoutNodes();
    interpretedDOM = PrintNode((parsedTreeRoot.get()), 0);
    interpretedCSS = CSSBuilder::CSSString(styles);
    BrowserConsole::Log("Fetch", "Finished fetching URL:" + url);
    loaded = true;
}
/// <summary>
/// Creates a duplicate DOM makes style changes, and then sets the DOM to the new DOM
/// </summary>
void Website::StyleAndLayoutNodes()
{
    auto styledNodes = ReadTree();
    std::unordered_map<std::string, RuleValue> emptyStyle;

    (*styledNodes).UpdateStyle(*this, emptyStyle);
    FixParentPointers(*styledNodes, nullptr);
    layout.LayoutChildren(*styledNodes, tab->contentSize.x, *this);
    interpretedDOM = PrintNode((styledNodes.get()), 0);
    rootNode.store(styledNodes);
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
