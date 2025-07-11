#include "TimerScreen.h"
#include "app/app.h"
#include "display/display.h"
#include "input/Knob/Knob.h"

const int buzzerPin = 7;
const int buzzerChannel = 0; // Use any free channel 0–15

//
void TimerScreen_setup(TFT_eSPI *ptft, U8g2_for_TFT_eSPI *pu8f)
{
    //
    // Attach the buzzer pin to a PWM channel
    ledcSetup(buzzerChannel, 2000, 8);       // 2 kHz, 8-bit resolution
    ledcAttachPin(buzzerPin, buzzerChannel); // Connect pin 47 to channel 0

    //
    ptft->fillScreen(TFT_BLACK);
    _log("Timer Screen Setup\n");

    // reset the timer
    JsonDocument &app = status();
    app["timer"] = 0.0;
    app["timerStarted"] = false;
    app["timerRing"] = false;
}

//
void TimerScreen_render(TFT_eSPI *ptft, U8g2_for_TFT_eSPI *pu8f)
{
    // retrieve time
    JsonDocument &app = status();
    bool timerStarted = app["timerStarted"].as<bool>();
    float timer = app["timer"].as<float>();
    static unsigned int last = millis();

    //
    String title;
    static String prevTitle = "";
    if (timerStarted)
    {
        //
        title = "TIMER";

        // reduce the timer
        timer -= (millis() - last) / 60000.0;

        // if the timer reaches below 0 then ring
        if (timer < 0)
        {
            timer = 0.0;
            app["timerRing"] = true;

            TimerScreen_ring();
        }
    }
    else
    {
        title = "SET TIMER";
        last = millis();
    }

    //
    int totalSeconds = round(timer * 60);
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    char buf[9]; // hh:mm + null
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", hours, minutes, seconds);
    String timerFormat = String(buf);
    static String prevTimerFormat = "";

    // RENDER TIMER
    if (prevTimerFormat != timerFormat || prevTitle != title)
    {
        //
        TFT_eSprite sprite = display_sprite();
        sprite.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT);
        sprite.fillSprite(TFT_BLACK);

        pu8f->begin(sprite);

        pu8f->setBackgroundColor(TFT_BLACK);
        pu8f->setForegroundColor(TFT_WHITE);
        pu8f->setFontMode(1);

        // Set and draw

        pu8f->setFont(u8g2_font_logisoso62_tn);

        // Measure text dimensions
        int16_t textWidth = pu8f->getUTF8Width(timerFormat.c_str());
        int16_t textHeight = pu8f->getFontAscent() - pu8f->getFontDescent();

        // Calculate centered coordinates
        int16_t x = (SCREEN_WIDTH - textWidth) / 2;
        int16_t y = (SCREEN_HEIGHT + textHeight) / 2; // baseline is at y, not top

        //_log("textWidth: %d x: %d textHeight: %d y: %d\n", textWidth, x, textHeight, y);

        //
        pu8f->setCursor(x, y);
        pu8f->print(timerFormat);

        //
        pu8f->setCursor(10, 50);
        pu8f->setFont(u8g2_font_logisoso22_tr);

        // Show that it's a timer
        pu8f->print(title);

        // push sprite
        sprite.pushSprite(0, 0);
        sprite.deleteSprite();
        pu8f->begin(*ptft);

        //
        prevTimerFormat = timerFormat;
        prevTitle = title;
    }
}

//
void TimerScreen_input(int key)
{
    JsonDocument &app = status();

    //
    if (key == KNOB_UP)
    {
        _log("up\n");

        //
        float timer = app["timer"].as<int>();
        if (timer < 30)
            timer += 1.0;
        else if (timer >= 30)
            timer += 5.0;

        app["timer"] = timer;
    }

    else if (key == KNOB_DOWN)
    {
        _log("down\n");
        //
        JsonDocument &app = status();
        float timer = app["timer"].as<int>();

        if (timer < 30)
            timer -= 1.0;
        else if (timer >= 30)
            timer -= 5.0;

        if (timer <= 0)
            timer = 0;

        app["timer"] = timer;
    }

    // move to timer screen at the knob click
    else if (key == 9)
    {
        // start the timer
        bool timerStarted = app["timerStarted"].as<bool>();
        timerStarted = !timerStarted;
        app["timerStarted"] = timerStarted;

        _log("Timer Started: %d\n", timerStarted);
    }

    // 1 - 8 button will stop the timer
    else if (key >= 1 && key <= 8)
    {
        app["timerRing"] = false;
        app["screen"] = CLOCKSCREEN;
    }
}

void TimerScreen_ring()
{
    _log("Timer Ringing\n");

    //
    JsonDocument &app = status();

    const int melody[][2] = {
        {1000, 300},
        {1200, 300},
        {1500, 400},
        {1000, 300},
        {0, 200}, // pause
        {1200, 300},
        {1000, 400}};
    const int noteCount = sizeof(melody) / sizeof(melody[0]);

    for (int j = 0; j < 10; j++)
    {
        for (int i = 0; i < noteCount; i++)
        {
            if (app["timerRing"].as<bool>() == false)
            {
                noTone(buzzerPin); // stop sound
                Serial.println("Ringtone stopped early.");
                return;
            }

            int freq = melody[i][0];
            int dur = melody[i][1];

            if (freq > 0)
            {
                tone(buzzerPin, freq);
            }
            else
            {
                noTone(buzzerPin); // silent pause
            }

            delay(dur);
        }
    }

    noTone(buzzerPin); // ensure it's turned off

    app["timerRing"] = false;
    app["screen"] = CLOCKSCREEN;
}