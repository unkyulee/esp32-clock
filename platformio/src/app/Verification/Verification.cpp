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
    //
    JsonDocument &app = status();

    // Get SSID and password from app status
    const char *ssid = app["wifi_ssid"] | "";
    const char *password = app["wifi_password"] | "";

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
        app["wifi"] = true;
        return true;
    }
    else
    {
        app["wifi"] = false;
        return false;
    }
}