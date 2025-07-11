#include "Buttons.h"
#include "app/app.h"
#include "display/display.h"
#include "input/Buttons/Buttons.h"

#include <Button2.h>

#define BUTTON_1 15
#define BUTTON_2 16
#define BUTTON_3 17
#define BUTTON_4 18
#define BUTTON_5 1
#define BUTTON_6 2
#define BUTTON_7 42
#define BUTTON_8 41
#define BUTTON_KNOB 40

Button2 button_1;
Button2 button_2;
Button2 button_3;
Button2 button_4;
Button2 button_5;
Button2 button_6;
Button2 button_7;
Button2 button_8;
Button2 button_knob;

void handleTap1(Button2 &b)
{
    display_input(1);
}

void handleTap2(Button2 &b)
{
    display_input(2);
}
void handleTap3(Button2 &b)
{
    display_input(3);
}
void handleTap4(Button2 &b)
{
    display_input(4);
}

void handleTap5(Button2 &b)
{
    display_input(5);
}

void handleTap6(Button2 &b)
{
    display_input(6);
}

void handleTap7(Button2 &b)
{
    display_input(7);
}

void handleTap8(Button2 &b)
{
    display_input(8);
}

void handleTapKnob(Button2 &b)
{
    display_input(9);
}

void buttons_setup()
{
    button_1.begin(BUTTON_1);
    button_1.setTapHandler(handleTap1);

    button_2.begin(BUTTON_2);
    button_2.setTapHandler(handleTap2);

    button_3.begin(BUTTON_3);
    button_3.setTapHandler(handleTap3);

    button_4.begin(BUTTON_4);
    button_4.setTapHandler(handleTap4);

    button_5.begin(BUTTON_5);
    button_5.setTapHandler(handleTap5);

    button_6.begin(BUTTON_6);
    button_6.setTapHandler(handleTap6);

    button_7.begin(BUTTON_7);
    button_7.setTapHandler(handleTap7);

    button_8.begin(BUTTON_8);
    button_8.setTapHandler(handleTap8);

    button_knob.begin(BUTTON_KNOB);
    button_knob.setTapHandler(handleTapKnob);
}

void buttons_loop()
{
    button_1.loop();
    button_2.loop();
    button_3.loop();
    button_4.loop();
    button_5.loop();
    button_6.loop();
    button_7.loop();
    button_8.loop();

    button_knob.loop();
}