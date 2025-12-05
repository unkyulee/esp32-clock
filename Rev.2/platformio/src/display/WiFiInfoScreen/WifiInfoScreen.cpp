#include "WifiInfoScreen.h"
#include "app/app.h"
#include "display/display.h"
#include "service/Knob/Knob.h"
#include <WiFi.h>

static String prevState = "";

void WiFiInfoScreen_setup(TFT_eSPI *ptft, U8g2_for_TFT_eSPI *pu8f)
{
    _log("WiFiInfo Screen Setup\n");
    prevState = "";
    ptft->fillScreen(TFT_BLACK);
}

void WiFiInfoScreen_render(TFT_eSPI *ptft, U8g2_for_TFT_eSPI *pu8f)
{
    JsonDocument &app = status();

    String ssid = WiFi.SSID();
    if (!ssid.length() && app["config"]["ssid"].is<String>())
        ssid = app["config"]["ssid"].as<String>();

    String ip = WiFi.localIP().toString();
    if (!ip.length() && app["ip"].is<String>())
        ip = app["ip"].as<String>();

    String mode = WiFi.getMode() == WIFI_MODE_AP ? "Access Point" : "Station";

    String stateKey = ssid + "|" + ip + "|" + mode;
    if (stateKey == prevState)
        return;
    prevState = stateKey;

    TFT_eSprite sprite = display_sprite();
    sprite.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT);
    sprite.fillSprite(TFT_BLACK);

    pu8f->begin(sprite);
    pu8f->setBackgroundColor(TFT_BLACK);
    pu8f->setForegroundColor(TFT_WHITE);
    pu8f->setFontMode(1);

    pu8f->setFont(u8g2_font_logisoso32_tf);
    pu8f->setCursor(20, 60);
    pu8f->print("WiFi Info");

    pu8f->setFont(u8g2_font_helvR14_tf);

    int y = 110;
    pu8f->setCursor(20, y);
    pu8f->print("SSID: ");
    pu8f->print(ssid.length() ? ssid : "-");
    y += 28;
    pu8f->setCursor(20, y);
    pu8f->print("IP: ");
    pu8f->print(ip.length() ? ip : "-");
    y += 28;
    pu8f->setCursor(20, y);
    pu8f->print("Mode: ");
    pu8f->print(mode);

    pu8f->setCursor(20, SCREEN_HEIGHT - 20);
    pu8f->setFont(u8g2_font_helvR10_tf);
    pu8f->print("Click knob to return to clock");

    sprite.pushSprite(0, 0);
    delay(10);
    sprite.deleteSprite();
    pu8f->begin(*ptft);
}

void WiFiInfoScreen_input(int key)
{
    if (key == KNOB_CLICK)
    {
        JsonDocument &app = status();
        app["screen"] = CLOCKSCREEN;
    }
}
