#pragma once

#define WIFISCREEN  0 // default screen
#define ERRORSCREEN 1
#define CLOCKSCREEN 2
#define TIMERSCREEN 3
#define MENUSCREEN 4
#define ALARMSCREEN 5
#define WIFIINFOSCREEN 6

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// TFT_eSPI setup
#include <SPI.h>
#include <TFT_eSPI.h>
#include <U8g2_for_TFT_eSPI.h>

//
void display_setup();
void display_loop();

//
TFT_eSprite& display_sprite();

//
void display_input(int key);
