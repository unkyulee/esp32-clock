#pragma once

#include <TFT_eSPI.h>
#include <U8g2_for_TFT_eSPI.h>

//
void ClockScreen_setup(TFT_eSPI* ptft, U8g2_for_TFT_eSPI *pu8f);

// 
void ClockScreen_render(TFT_eSPI* ptft, U8g2_for_TFT_eSPI *pu8f);

// 
void ClockScreen_input(int key);
