# Traffic Control System

This project implements an Arduino-based traffic control system that manages traffic lights using a round-robin approach, handles emergency traffic control, and incorporates train detection features.

## Features

### 1. Round Robin Traffic Control
- The system controls four sets of traffic lights in a round-robin manner.
- Each traffic light is green for a fixed interval, allowing smooth traffic flow.
- The interval can be adjusted as needed for specific traffic conditions.

### 2. Emergency Traffic Control
- The system prioritizes emergency vehicles by detecting specific RFID tags.
- When an emergency vehicle is detected, the current traffic lights turn red while the corresponding traffic light for the emergency vehicle turns green.
- This feature ensures that emergency vehicles can pass through intersections without delay.

### 3. Train Detection Traffic Control
- The system incorporates an IR sensor to detect approaching trains.
- When a train is detected, all traffic lights immediately turn red to ensure safety.
- Once the train passes, the system activates the yellow light to prepare vehicles for a green light, maintaining a safe traffic flow.

## Getting Started

### Prerequisites
- Arduino IDE
- Arduino board (e.g., Arduino Uno)
- Servo motors
- IR sensor
- Traffic light LEDs
- RFID module (for emergency vehicle detection)

