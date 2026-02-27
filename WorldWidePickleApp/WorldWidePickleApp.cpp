/**
* @file main.cpp
* @brief The main class responsible for starting up the browser.
*/

#include "Browser/Browser.h"
#include <iostream>

int main(int, char**)
{
    Browser browser = Browser();
    browser.SetupUI();
    browser.LoadGlobalStyles();
    browser.LoadHomePage();
    browser.MainLoop();
    browser.Cleanup();

    return 0;
}