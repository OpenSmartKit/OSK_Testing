#include <Arduino.h>

void setup()
{
	delay(1000);
	Serial.begin(115200);
	Serial.println("Starting blank blink...");

	pinMode(2, OUTPUT);
}

void loop()
{
	digitalWrite(2, !digitalRead(2));
	Serial.print("Ping: ");
	Serial.println(millis());
	delay(5000);
}
