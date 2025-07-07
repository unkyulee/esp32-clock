#include "Verification.h"
#include "app/app.h"

// check if file system is correctly loaded
bool filesystem_check()
{
    //
    JsonDocument &app = status();

    // Call File System for the first time to initialize
    gfs();

    // file system check pass
    return true;
}
