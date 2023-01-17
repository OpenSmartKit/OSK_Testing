/*
	Module configuration
	Define proper board in platformio.ini
  #define OSK_MAIN_DC_2_0
  #define OSK_MAIN_RELAY_1_2
  #define OSK_MAIN_RELAY2_1_0
*/

#include <Arduino.h>
#include <IO.h>
#include <Button.h>
#include <Motion.h>
#include <SPI.h>
#include <WiFi.h>
#include <ModuleSettings.h>
#include <NetworkManager.h>
#include <Debug.h>
#include <ArduinoOTA.h>

Motion *mKitchen;
Motion *mBathroom;
Motion *mLivingRoom;
Button *hBathroom;
TimerHandle_t tDebug;
TimerHandle_t tGreenLed;
TimerHandle_t tTest;
IO *io;

Settings settings;
ModuleSettings moduleSettings;
NetworkManager *nm;
bool isSomeoneInBathroom = false;

void debugPrint()
{
  //DBG("MS: %lu, HEAP: %d", millis(), ESP.getFreeHeap());
}

void greenLedBlink()
{
	io->set(OSK_GREEN_LED, !io->get(OSK_GREEN_LED));
}

void onLightKitchen() {
	DBG("On light Kitchen");
	io->set(OSK_RELAY1, HIGH);
}

void offLightKitchen() {
	DBG("Off light Kitchen");
	io->set(OSK_RELAY1, LOW);
}

void onLightBathroom() {
	DBG("On light Bathroom");
	io->set(OSK_RELAY2, HIGH);
}

void offLightBathroom() {
	DBG("Off light Bathroom");
	io->set(OSK_RELAY2, LOW);
}

void onLightLivingRoom() {
	DBG("On light Living room");
	io->set(OSK_RELAY3, HIGH);
}

void offLightLivingRoom() {
	DBG("Off light Living room");
	io->set(OSK_RELAY3, LOW);
}

void onHerkonOn() {
	DBG("Herkon ON!");
}

void onHerkonOff() {
	DBG("Herkon OFF!");
}

void onTelnetConnect(String ip) {
  DBG("Status: ");
	mKitchen->debug("Init Kitchen");
	mLivingRoom->debug("Init Living");
	mBathroom->debug("Init Bathroom");
}

/*
	Test Motion lib
*/
void motionDetection() {
	mKitchen = new Motion(OSK_IO1, true);
	mKitchen->onCallback(onLightKitchen);
	mKitchen->offCallback(offLightKitchen);
	mKitchen->begin(180);

	mBathroom = new Motion(OSK_IO2, true);
	mBathroom->onCallback(onLightBathroom);
	mBathroom->offCallback(offLightBathroom);
	mBathroom->begin(60);

	mLivingRoom = new Motion(OSK_IO4, true);
	mLivingRoom->onCallback(onLightLivingRoom);
	mLivingRoom->offCallback(offLightLivingRoom);
	mLivingRoom->begin(60);

	mKitchen->startDebug("Kitchen");
	mBathroom->startDebug("Bathroom");
	mLivingRoom->startDebug("Living Room");

	hBathroom = new Button(OSK_IO3);
	hBathroom->onLow(onHerkonOn);
	hBathroom->onHigh(onHerkonOff);
}


void setup()
{
	delay(1000);
	Serial.begin(115200);
	DBG("Starting relay 1...");

	ArduinoOTA.setHostname("home-relay-1");

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

	motionDetection();

	ArduinoOTA.begin();

	telnet.onConnect(onTelnetConnect);
}

void loop()
{
	ArduinoOTA.handle();
	#if OSK_DEBUG_USE_TELNET
  	telnet.loop();
	#endif
}
