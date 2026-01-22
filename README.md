# Smart Drone Hangar

An IoT embedded system implementing a smart hangar for drones, composed of an Arduino-based controller and a Java-based remote control unit.

**Assignment #02 - Embedded Systems & IoT**

This project realizes a **"Smart Drone Hangar"** system that automates the storage, takeoff, and landing procedures of a drone. It features intelligent environmental monitoring, sensor-based presence detection, and a graphical remote control interface on the PC.

## 📋 Project Overview

*   **Architecture:** Distributed Master-Slave System.
    *   **Drone Hangar (Arduino):** The embedded controller (Slave) handling sensors, actuators, and the system's Finite State Machine (FSM). Implemented using a Super-Loop with Cooperative Multitasking (Scheduler) and Object-Oriented C++.
    *   **Drone Remote Unit (Java PC):** A GUI-based application (Master) representing the drone/operator, communicating via serial line.
*   **Key Features:**
    *   **Automated Access:** Servo-controlled hangar door responding to Takeoff/Landing requests.
    *   **Sensor Fusion:** Uses PIR (Presence) and Sonar (Distance) to validate drone movements.
    *   **Safety Monitoring:** Multi-stage temperature alarm system (Pre-Alarm and Critical Alarm) to prevent overheating.
    *   **Remote Telemetry:** Real-time visualization of system state, temperature, and distance on the PC.

## 📂 Repository Structure

The repository is organized as follows:

- `drone-hangar/`: Source code of the Arduino-based subsystem.
- `drone-remote-unit/`: Source code of the PC-based part.
- `doc/`: Documentation including:
    - A brief report describing the solution.
    - FSM diagrams.
    - Breadboard/Schema representation (TinkerCad/Fritzing).
    - Demonstration video (or link).

## 📚 Libraries Used

### Arduino (Drone Hangar)
*   `LiquidCrystal_I2C` - For controlling the 16x2 LCD via I2C.
*   `Servo` - For controlling the hangar door servomotor.
*   `EnableInterrupt` - For handling pin change interrupts on the reset button.
*   `Arduino.h` - Core Arduino framework.

### Java (Drone Remote Unit)
*   `Java Swing` - Native Graphical User Interface.
*   `JSSC` (Java Simple Serial Connector) - For robust serial communication with the Arduino unit.
*   `SLF4J` - Simple Logging Facade for Java.

## 🛠️ Hardware Requirements

*   **Microcontroller:** 1x Arduino Uno (or compatible board)
*   **Actuators:**
    *   1x Servo Motor (SG90 or similar) - Hangar Door
*   **Sensors:**
    *   1x PIR Motion Sensor (Drone Presence Detector - DPD)
    *   1x Ultrasonic Sensor HC-SR04 (Drone Distance Detector - DDD)
    *   1x Analog Temperature Sensor (e.g., TMP36 or LM35)
*   **Interface:**
    *   1x LCD Display (16x2 with I2C)
    *   2x Green LEDs (Status L1, L2)
    *   1x Red LED (Alarm L3)
    *   1x Tactile Button (Reset)
*   **Power & Misc:** Resistors (220Ω for LEDs, 10kΩ for Buttons), Breadboard, Jumper Wires.

## 🔌 Wiring Map

| Component | Arduino Pin | Note |
| :--- | :--- | :--- |
| **L1 LED (Green)** | Digital 11 | Indicates "Drone Inside" / Idle |
| **L2 LED (Green)** | Digital 12 | Blinks during Takeoff/Landing |
| **L3 LED (Red)** | Digital 13 | Alarm Indicator |
| **Reset Button** | Digital 7 | Input (Interrupt) to reset Alarm |
| **PIR Sensor** | Digital 2 | Landing detection |
| **Servo Motor** | Digital 6 | Hangar Door control |
| **Sonar Trigger** | Digital 9 | Distance measurement |
| **Sonar Echo** | Digital 8 | Distance measurement |
| **Temp Sensor** | Analog A0 | Environmental monitoring |
| **LCD SDA** | Analog A4 | I2C Data |
| **LCD SCL** | Analog A5 | I2C Clock |

## 🔌 Communication Protocol

The system uses a text-based serial protocol to synchronize the PC and Arduino.

| Direction | Message | Function |
| :--- | :--- | :--- |
| **PC ➔ Arduino** | `TAKE OFF` | Request Hangar to open for takeoff |
| **PC ➔ Arduino** | `LANDING` | Request Hangar to open for landing |
| **Arduino ➔ PC** | `DRONE INSIDE` | Event: Drone safely inside / Rest state |
| **Arduino ➔ PC** | `TAKE OFF` | Event: Door open, waiting for departure |
| **Arduino ➔ PC** | `DRONE OUT` | Event: Drone has left the detection zone |
| **Arduino ➔ PC** | `LANDING` | Event: Door open, waiting for arrival |
| **Arduino ➔ PC** | `PRE ALARM` | Warning: High temperature, operations suspended |
| **Arduino ➔ PC** | `ALARM` | Critical: Max temperature, system locked |
| **Arduino ➔ PC** | `NORMAL` | Info: Temperature returned to safe levels |
| **Arduino ➔ PC** | `DIST:xx.xx` | Telemetry: Current sonar distance (cm) |
| **Arduino ➔ PC** | `TEMP:xx.xx` | Telemetry: Current temperature (°C) |

## 🎮 How to Use & System Behavior

1.  **Setup:** Connect the Arduino to the PC via USB. Ensure the correct port is configured in the Java application (default `COM6`).
2.  **Start:** Launch the Java application. The GUI will connect to the Arduino.
    *   *Arduino State:* **IDLE**. Door closed, L1 ON, LCD shows `DRONE INSIDE`.
3.  **Takeoff:**
    *   **User Action:** Click **TAKEOFF** on the Java GUI.
    *   **System Action:** Door opens, L2 blinks, LCD shows `TAKE OFF`.
    *   **Completion:** Simulate drone departure (remove object from Sonar range). When distance > 50cm for 2s, door closes, LCD shows `DRONE OUT`.
4.  **Landing:**
    *   **User Action:** Click **LAND** on the Java GUI.
    *   **System Action:** System checks PIR sensor. If motion detected: Door opens, L2 blinks, LCD shows `LANDING`.
    *   **Completion:** Simulate landing (move object closer to Sonar). When distance < 10cm for 2s, door closes, LCD shows `DRONE INSIDE`.
5.  **Safety & Alarms:**
    *   **Pre-Alarm:** If Temperature > 35°C (T1) for 5s, system enters Pre-Alarm (Orange status). New commands are ignored.
    *   **Alarm:** If Temperature > 45°C (T2) for 3s, system locks down (Red status). Door closes immediately.
    *   **Reset:** Alarm persists until temperature drops AND the physical **RESET** button on the Arduino is pressed.
