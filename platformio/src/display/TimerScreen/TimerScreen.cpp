#include "TimerScreen.h"
#include "app/app.h"
#include "display/display.h"
#include "input/Knob/Knob.h"

//
unsigned long lastTimer = 0;
String lastTitle = "";
String lastTimerText = "";

//
void TimerScreen_setup(TFT_eSPI *ptft, U8g2_for_TFT_eSPI *pu8f)
{
    //
    _log("Timer Screen Setup\n");

    // Clear Screen
    ptft->fillScreen(TFT_BLACK);

    //
    JsonDocument &app = status();
    app["timer"] = 0.0;
    app["timerStarted"] = false;
    app["ring"] = false;

    //
    lastTimer = 0;
    lastTitle = "";
    lastTimerText = "";
}

String format_timer(float timer)
{
    int totalSeconds = round(timer * 60);
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    char buf[9];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", hours, minutes, seconds);
    return String(buf);
}

void TimerScreen_draw(TFT_eSPI *ptft, U8g2_for_TFT_eSPI *pu8f, const String &title, const String &timerText)
{
    if (title == lastTitle && timerText == lastTimerText)
        return; // Skip rendering if nothing has changed

    lastTitle = title;
    lastTimerText = timerText;

    TFT_eSprite sprite = display_sprite();
    sprite.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT);
    sprite.fillSprite(TFT_BLACK);

    pu8f->begin(sprite);
    pu8f->setFont(u8g2_font_logisoso62_tn);

    // Title
    pu8f->setBackgroundColor(TFT_BLACK);
    pu8f->setForegroundColor(TFT_WHITE);
    pu8f->setFontMode(1);

    //
    pu8f->setFont(u8g2_font_logisoso22_tr);
    pu8f->setCursor(10, 50);
    pu8f->print(title);

    // Timer
    pu8f->setFont(u8g2_font_logisoso62_tn);

    int textWidth = pu8f->getUTF8Width(timerText.c_str());
    int textHeight = pu8f->getFontAscent() - pu8f->getFontDescent();
    int x = (SCREEN_WIDTH - textWidth) / 2;
    int y = (SCREEN_HEIGHT + textHeight) / 2;

    pu8f->setCursor(x, y);
    pu8f->print(timerText);

    sprite.pushSprite(0, 0);
    sprite.deleteSprite();
    pu8f->begin(*ptft);
}

void TimerScreen_render(TFT_eSPI *ptft, U8g2_for_TFT_eSPI *pu8f)
{
    JsonDocument &app = status();
    bool timerStarted = app["timerStarted"].as<bool>();
    float timer = app["timer"].as<float>();

    String title;
    String timerText;

    if (timerStarted)
    {
        title = "TIMER RUNNING";

        // Update the timer
        if (timer > 0.0)
        {
            unsigned int now = millis();
            timer -= (now - lastTimer) / 60000.0; // ms to minutes
            lastTimer = now;
        }
        else
        {
            // trigger ringing only once
            if (!app["ring_start"].as<bool>())
            {
                timer = 0.0;
                app["ring"] = true;
                app["ring_start"] = true;
            }
        }

        app["timer"] = timer;
    }
    else
    {
        title = "TIMER SET";
    }

    timerText = format_timer(timer);
    TimerScreen_draw(ptft, pu8f, title, timerText);
}

//
void TimerScreen_input(int key)
{
    JsonDocument &app = status();

    switch (key)
    {
    case KNOB_UP:
    {
        float timer = app["timer"].as<float>();
        timer += (timer < 30) ? 1.0f : 5.0f;
        app["timer"] = timer;
    }

    break;

    case KNOB_DOWN:
    {
        float timer = app["timer"].as<float>();
        timer -= (timer < 30) ? 1.0f : 5.0f;
        if (timer < 0.0f)
            timer = 0.0f;
        app["timer"] = timer;
    }

    break;

    case KNOB_CLICK: // KNOB Click to start/pause

        // when ringing is going on stop the ring
        if (app["timerStarted"].as<bool>())
        {
            //
            app["ring_stop"] = true;
            _log("Knob clicked stop ringing\n");

            //
            app["screen"] = CLOCKSCREEN;
        }
        else
        {
            // while setting the timer
            // start the timer
            app["timerStarted"] = true;

            //
            lastTimer = millis();
        }

        break;

    default:
        return; // Ignore unknown keys
    }
}
