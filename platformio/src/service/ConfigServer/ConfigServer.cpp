#include "ConfigServer.h"
#include "app/app.h"

#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);
void onConfigGet(AsyncWebServerRequest *request);
void onConfigSave(AsyncWebServerRequest *request);

//
void startWebServer()
{
    //
    server.onNotFound(onConfigGet);

    //
    server.on("/save", HTTP_POST, onConfigSave);

    //
    server.begin();

    _log("Configuration Web Server Started\n");
}

//
void onConfigGet(AsyncWebServerRequest *request)
{
    _log("onConfigGet\n");

    //
    JsonDocument &app = status();

    //
    File configFile = gfs()->open("/config.html", "r");
    if (configFile)
    {
        _log("[onConfigGet] opening file /config.html\n");

        // read the file
        String configString = configFile.readString();
        configFile.close();

        //
        if(app["config"]["name"].is<String>() == false) app["config"]["name"] = "ESP32 Clock";
        configString.replace("#name#", app["config"]["name"].as<String>().c_str());
        _debug("name: %s\n", app["config"]["name"].as<String>().c_str());
        
        //
        if(app["config"]["ssid"].is<String>() == false) app["config"]["ssid"] = "";
        configString.replace("#ssid#", app["config"]["ssid"].as<String>().c_str());
        _debug("ssid: %s\n", app["config"]["ssid"].as<String>().c_str());

        //
        if(app["config"]["password"].is<String>() == false) app["config"]["password"] = "";
        configString.replace("#password#", app["config"]["password"].as<String>().c_str());
        _debug("password: %s\n", app["config"]["password"].as<String>().c_str());

        //
        request->send(200, "text/html", configString);
    }
}

//
void onConfigSave(AsyncWebServerRequest *request)
{
    _log("onConfigSave\n");

    //
    JsonDocument &app = status();

    //
    String name = request->getParam("name", true)->value();
    _debug("name: %s\n", name.c_str());
    app["config"]["name"] = name;

    String ssid = request->getParam("ssid", true)->value();
    _debug("ssid: %s\n", ssid.c_str());
    app["config"]["ssid"] = ssid;

    String password = request->getParam("password", true)->value();
    _debug("password: %s\n", password.c_str());
    app["config"]["password"] = password;

    //
    config_save();

    //
    onConfigGet(request);
}