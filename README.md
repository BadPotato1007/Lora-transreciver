# Lora Transreciver
## ⚠️This Repositry is under heavy development, and there may be breaking changes. I assume no responsibility for the loss of anything you own.


Transmitter Side
Hardware:

Raspberry Pi Pico: Main controller.
Ebyte E32-400D30T LoRa Module: For long-range communication.
2.66" e-ink Display (Pervasive Displays EXT 3 Dev Kit): To show telemetry data.
Two Joysticks: For controlling ailerons and throttle.
Four Switches: Auxiliary controls for other functions (e.g., flaps, landing gear, etc.).
Key Tasks:

Reading Joystick Input: Use ADC channels on the Raspberry Pi Pico to read the analog inputs from the joysticks and convert them to PWM signals.
Transmit Control Signals: Send joystick positions and switch states via LoRa (Ebyte module) to the receiver.
Display Data on E-Ink: Show the telemetry data (live GPS coordinates, height, displacement, plane direction) on the e-ink display using the library you provided.
Calculate Displacement: Using GPS data from both the transmitter and receiver to compute the distance between them.
Receiver Side
Hardware:

ESP8266/Wemos D1: Main controller to receive data from the transmitter and handle PWM outputs.
Ebyte E32-400D30T LoRa Module: To receive signals from the transmitter.
GPS Module: To get real-time position data.
MPU6050 Gyro: For stabilization and orientation sensing.
BMP180 Barometer: For altitude measurement.
Key Tasks:

Receive Control Signals: Use the LoRa module to receive control signals from the transmitter.
Output PWM for Ailerons and Throttle: Based on joystick inputs, output PWM signals to control servos and the brushless motor.
Gyro Stabilization: Use the MPU6050 to sense orientation and adjust servo outputs for stabilization.
Send Telemetry Data: Send GPS coordinates, altitude (from the barometer), and orientation back to the transmitter.
Additional Features for the E-Ink Display:
Battery level monitoring for both the transmitter and receiver.
Flight time since last power-on.
Signal strength between the transmitter and receiver.
