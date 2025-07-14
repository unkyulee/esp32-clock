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
        if (app["config"]["name"].is<String>() == false)
            app["config"]["name"] = "ESP32 Clock";
        configString.replace("#name#", app["config"]["name"].as<String>().c_str());
        _debug("name: %s\n", app["config"]["name"].as<String>().c_str());

        //
        if (app["config"]["ssid"].is<String>() == false)
            app["config"]["ssid"] = "";
        configString.replace("#ssid#", app["config"]["ssid"].as<String>().c_str());
        _debug("ssid: %s\n", app["config"]["ssid"].as<String>().c_str());

        //
        if (app["config"]["password"].is<String>() == false)
            app["config"]["password"] = "";
        configString.replace("#password#", app["config"]["password"].as<String>().c_str());
        _debug("password: %s\n", app["config"]["password"].as<String>().c_str());

        //
        if (app["config"]["tzServer"].is<String>() == false)
            app["config"]["tzServer"] = "";
        configString.replace("#tzServer#", app["config"]["tzServer"].as<String>().c_str());
        _debug("tzServer: %s\n", app["config"]["tzServer"].as<String>().c_str());

        //
        if (app["config"]["tz"].is<String>() == false)
            app["config"]["tz"] = "";
        configString.replace("#tzServer#", app["config"]["tz"].as<String>().c_str());
        _debug("tz: %s\n", app["config"]["tz"].as<String>().c_str());

        //
        if (app["config"]["button_1"].is<String>() == false)
            app["config"]["button_1"] = "";
        configString.replace("#button_1#", app["config"]["button_1"].as<String>().c_str());
        _debug("button_1: %s\n", app["config"]["button_1"].as<String>().c_str());

        //
        if (app["config"]["button_2"].is<String>() == false)
            app["config"]["button_2"] = "";
        configString.replace("#button_2#", app["config"]["button_2"].as<String>().c_str());
        _debug("button_2: %s\n", app["config"]["button_2"].as<String>().c_str());

        //
        if (app["config"]["button_3"].is<String>() == false)
            app["config"]["button_3"] = "";
        configString.replace("#button_3#", app["config"]["button_3"].as<String>().c_str());
        _debug("button_3: %s\n", app["config"]["button_3"].as<String>().c_str());

        //
        if (app["config"]["button_4"].is<String>() == false)
            app["config"]["button_4"] = "";
        configString.replace("#button_4#", app["config"]["button_4"].as<String>().c_str());
        _debug("button_4: %s\n", app["config"]["button_4"].as<String>().c_str());

        //
        if (app["config"]["button_5"].is<String>() == false)
            app["config"]["button_5"] = "";
        configString.replace("#button_5#", app["config"]["button_5"].as<String>().c_str());
        _debug("button_5: %s\n", app["config"]["button_5"].as<String>().c_str());

        //
        if (app["config"]["button_6"].is<String>() == false)
            app["config"]["button_6"] = "";
        configString.replace("#button_6#", app["config"]["button_6"].as<String>().c_str());
        _debug("button_6: %s\n", app["config"]["button_6"].as<String>().c_str());

        //
        if (app["config"]["button_7"].is<String>() == false)
            app["config"]["button_7"] = "";
        configString.replace("#tzServer#", app["config"]["button_7"].as<String>().c_str());
        _debug("button_7: %s\n", app["config"]["button_7"].as<String>().c_str());

        //
        if (app["config"]["button_8"].is<String>() == false)
            app["config"]["button_8"] = "";
        configString.replace("#button_8#", app["config"]["button_8"].as<String>().c_str());
        _debug("button_8: %s\n", app["config"]["button_8"].as<String>().c_str());

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

    String tzServer = request->getParam("tzServer", true)->value();
    _debug("tzServer: %s\n", tzServer.c_str());
    app["config"]["tzServer"] = tzServer;

    String tz = request->getParam("tz", true)->value();
    _debug("tz: %s\n", tz.c_str());
    app["config"]["tz"] = tz;

    String button_1 = request->getParam("button_1", true)->value();
    _debug("button_1: %s\n", button_1.c_str());
    app["config"]["button_1"] = button_1;

    String button_2 = request->getParam("button_2", true)->value();
    _debug("button_2: %s\n", button_2.c_str());
    app["config"]["button_2"] = button_2;

    String button_3 = request->getParam("button_3", true)->value();
    _debug("button_3: %s\n", button_3.c_str());
    app["config"]["button_3"] = button_3;

    String button_4 = request->getParam("button_4", true)->value();
    _debug("button_4: %s\n", button_4.c_str());
    app["config"]["button_4"] = button_4;

    String button_5 = request->getParam("button_5", true)->value();
    _debug("button_5: %s\n", button_5.c_str());
    app["config"]["button_5"] = button_5;

    String button_6 = request->getParam("button_6", true)->value();
    _debug("button_6: %s\n", button_6.c_str());
    app["config"]["button_6"] = button_6;

    String button_7 = request->getParam("button_7", true)->value();
    _debug("button_7: %s\n", button_7.c_str());
    app["config"]["button_7"] = button_7;

    String button_8 = request->getParam("button_8", true)->value();
    _debug("button_8: %s\n", button_8.c_str());
    app["config"]["button_8"] = button_8;

    //
    config_save();

    //
    onConfigGet(request);
}