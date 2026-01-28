#include "Website.h"
#include <iostream>
#include "CSS/CSSBuilder.h"

using namespace std;

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
            for (const auto& [propertyName, value] : website.styles.at(selector)) {
                style.insert_or_assign(propertyName, value);
            }
        }
        selector = selector + " " + type;

        if (website.styles.find(selector) != website.styles.end())
        {
            cout << "Has Selector: " << selector << endl;
            for (const auto& [propertyName, value] : website.styles.at(selector)) {
                style.insert_or_assign(propertyName, value);
            }
        }
    }
    if (HasAttribute("style"))
    {
        std::unordered_map<std::string, RuleValue> elementStyles = CSSBuilder::ParseSingleSelector(attributes["style"]);
        for (const auto& [propertyName, value] : elementStyles) {
            style.insert_or_assign(propertyName, value);
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