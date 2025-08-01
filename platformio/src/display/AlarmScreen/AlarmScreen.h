#pragma once

#include <TFT_eSPI.h>
#include <U8g2_for_TFT_eSPI.h>

//
void AlarmScreen_setup(TFT_eSPI* ptft, U8g2_for_TFT_eSPI *pu8f);

// 
void AlarmScreen_render(TFT_eSPI* ptft, U8g2_for_TFT_eSPI *pu8f);

// 
void AlarmScreen_input(int key);
