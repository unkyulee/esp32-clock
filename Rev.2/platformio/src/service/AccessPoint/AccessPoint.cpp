#include "AccessPoint.h"
#include "app/app.h"

#include <WiFi.h>

// Starts an Access Point with the name "ESP32 Clock"
void startAccessPoint()
{
    JsonDocument &app = status();

    const char *ap_ssid = "The Clock";
    const char *ap_password = nullptr; // Open network, or set a password if desired

    if(app["config"]["name"].is<String>())
        ap_ssid = app["config"]["name"].as<String>().c_str();

    WiFi.mode(WIFI_AP);
    WiFi.softAP(ap_ssid, ap_password);

    // Optionally print the IP address
    IPAddress apIP = WiFi.softAPIP();
    _log("Access Point started. IP address: %s\n", apIP.toString().c_str());
    app["apIP"] = apIP.toString(); // save IP information to global
}