#include "AlarmScreen.h"
#include "app/app.h"
#include "display/display.h"
#include "service/Knob/Knob.h"

bool alarmEnabled = false;
int alarmHour = 7;
int alarmMinute = 30;
bool alarmDays[7] = {true, true, true, true, true, false, false}; // Mon-Sun
int alarmStep = 0; // 0 = toggle, 1 = hour, 2 = minute, 3..9 = days, 10 = exit
bool screenDirty = true;
const int totalSteps = 11;

void AlarmScreen_setup(TFT_eSPI *ptft, U8g2_for_TFT_eSPI *pu8f)
{
    _log("Alarm Screen Setup\n");

    JsonDocument &app = status();

    // Load alarm state from saved app state (if available)
    if (app["config"]["alarm"].is<const char *>())
    {
        String alarmStr = app["config"]["alarm"].as<String>(); // Format: "HH:MM"
        int sep = alarmStr.indexOf(':');
        if (sep > 0)
        {
            alarmHour = alarmStr.substring(0, sep).toInt();
            alarmMinute = alarmStr.substring(sep + 1).toInt();
        }
    }

    if (app["config"]["alarmEnabled"].as<bool>())
    {
        alarmEnabled = app["config"]["alarmEnabled"];
    }
    else
    {
        alarmEnabled = false; // default to off
    }

    // load day selection
    if (app["config"]["alarmDays"].is<String>())
    {
        String mask = app["config"]["alarmDays"].as<String>();
        if (mask.length() == 7)
        {
            for (int i = 0; i < 7; ++i)
            {
                alarmDays[i] = (mask.charAt(i) == '1');
            }
        }
    }
    else if (app["config"]["alarmWeekdays"].is<bool>())
    {
        bool weekdaysOnly = app["config"]["alarmWeekdays"].as<bool>();
        for (int i = 0; i < 7; ++i)
        {
            alarmDays[i] = (i < 5) ? weekdaysOnly : true; // default to weekdays if set, else all days
        }
    }
    // else keep defaults

    // Reset step and mark screen dirty so it renders immediately
    alarmStep = 0;
    screenDirty = true;

    // Clear screen
    ptft->fillScreen(TFT_BLACK);
}

