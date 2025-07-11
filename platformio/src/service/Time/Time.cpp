#include "Time.h"
#include "app/app.h"

//
#include <time.h>

void updateTime()
{
    JsonDocument &app = status();

    // Example: Central European Time (CET/CEST)
    // "CET-1CEST,M3.5.0/2,M10.5.0/3"
    // → CET = UTC+1 (standard time)
    // → CEST = UTC+2 (daylight saving time)
    // → DST starts last Sunday in March at 2:00, ends last Sunday in October at 3:00
    const char *tz = "CET-1CEST,M3.5.0/2,M10.5.0/3";
    if(app["config"]["tz"].is<const char*>()) tz = app["config"]["tz"].as<const char*>();
    
    //
    configTzTime(tz, "pool.ntp.org");

    struct tm timeinfo;
    while (!getLocalTime(&timeinfo))
    {
        _log("Waiting for time...\n");
        delay(1000);
    }

    // Print local time with DST applied
    _log("Time acquired:  %s", asctime(&timeinfo));
}