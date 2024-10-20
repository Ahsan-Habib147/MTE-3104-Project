#include <Servo.h>

Servo myServo1;  // Create a servo object for the first servo
Servo myServo2;  // Create a servo object for the second servo
int irSensorPin = 2; // Pin for the IR sensor

// Pin definitions for traffic lights 1 and 2
const int red1 = A0;
const int yellow1 = A1;
const int green1 = A2;
const int red2 = 5;
const int yellow2 = 6;
const int green2 = 7;

// Pin definitions for traffic lights 3 and 4
const int red3 = 8;
const int yellow3 = 9;
const int green3 = 10;
const int red4 = 11;
const int yellow4 = 12;
const int green4 = 13;

unsigned long lastSwitchTime = 0;   // Tracks the last time traffic light changed
const unsigned long interval = 5000; // Time interval for each light phase (5 seconds)

// Traffic light states
int currentLight = 1;  // Tracks the current traffic light (1 to 4)
bool trainDetected = false;  // Flag for train detection
unsigned long yellowStartTime = 0; // Time when yellow light was activated
bool yellowActive = false; // Flag to indicate if yellow light is active

void setup() {
  // Set pin modes for traffic lights
  pinMode(red1, OUTPUT);
  pinMode(yellow1, OUTPUT);
  pinMode(green1, OUTPUT);
  pinMode(red2, OUTPUT);
  pinMode(yellow2, OUTPUT);
  pinMode(green2, OUTPUT);
  
  pinMode(red3, OUTPUT);
  pinMode(yellow3, OUTPUT);
  pinMode(green3, OUTPUT);
  pinMode(red4, OUTPUT);
  pinMode(yellow4, OUTPUT);
  pinMode(green4, OUTPUT);

  myServo1.attach(3);  // Attach servos to pins
  myServo2.attach(4);
  
  pinMode(irSensorPin, INPUT);
  
  // Initialize all lights as red
  activateAllRed();
  
  // Set initial servo positions
  myServo1.write(60);
  myServo2.write(60);

  Serial.begin(9600); // Initialize serial communication
}

void loop() {
  // Check for emergency vehicle signals from Arduino 2
  if (Serial.available()) {
    int emergencyVehicleID = Serial.read(); // Read the emergency vehicle ID

    // Control traffic lights based on emergency vehicle ID
    switch (emergencyVehicleID) {

      
      case 1:
        Serial.println("Emergency vehicle detected: 67284119");
        activateTrafficLight(1); // Activate traffic light 1
        break;
      case 2:
        Serial.println("Emergency vehicle detected: 4693");
        activateTrafficLight(2); // Activate traffic light 2
        break;
      case 3:
        Serial.println("Emergency vehicle detected: -24397");
        activateTrafficLight(3); // Activate traffic light 3
        break;
      case 4:
        Serial.println("Emergency vehicle detected: 9290");
        activateTrafficLight(4); // Activate traffic light 4
        break;
      case 5:
        Serial.println("Emergency vehicle detected: 67284119");
        activateTrafficLight(1); // Activate traffic light 1 for ID 67284119
        break;
      default:
        deactivateAllLights(); // Turn off all lights for unknown ID
        break;
    }
  }

  // Check for train detection continuously
  controlTrafficWithServo();
  
  // If no train is detected, continue with the normal traffic light cycle
  if (!trainDetected) {
    unsigned long currentTime = millis();
    
    if (!yellowActive && currentTime - lastSwitchTime >= interval) {
      // Time to switch traffic light
      lastSwitchTime = currentTime;
      activateYellow();
      yellowStartTime = currentTime; // Record when yellow was activated
      yellowActive = true; // Set the flag to true
    }

    // Check if yellow light duration has elapsed
    if (yellowActive && currentTime - yellowStartTime >= 2000) {
      deactivateAllLights(); // Turn off all lights after yellow delay
      switchTrafficLight(); // Switch to the next traffic light
      yellowActive = false; // Reset the flag
    }
  }
}

// Function to control servos based on IR sensor
void controlTrafficWithServo() {
  int irState = digitalRead(irSensorPin);
  
  if (irState == LOW) {  // If train is detected (IR sensor reads 0)
    myServo1.write(0);  // Move the servo to position 0
    myServo2.write(0);
    if (!trainDetected) {  // If the train detection state is new
      trainDetected = true;
      allGreenOff();  // Turn off all green lights immediately
      trafficTrain();  // Immediately switch all lights to red
    }
  } else {
    myServo1.write(60);   // Default servo position
    myServo2.write(60);
    if (trainDetected) {  // Reset the train detection flag when train passes
      trainDetected = false;
      lastSwitchTime = millis();  // Reset the traffic light cycle timing
      activateYellow();  // Activate the yellow light immediately
      yellowStartTime = millis(); // Reset the yellow start time
      yellowActive = true; // Set yellow light active flag
    }
  }
}

