/*
	Module configuration
	Define proper board in platformio.ini
  #define OSK_MAIN_DC_S1
  #define OSK_MAIN_RELAY_S1
  #define OSK_MAIN_RELAY_S2
*/

#define IO_PCF8574_ADDR 0x27

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
bool relayState = LOW;

void debugPrint()
{

  DBG("MS: %lu, HEAP: %d", millis(), ESP.getFreeHeap());
	/*relayState = relayState == LOW ? HIGH : LOW;
	io->set(OSK_RELAY1, relayState);
	io->set(OSK_RELAY2, relayState);
	io->set(OSK_RELAY3, relayState);
	io->set(OSK_RELAY4, relayState);
	io->set(OSK_RELAY5, relayState);
	io->set(OSK_RELAY6, relayState);
	io->set(OSK_RELAY7, relayState);
	io->set(OSK_RELAY8, relayState);*/
}

void greenLedBlink()
{
	//io->set(OSK_GREEN_LED, !io->get(OSK_GREEN_LED));
	
}

void onLightKitchen() {
	DBG("On light Kitchen");
	//io->set(OSK_RELAY1, HIGH);
}

void offLightKitchen() {
	DBG("Off light Kitchen");
	//io->set(OSK_RELAY1, LOW);
}

void onLightBathroom() {
	DBG("On light Bathroom");
	//io->set(OSK_RELAY2, HIGH);
}

void offLightBathroom() {
	DBG("Off light Bathroom");
	//io->set(OSK_RELAY2, LOW);
}

void onLightLivingRoom() {
	DBG("On light Living room");
	//io->set(OSK_RELAY3, HIGH);
}

void offLightLivingRoom() {
	DBG("Off light Living room");
	//io->set(OSK_RELAY3, LOW);
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

/*
	Test Relays
*/
uint16_t relays[] = {OSK_RELAY1, OSK_RELAY2, OSK_RELAY3, OSK_RELAY4, OSK_RELAY5, OSK_RELAY6, OSK_RELAY7, OSK_RELAY8};
void testAllRelays() {
	for (byte i = 0; i < 8; i++) {
		DBG("Relay: %d", i+1);
		io->set(relays[i], HIGH);
		delay(1000);
		//io->set(relays[i], LOW);
		//delay(1500);
	}
}


void setup()
{
	delay(1000);
	Serial.begin(115200);
	DBG("Starting relay 1...");

	ArduinoOTA.setHostname("home-relay-1");


	EEPROM.begin(sizeof(Settings));

	
	moduleSettings.setWifiName("ASUS");
	moduleSettings.setWifiPassword("special5");
	moduleSettings.setModuleName("Test");

	nm = NetworkManager::getInstance();
	nm->init(&moduleSettings);
	nm->connect();

	io = IO::getInstance();
	io->mode(OSK_GREEN_LED, OUTPUT);
	io->set(OSK_GREEN_LED, LOW);

	//tDebug = xTimerCreate("debugPrint", pdMS_TO_TICKS(3000), pdTRUE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(debugPrint));
	//xTimerStart(tDebug, 0);

	//tGreenLed = xTimerCreate("greenLed", pdMS_TO_TICKS(500), pdTRUE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(greenLedBlink));
	//xTimerStart(tGreenLed, 0);

	motionDetection();

	ArduinoOTA.begin();

#if OSK_DEBUG_USE_TELNET
	telnet.onConnect(onTelnetConnect);
#endif
	//testAllRelays();
}

void loop()
{
	ArduinoOTA.handle();
	#if OSK_DEBUG_USE_TELNET
  	telnet.loop();
	#endif
}
