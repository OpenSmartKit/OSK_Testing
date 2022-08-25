#ifndef MODULE_SETTINGS_h
#define MODULE_SETTINGS_h

extern "C"
{
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
}
#include <functional>
#include <Arduino.h>
#include <NetworkManagerSettings.h>
#include <settings/Settings.h>

class ModuleSettings : public NetworkManagerSettings
{
public:
    ModuleSettings();
    virtual char *getModuleName();
    virtual void setModuleName(char *value);
    virtual char *getWifiName();
    virtual void setWifiName(char *value);
    virtual char *getWifiPassword();
    virtual void setWifiPassword(char *value);

    virtual uint8_t getState();
    virtual uint16_t getCurrentStep();
    virtual uint8_t getCountOfCases();
    virtual ThermCases *getCases();

    virtual void setState(uint8_t value);
    virtual void setCurrentStep(uint16_t value);
    virtual void setCountOfCases(uint8_t value);
    virtual void setCases(ThermCases *value, uint8_t size);

private:
    Settings _settings;

    void readSettings();
    void writeSettings();
};

#endif