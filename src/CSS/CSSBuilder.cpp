/**
* @file CSSBuilder.cpp
* @brief Creates a style sheet in memory.
*/

#include "Browser/BrowserConsole.h"
#include "Node.h"
#include "CSSBuilder.h"
#include "Util/StringUtils.h"

using namespace std;

unordered_map<string, unordered_map<string, RuleValue>> globalStyles;

std::unordered_map<std::string, ImU32> colors = {
   {"black", IM_COL32(0, 0, 0, 255)},
   {"white", IM_COL32(255, 255, 255, 255)},
   {"red", IM_COL32(255, 0, 0, 255)},
   {"green", IM_COL32(0, 255, 0, 255)},
   {"blue", IM_COL32(0, 0, 255, 255)},
   {"yellow", IM_COL32(255, 255, 0, 255)},
   {"cyan", IM_COL32(0, 255, 255, 255)},
   {"magenta", IM_COL32(255, 0, 255, 255)},
   {"gray", IM_COL32(128, 128, 128, 255)},
   {"grey", IM_COL32(128, 128, 128, 255)},
   {"LightSeaGreen", IM_COL32(32, 178, 170, 255)},
   {"transparent", IM_COL32(0, 0, 0, 0)}
};

bool TryCastToColor(const std::string& s, ImU32& result)
{
    if (colors.find(s) != colors.end())
    {
        result = colors[s];
        return true;
    }
    return false;
}

string stripBlockComments(const string& input) {
    string result = "";
    bool in_block_comment = false;

    for (size_t i = 0; i < input.length(); ++i) {
        if (!in_block_comment) {
            // Check for start of block comment
            if (i + 1 < input.length() && input[i] == '/' && input[i + 1] == '*') {
                in_block_comment = true;
                i++; // Skip the '*'
            }
            else {
                result += input[i];
            }
        }
        else {
            // Check for end of block comment
            if (i + 1 < input.length() && input[i] == '*' && input[i + 1] == '/') {
                in_block_comment = false;
                i++; // Skip the '/'
            }
            // If not the end of a comment, just ignore the character
        }
    }
    return result;
}
unordered_map<string, unordered_map<string, RuleValue>> CSSBuilder::ParseCSS()
{
    unordered_map<string, unordered_map<string, RuleValue>> styles;
    if (css.find("Page not found") != string::npos)
    {
        BrowserConsole::Log("CSS", "Could not find CSS");
        return styles;
    }
    /*css.erase(
        std::remove_if(css.begin(), css.end(),
            [](unsigned char c) { return std::isspace(c); }),
        css.end()
    );*/
    css = stripBlockComments(css);

    while (pointer < css.size())
    {
        if (css[pointer] == '@') inUnsupported = true;

        if (css[pointer] == '{' && !(inUnsupported || inComment)) StartSelector();
        else if (css[pointer] == ';' && !(inUnsupported || inComment))
        {
            ParseComponent();
        }
        else if (css[pointer] == '}' && !inComment)
        {
            EndSelector(styles);
        }

        pointer++;
#ifdef INDETPH_CSS_DEBUG
        cout << "mvptr to " << css[pointer] << " " << pointer << endl;
#endif
    }
    return styles;
}