// Function to switch traffic lights in sequence
void switchTrafficLight() {
  // Switch based on the current traffic light
  switch (currentLight) {
    case 1:
      digitalWrite(green1, HIGH); // Turn on green for the current light
      digitalWrite(red2, HIGH);
      digitalWrite(red3, HIGH);
      digitalWrite(red4, HIGH);
      break;
      
    case 2:
      digitalWrite(green2, HIGH);
      digitalWrite(red1, HIGH);
      digitalWrite(red3, HIGH);
      digitalWrite(red4, HIGH);
      break;
      
    case 3:
      digitalWrite(green3, HIGH);
      digitalWrite(red1, HIGH);
      digitalWrite(red2, HIGH);
      digitalWrite(red4, HIGH);
      break;
      
    case 4:
      digitalWrite(green4, HIGH);
      digitalWrite(red1, HIGH);
      digitalWrite(red2, HIGH);
      digitalWrite(red3, HIGH);
      break;
  }
  
  // Cycle to the next traffic light
  currentLight = (currentLight % 4) + 1;
}

// Function to activate traffic light based on emergency vehicle
void activateTrafficLight(int lightNumber) {
  deactivateAllLights(); // Turn off all lights before activating a specific one

  // Activate the specified traffic light and ensure others are red
  switch (lightNumber) {
    case 1:
      digitalWrite(green1, HIGH); // Activate green for traffic light 1
      digitalWrite(red2, HIGH);    // Ensure red for other lights
      digitalWrite(red3, HIGH);
      digitalWrite(red4, HIGH);
      break;
    case 2:
      digitalWrite(green2, HIGH); // Activate green for traffic light 2
      digitalWrite(red1, HIGH);    // Ensure red for other lights
      digitalWrite(red3, HIGH);
      digitalWrite(red4, HIGH);
      break;
    case 3:
      digitalWrite(green3, HIGH); // Activate green for traffic light 3
      digitalWrite(red1, HIGH);    // Ensure red for other lights
      digitalWrite(red2, HIGH);
      digitalWrite(red4, HIGH);
      break;
    case 4:
      digitalWrite(green4, HIGH); // Activate green for traffic light 4
      digitalWrite(red1, HIGH);    // Ensure red for other lights
      digitalWrite(red2, HIGH);
      digitalWrite(red3, HIGH);
      break;
    default:
      deactivateAllLights(); // Turn off all lights if an unknown ID is received
      break;
  }
}

// Function to turn off all traffic lights
void deactivateAllLights() {
  digitalWrite(red1, LOW);
  digitalWrite(yellow1, LOW);
  digitalWrite(green1, LOW);
  
  digitalWrite(red2, LOW);
  digitalWrite(yellow2, LOW);
  digitalWrite(green2, LOW);
  
  digitalWrite(red3, LOW);
  digitalWrite(yellow3, LOW);
  digitalWrite(green3, LOW);
  
  digitalWrite(red4, LOW);
  digitalWrite(yellow4, LOW);
  digitalWrite(green4, LOW);
}

// Function to set all traffic lights to red (train mode)
void trafficTrain() {
  // Set all traffic lights to red
  activateAllRed();
  // Optional: Add a delay if you want to simulate the train duration here.
}

// Function to turn off all green lights immediately
void allGreenOff() {
  digitalWrite(green1, LOW);
  digitalWrite(green2, LOW);
  digitalWrite(green3, LOW);
  digitalWrite(green4, LOW);
}

// Function to activate all red lights
void activateAllRed() {
  digitalWrite(red1, HIGH);
  digitalWrite(red2, HIGH);
  digitalWrite(red3, HIGH);
  digitalWrite(red4, HIGH);
}

// Function to activate yellow lights for a specific time before switching
void activateYellow() {
  switch (currentLight) {
    case 1:
      digitalWrite(yellow1, HIGH);
      break;
    case 2:
      digitalWrite(yellow2, HIGH);
      break;
    case 3:
      digitalWrite(yellow3, HIGH);
      break;
    case 4:
      digitalWrite(yellow4, HIGH);
      break;
  }
}
