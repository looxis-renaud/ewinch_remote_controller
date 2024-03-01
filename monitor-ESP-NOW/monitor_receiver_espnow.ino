/*
 * electric paraglider winch remote control monitor 
 * use it on an Lilygo T-Display S3 to display winch values on your paraglider Cockpit.
 * This monitor connects with the TTGO Lora ESP32 Paxounter Remote Transmitter via ESP-NOW
 * to display useful data in your eye-sight (rather than having to turn your head
 * to view the onboard OLED display of your remote)
 * Code Basis of ESP-Now communication by Rui Santos
 * Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files.
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
*/

#include "Arduino.h" 
#include "pin_config.h"
#include <esp_now.h>
#include <WiFi.h>

// TFT Display
#include <TFT_eSPI.h>
// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

#define WAIT 100
unsigned long targetTime = 0; // Used for testing draw times

// Structure to receive data
// Must match the sender structure
struct EspNowTxMessage {
  int8_t pullValue;
  int currentPull;
  int8_t currentState;
  bool servo;
  bool relay;
  uint8_t tachometer;
  uint8_t dutyCycleNow;
} ;

// Create a struct message called EspNowTXMessage
struct EspNowTxMessage EspNowTxMessage;

int xpos = 10;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&EspNowTxMessage, incomingData, sizeof(EspNowTxMessage));
  // Serial.print("Bytes received: ");
  // Serial.println(len);
  // Serial.print("Target Pull: ");
  // Serial.println(EspNowTxMessage.pullValue);
  // Serial.print("Current Pull: ");
  // Serial.println(EspNowTxMessage.currentPull);
  // Serial.print("State: ");
  // Serial.println(EspNowTxMessage.currentState);
  // Serial.print("Servo: ");
  // Serial.println(EspNowTxMessage.servo);
  // Serial.print("Relay: ");
  // Serial.println(EspNowTxMessage.relay);
  // Serial.print("Line: ");
  // Serial.println(EspNowTxMessage.tachometer);
  // Serial.print("Speed%: ");
  // Serial.println(EspNowTxMessage.dutyCycleNow);
  // Serial.println();

  draw();

}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  Serial.println("Hello T-Display-S3");

  // Initialize TFT Display
  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);

  tft.begin();
  tft.setRotation(3);
  tft.setSwapBytes(true);

  ledcSetup(0, 10000, 8);
  ledcAttachPin(38, 0);
  ledcWrite(0, 110);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  esp_now_init();
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}


void draw() {
  tft.fillScreen(TFT_BLACK); // Clear the screen
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("Pull: " + String(EspNowTxMessage.pullValue) + "/" + String(EspNowTxMessage.currentPull) + "kg", 0, 0, 4); // Display current pull
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("Line: " + String(EspNowTxMessage.tachometer) + " Meter", 0, 40, 4); // Display deployed line length in meters
  tft.drawString("Speed: " + String(EspNowTxMessage.dutyCycleNow) + "%", 0 ,65, 4);  // Display %of max Speed before VESC goes bust
  tft.setTextColor(TFT_MAGENTA, TFT_BLACK);
  int xpos = 0;
  xpos += tft.drawString("Fan: " + String(EspNowTxMessage.relay ? "ON" : "OFF") + " | ", 0, 100, 2); // Display servo state
  tft.drawString("LineCutter: " + String(EspNowTxMessage.servo ? "EMERGENCY" : "Ready!"), xpos, 100, 2); // Display servo state
}
 
void loop() {


}
