#pragma once

#include <TFT_eSPI.h>

//
void WiFiScreen_setup(TFT_eSPI* ptft);

// 
void WiFiScreen_render(TFT_eSPI* ptft);

// 
void WiFiScreen_keyboard(char key);
