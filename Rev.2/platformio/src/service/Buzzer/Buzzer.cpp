#include "Buzzer.h"
#include "app/app.h"

//
void buzzer_setup()
{
    // setup buzzer pin
    ledcSetup(buzzerChannel, 2000, 8);       // 2 kHz, 8-bit resolution
    ledcAttachPin(buzzerPin, buzzerChannel); // Connect pin 7 to channel 0
}

//
void buzzer_loop()
{
    static unsigned int last = 0;
    if (millis() - last > 1000)
    {
        last = millis();

        //
        JsonDocument &app = status();
        bool ring = app["ring"].as<bool>();

        // start ringing
        if (ring)
        {
            //
            app["ring"] = false;

            //
            _log("[buzzer_loop] Start Ringing\n");

            //
            app["ring_start"] = true;
            buzzer_ring();
            app["ring_start"] = false;
        }
    }
}

//
void buzzer_ring()
{
    JsonDocument &app = status();

    const int melody[][2] = {
        {1000, 300}, {1200, 300}, {1500, 400}, {1000, 300}, {0, 200}, {1200, 300}, {1000, 400}};
    const int noteCount = sizeof(melody) / sizeof(melody[0]);

    for (int j = 0; j < 10; j++)
    {
        for (int i = 0; i < noteCount; i++)
        {
            if (app["ring_stop"].as<bool>())
            {
                app["ring_stop"] = false;
                noTone(buzzerPin);
                _log("[buzzer_ring] Ringtone stopped early.");
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
}