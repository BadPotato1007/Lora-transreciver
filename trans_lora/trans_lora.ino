#include <PDLS_EXT3_Basic_Global.h>
#include <Wire.h>
#include <SPI.h>
#include <LoRa_E32.h>

// Pin definitions
#define LORA_RX_PIN 0       // RX pin for LoRa
#define LORA_TX_PIN 1       // TX pin for LoRa
#define AUX_PIN 2           // AUX pin for LoRa
#define M0_PIN 3            // M0 pin for LoRa
#define M1_PIN 4            // M1 pin for LoRa

#define JOYSTICK_X_PIN A0   // Joystick X-axis pin
#define JOYSTICK_Y_PIN A1   // Joystick Y-axis pin
#define BUTTON_PIN 7        // Button or switch pin

// E-ink display setup
Screen_EPD_EXT3 display(eScreen_EPD_266_CS_0C, boardRaspberryPiPico_RP2040);
// LoRa setup
LoRa_E32 e32(&Serial1, AUX_PIN, M0_PIN, M1_PIN);   // LoRa module setup using Serial1

// Variables to hold joystick values
int joystickX = 0;
int joystickY = 0;
int buttonState = 0;

void setup() {
  // Start the built-in serial monitor for debugging
  Serial.begin(9600);

  // Initialize the hardware serial (Serial1) for LoRa communication
  Serial1.begin(9600);

  // Initialize the e-ink display
  display.begin();
  display.clear();
  
  // Initialize LoRa module
  e32.begin();
  
  // Set joystick and button pin modes
  pinMode(JOYSTICK_X_PIN, INPUT);
  pinMode(JOYSTICK_Y_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // Read joystick values (raw analog values from 0 to 1023)
  joystickX = analogRead(JOYSTICK_X_PIN);
  joystickY = analogRead(JOYSTICK_Y_PIN);
  
  // Map joystick values to percentages
  int joystickXPercent = map(joystickX, 0, 1023, 0, 100);
  int joystickYPercent = map(joystickY, 0, 1023, 0, 100);

  // Read the button state
  buttonState = digitalRead(BUTTON_PIN);

  // Clear the display before updating with new values
  display.clear();
  
  // Display the joystick values on the e-ink screen
  display.setCursor(0, 0);
  display.print("Joystick X: ");
  display.print(joystickXPercent);
  display.println("%");
  
  display.setCursor(0, 16);
  display.print("Joystick Y: ");
  display.print(joystickYPercent);
  display.println("%");
  
  // Display button state (example, you can modify it for your switches)
  display.setCursor(0, 32);
  if (buttonState == LOW) {
    display.println("Button Pressed");
  } else {
    display.println("Button Released");
  }
  
  // Send joystick and button data over LoRa
  String dataToSend = String(joystickXPercent) + "," + String(joystickYPercent) + "," + String(buttonState);
  e32.sendFixedMessage(0x00, 0x00, dataToSend.c_str());
  
  // Display a message showing the sent data
  display.setCursor(0, 48);
  display.print("Sent: ");
  display.println(dataToSend);

  // Update the display with the new data
  display.update();

  // Delay before the next loop iteration
  delay(500);
}
