#include "display.h"
#include "app/app.h"

//
#include "display/ErrorScreen/ErrorScreen.h"
#include "display/WiFiScreen/WiFiScreen.h"
#include "display/ClockScreen/ClockScreen.h"
#include "display/TimerScreen/TimerScreen.h"
#include "display/Menu/Menu.h"
#include "display/AlarmScreen/AlarmScreen.h"

// Invoke library, pins defined in platformio.ini
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite(&tft);
U8g2_for_TFT_eSPI u8f; // U8g2 font instance

//
TFT_eSprite &display_sprite()
{
  return sprite;
}

//
void display_setup()
{
  _log("DISPLAY ILI9341 SETUP\n");

  // Initialise the TFT screen
  tft.begin();

  // connect u8g2 procedures to TFT_eSPI
  u8f.begin(tft);

  //
  tft.setRotation(1);

  // Fill screen with grey so we can see the effect of printing with and without
  // a background colour defined
  tft.fillScreen(TFT_BLACK);
}

//
void display_loop()
{
  static unsigned int last = 0;
  if (millis() - last > 150)
  {
    last = millis();

    JsonDocument &app = status();
    int screen = app["screen"].as<int>();
    int screen_prev = app["screen_prev"].as<int>();

    // Error Screen
    if (screen == ERRORSCREEN)
    {
      // setup only once
      if (screen != screen_prev)
        ErrorScreen_setup(&tft);
      else
        // loop
        ErrorScreen_render(&tft);
    }

    // Wifi Check and Setup Screen (Default Screen)
    else if (screen == WIFISCREEN)
    {
      // setup only once
      if (screen != screen_prev)
        WiFiScreen_setup(&tft);
      else
        // loop
        WiFiScreen_render(&tft);
    }

    // Clock Screen
    else if (screen == CLOCKSCREEN)
    {
      // setup only once
      if (screen != screen_prev)
        ClockScreen_setup(&tft, &u8f);
      else
        // loop
        ClockScreen_render(&tft, &u8f);
    }

    // Timer Screen
    else if (screen == TIMERSCREEN)
    {
      // setup only once
      if (screen != screen_prev)
        TimerScreen_setup(&tft, &u8f);
      else
        // loop
        TimerScreen_render(&tft, &u8f);
    }

    // Menu Screen
    else if (screen == MENUSCREEN)
    {
      // setup only once
      if (screen != screen_prev)
        Menu_setup(&tft, &u8f);
      else
        // loop
        Menu_render(&tft, &u8f);
    }

    // Alarm Screen
    else if (screen == ALARMSCREEN)
    {
      // setup only once
      if (screen != screen_prev)
        AlarmScreen_setup(&tft, &u8f);
      else
        // loop
        AlarmScreen_render(&tft, &u8f);
    }

    //
    app["screen_prev"] = screen;
  }
}

//
void display_input(int key)
{
  _log("[display_input] %d\n", key);

  //
  JsonDocument &app = status();
  int screen = app["screen"].as<int>();

  if (screen == TIMERSCREEN)
  {
    TimerScreen_input(key);
  }

  else if (screen == CLOCKSCREEN)
  {
    ClockScreen_input(key);
  }

  else if (screen == MENUSCREEN)
  {
    Menu_input(key);
  }

  else if (screen == ALARMSCREEN)
  {
    AlarmScreen_input(key);
  }
}