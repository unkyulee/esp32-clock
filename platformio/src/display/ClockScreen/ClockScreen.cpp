#include "ClockScreen.h"
#include "app/app.h"
#include "display/display.h"

const int screen_width = 320;
const int screen_height = 240;

//
void ClockScreen_setup(TFT_eSPI *ptft, U8g2_for_TFT_eSPI *pu8f)
{
    ptft->fillScreen(TFT_BLACK);
    _log("Clock Screen Setup\n");
}

//
void ClockScreen_render(TFT_eSPI *ptft, U8g2_for_TFT_eSPI *pu8f)
{
    static String prevTime = "";

    // retrieve time
    struct tm timeinfo;
    if (getLocalTime(&timeinfo))
    {
        char buf[6]; // hh:mm + null terminator
        strftime(buf, sizeof(buf), "%H:%M", &timeinfo);
        String time = String(buf);

        //
        if (prevTime != time)
        {
            _debug("New Time: %s\n", time);
            JsonDocument &app = status();

            //
            TFT_eSprite sprite = display_sprite();
            sprite.createSprite(screen_width, screen_height);
            sprite.fillSprite(TFT_BLACK);

            pu8f->begin(sprite);
            pu8f->setFont(u8g2_font_logisoso92_tn);
            pu8f->setBackgroundColor(TFT_BLACK);
            pu8f->setForegroundColor(TFT_WHITE);
            pu8f->setFontMode(1);

            // Measure text dimensions
            int16_t textWidth = pu8f->getUTF8Width(time.c_str());
            int16_t textWidthOne = pu8f->getUTF8Width(" ") / 2;
            int16_t textHeight = pu8f->getFontAscent() - pu8f->getFontDescent();

            // Calculate centered coordinates
            int16_t x = (screen_width - textWidth - textWidthOne) / 2;
            int16_t y = (screen_height + textHeight - 25) / 2; // baseline is at y, not top

            // Set and draw
            pu8f->setCursor(x, y);

            //
            pu8f->print(time);

            // push sprite
            sprite.pushSprite(0, 0);
            delay(10);
            sprite.deleteSprite();
            pu8f->begin(*ptft);

            //
            prevTime = time;
        }
    }
    else
    {
        Serial.println("Failed to obtain time");
    }
}
