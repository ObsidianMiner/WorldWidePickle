/**
* @file ReferenceLoading.cpp
* @brief Combination of Node::LoadReferences, and verifying url user input (for some reason).
*/

#include "Node.h"
#include "Website.h"
#include "CSS/CSSBuilder.h"
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
                    if (website.styles.contains(selector))
                    {
                        website.styles[selector].insert(declarations.begin(), declarations.end());
                    }
                    else
                    {
                        website.styles.insert_or_assign(selector, declarations);
                    }
                }
				cout << "Loaded CSS from " << realURL << endl;
            }
        }
    }
    else if (type == "style" && HasAttribute("CDATA"))
    {
        CSSBuilder builder(attributes["CDATA"]);
        for (auto& [selector, declarations] : builder.ParseCSS()) {
            if (website.styles.contains(selector))
            {
                website.styles[selector].insert(declarations.begin(), declarations.end());
            }
            else
            {
                website.styles.insert_or_assign(selector, declarations);
            }
        }
        cout << "Loaded CSS from style element" << endl;
    }
    for (size_t i = 0; i < children.size(); i++)
    {
        children[i].LoadReferences(website);
    }
}

