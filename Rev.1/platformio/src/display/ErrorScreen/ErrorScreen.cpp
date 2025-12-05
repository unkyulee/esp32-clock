#include "ErrorScreen.h"
#include "app/app.h"

//
void ErrorScreen_setup(TFT_eSPI *ptft)
{
    ptft->fillScreen(TFT_BLACK);
    _log("Error Screen Setup\n");
}

//
void ErrorScreen_render(TFT_eSPI *ptft)
{
    // Text to be displayed
    JsonDocument &app = status();
    String text = app["error"].as<String>();

    ///
    ptft->setCursor(0, 0, 2);
    ptft->setTextSize(1); // Adjust text size as needed
    ptft->setTextColor(TFT_WHITE, TFT_RED);

    // Draw text in the middle of the screen
    ptft->setCursor(0, 0, 2);
    ptft->print(text);
}
