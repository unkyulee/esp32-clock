#include "Knob.h"
#include "display/display.h"
#include "app/app.h"

#include <RotaryEncoder.h>
#include <Button2.h>

RotaryEncoder *encoder = nullptr;
Button2 button_knob;

#define KNOB_A 45
#define KNOB_B 48
#define BUTTON_KNOB 40

// This interrupt routine will be called on any change of one of the input signals
void checkPosition()
{
    encoder->tick(); // just call tick() to check the state.
}

void handleTapKnob(Button2 &b)
{
    display_input(9);
}

void knob_setup()
{
    encoder = new RotaryEncoder(KNOB_A, KNOB_B, RotaryEncoder::LatchMode::TWO03);
    // register interrupt routine
    attachInterrupt(digitalPinToInterrupt(KNOB_A), checkPosition, CHANGE);
    attachInterrupt(digitalPinToInterrupt(KNOB_B), checkPosition, CHANGE);

    //
    button_knob.begin(BUTTON_KNOB);
    button_knob.setTapHandler(handleTapKnob);
}

//
void knob_loop()
{
    static int lastPos = 0;
    static int lastDir = 0;
    static int dirCount = 0;
    static unsigned long lastMoveTime = 0;

    encoder->tick(); // just call tick() to check the state.
    button_knob.loop();

    int newPos = encoder->getPosition();
    int newDir = (int)encoder->getDirection();

    if (newPos != lastPos)
    {
        unsigned long now = millis();

        if (newDir == lastDir && newDir != 0)
        {
            dirCount++;
            if (dirCount == 2)
            {
                _debug("Valid rotation detected! Direction: %d\n", newDir);
                dirCount = 0; // Reset after valid rotation

                // newDir - 1 UP
                // newDir - -1 DOWN
                if (newDir == 1)
                {
                    display_input(KNOB_UP);
                }
                else if (newDir == -1)
                {
                    display_input(KNOB_DOWN);
                }
            }
        }
        else
        {
            dirCount = 1; // Start new count for new direction
        }

        lastDir = newDir;
        lastMoveTime = now;
        lastPos = newPos;
    }

    // Reset if no action within 1 second
    if (dirCount > 0 && (millis() - lastMoveTime > 1000))
    {
        dirCount = 0;
        lastDir = 0;
    }
}