#include "Forms.h"
#include <curl/curl.h>
#include "Browser/BrowserConsole.h"
#include <algorithm>
#include "Browser/Tab.h"

std::string EncodePayload(std::string payload, std::string encType)
{
    if (encType == "text/plain")
    {
        return payload;
    }
    else if (encType == "multipart/form-data")
    {
        BrowserConsole::GetInstance().Error("Form", "Attempted to submit multipart form-data. Not supported yet.");
        return "NOT+SUPPORTED+YET";
    }

    // Default behaviour: application/x-www-form-urlencoded
    std::ranges::replace(payload, ' ', '+');

    static const char hexDigits[] = "0123456789ABCDEF";

    for (size_t i = 0; i < payload.size(); i++)
    {
        if (!std::isalnum(payload[i]))
        {
            payload.insert(i + 1, 1, hexDigits[payload[i] & 0xF0]); // Higher 4 bits
            payload[i] = hexDigits[payload[i] & 0x0F]; // Lower 4 bits
        }
    }
    return payload;
}
std::string GetPayload(const Node* formChild, const Website& website, std::string encType)
{
    std::string payload = "";

    if (formChild->type == "input" && formChild->HasAttribute("name"))
    {
        if (!formChild->HasAttribute("type") || formChild->attributes.at("type") == "text")
        {
            std::string name = formChild->attributes.at("name");
            std::string inputText = static_cast<TextInputMutableState*>(website.mutableState.at(formChild->id).get())->inputText;
            if (payload.size() > 0) payload += "&";
            payload += name + "=" + EncodePayload(inputText, encType);
        }
    }
    for (size_t i = 0; i < formChild->children.size(); i++)
    {
        std::string additionalPayload = GetPayload(formChild->children[i].get(), website, encType);
        if (additionalPayload.size() > 0)
        {
            if (payload.size() > 0) payload += "&";
            payload += additionalPayload;
        }
    }
    return payload;
}

size_t FormWriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t totalSize = size * nmemb;
    std::string* response = static_cast<std::string*>(userp);
    response->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}
void SetupFormCurlHeader(CURL* curl)
{
    curl_easy_setopt(curl, CURLOPT_CAINFO, "Cert\\cacert.pem");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "WorldWidePickle/1.0");
}

void SubmitForm(const Node* formNode, const Website& website)
{
    std::string payload;

    std::string method = "post";
    std::string actionURL;
    std::string encType = "application/x-www-form-urlencoded";
    
    if (!formNode->HasAttribute("action"))
    {
        BrowserConsole::GetInstance().Error("Form", "Attemped to submit a form without an action URL.");
        return;
    }
    actionURL = formNode->attributes.at("action");
    actionURL = website.ConvertRelativeURLs(actionURL);


    if (formNode->HasAttribute("method")) method = formNode->attributes.at("method");
    if (formNode->HasAttribute("enctype")) encType = formNode->attributes.at("enctype");

    try { payload = GetPayload(formNode, website, encType); }
    catch (const std::runtime_error& e)
    {
        BrowserConsole::GetInstance().Error("Form", std::string("Error trying to submit form: ") + e.what());
        return;
    }

    CURL* curl = curl_easy_init();
    if (!curl)
    {
        BrowserConsole::GetInstance().Error("Form", "CURL ERR ON FORM");
        return;
    }
    std::string responseString;

    curl_easy_setopt(curl, CURLOPT_URL, actionURL.c_str());
    
    // TODO: Allow other methods later
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FormWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);
    SetupFormCurlHeader(curl);

    BrowserConsole::GetInstance().Log("Form", "Sending " + method + " to " + actionURL);
    BrowserConsole::GetInstance().Log("Form", "PostFields Sent: " + payload);
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res != CURLE_OK)
    {
        std::string error = curl_easy_strerror(res);
        BrowserConsole::GetInstance().Log("Form", "CURL ERROR: " + error);
    }
    else
    {
        BrowserConsole::GetInstance().Log("Form", "Response Recived: " + responseString);
        website.tab->AddAndDisplayCachedPage(website.url, responseString);
    }

    return;
}