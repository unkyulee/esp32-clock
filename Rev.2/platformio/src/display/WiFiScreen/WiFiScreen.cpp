#include "WiFiScreen.h"
#include "app/app.h"
#include "display/display.h"

//
#include "service/AccessPoint/AccessPoint.h"
#include "service/ConfigServer/ConfigServer.h"
#include "service/Time/Time.h"

//
#include <WiFi.h>
#include <functional>

//
void WiFiScreen_setup(TFT_eSPI *ptft)
{
    ptft->fillScreen(TFT_BLACK);
    _log("WiFi Screen Setup\n");
}

#define WIFISCREEN_NOT_CONNECTED 0
#define WIFISCREEN_CONNECTED 1
#define WIFISCREEN_AP 2

static const uint16_t BG = 0x1082;       // deep gray-blue
static const uint16_t CARD = 0x2104;     // slightly lighter background
static const uint16_t ACCENT = 0x4E9B;   // teal-ish accent
static const uint16_t TEXT_MAIN = TFT_WHITE;
static const uint16_t TEXT_MUTED = 0xCE59;

static void drawLine(TFT_eSprite &sprite, int y)
{
    sprite.drawLine(20, y, SCREEN_WIDTH - 20, y, ACCENT);
}

static void drawRow(TFT_eSprite &sprite, int x, int y, const char *label, const String &value)
{
    sprite.setTextColor(TEXT_MUTED, CARD);
    sprite.setTextSize(1);
    sprite.setTextFont(2);
    sprite.setCursor(x, y);
    sprite.printf("%s", label);

    sprite.setTextColor(TEXT_MAIN, CARD);
    sprite.setCursor(x + 120, y);
    sprite.printf("%s", value.c_str());
}

static void renderCard(TFT_eSPI *ptft, const char *title, const char *subtitle, std::function<void(TFT_eSprite &)> body)
{
    TFT_eSprite sprite = display_sprite();
    sprite.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT);
    sprite.fillSprite(BG);

    int cardX = 12;
    int cardY = 26;
    int cardW = SCREEN_WIDTH - cardX * 2;
    int cardH = SCREEN_HEIGHT - cardY * 2;

    sprite.fillRoundRect(cardX, cardY, cardW, cardH, 12, CARD);
    sprite.drawRoundRect(cardX, cardY, cardW, cardH, 12, ACCENT);

    sprite.setTextColor(TEXT_MAIN, CARD);
    sprite.setTextSize(1);
    sprite.setTextFont(4);
    sprite.setCursor(cardX + 16, cardY + 16);
    sprite.printf("%s", title);

    sprite.setTextColor(TEXT_MUTED, CARD);
    sprite.setTextFont(2);
    sprite.setCursor(cardX + 16, cardY + 48); // add breathing room from title
    sprite.printf("%s", subtitle);

    drawLine(sprite, cardY + 70); // push divider below subtitle

    if (body)
    {
        body(sprite);
    }

    sprite.pushSprite(0, 0);
    sprite.deleteSprite();
}

//
void WiFiScreen_render(TFT_eSPI *ptft)
{
    int mode = 0;
    static int prevMode = 0;
    static bool serverStarted = false;
    static unsigned long connectedShownAt = 0;
    static String prevState = "";
    bool shouldRender = true;

    // Text to be displayed
    JsonDocument &app = status();

    if (app["wifi"])
    {
        mode = WIFISCREEN_CONNECTED;
        if (mode != prevMode)
        {
            connectedShownAt = millis();
        }

        String ssid = WiFi.SSID();
        if (!ssid.length() && app["config"]["ssid"].is<String>())
            ssid = app["config"]["ssid"].as<String>();
        String ip = WiFi.localIP().toString();
        if (!ip.length() && app["ip"].is<String>())
            ip = app["ip"].as<String>();
        String rssi = String(WiFi.RSSI()) + " dBm";

        // Build render state key to avoid unnecessary redraws
        String stateKey = "connected|" + ssid + "|" + ip + "|" + rssi;
        if (stateKey == prevState && mode == prevMode)
        {
            shouldRender = false;
        }
        else
        {
            prevState = stateKey;
        }

        if (shouldRender)
        {
            renderCard(ptft,
                       "Connected",
                       "Wi-Fi link is up",
                       [&](TFT_eSprite &sprite)
                       {
                           drawRow(sprite, 24, 100, "Network", ssid.length() ? ssid : "-");
                           drawRow(sprite, 24, 126, "IP Address", ip.length() ? ip : "-");
                           drawRow(sprite, 24, 152, "Signal", rssi);
                           drawRow(sprite, 24, 178, "Mode", "Station");
                       });
        }

        if (!serverStarted)
        {
            startWebServer();
            serverStarted = true;
        }

        // update time once connected
        updateTime();

        // Show info briefly before moving on
        if (millis() - connectedShownAt > 1200)
        {
            app["screen"] = CLOCKSCREEN;
        }
    }

    // Start AP
    else
    {
        if (app["apIP"].is<String>() == false)
        {
            mode = WIFISCREEN_NOT_CONNECTED;
            String stateKey = "starting_ap";
            if (stateKey != prevState || mode != prevMode)
            {
                prevState = stateKey;
                renderCard(ptft,
                           "Connecting...",
                           "No Wi-Fi, starting config portal",
                           [&](TFT_eSprite &sprite)
                           {
                               drawRow(sprite, 24, 100, "Status", "Starting Access Point");
                               drawRow(sprite, 24, 126, "Action", "Join AP to setup Wi-Fi");
                           });
            }
            // make wifi access point and display the IP information
            startAccessPoint();
            // start configuration web server
            if (!serverStarted)
            {
                startWebServer();
                serverStarted = true;
            }
        }
        else
        {
            mode = WIFISCREEN_AP;
            String apIP = app["apIP"].as<String>();
            String stateKey = "ap|" + apIP;
            if (stateKey != prevState || mode != prevMode)
            {
                prevState = stateKey;
                renderCard(ptft,
                           "Config Portal",
                           "Connect to configure Wi-Fi",
                           [&](TFT_eSprite &sprite)
                           {
                               drawRow(sprite, 24, 100, "SSID", WiFi.softAPSSID());
                               drawRow(sprite, 24, 126, "IP Address", apIP);
                               drawRow(sprite, 24, 152, "Mode", "Access Point");
                           });
            }
        }
    }

    //
    prevMode = mode;
}
