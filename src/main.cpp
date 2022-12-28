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
#include <settings/Settings.h>
#include <settings/ModuleSettings.h>
#include <NetworkManager.h>
#include <AsyncElegantOTA.h>
#include <Debug.h>
#include <I2CScanner.h>

//#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
//#include <ArduinoJson.h>

Button *btn1;
Button *btn2;
Button *btn3;
Button *btn4;
Button *btn5;
Motion *mKitchen;
Motion *mBathroom;
TimerHandle_t tDebug;
TimerHandle_t tGreenLed;
TimerHandle_t tTest;
IO *io;

Settings settings;
ModuleSettings moduleSettings;
NetworkManager *nm;

AsyncWebServer server(80);

I2CScanner scanner;

void debugPrint()
{
	char buffer [30];
  sprintf (buffer, "MS: %lu, HEAP: %d", millis(), ESP.getFreeHeap());
  DEBUG_MSG_NL(buffer);
}

void greenLedBlink()
{
	io->set(OSK_GREEN_LED, !io->get(OSK_GREEN_LED));
}


/* ==============
		DEBUG OUTPUT FUNCTIONS
   =============== */
void onLightKitchen() {
	DEBUG_MSG_NL("On light Kitchen");
	io->ledDim(OSK_DCO1, 100, 2000);
}

void offLightKitchen() {
	DEBUG_MSG_NL("Off light Kitchen");
	io->ledDim(OSK_DCO1, 0, 2000);
}

void onLightBathroom() {
	DEBUG_MSG_NL("On light Bathroom");
	io->ledDim(OSK_DCO3, 100, 2000);
}

void offLightBathroom() {
	DEBUG_MSG_NL("Off light Bathroom");
	io->ledDim(OSK_DCO3, 0, 2000);
}

void onTriggerIO1(bool value) {
	DEBUG_MSG("IO1: ");
	DEBUG_MSG_NL(value);
}

void onTriggerIO2(bool value) {
	DEBUG_MSG("IO2: ");
	DEBUG_MSG_NL(value);
}

void onTriggerIO3(bool value) {
	DEBUG_MSG("IO3: ");
	DEBUG_MSG_NL(value);
}

void onTriggerIO4(bool value) {
	DEBUG_MSG("IO4: ");
	DEBUG_MSG_NL(value);
}

void onTriggerIO5(bool value) {
	DEBUG_MSG("IO5: ");
	DEBUG_MSG_NL(value);
}

void onTriggerIO6(bool value) {
	DEBUG_MSG("IO6: ");
	DEBUG_MSG_NL(value);
}

void onTriggerIO7(bool value) {
	DEBUG_MSG("IO7: ");
	DEBUG_MSG_NL(value);
}

void onTriggerIO8(bool value) {
	DEBUG_MSG("IO8: ");
	DEBUG_MSG_NL(value);
}

void onTriggerIO9(bool value) {
	DEBUG_MSG("IO9: ");
	DEBUG_MSG_NL(value);
}

void onTriggerIO10(bool value) {
	DEBUG_MSG("IO10: ");
	DEBUG_MSG_NL(value);
}

void onTriggerIO11(bool value) {
	DEBUG_MSG("IO11: ");
	DEBUG_MSG_NL(value);
}

void onTriggerIO12(bool value) {
	DEBUG_MSG("IO12: ");
	DEBUG_MSG_NL(value);
}

void onTriggerIO13(bool value) {
	DEBUG_MSG("IO13: ");
	DEBUG_MSG_NL(value);
}

void onTriggerIO14(bool value) {
	DEBUG_MSG("IO14: ");
	DEBUG_MSG_NL(value);
}

void onTriggerIO15(bool value) {
	DEBUG_MSG("IO15: ");
	DEBUG_MSG_NL(value);
}

void onTriggerIO16(bool value) {
	DEBUG_MSG("IO16: ");
	DEBUG_MSG_NL(value);
}

void btnChanged() {
	DEBUG_MSG_NL("IO1 Changed");
}

void btnHigh() {
	DEBUG_MSG_NL("IO2 HIGH");
}

void btnLOW() {
	DEBUG_MSG_NL("IO3 LOW");
}

void btnClick() {
	DEBUG_MSG_NL("IO4 Click");
}

void btnLongClick() {
	DEBUG_MSG_NL("IO5 long Click");
}
/* ==============
		END DEBUG OUTPUT FUNCTIONS
   =============== */

