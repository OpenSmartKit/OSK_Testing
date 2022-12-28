#ifndef SETTINGS_h
#define SETTINGS_h

#include <Arduino.h>
#include <EEPROM.h>

#define START_ADDR 0
#define SIGNATURE 0x123544F0

struct Settings
{
  uint32_t signature;
  char name[32];
  char WiFiName[32];
  char WiFiPass[32];
};

Settings getSettings();
void saveSettings(Settings newSettings);
Settings _getDefaultSettings();

#endif