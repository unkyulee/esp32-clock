#include "Buttons.h"
#include "app/app.h"
#include "display/display.h"
#include "input/Buttons/Buttons.h"

#include <Button2.h>

#define BUTTON_1 15
#define BUTTON_2 16
#define BUTTON_3 17
#define BUTTON_4 18
#define BUTTON_5 1
#define BUTTON_6 2
#define BUTTON_7 42
#define BUTTON_8 41

Button2 button_1;
Button2 button_2;
Button2 button_3;
Button2 button_4;
Button2 button_5;
Button2 button_6;
Button2 button_7;
Button2 button_8;

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
    {
        bleKeyboard.write(token.c_str()[0]);
    }
}

void handleTap1(Button2 &b)
{
    //
    if (!bleKeyboard.isConnected())
        return;

    //
    JsonDocument &app = status();
    String tokenList = app["config"]["button_1"].as<String>();
    int start = 0;
    while (start < tokenList.length())
    {
        int end = tokenList.indexOf(',', start);
        if (end == -1)
            end = tokenList.length();

        String token = tokenList.substring(start, end);
        token.trim(); // Remove whitespace

        playToken(token);

        start = end + 1;
    }
}

void handleTap2(Button2 &b)
{
    //
    if (!bleKeyboard.isConnected())
        return;

    //
    JsonDocument &app = status();
    String tokenList = app["config"]["button_2"].as<String>();
    int start = 0;
    while (start < tokenList.length())
    {
        int end = tokenList.indexOf(',', start);
        if (end == -1)
            end = tokenList.length();

        String token = tokenList.substring(start, end);
        token.trim(); // Remove whitespace

        playToken(token);

        start = end + 1;
    }
}
void handleTap3(Button2 &b)
{
    //
    if (!bleKeyboard.isConnected())
        return;

    //
    JsonDocument &app = status();
    String tokenList = app["config"]["button_3"].as<String>();
    int start = 0;
    while (start < tokenList.length())
    {
        int end = tokenList.indexOf(',', start);
        if (end == -1)
            end = tokenList.length();

        String token = tokenList.substring(start, end);
        token.trim(); // Remove whitespace

        playToken(token);

        start = end + 1;
    }
}
void handleTap4(Button2 &b)
{
    //
    if (!bleKeyboard.isConnected())
        return;

    //
    JsonDocument &app = status();
    String tokenList = app["config"]["button_4"].as<String>();
    int start = 0;
    while (start < tokenList.length())
    {
        int end = tokenList.indexOf(',', start);
        if (end == -1)
            end = tokenList.length();

        String token = tokenList.substring(start, end);
        token.trim(); // Remove whitespace

        playToken(token);

        start = end + 1;
    }
}

void handleTap5(Button2 &b)
{
    //
    if (!bleKeyboard.isConnected())
        return;

    //
    JsonDocument &app = status();
    String tokenList = app["config"]["button_5"].as<String>();
    int start = 0;
    while (start < tokenList.length())
    {
        int end = tokenList.indexOf(',', start);
        if (end == -1)
            end = tokenList.length();

        String token = tokenList.substring(start, end);
        token.trim(); // Remove whitespace

        playToken(token);

        start = end + 1;
    }
}

void handleTap6(Button2 &b)
{
    //
    if (!bleKeyboard.isConnected())
        return;

    //
    JsonDocument &app = status();
    String tokenList = app["config"]["button_6"].as<String>();
    int start = 0;
    while (start < tokenList.length())
    {
        int end = tokenList.indexOf(',', start);
        if (end == -1)
            end = tokenList.length();

        String token = tokenList.substring(start, end);
        token.trim(); // Remove whitespace

        playToken(token);

        start = end + 1;
    }
}

void handleTap7(Button2 &b)
{
    //
    if (!bleKeyboard.isConnected())
        return;

    //
    JsonDocument &app = status();
    String tokenList = app["config"]["button_7"].as<String>();
    int start = 0;
    while (start < tokenList.length())
    {
        int end = tokenList.indexOf(',', start);
        if (end == -1)
            end = tokenList.length();

        String token = tokenList.substring(start, end);
        token.trim(); // Remove whitespace

        playToken(token);

        start = end + 1;
    }
}

void handleTap8(Button2 &b)
{
    //
    if (!bleKeyboard.isConnected())
        return;

    //
    JsonDocument &app = status();
    String tokenList = app["config"]["button_8"].as<String>();
    int start = 0;
    while (start < tokenList.length())
    {
        int end = tokenList.indexOf(',', start);
        if (end == -1)
            end = tokenList.length();

        String token = tokenList.substring(start, end);
        token.trim(); // Remove whitespace

        playToken(token);

        start = end + 1;
    }
}

void buttons_setup()
{
    button_1.begin(BUTTON_1);
    button_1.setTapHandler(handleTap1);

    button_2.begin(BUTTON_2);
    button_2.setTapHandler(handleTap2);

    button_3.begin(BUTTON_3);
    button_3.setTapHandler(handleTap3);

    button_4.begin(BUTTON_4);
    button_4.setTapHandler(handleTap4);

    button_5.begin(BUTTON_5);
    button_5.setTapHandler(handleTap5);

    button_6.begin(BUTTON_6);
    button_6.setTapHandler(handleTap6);

    button_7.begin(BUTTON_7);
    button_7.setTapHandler(handleTap7);

    button_8.begin(BUTTON_8);
    button_8.setTapHandler(handleTap8);

    bleKeyboard.setName("ESP32 Macro Pad");
    bleKeyboard.begin();
}

void buttons_loop()
{
    button_1.loop();
    button_2.loop();
    button_3.loop();
    button_4.loop();
    button_5.loop();
    button_6.loop();
    button_7.loop();
    button_8.loop();
}