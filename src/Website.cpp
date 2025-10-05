#include "Website.h"
#include <curl/curl.h>
#include <thread>
#include <iostream>
#include <regex>
#include <tidy/tidybuffio.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t totalSize = size * nmemb;
    std::string* response = static_cast<std::string*>(userp);
    response->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

std::string getWebsite(std::string url)
{
    std::string website = "";
    CURL* curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_CAINFO, "C:\\Users\\JGEle\\Downloads\\curl-8.16.0_1-win64-mingw(1)\\curl-8.16.0_1-win64-mingw\\bin\\cacert.pem");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &website);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "WorldWidePickle/1.0");
        // Perform the request, res will get the return code
        res = curl_easy_perform(curl);
        // Check for errors
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            switch (res)
            {
            case CURLE_BAD_PASSWORD_ENTERED:
                website = "Bad Password (Not supported yet)";
                break;
            case CURLE_GOT_NOTHING:
                website = "Got nothing";
                break;
            default:
                website = curl_easy_strerror(res);
                break;
            }
        }
        // always cleanup
        curl_easy_cleanup(curl);
    }
    return website;
}

Website::Website(std::string url)
{
    loaded = false;
	this->url = url;
    content = "Loading...";
	rawTextVersion = "Loading...";
    fetchContent();
	//std::thread(&Website::fetchContent, this).detach();
}

Website::~Website()
{

}

void Website::fetchContent()
{
    std::cout << "Fetching URL:" << url << std::endl;
    content = getWebsite(url);
	CreateRawTextVersion();
    std::cout << "Fetched HTML" << std::endl << content << std::endl;
	nodes = ParseNodes(content);
	loaded = true;
	std::cout << "Finished fetching URL:" << url << std::endl;
}

std::string stripHiddenTags(const std::string& html)
{
    std::regex multiTagRegex(R"(<(header|nav|footer|head|style|script)\b[^>]*?>[\s\S]*?<\/\1>)", std::regex::icase);
    std::string result = html;
    std::string old;

    // Loop until no more changes (handles multiple occurrences)
    do {
        old = result;
        result = std::regex_replace(result, multiTagRegex, "");
    } while (result != old);

    return result;
}
std::string removeDoctype(const std::string& html)
{
    std::regex doctypeRegex(R"(<!DOCTYPE[^>]*>)", std::regex::icase);
    return std::regex_replace(html, doctypeRegex, "");
}
std::string removeOuterHtmlTag(const std::string& html)
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
std::string removeHtmlComments(const std::string& input) {
    std::regex commentRegex(R"(<!--.*?-->)"); // non-greedy
    return std::regex_replace(input, commentRegex, "");
}
std::string tidyFormat(const std::string& html)
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

        //Remove all headers
        std::regex headRegex(R"(<head\b[^>]*>[\s\S]*?<\/head>\s*)", std::regex::icase);
        prettyHtml = std::regex_replace(prettyHtml, headRegex, "");
        return prettyHtml;
    }

    tidyBufFree(&output);
    tidyBufFree(&errbuf);
    tidyRelease(tdoc);
    return html;
}
void Website::CreateRawTextVersion()
{
	content = tidyFormat(content);
    content = removeHtmlComments(content);
    content = removeDoctype(content);
    rawTextVersion = content;
    rawTextVersion = stripHiddenTags(rawTextVersion);
    rawTextVersion = removeOuterHtmlTag(rawTextVersion);
    rawTextVersion = tidyFormat(rawTextVersion);
    rawTextVersion = removeDoctype(rawTextVersion);
}

const char* Website::getContent() const
{
	return nullptr;
}
