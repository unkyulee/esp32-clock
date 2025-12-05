#include "AlarmScreen.h"
#include "app/app.h"
#include "display/display.h"
#include "input/Knob/Knob.h"

bool alarmEnabled = false;
int alarmHour = 7;
int alarmMinute = 30;
int alarmStep = 0; // 0 = toggle, 1 = hour, 2 = minute
bool screenDirty = true;

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
    pu8f->setFont(u8g2_font_logisoso32_tf);
    pu8f->setFontMode(1);
    pu8f->setBackgroundColor(TFT_BLACK);

    const char *labels[3] = {"Alarm", "Hour", "Minute"};
    const int yStart = 40;
    const int yStep = 50;

    for (int i = 0; i < 3; ++i)
    {
        String value;
        if (i == 0)
            value = alarmEnabled ? "ON" : "OFF";
        else if (i == 1)
            value = (alarmHour < 10 ? "0" : "") + String(alarmHour);
        else
            value = (alarmMinute < 10 ? "0" : "") + String(alarmMinute);

        String line = String(labels[i]) + ": " + value;

        // Measure actual text dimensions
        int textWidth = pu8f->getUTF8Width(line.c_str());
        int textHeight = pu8f->getFontAscent() - pu8f->getFontDescent();
        int x = (SCREEN_WIDTH - textWidth) / 2;
        int y = yStart + i * yStep;

        // Highlight box behind text
        if (i == alarmStep)
        {
            sprite.fillRect(0, y - textHeight, SCREEN_WIDTH, textHeight + 10, TFT_WHITE); // +10 padding
            pu8f->setForegroundColor(TFT_BLACK);
        }
        else
        {
            pu8f->setForegroundColor(TFT_WHITE);
        }

        pu8f->setCursor(x, y);
        pu8f->print(line);
    }

    sprite.pushSprite(0, 0);
    sprite.deleteSprite();
    pu8f->begin(*ptft);
}

void AlarmScreen_input(int key)
{
    JsonDocument &app = status();

    switch (key)
    {
    case KNOB_UP:
        switch (alarmStep)
        {
        case 0:
            alarmEnabled = true;
            break;
        case 1:
            alarmHour = (alarmHour + 1) % 24;
            break;
        case 2:
            alarmMinute = (alarmMinute + 1) % 60;
            break;
        }
        screenDirty = true;
        break;

    case KNOB_DOWN:
        switch (alarmStep)
        {
        case 0:
            alarmEnabled = false;
            break;
        case 1:
            alarmHour = (alarmHour - 1 + 24) % 24;
            break;
        case 2:
            alarmMinute = (alarmMinute - 1 + 60) % 60;
            break;
        }
        screenDirty = true;
        break;

    case KNOB_CLICK:
        alarmStep = (alarmStep + 1) % 3;
        if (alarmStep == 0)
        {
            // Save to app when user exits
            char buf[6];
            sprintf(buf, "%02d:%02d", alarmHour, alarmMinute);
            app["config"]["alarm"] = String(buf);
            app["config"]["alarmEnabled"] = alarmEnabled;
            config_save();

            // move to clock screen
            app["screen"] = CLOCKSCREEN;
        }
        screenDirty = true;
        break;
    }
}
