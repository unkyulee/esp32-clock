//
#include "app.h"
#include "display/display.h"
#include "app/FileSystem/FileSystemSPIFFS.h"
#include <Arduino.h>
#include <time.h>

//
#include "service/Buzzer/Buzzer.h"

// When app is not ready. Such as file system is not initialized
// then no more operation should occur.
// this is a flag to check if the app is ready
bool _ready = false;
bool app_ready()
{
    return _ready;
}

// track daily reboot
static int lastRebootDay = -1;
static unsigned long lastRebootCheck = 0;

// Main object for the setup is to initialize the device
// set up the serial communication
// file system will be initialized here
void app_setup()
{
    // Setup Serial Communication
    Serial.begin(115200);
    _log("Setting Baud Rate: %d\n", 115200);

    // File System Check
    if (filesystem_check() == false)
    {
        // if file system check fails then do not proceed further
        _log("File system check failed. Exiting setup.\n");
        return;
    }

    // Load Configuration for the first time
    if (config_load() == false)
    {
        _log("Configuration load failed. Exiting setup.\n");
        return;
    }

    // Wifi connection check
    if (wifi_check() == false)
    {
        _log("Wifi Connection failed\n");
    }

    // Buzzer Setup
    buzzer_setup();

    // app ready
    _ready = true;
    _log("App is ready\n");
}

// This is a loop where background tasks will be placed
void app_loop()
{
    // wait until the app is ready
    if (app_ready() == false)
    {
        // wait for a while
        delay(100);
        return;
    }

    //
    buzzer_loop();

    // daily restart at 01:00 to reset state
    if (millis() - lastRebootCheck > 1000)
    {
        lastRebootCheck = millis();
        struct tm timeinfo;
        if (getLocalTime(&timeinfo))
        {
            if (timeinfo.tm_hour == 1 && timeinfo.tm_min == 0 && timeinfo.tm_mday != lastRebootDay)
            {
                lastRebootDay = timeinfo.tm_mday;
                _log("Scheduled reboot at 01:00\n");
                delay(60* 1000); // Prevent multiple reboot within the same minute
                ESP.restart();
            }
        }
    }
}

// status storage
JsonDocument _status;
JsonDocument &status()
{
    return _status;
}

// file system instance
FileSystem *fileSystem = nullptr;
FileSystem *gfs()
{
    if (fileSystem == nullptr)
    {
        // ESP32 will use SD card as a main file system
        fileSystem = new FileSystemSPIFFS();
        if (!fileSystem->begin())
        {
            //
            JsonDocument &app = status();
            app["error"] = "File System Failed\n";
            app["screen"] = ERRORSCREEN; // ERROR SCREEN IS 0
            _log(app["error"]);
        }
        else
        {
            _log("File System Initialized\n");
        }
    }

    return fileSystem;
}
