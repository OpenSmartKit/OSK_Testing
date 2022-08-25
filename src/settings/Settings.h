#ifndef SETTINGS_h
#define SETTINGS_h

#include <Arduino.h>
#include <EEPROM.h>
#include <settings/ThermCases.h>

#define START_ADDR 0
#define SIGNATURE 0x123544F0

struct Settings
{
  uint32_t signature;
  char name[32];
  char WiFiName[32];
  char WiFiPass[32];

  uint8_t state;
  uint16_t currentStep;
  uint8_t countOfCases;
  ThermCases cases[12];
};

Settings getSettings();
void saveSettings(Settings newSettings);
Settings _getDefaultSettings();

#endif