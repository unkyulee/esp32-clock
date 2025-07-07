#pragma once

#define ERRORSCREEN 1
#define CLOCKSCREEN 0 // default screen

// TFT_eSPI setup
#include <SPI.h>
#include <TFT_eSPI.h>

//
void display_setup();
void display_loop();

//
TFT_eSprite& display_sprite();
