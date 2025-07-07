#include <Arduino.h>

// logging and status
#include "app/app.h"

// display
#include "display/display.h"

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
}

//
void loop()
{
    // in ESP32 main core ID is 1
    display_loop();

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
    while (1)
    {
        //
        app_loop();
        
        //
        yield();
    }
}
