#include "WiFiScreen.h"
#include "app/app.h"
#include "display/display.h"

//
#include "service/AccessPoint/AccessPoint.h"
#include "service/ConfigServer/ConfigServer.h"
#include "service/Time/Time.h"

//
#include <WiFi.h>

//
void WiFiScreen_setup(TFT_eSPI *ptft)
{
    ptft->fillScreen(TFT_BLACK);
    _log("WiFi Screen Setup\n");
}

#define WIFISCREEN_NOT_CONNECTED 0
#define WIFISCREEN_CONNECTED 1
#define WIFISCREEN_AP 2

//
void WiFiScreen_render(TFT_eSPI *ptft)
{
    int mode = 0;
    static int prevMode = 0;

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
        mode = WIFISCREEN_CONNECTED;
        if (mode != prevMode)
            ptft->fillScreen(TFT_BLACK);

        ptft->printf("WiFi connected: %s", app["ip"].as<const char *>());
        
        // start configuration web server
        startWebServer();

        // update time
        updateTime();

        // Move to Clock Screen
        app["screen"] = CLOCKSCREEN;
    }

    // Start AP
    else
    {
        if (app["apIP"].is<String>() == false)
        {
            mode = WIFISCREEN_NOT_CONNECTED;
            if (mode != prevMode)
                ptft->fillScreen(TFT_BLACK);

            ptft->print("WiFi not connected. Starting Access Point");
            // make wifi access point and display the IP information
            startAccessPoint();
            // start configuration web server
            startWebServer();
        }
        else
        {
            mode = WIFISCREEN_AP;
            if (mode != prevMode)
                ptft->fillScreen(TFT_BLACK);

            //
            String apIP = app["apIP"].as<String>();
            ptft->printf("Access Point: %s\n", WiFi.softAPSSID());
            ptft->printf("IP: %s\n", apIP.c_str());
        }
    }

    //
    prevMode = mode;
}
