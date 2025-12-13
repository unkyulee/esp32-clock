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
        // Load alarm days mask (Mon-Sun) or fallback to weekday flag
        bool alarmDays[7] = {true, true, true, true, true, false, false};
        if (app["config"]["alarmDays"].is<String>())
        {
            String mask = app["config"]["alarmDays"].as<String>();
            if (mask.length() == 7)
            {
                for (int i = 0; i < 7; ++i)
                    alarmDays[i] = (mask.charAt(i) == '1');
            }
        }
        else if (app["config"]["alarmWeekdays"].is<bool>())
        {
            bool weekdaysOnly = app["config"]["alarmWeekdays"].as<bool>();
            for (int i = 0; i < 7; ++i)
                alarmDays[i] = (i < 5) ? weekdaysOnly : true;
        }

        // Precompute alarm hour/minute once
        bool alarmConfigured = alarmEnabled && alarmTime.length() == 5;
        int alarmHour = alarmConfigured ? alarmTime.substring(0, 2).toInt() : -1;
        int alarmMinute = alarmConfigured ? alarmTime.substring(3, 5).toInt() : -1;
        int currentDay = (timeinfo.tm_wday + 6) % 7; // 0 = Mon

        // Decide if alarm should ring right now (independent of banner)
        if (alarmConfigured && timeinfo.tm_hour == alarmHour && timeinfo.tm_min == alarmMinute)
        {
            if (alarmDays[currentDay])
            {
                app["ring"] = true;
                app["ring_start"] = true;
            }
        }

        // Decide if alarm banner should show (within next 24 hours)
        bool showAlarmBanner = false;
        String alarmBannerText = alarmTime;
        if (alarmConfigured)
        {
            int nowMinutes = currentDay * 1440 + timeinfo.tm_hour * 60 + timeinfo.tm_min;
            int bestDiff = 7 * 1440 + 1;
            for (int i = 0; i < 7; ++i)
            {
                int dayIdx = (currentDay + i) % 7;
                if (!alarmDays[dayIdx])
                    continue;
                int candidate = (currentDay + i) * 1440 + alarmHour * 60 + alarmMinute;
                int diff = candidate - nowMinutes;
                if (diff < 0)
                    diff += 7 * 1440;
                if (diff < bestDiff)
                    bestDiff = diff;
            }
            if (bestDiff <= 24 * 60)
                showAlarmBanner = true;
        }

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
            int16_t x = (SCREEN_WIDTH - textWidth) / 2 - 5; // shift slightly left
            pu8f->setCursor(x, y);
            pu8f->print(currentTime);

            // === Draw alarm indicator as a bottom banner if upcoming ===
            if (showAlarmBanner)
            {
                int barH = 26;
                sprite.fillRect(0, SCREEN_HEIGHT - barH, SCREEN_WIDTH, barH, TFT_DARKGREY);
                pu8f->setFont(u8g2_font_helvR14_tf);
                pu8f->setBackgroundColor(TFT_DARKGREY);
                pu8f->setForegroundColor(TFT_WHITE);
                int16_t tW = pu8f->getUTF8Width(alarmBannerText.c_str());
                int16_t tH = pu8f->getFontAscent() - pu8f->getFontDescent();
                int16_t xAlarm = (SCREEN_WIDTH - tW) / 2;
                int16_t yAlarm = SCREEN_HEIGHT - barH + (barH + tH) / 2 - 5;
                pu8f->setCursor(xAlarm, yAlarm);
                pu8f->print(alarmBannerText);
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