string CSSBuilder::CSSString(const unordered_map<string, unordered_map<string, RuleValue>>& styles)
{
    string result;
    result.reserve(styles.size() * 60);
    for (const auto& selector : styles)
    {
        result += "Selector: " + selector.first + "\n";
        for (const auto& component : selector.second)
        {
            result += CSSRuleString(component);
        }
    }
    return result;
}
string CSSBuilder::CSSRuleString(const std::pair<std::string, RuleValue> component)
{
    std::string result = "\t" + component.first + ":";
    std::visit([&result](const auto& value) {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, string>) result += value;
        if constexpr (std::is_same_v<T, int>) result += std::to_string(value) + "   { int }";
        if constexpr (std::is_same_v<T, float>) result += std::to_string(value) + "   { float }";
        if constexpr (std::is_same_v<T, ImU32>) {
            string r = to_string(((value >> 0) & 0xFF));
            string g = to_string(((value >> 8) & 0xFF));
            string b = to_string(((value >> 16) & 0xFF));
            result += " r:" + r + " g:" + g + " b:" + b + "   { color }";
        }
    }, component.second);
    result += "\n";
    return result;
}
string CSSBuilder::NodeCSSString(const Node* node)
{
    if (node->style.empty()) return "";
    std::string result;
    result.reserve(node->style.size() * 30);
    for (const auto& component : node->style)
    {
        result += CSSRuleString(component);
    }
    return result;
}
std::string TrimStr(const std::string& str) {
    auto is_not_space = [](unsigned char ch) {
        return !std::isspace(ch);
        };

    auto begin = std::find_if(str.begin(), str.end(), is_not_space);
    if (begin == str.end()) return ""; // All whitespace

    auto end = std::find_if(str.rbegin(), str.rend(), is_not_space).base();
    return std::string(begin, end);
}
void CSSBuilder::StartSelector()
{
    selector = TrimStr(css.substr(componentStart, pointer - componentStart));
    inSel = true;
    componentStart = pointer + 1;
#ifdef INDETPH_CSS_DEBUG
    cout << "In Selector: " << selector << endl;
#endif
}
void CSSBuilder::EndSelector(unordered_map<string, unordered_map<string, RuleValue>>& styles)
{
    if (inUnsupported)
    {
        inUnsupported = false;
        return;
    }
    inSel = false;
    styles[selector] = buildingStyleComponents;
    componentStart = pointer + 1;
#ifdef INDETPH_CSS_DEBUG
    cout << "Selector Complete: " << selector << endl;
#endif
    selector = "";
    buildingStyleComponents.clear();
}
unordered_map<string, RuleValue> CSSBuilder::ParseSingleSelector(string css)
{
    unordered_map<string, RuleValue> style;

    int pointer = 0;
    bool inUnsupported = false;
    int componentStart = 0;
    while (pointer < css.size())
    {
        if (css[pointer] == '@') inUnsupported = true;
        if (css[pointer] == ';' && !(inUnsupported))
        {
            int colonLoc = css.find(':', componentStart);
            
            if (colonLoc == string::npos || colonLoc > pointer) BrowserConsole::Log("CSS", "No colon found for property");
            else
            {
                string property = TrimStr(css.substr(componentStart, colonLoc - componentStart));
                string value = TrimStr(css.substr(colonLoc + 1, pointer - (colonLoc + 1)));
                ParseValue(style[property], value);
            }
            componentStart = pointer + 1;
        }
        pointer++;
    }

    return style;
}
void CSSBuilder::ParseValue(RuleValue& component, string value)
{
    int valueInt = 0;
    float valueFloat = 0.0f;
    ImU32 valueCol = 0;
    if (TryCastToInt(value, valueInt))
    {
        component = valueInt;
    }
    else if (TryCastToFloat(value, valueFloat))
    {
        component = valueFloat;
    }
    else if (TryCastToColor(value, valueCol))
    {
        component = valueCol;
    }
    else
    {
        component = value;
    }
#ifdef INDETPH_CSS_DEBUG
    cout << "found component " << property << " with value " << value << endl;
#endif
}
void CSSBuilder::ParseComponent()
{
    if (!inSel) BrowserConsole::Log("CSS", "; found outside requirement");
    else
    {
        int colonLoc = css.find(':', componentStart);
        if (colonLoc == string::npos || colonLoc > pointer) BrowserConsole::Log("CSS", "No colon found for property (pt2)");
        else
        {
            string property = TrimStr(css.substr(componentStart, colonLoc - componentStart));
            string value = TrimStr(css.substr(colonLoc + 1, pointer - (colonLoc + 1)));
            ParseValue(buildingStyleComponents[property], value);
            componentStart = pointer + 1;
        }
    }
}
