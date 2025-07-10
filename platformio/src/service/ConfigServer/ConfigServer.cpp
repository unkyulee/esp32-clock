#include "ConfigServer.h"
#include "app/app.h"

#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

void startWebServer()
{
    server.onNotFound([](AsyncWebServerRequest *request)
                      { request->send(SPIFFS, "/config.html", "text/html"); });

    /*

    server.on("/submit", HTTP_GET, [](AsyncWebServerRequest *request){
String ssid = request->getParam("ssid")->value();
String password = request->getParam("password")->value();
Serial.println("Received config:");
Serial.println("SSID: " + ssid);
Serial.println("Password: " + password);
request->send(200, "text/plain", "Settings received!");
});
    */
    server.begin();
}

// Call startWebServer() after starting Access Point