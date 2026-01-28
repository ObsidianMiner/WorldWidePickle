#pragma once
#include <string>
#include <unordered_map>
#include <variant>
using namespace std;

extern unordered_map<string, unordered_map<string, RuleValue>> globalStyles;
class CSSBuilder
{
public:
	string css;
	static unordered_map<string, RuleValue> ParseSingleSelector(string css);
	unordered_map<string, unordered_map<string, RuleValue>> ParseCSS();
	static string CSSString(const unordered_map<string, unordered_map<string, RuleValue>>& styles);
	CSSBuilder(string css) : css(css), pointer(0), componentStart(0), inSel(false), inUnsupported(false), inComment(false) {}
private:
	int pointer;
	int componentStart;
	bool inSel;
	bool inUnsupported;
	bool inComment;
	string selector;
	unordered_map<string, RuleValue> buildingStyleComponents;
	void StartSelector();
	void ParseComponent();
	void EndSelector(unordered_map<string, unordered_map<string, RuleValue>>& styles);
	static void ParseValue(RuleValue& component, string value);
};