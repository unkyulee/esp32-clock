#include "Verification.h"
#include "app/app.h"

//
#include <WiFi.h> // Add this include for ESP32 WiFi

// check if file system is correctly loaded
bool filesystem_check()
{
    //
    JsonDocument &app = status();

    // Call File System for the first time to initialize
    gfs();

    // file system check pass
    return true;
}

// check if wifi is connected
// when connected set app["wifi"] = true
// otherwise the screen will go to AP screen
bool wifi_check()
{
    _log("wifi_check started\n");

    //
    JsonDocument &app = status();

    // Get SSID and password from app status
    const char *ssid = "";
    if (app["config"]["ssid"].is<const char*>())
        ssid = app["config"]["ssid"].as<const char*>();

    const char *password = "";
    if (app["config"]["password"].is<const char*>())
        password = app["config"]["password"].as<const char*>();

    //
    _debug("ssid: %s\n", ssid);
    _debug("password: %s\n", password);    

    //
    if (strlen(ssid) == 0)
    {
        app["wifi"] = false;
        return false;
    }

    // Start WiFi connection if not already connected
    if (WiFi.status() != WL_CONNECTED)
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);

        // Wait for connection (timeout after 10 seconds)
        unsigned long startAttemptTime = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000)
        {
            delay(100);
        }
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        //
        app["wifi"] = true;

        // IP Address
        app["ip"] = WiFi.localIP().toString().c_str();
        _log("[wifi_check] %s\n", app["ip"].as<const char*>());

        return true;
    }
    else
    {
        app["wifi"] = false;
        return false;
    }
}
