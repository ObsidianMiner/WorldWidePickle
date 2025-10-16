#include <iostream>
#include "DOM.h"
#include "CSSBuilder.h"
#include "StringCasting.h"

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
        cout << "Not valid CSS" << endl;
        return styles;
    }
    css.erase(
        std::remove_if(css.begin(), css.end(),
            [](unsigned char c) { return std::isspace(c); }),
        css.end()
    );
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
    string result = "";
    for (auto& selector : styles)
    {
        result += + "Selector: " + selector.first + "\n";
        for (auto& component : selector.second)
        {
            if (std::holds_alternative<string>(component.second)) result += '\t' + component.first + ":" + get<string>(component.second) + "\n";
            if (std::holds_alternative<int>(component.second))result += '\t' + component.first + ":" + to_string(get<int>(component.second)) + "   { int } " + "\n";
            if (std::holds_alternative<float>(component.second))result += '\t' + component.first + ":" + to_string(get<float>(component.second)) + "   { float } " + "\n";
            if (std::holds_alternative<ImU32>(component.second))result += '\t' + component.first + ":" + to_string(get<ImU32>(component.second)) + "   { color } " + "\n";
        }
    }
    return result;
}

void CSSBuilder::StartSelector()
{
    selector = css.substr(componentStart, pointer - componentStart);
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
}

void CSSBuilder::ParseComponent()
{
    buildingStyleComponents.clear();
    if (!inSel) cout << "; found outside requirement" << endl;
    else
    {
        int colonLoc = css.find(':', componentStart);
        if (colonLoc == string::npos || colonLoc > pointer) cout << "No colon found for property" << endl;
        else
        {
            string property = css.substr(componentStart, colonLoc - componentStart);
            string value = css.substr(colonLoc + 1, pointer - (colonLoc + 1));
            int valueInt = 0;
            float valueFloat = 0.0f;
			ImU32 valueCol = 0;
            if (TryCastToInt(value, valueInt))
            {
                buildingStyleComponents[property] = valueInt;
            }
            else if (TryCastToFloat(value, valueFloat))
            {
                buildingStyleComponents[property] = valueFloat;
            }
            else if (TryCastToColor(value, valueCol))
            {
                buildingStyleComponents[property] = valueCol;
			}
            else
            {
                buildingStyleComponents[property] = value;
            }
            componentStart = pointer + 1;
#ifdef INDETPH_CSS_DEBUG
            cout << "found component " << property << " with value " << value << endl;
#endif
        }
    }
}