#include "app/FileSystem/FileSystem.h"
#include "app/app.h"
#include "display/display.h"

class FileSystemSPIFFS : public FileSystem
{
public:
    bool begin() override
    {
        if (!SPIFFS.begin())
        {
            // file system failed
            JsonDocument &app = status();
            app["error"] = "File System Failed\n";
            app["screen"] = ERRORSCREEN; // ERROR SCREEN IS 0
            _log(app["error"]);

            //
            return false;
        }

        return true;
    }
    File open(const char *path, const char *mode) override
    {
        return SPIFFS.open(path, mode);
    }
    bool exists(const char *path) override { return SPIFFS.exists(path); }
    bool remove(const char *path) override { return SPIFFS.remove(path); }
    bool rename(const char *pathFrom, const char *pathTo) override
    {
        return SPIFFS.rename(pathFrom, pathTo);
    }
};
