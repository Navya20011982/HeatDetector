Heater Control System - upliance.ai Embedded Systems Intern Assignment
Overview
This project implements a basic heater control system as part of the Embedded Systems Intern Assignment for upliance.ai. The system uses an Arduino Nano to monitor temperature via a DHT22 sensor and control a simulated heater (via a relay module) based on predefined temperature thresholds. It features a state machine with five states: Idle, Heating, Stabilizing, TargetReached, and Overheat. Visual feedback is provided through an RGB LED, which changes color based on the system's state, and temperature/state data is logged over Serial.

Components Used:-
Arduino Nano: Microcontroller for running the system.
DHT22 Sensor: Measures temperature (used for heater control).
Relay Module: Simulates the heater (turns on/off based on temperature).
RGB LED: Provides visual feedback for system states (common cathode).
Idle: Off
Heating: Blue
Stabilizing: Cyan
TargetReached: Green
Overheat: Red


1000-ohm Resistor: Pull-up resistor for the DHT22 data pin.

Project Structure

heater_control.ino: The Arduino sketch implementing the heater control system.
diagram.json: Wokwi configuration file defining the circuit connections.
README.md: This file, providing project documentation.

Setup Instructions
1. Development Environment (VS Code)
I developed this project using Visual Studio Code (VS Code) with the PlatformIO extension for Arduino development. Here's how to set it up:

Install VS Code: Download and install VS Code from code.visualstudio.com.
Install PlatformIO Extension:
Open VS Code, go to Extensions (Ctrl+Shift+X), and search for "PlatformIO IDE".
Install the extension.


Create a PlatformIO Project:
Open PlatformIO Home (click the PlatformIO icon in the sidebar).
Click "New Project", name it (e.g., HeaterControlSystem), select "Arduino Nano" as the board, and "Arduino" as the framework.
PlatformIO will create a project structure with a src folder.


Add the Arduino Code:
Copy the contents of heater_control.ino into src/main.cpp.
Update platformio.ini to include the DHT library:[env:nanoatmega328]
platform = atmelavr
board = nanoatmega328
framework = arduino
lib_deps = adafruit/DHT sensor library




Add the Wokwi Configuration:
Copy diagram.json into the project folder (e.g., HeaterControlSystem/diagram.json).



2. Simulation (Wokwi)
Since VS Code doesn't simulate circuits, I used Wokwi to test the system. Follow these steps to simulate the project:

Open Wokwi:
Go to wokwi.com and create a new project.
Select "Arduino Nano" as the board.


Load the Circuit:
Copy the contents of diagram.json and paste them into Wokwi's "diagram.json" tab, or upload the file directly.


Load the Code:
Copy the contents of heater_control.ino and paste them into Wokwi's code editor.


Run the Simulation:
Click the "Start" button in Wokwi.
Adjust the DHT22's temperature slider to simulate different temperatures (e.g., 20°C to 40°C).
Observe the relay toggling (heater on/off), the RGB LED changing colors based on the state, and the Serial Monitor logs (9600 baud).



3. Physical Hardware
Connect the Arduino Nano to your computer via USB.
Open the Serial Monitor (9600 baud) to view temperature and state logs.

System Functionality

Temperature Thresholds:
Target Temperature: 30°C
Stabilizing Range: ±1°C (29°C to 31°C)
Overheat Threshold: >35°C


States:
Idle: Temperature below 29°C; heater off, RGB LED off.
Heating: Temperature below 29°C; heater on, RGB LED blue.
Stabilizing: Temperature between 29°C and 31°C; heater adjusts to maintain range, RGB LED cyan.
TargetReached: Temperature above 31°C; heater off, RGB LED green.
Overheat: Temperature above 35°C; heater off, RGB LED red.


Serial Logs: The system logs temperature, state, and heater status every 2 seconds via Serial (9600 baud).

Pin Connections

DHT22:
VCC → Arduino 5V
GND → Arduino GND
Data → D2 (with 1000-ohm pull-up resistor to 5V)


Relay Module:
VCC → Arduino 5V
GND → Arduino GND
IN → D6


RGB LED (common cathode):
Red → D3
Green → D4
Blue → D5
Cathode → GND


