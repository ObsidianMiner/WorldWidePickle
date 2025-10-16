#include "Website.h"
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <d3d10_1.h>
#include <d3d10.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "PickleGUI.h"


using namespace std;

std::string GetTextFromFile(const std::string& url)
{
    std::ifstream file(url);
    if (!file)
    {
        std::cerr << "Could not open the file - " << url << std::endl;
        return "Could not open file - " + url;
    }
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

std::string GetTextFromFileOrURL(std::string& url, bool isHTMLWebsite)
{
    if (url.rfind("http://", 0) == 0 || url.rfind("https://", 0) == 0)
    {
        return GetTextFromURL(url, isHTMLWebsite);
    }
    else
    {
        return GetTextFromFile(url);
    }
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t totalSize = size * nmemb;
    std::string* response = static_cast<std::string*>(userp);
    response->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

size_t TexWriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::vector<unsigned char>* data = (std::vector<unsigned char>*)userp;
    data->insert(data->end(), (unsigned char*)contents, (unsigned char*)contents + totalSize);
    return totalSize;
}

void SetupCurlHeader(CURL* curl)
{
    curl_easy_setopt(curl, CURLOPT_CAINFO, "C:\\Users\\JGEle\\Downloads\\curl-8.16.0_1-win64-mingw(1)\\curl-8.16.0_1-win64-mingw\\bin\\cacert.pem");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "WorldWidePickle/1.0");
}

bool DownloadImage(const std::string& url, std::vector<unsigned char>& imageBuffer) {
    CURL* curl = curl_easy_init();
    if (!curl)
    {
        std::cout << "CURL NO IMG";
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    SetupCurlHeader(curl);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, TexWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &imageBuffer);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res != CURLE_OK)
    {
        std::cout << curl_easy_strerror(res) << std::endl;
    }
    return res == CURLE_OK;
}
ID3D10ShaderResourceView* CreateTextureFromPixels(ID3D10Device* device, unsigned char* pixels, int width, int height) {
    D3D10_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D10_USAGE_DEFAULT;
    desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;

    D3D10_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = pixels;
    initData.SysMemPitch = width * 4;

    ID3D10Texture2D* tex = nullptr;
    if (FAILED(device->CreateTexture2D(&desc, &initData, &tex)))
        return nullptr;

    ID3D10ShaderResourceView* srv = nullptr;
    if (FAILED(device->CreateShaderResourceView(tex, nullptr, &srv))) {
        tex->Release();
        return nullptr;
    }

    tex->Release(); // SRV holds a reference
    return srv;
}

PImage LoadImgFromWebsiteURL(const std::string& url) {
    std::cout << "Fetching Image:" << url << std::endl;

    std::vector<unsigned char> imageBuffer;
    PImage image = PImage();
    image.img = nullptr;

    if (!DownloadImage(url, imageBuffer)) {
        printf("Failed to download image.\n");
        return image;
    }

    int width = 0, height = 0, channels = 0;
    unsigned char* pixels = stbi_load_from_memory(imageBuffer.data(), imageBuffer.size(), &width, &height, &channels, 4); // force RGBA
    if (!pixels) {
        printf("Failed to decode image.\n");
        return image;
    }

    ID3D10ShaderResourceView* textureSRV = CreateTextureFromPixels(PGUI::pd3dDevice, pixels, width, height);
    stbi_image_free(pixels); // Clean up pixel data

    if (!textureSRV) {
        printf("Failed to create texture.\n");
        return image;
    }
    image.img = textureSRV;
    image.width = width;
    image.height = height;
    if (image.width == 0 || image.height == 0 || image.img == nullptr)
    {
        std::cout << "INVALID IMAGE" << std::endl;
    }

    return image;
}
std::string GetTextFromURL(std::string& url, bool isHTMLWebsite)
{
    std::string siteText = "";
    CURL* curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        SetupCurlHeader(curl);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &siteText);
        if (isHTMLWebsite) curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        // Perform the request, res will get the return code
        res = curl_easy_perform(curl);
        // Check for errors
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            switch (res)
            {
            case CURLE_BAD_PASSWORD_ENTERED:
                siteText = "Bad Password (Not supported yet)";
                break;
            case CURLE_GOT_NOTHING:
                siteText = "Got nothing";
                break;
            default:
                siteText = curl_easy_strerror(res);
                break;
            }
        }
        else if (isHTMLWebsite)
        {
            char* finalURL;
            curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &finalURL);
            url = finalURL;
        }
        // always cleanup
        curl_easy_cleanup(curl);
    }
    return siteText;
}
