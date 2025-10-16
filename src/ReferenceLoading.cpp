#include "DOM.h"
#include "Website.h"
#include "CSSBuilder.h"
#include <iostream>

std::string GetRealURL(const std::string& possiblyRelativeURL, Website& website)
{
    std::string realImgURL = possiblyRelativeURL;
    if (realImgURL.find("http") == std::string::npos)
    {
        if (realImgURL[0] != '/')
        {
            realImgURL = "/" + realImgURL; // Ensure it starts with a slash
        }
        realImgURL = website.url + realImgURL;
    }
    return realImgURL;
}

void Node::LoadReferences(Website& website)
{
	bool isValidImage = (type == "img" && attributes.find("src") != attributes.end());

    if (isValidImage)
    {
        website.images.emplace(attributes["src"], LoadImgFromWebsiteURL(GetRealURL(attributes["src"], website)));
        auto it = website.images.find(attributes["src"]);
        std::cout << (it != website.images.end());
    }
    else if (type == "title")
    {
        if(children.size() == 1 && children[0].HasAttribute("text")) website.name = children[0].attributes.at("text");
    }
    else if (type == "link")
    {
        if (attributes.find("rel") != attributes.end())
        {
            if (attributes["rel"] == "stylesheet" && attributes.find("href") != attributes.end())
            {
                string realURL = GetRealURL(attributes["href"], website);
				string csstxt = GetTextFromURL(realURL);
                CSSBuilder builder(csstxt);
                for (auto& [selector, declarations] : builder.ParseCSS()) {
                    website.styles.insert_or_assign(selector, declarations);
                }
				cout << "Loaded CSS from " << realURL << endl;
            }
        }
    }
    for (size_t i = 0; i < children.size(); i++)
    {
        children[i].LoadReferences(website);
    }
}
const std::vector<std::string> inheritedProperties = {
    "azimuth",
    "border-collapse",
    "border-spacing",
    "caption-side",
    "color",
    "cursor",
    "direction",
    "elevation",
    "empty-cells",
    "font-family",
    "font-size",
    "font-style",
    "font-variant",
    "font-weight",
    "font",
    "letter-spacing",
    "line-height",
    "list-style-image",
    "list-style-position",
    "list-style-type",
    "list-style",
    "orphans",
    "pitch-range",
    "pitch",
    "quotes",
    "richness",
    "speak-header",
    "speak-numeral",
    "speak-punctuation",
    "speak",
    "speech-rate",
    "stress",
    "text-align",
    "text-indent",
    "text-transform",
    "visibility",
    "voice-family",
    "volume",
    "white-space",
    "widows",
    "word-spacing"
};
void Node::UpdateStyle(const Website& website, const std::unordered_map<std::string, RuleValue>& inheritedStyle)
{
    style = inheritedStyle;
    if (website.styles.find(type) != website.styles.end())
    {
        for (const auto& [propertyName, value] : website.styles.at(type)) {
            style.insert_or_assign(propertyName, value);
        }
    }
    if (attributes.find("class") != attributes.end())
    {
        string selector = '.' + attributes["class"];
        if (website.styles.find(selector) != website.styles.end())
        {
            cout << "Found Style: " << selector;
            for (const auto& [propertyName, value] : website.styles.at(selector)) {
                style.insert_or_assign(propertyName, value);
            }
        }
    }
    for (size_t i = 0; i < children.size(); i++)
    {
        std::unordered_map<std::string, RuleValue> passedDownStyle;
        std::vector<std::string> keysToRemove;
        for (auto s : style)
        {
            if (std::find(inheritedProperties.begin(), inheritedProperties.end(), s.first) != inheritedProperties.end())
            {
                passedDownStyle.emplace(s);
            }
        }
        children[i].UpdateStyle(website, passedDownStyle);
    }
}
