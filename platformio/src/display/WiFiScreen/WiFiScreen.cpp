#include "WiFiScreen.h"
#include "app/app.h"

//
void WiFiScreen_setup(TFT_eSPI *ptft)
{
    ptft->fillScreen(TFT_BLACK);
    _log("WiFi Screen Setup\n");
}

//
void WiFiScreen_render(TFT_eSPI *ptft)
{
    // Text to be displayed
    JsonDocument &app = status();

    ///
    ptft->setCursor(0, 0, 2);
    ptft->setTextSize(1); // Adjust text size as needed
    ptft->setTextColor(TFT_WHITE, TFT_BLACK);

    // Draw text in the middle of the screen
    ptft->setCursor(2, 0, 2);

    if (app["wifi"])
    {
        ptft->print("WiFi connected");
        // wifi is connected
        // go ahead and perform the first downloads and display the clock screen
    }
    else
    {
        ptft->print("WiFi not connected");
        // make wifi access point and display the IP information
    }
}