void AlarmScreen_render(TFT_eSPI *ptft, U8g2_for_TFT_eSPI *pu8f)
{
    if (!screenDirty)
        return;
    screenDirty = false;

    TFT_eSprite sprite = display_sprite();
    sprite.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT);
    sprite.fillSprite(TFT_BLACK);

    pu8f->begin(sprite);
    pu8f->setFont(u8g2_font_logisoso20_tf);
    pu8f->setFontMode(1);
    pu8f->setBackgroundColor(TFT_BLACK);

    auto drawLine = [&](int stepIndex, const char *label, const String &value, int y) {
        String line = value.length() > 0 ? String(label) + ": " + value : String(label);
        int textWidth = pu8f->getUTF8Width(line.c_str());
        int textHeight = pu8f->getFontAscent() - pu8f->getFontDescent();
        int x = (SCREEN_WIDTH - textWidth) / 2;

        bool selected = (alarmStep == stepIndex);
        pu8f->setFontMode(0); // opaque text to avoid artifacts
        pu8f->setBackgroundColor(selected ? TFT_WHITE : TFT_BLACK);
        pu8f->setForegroundColor(selected ? TFT_BLACK : TFT_WHITE);
        if (selected)
        {
            sprite.fillRect(0, y - textHeight, SCREEN_WIDTH, textHeight + 6, TFT_WHITE);
        }
        pu8f->setCursor(x, y);
        pu8f->print(line);
    };

    const int yStart = 28;
    const int yStep = 26;

    // Alarm, Hour, Minute
    drawLine(0, "Alarm", alarmEnabled ? "ON" : "OFF", yStart);
    drawLine(1, "Hour", (alarmHour < 10 ? "0" : "") + String(alarmHour), yStart + yStep);
    drawLine(2, "Minute", (alarmMinute < 10 ? "0" : "") + String(alarmMinute), yStart + 2 * yStep);

    // Weekday dots row
    const char dayLabels[7] = {'M', 'T', 'W', 'T', 'F', 'S', 'S'};
    int radius = 12;
    int xStart = 12;
    int spacing = (SCREEN_WIDTH - 2 * xStart) / 7;
    int yDays = yStart + 3 * yStep + 14;

    pu8f->setFont(u8g2_font_helvR10_tf);
    pu8f->setFontMode(0); // opaque for in-circle labels
    for (int i = 0; i < 7; ++i)
    {
        int stepIndex = 3 + i;
        int xCenter = xStart + spacing * i + spacing / 2;
        bool selected = (alarmStep == stepIndex);

        if (selected)
        {
            sprite.fillRoundRect(xCenter - radius - 6, yDays - radius - 8, radius * 2 + 12, radius * 2 + 22, 8, TFT_WHITE);
            sprite.fillRoundRect(xCenter - radius - 4, yDays - radius - 6, radius * 2 + 8, radius * 2 + 18, 8, TFT_BLACK);
        }

        uint16_t dotColor = alarmDays[i] ? TFT_WHITE : TFT_DARKGREY;
        sprite.fillCircle(xCenter, yDays, radius, dotColor);
        sprite.drawCircle(xCenter, yDays, radius, TFT_WHITE);

        pu8f->setFont(u8g2_font_helvR12_tf);
        pu8f->setBackgroundColor(dotColor);
        pu8f->setForegroundColor(alarmDays[i] ? TFT_BLACK : TFT_WHITE); // contrast
        char labelStr[2] = {dayLabels[i], 0};
        int labelWidth = pu8f->getUTF8Width(labelStr);
        int labelHeight = pu8f->getFontAscent() - pu8f->getFontDescent();
        pu8f->setCursor(xCenter - labelWidth / 2, yDays + labelHeight / 2);
        pu8f->print(labelStr);
    }
    pu8f->setFontMode(1); // restore transparent mode
    pu8f->setBackgroundColor(TFT_BLACK);
    pu8f->setForegroundColor(TFT_WHITE);

    // Exit
    pu8f->setFont(u8g2_font_logisoso18_tf);
    drawLine(10, "Exit", "", yDays + radius + 36);

    sprite.pushSprite(0, 0);
    sprite.deleteSprite();
    pu8f->begin(*ptft);
}

void AlarmScreen_input(int key)
{
    JsonDocument &app = status();

    switch (key)
    {
    case KNOB_HOLD:
        // accelerate increment while holding on hour/minute
        if (alarmStep == 1)
            alarmHour = (alarmHour + 1) % 24;
        else if (alarmStep == 2)
            alarmMinute = (alarmMinute + 1) % 60;
        screenDirty = true;
        break;
    case KNOB_UP:
        alarmStep = (alarmStep + 1) % totalSteps;
        screenDirty = true;
        break;

    case KNOB_DOWN:
        alarmStep = (alarmStep - 1 + totalSteps) % totalSteps;
        screenDirty = true;
        break;

    case KNOB_CLICK:
        if (alarmStep == 0)
        {
            alarmEnabled = !alarmEnabled;
        }
        else if (alarmStep == 1)
        {
            alarmHour = (alarmHour + 1) % 24;
        }
        else if (alarmStep == 2)
        {
            alarmMinute = (alarmMinute + 1) % 60;
        }
        else if (alarmStep >= 3 && alarmStep <= 9)
        {
            alarmDays[alarmStep - 3] = !alarmDays[alarmStep - 3];
        }
        else if (alarmStep == 10)
        {
            // Save to app when exiting
            char buf[6];
            sprintf(buf, "%02d:%02d", alarmHour, alarmMinute);
            app["config"]["alarm"] = String(buf);
            app["config"]["alarmEnabled"] = alarmEnabled;
            String mask = "";
            for (int i = 0; i < 7; ++i)
                mask += alarmDays[i] ? "1" : "0";
            app["config"]["alarmDays"] = mask;
            app["config"]["alarmWeekdays"] = alarmDays[0] && alarmDays[1] && alarmDays[2] && alarmDays[3] && alarmDays[4] && !alarmDays[5] && !alarmDays[6];
            config_save();

            app["screen"] = CLOCKSCREEN;
        }

        screenDirty = true;
        break;
    }
}
