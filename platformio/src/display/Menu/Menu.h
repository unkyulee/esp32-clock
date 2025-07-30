#pragma once

#include <TFT_eSPI.h>
#include <U8g2_for_TFT_eSPI.h>

//
void Menu_setup(TFT_eSPI* ptft, U8g2_for_TFT_eSPI *pu8f);

// 
void Menu_render(TFT_eSPI* ptft, U8g2_for_TFT_eSPI *pu8f);

// 
void Menu_input(int key);