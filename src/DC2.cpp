/*
	Module configuration
	Define proper board in platformio.ini
  #define OSK_MAIN_DC_S1
  #define OSK_MAIN_RELAY_S1
  #define OSK_MAIN_RELAY_S2
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

//#include <ESP32CAN.h>
//#include <CAN_config.h>

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


//CAN_device_t CAN_cfg;               // CAN Config
unsigned long previousMillis = 0;   // will store last time a CAN Message was send
const int interval = 1000;          // interval at which send CAN Messages (milliseconds)
const int rx_queue_size = 10;       // Receive Queue size


void debugPrint()
{
  //DBG("MS: %lu, HEAP: %d", millis(), ESP.getFreeHeap());

	uint32_t xtal = getXtalFrequencyMhz();
	uint32_t cpuf = getCpuFrequencyMhz();
	uint32_t apbf = getApbFrequency();

	DBG("CPUf: %d", cpuf);
	DBG("XTALf: %d", xtal);
	DBG("ABPf: %d", apbf);
}

void greenLedBlink()
{
	//io->set(OSK_GREEN_LED, !io->get(OSK_GREEN_LED));

	//Serial2.println("RS485 OK!");
	/*CAN_frame_t tx_frame;
	tx_frame.FIR.B.FF = CAN_frame_std;
	tx_frame.MsgID = 0x001;
	tx_frame.FIR.B.DLC = 1;
	tx_frame.data.u8[0] = io->get(OSK_GREEN_LED);
	ESP32Can.CANWriteFrame(&tx_frame);*/

	
}

void onLightKitchen() {
	DBG("On light Kitchen");
	io->ledDim(OSK_DCO1, 100, 2000);
}

void offLightKitchen() {
	DBG("Off light Kitchen");
	io->ledDim(OSK_DCO1, 0, 2000);
}

void onLightBathroom() {
	DBG("On light Bathroom");
	io->ledDim(OSK_DCO2, 100, 2000);
}

void offLightBathroom() {
	DBG("Off light Bathroom");
	io->ledDim(OSK_DCO2, 0, 2000);
}

void onLightLivingRoom() {
	DBG("On light Living room");
	io->ledDim(OSK_DCO3, 100, 2000);
}

void offLightLivingRoom() {
	DBG("Off light Living room");
	io->ledDim(OSK_DCO3, 0, 2000);
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
	mKitchen = new Motion(OSK_IO1, false);
	mKitchen->onCallback(onLightKitchen);
	mKitchen->offCallback(offLightKitchen);
	mKitchen->begin(5);

	/*mBathroom = new Motion(OSK_IO2, true);
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
	hBathroom->onHigh(onHerkonOff);*/
}


void setup()
{
	delay(1000);
	Serial.begin(115200);
	DBG("Starting DC2...");

	//setCpuFrequencyMhz(80);

	ArduinoOTA.setHostname("home-dc-2");

	EEPROM.begin(sizeof(Settings));

	nm = NetworkManager::getInstance();
	nm->init(&moduleSettings);
	nm->connect();

	io = IO::getInstance();
	io->mode(OSK_GREEN_LED, OUTPUT);
	io->set(OSK_GREEN_LED, LOW);

	tDebug = xTimerCreate("debugPrint", pdMS_TO_TICKS(5000), pdTRUE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(debugPrint));
	xTimerStart(tDebug, 0);

	tGreenLed = xTimerCreate("greenLed", pdMS_TO_TICKS(500), pdTRUE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(greenLedBlink));
	xTimerStart(tGreenLed, 0);

	motionDetection();

	ArduinoOTA.begin();

	telnet.onConnect(onTelnetConnect);

	io->mode(4, OUTPUT);
	io->set(4, LOW);

	#define RXD2 16
	#define TXD2 17
	Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

	/*CAN_cfg.speed = CAN_SPEED_125KBPS;
  CAN_cfg.tx_pin_id = GPIO_NUM_17;
  CAN_cfg.rx_pin_id = GPIO_NUM_16;
  CAN_cfg.rx_queue = xQueueCreate(rx_queue_size, sizeof(CAN_frame_t));
  // Init CAN Module
  ESP32Can.CANInit();*/
}

void loop()
{
	ArduinoOTA.handle();
	#if OSK_DEBUG_USE_TELNET
  	telnet.loop();
	#endif

	//CAN_frame_t rx_frame;

  // Receive next CAN frame from queue
 /* if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE) {

    if (rx_frame.FIR.B.FF == CAN_frame_std) {
      printf("New standard frame");
    }
    else {
      printf("New extended frame");
    }

    if (rx_frame.FIR.B.RTR == CAN_RTR) {
      printf(" RTR from 0x%08X, DLC %d\r\n", rx_frame.MsgID,  rx_frame.FIR.B.DLC);
    }
    else {
      printf(" from 0x%08X, DLC %d, Data ", rx_frame.MsgID,  rx_frame.FIR.B.DLC);
			
			printf(rx_frame.data.u8[0] ? " ON " : " OFF ");

			io->set(OSK_GREEN_LED, rx_frame.data.u8[0]);

      printf("\n");
    }
  }*/

	/*while (Serial2.available()) {
    Serial.print(char(Serial2.read()));
  }*/
}
