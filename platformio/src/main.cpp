#include <Arduino.h>

// logging and status
#include "app/app.h"

// display
#include "display/display.h"

// inputs
#include "input/Knob/Knob.h"
#include "input/Buttons/Buttons.h"

//
void TaskCore1(void *pvParameters);

/*----------------------------------------------
Dual Core: First Core
First core will have slightly higher priority
to it will take the critical tasks such as
keyboard input and user interactionss
----------------------------------------------*/
void setup()
{
    // initialize app
    app_setup();

    //
    display_setup();

    //
    knob_setup();
    buttons_setup();

    // Start the second core task
    xTaskCreatePinnedToCore(
        TaskCore1,   // Function to run
        "TaskCore1", // Name
        8192,        // Stack size
        NULL,        // Parameters
        1,           // Priority
        NULL,        // Task handle
        1            // Core 1
    );
}

//
void loop()
{
    // in ESP32 main core ID is 1
    display_loop();

    //
    knob_loop();

    //
    buttons_loop();

    // try to yield to avoid infinite loop
    yield();
}

/*----------------------------------------------
Dual Core: Second Core
Second core will contain tasks that can get blocked
and slowed and yet has still less impact.
Such as background tasks.
----------------------------------------------*/
//
void TaskCore1(void *pvParameters)
{
    _log("TaskCore1 Started\n");

    //
    while (1)
    {
        //
        app_loop();

        //
        yield();
    }
}
