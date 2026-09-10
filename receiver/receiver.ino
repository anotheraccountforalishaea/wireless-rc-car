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

// Motor pins
int IN1 = 3;
int IN2 = 4;
int IN3 = 5;
int IN4 = 6;
int ENA = 9;
int ENB = 10;

unsigned long lastSignal = 0;

void setup() {
  Serial.begin(9600);

  Serial.println("🚗 Receiver Starting...");

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  if (!radio.begin()) {
    Serial.println("❌ NRF NOT DETECTED!");
    while (1);
  }

  Serial.println("✅ NRF Initialized");

  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(0, address);
  radio.startListening();

  Serial.println("📡 Waiting for signal...");
}

void loop() {

  if (radio.available()) {
    radio.read(&data, sizeof(data));
    lastSignal = millis();

    Serial.print("Received -> X: ");
    Serial.print(data.x);
    Serial.print(" Y: ");
    Serial.print(data.y);
    Serial.print(" KEY: ");
    Serial.println(data.key);

    // Password check
    if (data.key != PASSWORD) {
      Serial.println("❌ WRONG PASSWORD!");
      stopMotors();
      return;
    }

    controlMotors(data.x, data.y);
  }

  // Fail-safe: stop if no signal
  if (millis() - lastSignal > 500) {
    Serial.println("⚠️ No Signal - Stopping");
    stopMotors();
  }
}

// ===== CONTROL LOGIC =====

void controlMotors(int x, int y) {

  // Dead zone
  if (x > 450 && x < 550 && y > 450 && y < 550) {
    Serial.println("⏹ STOP");
    stopMotors();
  }

  // Forward
  else if (y > 600) {
    if (x > 600) {
      Serial.println("↗️ FORWARD RIGHT");
      forwardRight();
    } 
    else if (x < 400) {
      Serial.println("↖️ FORWARD LEFT");
      forwardLeft();
    } 
    else {
      Serial.println("⬆ FORWARD");
      forward();
    }
  }

  // Backward
  else if (y < 400) {
    if (x > 600) {
      Serial.println("↘️ BACKWARD RIGHT");
      backwardRight();
    } 
    else if (x < 400) {
      Serial.println("↙️ BACKWARD LEFT");
      backwardLeft();
    } 
    else {
      Serial.println("⬇ BACKWARD");
      backward();
    }
  }

  // Turn in place
  else if (x > 600) {
    Serial.println("➡ RIGHT");
    right();
  }
  else if (x < 400) {
    Serial.println("⬅ LEFT");
    left();
  }
}

// ===== MOTOR FUNCTIONS =====

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 180);
  analogWrite(ENB, 180);
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 180);
  analogWrite(ENB, 180);
}

void left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 180);
  analogWrite(ENB, 180);
}

void right() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 180);
  analogWrite(ENB, 180);
}

void forwardLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 100);
  analogWrite(ENB, 200);
}

void forwardRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 200);
  analogWrite(ENB, 100);
}

void backwardLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 100);
  analogWrite(ENB, 200);
}

void backwardRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 200);
  analogWrite(ENB, 100);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
