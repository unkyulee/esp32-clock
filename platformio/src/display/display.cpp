#include "display.h"
#include "app/app.h"

//
#include "display/ErrorScreen/ErrorScreen.h"
#include "display/WiFiScreen/WiFiScreen.h"
#include "display/ClockScreen/ClockScreen.h"

// Invoke library, pins defined in platformio.ini
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite(&tft);

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

  //
  tft.setRotation(3);

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
        ClockScreen_setup(&tft);
      else
        // loop
        ClockScreen_render(&tft);
    }

    //
    app["screen_prev"] = screen;
  }
}
