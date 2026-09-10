#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);
const byte address[6] = "00001";

#define PASSWORD 12345

struct Data {
  int x;
  int y;
  int key;
};

Data data;

int joyX = A0;
int joyY = A1;

void setup() {
  Serial.begin(9600);

  Serial.println("Controller Starting...");

  if (!radio.begin()) {
    Serial.println("NRF NOT DETECTED!");
    while (1);
  }

  Serial.println("NRF Initialized");

  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.setRetries(3, 5);

  radio.openWritingPipe(address);
  radio.stopListening();

  Serial.println("Transmitter Ready");
}

void loop() {

  // Read joystick values
  data.x = analogRead(joyX);
  data.y = analogRead(joyY);

  // Send password/key expected by receiver
  data.key = PASSWORD;

  // Send data
  bool success = radio.write(&data, sizeof(data));

  // Serial monitoring
  Serial.print("X: ");
  Serial.print(data.x);

  Serial.print("  Y: ");
  Serial.print(data.y);

  Serial.print("  KEY: ");
  Serial.print(data.key);

  Serial.print("  TX: ");
  Serial.println(success ? "OK" : "FAIL");

  delay(50);
}
