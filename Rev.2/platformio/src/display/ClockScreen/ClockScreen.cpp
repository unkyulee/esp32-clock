#include "ClockScreen.h"
#include "app/app.h"
#include "display/display.h"
#include "service/Knob/Knob.h"

// Refresh Screen Trigger
String prevTime = "";

//
void ClockScreen_setup(TFT_eSPI *ptft, U8g2_for_TFT_eSPI *pu8f)
{
    _log("Clock Screen Setup\n");

    //
    ptft->fillScreen(TFT_BLACK);

    // reset refresh screen
    prevTime = "";
}
void ClockScreen_render(TFT_eSPI *ptft, U8g2_for_TFT_eSPI *pu8f)
{
    static unsigned int last = 0;
    if (millis() - last > 1000)
    {
        last = millis();

        struct tm timeinfo;
        if (!getLocalTime(&timeinfo))
            return;

        char buf[6];
        strftime(buf, sizeof(buf), "%H:%M", &timeinfo);
        String currentTime = String(buf);

        JsonDocument &app = status();
        bool alarmEnabled = app["config"]["alarmEnabled"].as<bool>();
        String alarmTime = "";
        if (app["config"]["alarm"].is<String>())
            alarmTime = app["config"]["alarm"].as<String>();

        // === Only re-render screen if time changed ===
        if (prevTime != currentTime)
        {
            TFT_eSprite sprite = display_sprite();
            sprite.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT);
            sprite.fillSprite(TFT_BLACK);

            pu8f->begin(sprite);
            pu8f->setFont(u8g2_font_logisoso92_tn);
            pu8f->setBackgroundColor(TFT_BLACK);
            pu8f->setForegroundColor(TFT_WHITE);
            pu8f->setFontMode(1);

            // Draw the big centered clock
            int16_t ascent = pu8f->getFontAscent();
            int16_t descent = pu8f->getFontDescent();
            int16_t textHeight = ascent - descent;
            int16_t y = (SCREEN_HEIGHT / 2) - (textHeight / 2) + ascent; // center vertically using ascent/descent

            int16_t textWidth = pu8f->getUTF8Width(currentTime.c_str());
            int16_t x = (SCREEN_WIDTH - textWidth) / 2 - 10; // shift slightly left
            pu8f->setCursor(x, y);
            pu8f->print(currentTime);

            // === Draw alarm time in top-right corner ===
            if (alarmEnabled && alarmTime.length() > 0)
            {
                pu8f->setFont(u8g2_font_6x13_tf); // small, unobtrusive font
                pu8f->setForegroundColor(TFT_YELLOW);
                pu8f->setCursor(10, 220);
                pu8f->print(alarmTime);

                // === Trigger alarm ringing ===
                if (alarmTime == currentTime && alarmTime.length() > 0)
                {
                    app["ring"] = true;
                    app["ring_start"] = true;
                }
            }

            sprite.pushSprite(0, 0);
            delay(10);
            sprite.deleteSprite();
            pu8f->begin(*ptft);

            prevTime = currentTime;
        }
    }
}

//
void ClockScreen_input(int key)
{
    JsonDocument &app = status();

    // STOP RINGING WHEN KNOB IS CLICKED
    if (app["ring_start"].as<bool>())
    {
        //
        app["ring_stop"] = true;
        _log("Knob clicked stop ringing\n");

        //
        app["screen"] = CLOCKSCREEN;

        //
        return;
    }

    // move to menu screen when knob is moved or clicked
    if (key == KNOB_UP || key == KNOB_DOWN || key == KNOB_CLICK)
    {
        // move to timer screen when knob is moved
        app["screen"] = MENUSCREEN;
    }
}
