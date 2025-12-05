#include "ConfigServer.h"
#include "app/app.h"

#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Update.h>

AsyncWebServer server(80);
void onConfigGet(AsyncWebServerRequest *request);
void onConfigSave(AsyncWebServerRequest *request);
void onConfigScan(AsyncWebServerRequest *request);
void onFirmwareUpload(AsyncWebServerRequest *request);
void handleFirmwareUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
void onHtmlUpload(AsyncWebServerRequest *request);
void handleHtmlUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);

//
void startWebServer()
{
    //
    server.onNotFound(onConfigGet);

    //
    server.on("/save", HTTP_POST, onConfigSave);
    server.on("/scan", HTTP_GET, onConfigScan);
    server.on("/update", HTTP_POST, onFirmwareUpload, handleFirmwareUpload);
    server.on("/update-config", HTTP_POST, onHtmlUpload, handleHtmlUpload);

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
        if (app["config"]["tzServer"].is<String>() == false || app["config"]["tzServer"].as<String>().length() == 0)
            app["config"]["tzServer"] = "pool.ntp.org";
        configString.replace("#tzServer#", app["config"]["tzServer"].as<String>().c_str());
        _debug("tzServer: %s\n", app["config"]["tzServer"].as<String>().c_str());

        //
        if (app["config"]["tz"].is<String>() == false)
            app["config"]["tz"] = "";
        configString.replace("#tz#", app["config"]["tz"].as<String>().c_str());
        _debug("tz: %s\n", app["config"]["tz"].as<String>().c_str());

        //
        request->send(200, "text/html", configString);
    }
}

static bool _updateFailed = false;
static bool _htmlUpdateFailed = false;
static File _htmlUploadFile;

//
void handleFirmwareUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (!index)
    {
        _log("[OTA] Starting update: %s\n", filename.c_str());
        _updateFailed = false;
        if (!Update.begin(UPDATE_SIZE_UNKNOWN))
        {
            _log("[OTA] Update begin failed: %s\n", Update.errorString());
            _updateFailed = true;
        }
    }

    if (!_updateFailed)
    {
        if (Update.write(data, len) != len)
        {
            _log("[OTA] Write failed: %s\n", Update.errorString());
            _updateFailed = true;
        }
    }

    if (final)
    {
        if (!_updateFailed && Update.end(true))
        {
            _log("[OTA] Update success, restarting soon.\n");
        }
        else
        {
            _log("[OTA] Update end failed: %s\n", Update.errorString());
            _updateFailed = true;
        }
    }
}

//
void handleHtmlUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (!index)
    {
        _log("[HTML] Upload starting: %s\n", filename.c_str());
        _htmlUpdateFailed = false;
        if (_htmlUploadFile)
            _htmlUploadFile.close();
        _htmlUploadFile = gfs()->open("/config.html", "w");
        if (!_htmlUploadFile)
        {
            _log("[HTML] Failed to open /config.html for write\n");
            _htmlUpdateFailed = true;
        }
    }

    if (!_htmlUpdateFailed && _htmlUploadFile)
    {
        size_t written = _htmlUploadFile.write(data, len);
        if (written != len)
        {
            _log("[HTML] Write failed (expected %u wrote %u)\n", (unsigned)len, (unsigned)written);
            _htmlUpdateFailed = true;
        }
    }

    if (final)
    {
        if (_htmlUploadFile)
            _htmlUploadFile.close();
        if (_htmlUpdateFailed)
        {
            _log("[HTML] Upload failed\n");
        }
        else
        {
            _log("[HTML] Upload complete\n");
        }
    }
}

//
void onConfigScan(AsyncWebServerRequest *request)
{
    _log("onConfigScan\n");

    // Start async scan if not running
    WiFi.mode(WIFI_AP_STA);
    int16_t result = WiFi.scanComplete(); // -1 scanning, -2 not started
    _log("scan status: %d\n", result);

    if (result < 0)
    {
        // kick off a new async scan if idle
        if (result == -2)
        {
            WiFi.scanNetworks(true, true); // async scan with hidden nets and filter duplicates
            _log("starting async scan\n");
        }

        JsonDocument doc;
        doc["status"] = "scanning";
        doc["networks"].to<JsonArray>(); // empty list

        String payload;
        serializeJson(doc, payload);
        request->send(200, "application/json", payload);
        return;
    }

    // Completed with results
    JsonDocument doc;
    JsonArray networks = doc["networks"].to<JsonArray>();
    doc["status"] = "complete";

    for (int i = 0; i < result; i++)
    {
        JsonObject net = networks.add<JsonObject>();
        net["ssid"] = WiFi.SSID(i);
        net["rssi"] = WiFi.RSSI(i);
        wifi_auth_mode_t auth = WiFi.encryptionType(i);
        net["secure"] = auth != WIFI_AUTH_OPEN;
        net["channel"] = WiFi.channel(i);
    }

    String payload;
    serializeJson(doc, payload);
    request->send(200, "application/json", payload);
    _log(payload.c_str()); 
}

//
void onConfigSave(AsyncWebServerRequest *request)
{
    _log("onConfigSave\n");

    //
    JsonDocument &app = status();

    //
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

    //
    config_save();

    //
    onConfigGet(request);

    // Restart ESP32 after a short delay to apply new settings
    delay(1000);
    ESP.restart();
}

//
void onFirmwareUpload(AsyncWebServerRequest *request)
{
    _log("onFirmwareUpload\n");

    if (_updateFailed || Update.hasError())
    {
        request->send(500, "text/plain", "Firmware update failed");
        _updateFailed = false; // reset
        return;
    }

    request->send(200, "text/plain", "Update successful. Rebooting...");
    delay(500);
    ESP.restart();
}

//
void onHtmlUpload(AsyncWebServerRequest *request)
{
    _log("onHtmlUpload\n");

    if (_htmlUpdateFailed)
    {
        request->send(500, "text/plain", "Config upload failed");
        _htmlUpdateFailed = false; // reset
        return;
    }

    request->send(200, "text/plain", "Config page updated");
}
