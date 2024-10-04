#include <Wire.h>
#include <Servo.h>
#include <LoRa_E32.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>

// Pin Definitions
#define LORA_RX_PIN 1  // UART RX
#define LORA_TX_PIN 0  // UART TX
#define M0_PIN 2
#define M1_PIN 3
#define AUX_PIN 4
#define AILERON_PIN 5  // PWM pin for aileron servo
#define MOTOR_PIN 6    // PWM pin for motor (ESC)
#define GYRO_INT_PIN 7

// LoRa E32 object
LoRa_E32 e32(LORA_RX_PIN, LORA_TX_PIN, AUX_PIN, M0_PIN, M1_PIN);

// Servo objects for ailerons and motor
Servo aileronServo;
Servo motorServo;

// Sensors
Adafruit_MPU6050 mpu;
Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(9600);
  
  // Initialize LoRa module
  e32.begin();

  // Initialize Servos
  aileronServo.attach(AILERON_PIN);
  motorServo.attach(MOTOR_PIN);

  // Initialize Gyro (MPU6050)
  if (!mpu.begin()) {
    Serial.println("Failed to initialize MPU6050");
    while (1);
  }

  // Initialize Barometer (BMP180)
  if (!bmp.begin()) {
    Serial.println("Failed to initialize BMP180");
    while (1);
  }

  // Setup the gyro interrupt pin
  pinMode(GYRO_INT_PIN, INPUT);

  Serial.println("Receiver Initialized");
}

void loop() {
  // Receive LoRa data
  ResponseContainer rc = e32.receiveMessage();
  if (rc.status.code == 1) {
    String receivedData = rc.data;
    Serial.println("Received: " + receivedData);
    
    // Parse the received data
    int joystickX = receivedData.substring(0, receivedData.indexOf(',')).toInt();
    int joystickY = receivedData.substring(receivedData.indexOf(',') + 1, receivedData.lastIndexOf(',')).toInt();
    
    // Set servo positions based on joystick values
    int aileronPos = map(joystickX, 0, 4095, 0, 180);  // Map to servo range
    int motorSpeed = map(joystickY, 0, 4095, 0, 180);  // Map to ESC throttle range
    
    aileronServo.write(aileronPos);
    motorServo.write(motorSpeed);
  }

  // Read gyro data for stabilization
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  // Implement stabilization based on gyro data (simple proportional controller)
  int correction = map(g.gyro.x, -90, 90, -30, 30);  // Adjust range as needed
  int aileronPosWithStab = constrain(aileronServo.read() + correction, 0, 180);
  aileronServo.write(aileronPosWithStab);

  // Optional: Read altitude from BMP180 and display or send back to transmitter
  int altitude = bmp.readAltitude();
  Serial.println("Altitude: " + String(altitude) + " meters");

  delay(100);  // Adjust loop delay based on communication speed
}
