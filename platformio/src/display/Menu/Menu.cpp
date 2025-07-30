#include "Menu.h"
#include "app/app.h"
#include "display/display.h"
#include "input/Knob/Knob.h"

//
const int totalMenu = 3;
int selectedMenu = 0;

//
void Menu_setup(TFT_eSPI *ptft, U8g2_for_TFT_eSPI *pu8f)
{
    _log("Menu Screen Setup\n");

    // Clear Screen
    ptft->fillScreen(TFT_BLACK);
}

//
void Menu_render(TFT_eSPI *ptft, U8g2_for_TFT_eSPI *pu8f)
{
    // Setup the sprite buffer
    TFT_eSprite sprite = display_sprite();
    sprite.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT);
    sprite.fillSprite(TFT_BLACK);

    // Setup font rendering with u8g2
    pu8f->begin(sprite);
    pu8f->setFont(u8g2_font_logisoso32_tf); // Medium readable font
    pu8f->setBackgroundColor(TFT_BLACK);
    pu8f->setFontMode(1);

    const char *menuItems[totalMenu] = {"Clock", "Timer", "Alarm"};

    // Layout
    int itemHeight = 40;
    int startY = (SCREEN_HEIGHT - itemHeight * totalMenu) / 2;

    for (int i = 0; i < totalMenu; ++i)
    {
        const char *label = menuItems[i];

        int textWidth = pu8f->getUTF8Width(label);
        int textHeight = pu8f->getFontAscent() - pu8f->getFontDescent();
        int x = (SCREEN_WIDTH - textWidth) / 2;
        int y = startY + i * itemHeight + textHeight;

        // Highlight selected item
        if (i == selectedMenu)
        {
            sprite.fillRect(0, y - textHeight, SCREEN_WIDTH, itemHeight, TFT_WHITE);
            pu8f->setForegroundColor(TFT_BLACK);
        }
        else
        {
            pu8f->setForegroundColor(TFT_WHITE);
        }

        pu8f->setCursor(x, y);
        pu8f->print(label);
    }

    // Render and cleanup
    sprite.pushSprite(0, 0);
    delay(10);
    sprite.deleteSprite();
    pu8f->begin(*ptft);
}

//
void Menu_input(int key)
{
    JsonDocument &app = status();

    switch (key)
    {
    case KNOB_UP:
    {
        selectedMenu++;
        if (selectedMenu >= totalMenu)
            selectedMenu = 0;
    }

    break;

    case KNOB_DOWN:
    {
        selectedMenu--;
        if (selectedMenu < 0)
            selectedMenu = totalMenu - 1;
    }

    break;

    case KNOB_CLICK: // KNOB Click to start/pause

        switch (selectedMenu)
        {
        case 0:
            app["screen"] = CLOCKSCREEN;
            break;

        case 1:
            app["screen"] = TIMERSCREEN;
            break;

        case 2:
            app["screen"] = ALARMSCREEN;
            break;
        }

        break;

    default:
        return; // Ignore unknown keys
    }
}