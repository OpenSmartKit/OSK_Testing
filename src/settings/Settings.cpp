#include <settings/Settings.h>

Settings getSettings()
{
    Settings settings;
    EEPROM.readBytes(START_ADDR, &settings, sizeof(Settings));

    if (settings.signature != SIGNATURE)
    {
        settings = _getDefaultSettings();
        saveSettings(settings);
    }

    return settings;
}

void saveSettings(Settings newSettings)
{
    newSettings.signature = SIGNATURE;
    EEPROM.writeBytes(START_ADDR, &newSettings, sizeof(Settings));
    EEPROM.commit();
}

Settings _getDefaultSettings()
{
    Settings settings = {SIGNATURE, "DEVICE", "", ""};
    return settings;
}