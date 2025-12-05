#pragma once

#include <TFT_eSPI.h>
#include <U8g2_for_TFT_eSPI.h>

//
void TimerScreen_setup(TFT_eSPI* ptft, U8g2_for_TFT_eSPI *pu8f);

// 
void TimerScreen_render(TFT_eSPI* ptft, U8g2_for_TFT_eSPI *pu8f);
void TimerScreen_render_set(TFT_eSPI* ptft, U8g2_for_TFT_eSPI *pu8f);
void TimerScreen_render_run(TFT_eSPI* ptft, U8g2_for_TFT_eSPI *pu8f);


// 
void TimerScreen_input(int key);

//
void TimerScreen_ring();
