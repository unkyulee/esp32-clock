#include "TimerScreen.h"
#include "app/app.h"
#include "display/display.h"
#include "input/Knob/Knob.h"

const int buzzerPin = 7;
const int buzzerChannel = 0; // Use any free channel 0–15

//
void TimerScreen_setup(TFT_eSPI *ptft, U8g2_for_TFT_eSPI *pu8f)
{
    ledcSetup(buzzerChannel, 2000, 8);       // 2 kHz, 8-bit resolution
    ledcAttachPin(buzzerPin, buzzerChannel); // Connect pin 7 to channel 0

    ptft->fillScreen(TFT_BLACK);
    _log("Timer Screen Setup\n");

    JsonDocument &app = status();
    app["timer"] = 0.0;
    app["timerStarted"] = false;
    app["timerRing"] = false;
}

//
void TimerScreen_render(TFT_eSPI *ptft, U8g2_for_TFT_eSPI *pu8f)
{
    JsonDocument &app = status();

    static unsigned int last = millis();
    static String prevTitle = "";
    static String prevTimerFormat = "";

    bool timerStarted = app["timerStarted"].as<bool>();
    float timer = app["timer"].as<float>();
    String title;

    if (timerStarted)
    {
        title = "TIMER";

        // Update timer
        unsigned int now = millis();
        timer -= (now - last) / 60000.0; // Convert ms to minutes
        last = now;

        if (timer < 0.0f)
        {
            timer = 0.0;
            app["timerRing"] = true;
            TimerScreen_ring();
        }

        app["timer"] = timer; // Save updated timer
    }
    else
    {
        title = "SET TIMER";
        last = millis(); // Reset last to now to avoid skipping when started
    }

    // Format timer into hh:mm:ss
    int totalSeconds = round(timer * 60);
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    char buf[9];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", hours, minutes, seconds);
    String timerFormat(buf);

    if (prevTimerFormat != timerFormat || prevTitle != title)
    {
        TFT_eSprite sprite = display_sprite();
        sprite.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT);
        sprite.fillSprite(TFT_BLACK);

        pu8f->begin(sprite);
        pu8f->setBackgroundColor(TFT_BLACK);
        pu8f->setForegroundColor(TFT_WHITE);
        pu8f->setFontMode(1);

        pu8f->setFont(u8g2_font_logisoso62_tn);
        int textWidth = pu8f->getUTF8Width(timerFormat.c_str());
        int textHeight = pu8f->getFontAscent() - pu8f->getFontDescent();
        int x = (SCREEN_WIDTH - textWidth) / 2;
        int y = (SCREEN_HEIGHT + textHeight) / 2;

        pu8f->setCursor(x, y);
        pu8f->print(timerFormat);

        pu8f->setFont(u8g2_font_logisoso22_tr);
        pu8f->setCursor(10, 50);
        pu8f->print(title);

        sprite.pushSprite(0, 0);
        sprite.deleteSprite();
        pu8f->begin(*ptft);

        prevTimerFormat = timerFormat;
        prevTitle = title;
    }
}

//
void TimerScreen_input(int key)
{
    JsonDocument &app = status();
    float timer = app["timer"].as<float>();

    switch (key)
    {
    case KNOB_UP:
        _log("up\n");
        timer += (timer < 30) ? 1.0f : 5.0f;
        break;

    case KNOB_DOWN:
        _log("down\n");
        timer -= (timer < 30) ? 1.0f : 5.0f;
        if (timer < 0.0f)
            timer = 0.0f;
        break;

    case 9: // Click to start/pause
        if (app["timer"].as<int>() <= 0.0)
        {
            app["timerRing"] = false;
            app["screen"] = CLOCKSCREEN;
        }

        //
        app["timerStarted"] = !app["timerStarted"].as<bool>();
        break;

    default:
        return; // Ignore unknown keys
    }

    app["timer"] = timer;
}

void TimerScreen_ring()
{
    _log("Timer Ringing\n");

    JsonDocument &app = status();

    const int melody[][2] = {
        {1000, 300}, {1200, 300}, {1500, 400}, {1000, 300}, {0, 200}, {1200, 300}, {1000, 400}};
    const int noteCount = sizeof(melody) / sizeof(melody[0]);

    for (int j = 0; j < 10; j++)
    {
        for (int i = 0; i < noteCount; i++)
        {
            if (!app["timerRing"].as<bool>())
            {
                noTone(buzzerPin);
                Serial.println("Ringtone stopped early.");
                return;
            }

            int freq = melody[i][0];
            int dur = melody[i][1];

            if (freq > 0)
                tone(buzzerPin, freq);
            else
                noTone(buzzerPin);

            delay(dur);
        }
    }

    noTone(buzzerPin);
    app["timerRing"] = false;
    app["screen"] = CLOCKSCREEN;
}