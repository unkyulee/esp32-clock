#include "ClockScreen.h"
#include "app/app.h"

//
void ClockScreen_setup(TFT_eSPI *ptft)
{
    ptft->fillScreen(TFT_BLACK);
    _log("Clock Screen Setup\n");
}

//
void ClockScreen_render(TFT_eSPI *ptft)
{
    JsonDocument &app = status();
    
    ///
    ptft->setCursor(0, 0, 2);
    ptft->setTextSize(2); // Adjust text size as needed
    ptft->setTextColor(TFT_WHITE, TFT_BLACK);

    // Draw text in the middle of the screen
    ptft->setCursor(0, 0, 2);
    ptft->print("Clock Screen");
}
