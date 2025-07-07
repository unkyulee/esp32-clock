#pragma once

#include <TFT_eSPI.h>

//
void ClockScreen_setup(TFT_eSPI* ptft);

// 
void ClockScreen_render(TFT_eSPI* ptft);

// 
void ClockScreen_keyboard(char key);
