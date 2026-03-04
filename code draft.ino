#include <Servo.h>  // Allows control of VEX motor controllers (servo-style signal)

// ----------------------
// MOTOR OBJECTS
// ----------------------
// These represent the three movement motors and one reload motor
Servo motorLR;      // Left/Right aiming motor
Servo motorUD;      // Up/Down aiming motor
Servo motorFire;    // Firing motor
Servo motorReload;  // Reload conveyor motor


// ----------------------
// BUTTON PIN ASSIGNMENTS
// ----------------------
const int btnLeft  = 2;   // Move cannon left
const int btnRight = 3;   // Move cannon right
const int btnUp    = 4;   // Move cannon up
const int btnDown  = 5;   // Move cannon down
const int btnFire  = 6;   // Fire button


// ----------------------
// ULTRASONIC SENSOR PINS
// ----------------------
const int trigPin = 7;    // Sends sound pulse
const int echoPin = 8;    // Receives reflected pulse


// ----------------------
// MOTOR SIGNAL VALUES
// ----------------------
// 90 = stop (neutral signal for continuous motors)
// >90 = one direction
// <90 = opposite direction
const int stopSignal = 90;
const int forward = 120;
const int reverse = 60;


// Distance (in cm) where reload motor stops
const int detectionDistance = 6;


// =====================================================
// SETUP FUNCTION (Runs once at startup)
// =====================================================
void setup() {

  // Attach motors to their signal pins
  motorLR.attach(9);
  motorUD.attach(10);
  motorFire.attach(11);
  motorReload.attach(12);

  // Set button pins as INPUT_PULLUP
  // (Button reads LOW when pressed)
  pinMode(btnLeft, INPUT_PULLUP);
  pinMode(btnRight, INPUT_PULLUP);
  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  pinMode(btnFire, INPUT_PULLUP);

  // Set ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Stop all motors at startup
  motorLR.write(stopSignal);
  motorUD.write(stopSignal);
  motorFire.write(stopSignal);
  motorReload.write(stopSignal);

  Serial.begin(9600);  // Start serial monitor for debugging
}


// =====================================================
// MAIN LOOP (Runs repeatedly)
// =====================================================
void loop() {

  // ----------------------
  // AIM CONTROL SECTION
  // ----------------------

  // Left / Right movement
  if (digitalRead(btnLeft) == LOW) {
    motorLR.write(reverse);   // Move left
  } 
  else if (digitalRead(btnRight) == LOW) {
    motorLR.write(forward);   // Move right
  } 
  else {
    motorLR.write(stopSignal);  // Stop if no button pressed
  }

  // Up / Down movement
  if (digitalRead(btnUp) == LOW) {
    motorUD.write(forward);   // Move up
  } 
  else if (digitalRead(btnDown) == LOW) {
    motorUD.write(reverse);   // Move down
  } 
  else {
    motorUD.write(stopSignal);  // Stop if no button pressed
  }


  // ----------------------
  // FIRE SEQUENCE
  // ----------------------
  if (digitalRead(btnFire) == LOW) {

    Serial.println("FIRE!");

    // Spin firing motor to launch projectile
    motorFire.write(reverse);
    delay(1000);  // Fire duration (1 second)
    motorFire.write(stopSignal);

    Serial.println("Reloading...");

    // Start reload conveyor motor
    motorReload.write(forward);

    // Keep reloading until ultrasonic detects a ball
    while (true) {

      long distance = readUltrasonicCM();

      // If ball is detected within set distance
      if (distance > 0 && distance <= detectionDistance) {
        motorReload.write(stopSignal);  // Stop reload motor
        Serial.println("Ball Loaded!");
        break;  // Exit loop
      }
    }

    delay(500);  // Small pause before returning to normal operation
  }
}


// =====================================================
// ULTRASONIC DISTANCE FUNCTION
// Sends sound pulse and calculates distance in cm
// =====================================================
long readUltrasonicCM() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);  // Measure echo time

  long distance = duration * 0.034 / 2;    // Convert time to cm

  return distance;
}