/*
	Test IO inputs using IO lib
*/
void directTestIOInput() {
	io->mode(OSK_IO1, INPUT);
	io->mode(OSK_IO2, INPUT);
	io->mode(OSK_IO3, INPUT);
	io->mode(OSK_IO4, INPUT);
	io->mode(OSK_IO5, INPUT);
	io->mode(OSK_IO6, INPUT);
	io->mode(OSK_IO7, INPUT);
	io->mode(OSK_IO8, INPUT);
	io->mode(OSK_IO9, INPUT);
	io->mode(OSK_IO10, INPUT);
	io->mode(OSK_IO11, INPUT);
	io->mode(OSK_IO12, INPUT);
	io->mode(OSK_IO13, INPUT);
	io->mode(OSK_IO14, INPUT);
	io->mode(OSK_IO15, INPUT);
	io->mode(OSK_IO16, INPUT);

	io->on(OSK_IO1, CHANGE, onTriggerIO1);
	io->on(OSK_IO2, CHANGE, onTriggerIO2);
	io->on(OSK_IO3, CHANGE, onTriggerIO3);
	io->on(OSK_IO4, CHANGE, onTriggerIO4);
	io->on(OSK_IO5, CHANGE, onTriggerIO5);
	io->on(OSK_IO6, CHANGE, onTriggerIO6);
	io->on(OSK_IO7, CHANGE, onTriggerIO7);
	io->on(OSK_IO8, CHANGE, onTriggerIO8);
	io->on(OSK_IO9, CHANGE, onTriggerIO9);
	io->on(OSK_IO10, CHANGE, onTriggerIO10);
	io->on(OSK_IO11, CHANGE, onTriggerIO11);
	io->on(OSK_IO12, CHANGE, onTriggerIO12);
	io->on(OSK_IO13, CHANGE, onTriggerIO13);
	io->on(OSK_IO14, CHANGE, onTriggerIO14);
	io->on(OSK_IO15, CHANGE, onTriggerIO15);
	io->on(OSK_IO16, CHANGE, onTriggerIO16);
}

/*
	Test IO inputs using Button lib
*/
void testIOButtons() {
	btn1 = new Button(OSK_IO1);
	btn1->onChange(btnChanged);

	btn2 = new Button(OSK_IO2);
	btn2->onHigh(btnHigh);

	btn3 = new Button(OSK_IO3);
	btn3->onLow(btnLOW);

	btn4 = new Button(OSK_IO4);
	btn4->onClick(btnClick);

	btn5 = new Button(OSK_IO5);
	btn5->onLongClick(btnLongClick);
}

/*
	Test Motion lib
*/
void testMotionLib() {
	mKitchen = new Motion(OSK_IO6, false);
	mKitchen->onCallback(onLightKitchen);
	mKitchen->offCallback(offLightKitchen);
	mKitchen->begin(10);

	mBathroom = new Motion(OSK_IO7, false);
	mBathroom->onCallback(onLightBathroom);
	mBathroom->offCallback(offLightBathroom);
	mBathroom->begin(20);
}


void setup()
{
	delay(1000);
	Serial.begin(115200);
	DEBUG_MSG_NL("Starting...");

	// Scan I2C for debug
	scanner.Init();
	scanner.Scan();

	EEPROM.begin(sizeof(Settings));

	nm = NetworkManager::getInstance();
	//nm->addCustomBLEFunc(addCustomBLEFunc);
	nm->init(&moduleSettings);
	nm->connect();

	io = IO::getInstance();
	io->mode(OSK_GREEN_LED, OUTPUT);
	io->set(OSK_GREEN_LED, HIGH);

	tDebug = xTimerCreate("debugPrint", pdMS_TO_TICKS(5000), pdTRUE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(debugPrint));
	xTimerStart(tDebug, 0);

	tGreenLed = xTimerCreate("greenLed", pdMS_TO_TICKS(500), pdTRUE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(greenLedBlink));
	xTimerStart(tGreenLed, 0);

	// Call test function. Also use it as examples
	// directTestIOInput();
	testIOButtons();
	testMotionLib();

	AsyncElegantOTA.begin(&server);
	server.begin();
}

void loop()
{
	#if DEBUG_USE_TELNET
  	telnet.loop();
	#endif
}