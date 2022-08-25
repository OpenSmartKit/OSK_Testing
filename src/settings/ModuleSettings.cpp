#include <settings/ModuleSettings.h>

ModuleSettings::ModuleSettings()
{
}

char *ModuleSettings::getModuleName()
{
    readSettings();
    return _settings.name;
}

void ModuleSettings::setModuleName(char *value)
{
    strlcpy(_settings.name, value, sizeof(_settings.name));
    writeSettings();
}

char *ModuleSettings::getWifiName()
{
    readSettings();
    return _settings.WiFiName;
}

void ModuleSettings::setWifiName(char *value)
{
    strlcpy(_settings.WiFiName, value, sizeof(_settings.WiFiName));
    writeSettings();
}

char *ModuleSettings::getWifiPassword()
{
    readSettings();
    return _settings.WiFiPass;
}

void ModuleSettings::setWifiPassword(char *value)
{
    strlcpy(_settings.WiFiPass, value, sizeof(_settings.WiFiPass));
    writeSettings();
}

void ModuleSettings::readSettings()
{
    _settings = getSettings();
}

void ModuleSettings::writeSettings()
{
    saveSettings(_settings);
}




uint8_t ModuleSettings::getState()
{
    readSettings();
    return _settings.state;
}

uint16_t ModuleSettings::getCurrentStep()
{
    readSettings();
    return _settings.currentStep;
}

uint8_t ModuleSettings::getCountOfCases()
{
    readSettings();
    return _settings.countOfCases;
}

ThermCases *ModuleSettings::getCases()
{
    readSettings();
    return _settings.cases;
}


void ModuleSettings::setState(uint8_t value)
{
    _settings.state = value;
    writeSettings();
}

void ModuleSettings::setCurrentStep(uint16_t value)
{
    _settings.currentStep = value;
    writeSettings();
}

void ModuleSettings::setCountOfCases(uint8_t value)
{
    _settings.countOfCases = value;
    writeSettings();
}

void ModuleSettings::setCases(ThermCases *value, uint8_t size)
{
    for (int8_t i=0; i<size; i++) {
        _settings.cases[i].temp = value[i].temp;
        _settings.cases[i].duration = value[i].duration;
    }
    writeSettings();
}