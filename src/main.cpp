#include <OSK_boards.h>

/*
	Module configuration
*/
#define OSK_MAIN_BOARD OSK_MAIN_DC_2_0
#define OSK_CONTROLLER_BOARD OSK_ESP32_1_0
#define OSK_IO_BOARD OSK_IO_2_0

#include <OSK_config.h>
#include <Arduino.h>
#include <IO.h>
#include <Button.h>
#include <Motion.h>
#include <SPI.h>
#include <WiFi.h>
#include <settings/Settings.h>
#include <settings/ModuleSettings.h>
#include <NetworkManager.h>
#include <AsyncElegantOTA.h>
#include <Debug.h>

//#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
//#include <ArduinoJson.h>

Button *btn1;
Button *btn2;
Motion *m1;
TimerHandle_t tDebug;
TimerHandle_t tGreenLed;
TimerHandle_t tTest;
IO *io;

Settings settings;
ModuleSettings moduleSettings;
NetworkManager *nm;

AsyncWebServer server(80);

void debugPrint()
{
	char buffer [30];
  sprintf (buffer, "MS: %lu, HEAP: %d", millis(), ESP.getFreeHeap());
  DEBUG_MSG_NL(buffer);
}

bool testRelay = false;
void greenLedBlink()
{
	io->set(OSK_GREEN_LED, !io->get(OSK_GREEN_LED));
}

void onLight() {
	DEBUG_MSG_NL("On light");
	io->ledDim(OSK_DCO1, 100, 2000);
}

void offLight() {
	DEBUG_MSG_NL("Off light");
	io->ledDim(OSK_DCO1, 0, 2000);
}

void setup()
{
	delay(1000);
	Serial.begin(115200);
	DEBUG_MSG_NL("Starting...");

	EEPROM.begin(sizeof(Settings));

	nm = NetworkManager::getInstance();
	nm->init(&moduleSettings);
	nm->connect();

	io = IO::getInstance();
	io->mode(OSK_GREEN_LED, OUTPUT);
	io->set(OSK_GREEN_LED, HIGH);

	tDebug = xTimerCreate("debugPrint", pdMS_TO_TICKS(5000), pdTRUE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(debugPrint));
	xTimerStart(tDebug, 0);

	tGreenLed = xTimerCreate("greenLed", pdMS_TO_TICKS(500), pdTRUE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(greenLedBlink));
	xTimerStart(tGreenLed, 0);

	m1 = new Motion(OSK_IO1, true);
	m1->onCallback(onLight);
	m1->offCallback(offLight);
	m1->begin(120);

	AsyncElegantOTA.begin(&server);
	server.begin();
}

void loop()
{
	#if DEBUG_USE_TELNET
  	telnet.loop();
	#endif
}
