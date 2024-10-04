#include <Wire.h>
#include <LoRa_E32.h>
#include "PDLS_EXT3_Basic_Global.h"  // E-Ink display library

// Pin Definitions
#define LORA_RX_PIN 1  // UART RX
#define LORA_TX_PIN 0  // UART TX
#define M0_PIN 2
#define M1_PIN 3
#define AUX_PIN 4
#define JOYSTICK_X_PIN 26  // ADC for Joystick X-axis
#define JOYSTICK_Y_PIN 27  // ADC for Joystick Y-axis
#define SWITCH1_PIN 14
#define SWITCH2_PIN 15
#define SWITCH3_PIN 16
#define SWITCH4_PIN 17

// E-Ink Display object
PDLS_EXT3_Basic_Global display;

// LoRa E32 object
LoRa_E32 e32(LORA_RX_PIN, LORA_TX_PIN, AUX_PIN, M0_PIN, M1_PIN);

// Variables for joystick and switches
int joystickX_raw, joystickY_raw;
float joystickX_percent, joystickY_percent;
bool switch1, switch2, switch3, switch4;

void setup() {
  Serial.begin(9600);
  pinMode(SWITCH1_PIN, INPUT_PULLUP);
  pinMode(SWITCH2_PIN, INPUT_PULLUP);
  pinMode(SWITCH3_PIN, INPUT_PULLUP);
  pinMode(SWITCH4_PIN, INPUT_PULLUP);
  
  // Initialize the LoRa module
  e32.begin();
  
  // E-Ink Display Initialization
  display.begin();
  display.clear();
  display.setFont("Roboto");
  display.setTextSize(1);
  
  // Display initial message
  display.setCursor(0, 0);
  display.println("Transmitter Initialized");
  display.display();
}

void loop() {
  // Read Joystick Inputs (12-bit ADC resolution)
  joystickX_raw = analogRead(JOYSTICK_X_PIN);
  joystickY_raw = analogRead(JOYSTICK_Y_PIN);
  
  // Convert raw values to percentages (0% - 100%)
  joystickX_percent = (joystickX_raw / 4095.0) * 100.0;
  joystickY_percent = (joystickY_raw / 4095.0) * 100.0;
  
  // Read Switch Inputs
  switch1 = digitalRead(SWITCH1_PIN) == LOW;
  switch2 = digitalRead(SWITCH2_PIN) == LOW;
  switch3 = digitalRead(SWITCH3_PIN) == LOW;
  switch4 = digitalRead(SWITCH4_PIN) == LOW;
  
  // Package data to send via LoRa
  String dataToSend = String(joystickX_raw) + "," + String(joystickY_raw) + "," +
                      String(switch1) + "," + String(switch2) + "," +
                      String(switch3) + "," + String(switch4);
  
  // Send data over LoRa
  ResponseStatus rs = e32.sendMessage(dataToSend);
  if (rs.code != 1) {
    Serial.println("Failed to send data");
  } else {
    Serial.println("Data sent: " + dataToSend);
  }
  
  // Update the E-Ink Display with Joystick and Switch status
  display.clear();
  display.setCursor(0, 0);
  display.println("Joystick X: " + String(joystickX_percent, 1) + "%");
  display.println("Joystick Y: " + String(joystickY_percent, 1) + "%");
  display.println("Switch 1: " + String(switch1));
  display.println("Switch 2: " + String(switch2));
  display.println("Switch 3: " + String(switch3));
  display.println("Switch 4: " + String(switch4));
  display.display();

  delay(200);  // Small delay to limit transmission rate
}
