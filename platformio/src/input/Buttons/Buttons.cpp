#include "Buttons.h"
#include "app/app.h"
#include "display/display.h"
#include "input/Buttons/Buttons.h"

#include <Button2.h>

#define NUM_BUTTONS 8

const int buttonPins[NUM_BUTTONS] = {15, 16, 17, 18, 1, 2, 42, 41};
Button2 buttons[NUM_BUTTONS];

#include <BleKeyboard.h>

BleKeyboard bleKeyboard;

void playToken(String token)
{
    if (token == "NEXT_TRACK")
        bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
    else if (token == "PREVIOUS_TRACK")
        bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
    else if (token == "STOP")
        bleKeyboard.write(KEY_MEDIA_STOP);
    else if (token == "PLAY_PAUSE")
        bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
    else if (token == "MUTE")
        bleKeyboard.write(KEY_MEDIA_MUTE);
    else if (token == "VOLUME_UP")
        bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
    else if (token == "VOLUME_DOWN")
        bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
    else if (token == "WWW_HOME")
        bleKeyboard.write(KEY_MEDIA_WWW_HOME);
    else if (token == "LOCAL_MACHINE_BROWSER")
        bleKeyboard.write(KEY_MEDIA_LOCAL_MACHINE_BROWSER);
    else if (token == "CALCULATOR")
        bleKeyboard.write(KEY_MEDIA_CALCULATOR);
    else if (token == "WWW_BOOKMARKS")
        bleKeyboard.write(KEY_MEDIA_WWW_BOOKMARKS);
    else if (token == "WWW_SEARCH")
        bleKeyboard.write(KEY_MEDIA_WWW_SEARCH);
    else if (token == "WWW_STOP")
        bleKeyboard.write(KEY_MEDIA_WWW_STOP);
    else if (token == "WWW_BACK")
        bleKeyboard.write(KEY_MEDIA_WWW_BACK);
    else if (token == "CONSUMER_CONTROL_CONFIGURATION")
        bleKeyboard.write(KEY_MEDIA_CONSUMER_CONTROL_CONFIGURATION);
    else if (token == "EMAIL_READER")
        bleKeyboard.write(KEY_MEDIA_EMAIL_READER);
    else
        bleKeyboard.write(token.c_str()[0]);
}

void handleTap(Button2 &b)
{

    JsonDocument &app = status();

    int buttonId = b.getPin(); // get the pin number
    int buttonIndex = -1;

    // Map pin number to button index (1–8)
    for (int i = 0; i < 8; i++)
    {
        if (b.getPin() == buttonPins[i])
        {
            buttonIndex = i + 1;
            break;
        }
    }
    _log("Button %d clicked\n", buttonIndex);

    if (buttonIndex == -1)
        return; // invalid button

    if (!bleKeyboard.isConnected())
        return;

    String key = "button_" + String(buttonIndex);
    String tokenList = app["config"][key].as<String>();

    int start = 0;
    while (start < tokenList.length())
    {
        int end = tokenList.indexOf(',', start);
        if (end == -1)
            end = tokenList.length();

        String token = tokenList.substring(start, end);
        token.trim();

        playToken(token);

        start = end + 1;
    }
}

void buttons_setup()
{
    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        buttons[i].begin(buttonPins[i]);
        buttons[i].setTapHandler(handleTap);
    }

    bleKeyboard.setName("Macro Pad");
    bleKeyboard.begin();
}

void buttons_loop()
{
    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        buttons[i].loop();
    }
}