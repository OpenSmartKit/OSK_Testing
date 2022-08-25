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

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

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
//AsyncWebSocket ws("/ws");

void debugPrint()
{
	Serial.printf(" - MS: %lu, HEAP: %d\n", millis(), ESP.getFreeHeap());
  //writeRS485();
  //nm->debug("Debug: ");
}

bool testRelay = false;
void greenLedBlink()
{
	io->set(OSK_GREEN_LED, !io->get(OSK_GREEN_LED));
}

void notifyClients() {
  //ws.textAll(String("test"));
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "toggle") == 0) {
      notifyClients();
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  //ws.onEvent(onEvent);
  //server.addHandler(&ws);
}

void setup()
{
	delay(1000);
	Serial.begin(115200);
	Serial.println("Starting...");

	EEPROM.begin(sizeof(Settings));

	nm = NetworkManager::getInstance();
	nm->init(&moduleSettings);
	nm->connect();

	io = IO::getInstance();
	io->mode(OSK_GREEN_LED, OUTPUT);
	io->set(OSK_GREEN_LED, HIGH);

	io->ledDim(OSK_DCO3, 30, 5000);

	tDebug = xTimerCreate("debugPrint", pdMS_TO_TICKS(5000), pdTRUE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(debugPrint));
	xTimerStart(tDebug, 0);

	tGreenLed = xTimerCreate("greenLed", pdMS_TO_TICKS(1000), pdTRUE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(greenLedBlink));
	xTimerStart(tGreenLed, 0);

	initWebSocket();

	server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request){
      AsyncResponseStream *response = request->beginResponseStream("application/json");
      DynamicJsonDocument json(1024);
      json["name"] = moduleSettings.getModuleName();
      json["ssid"] = WiFi.SSID();
      json["ip"] = WiFi.localIP().toString();
      serializeJson(json, *response);
      request->send(response);
  });

  AsyncElegantOTA.begin(&server);
	server.begin();
}

void loop()
{
	//ws.cleanupClients();
}
