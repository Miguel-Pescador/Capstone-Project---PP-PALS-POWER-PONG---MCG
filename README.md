# Power Pong Pals Machine 🎮

## Overview

The Power Pong Pals Machine is an interactive arcade-style game that launches ping pong balls into cups using a motorized cannon system. Players control the launcher using arcade buttons to aim and fire. The system automatically reloads balls and tracks scoring using sensors, creating a fully automated and engaging experience.

This project was designed as a transportable mini game that can be easily moved and powered anywhere.

---

## Features

* 🎯 Arcade-style aiming and shooting system
* 🔁 Automatic ball reload mechanism
* 📊 Score tracking using IR break beam sensors
* 🏆 High score tracking between rounds
* ⏱ 2 minute 30 second game timer
* 🔢 LED matrix scoreboard display
* 📺 LCD countdown and status animations
* 🎮 Arcade push buttons for user input
* 🤖 Fully controlled by Arduino Mega

---

## How It Works

Players use arcade buttons to control the launcher’s movement. The left and right buttons rotate the launcher, while the up and down buttons adjust the angle. When the fire button is pressed, the launcher fires a ping pong ball toward the cups.

After firing, the launcher automatically reverses and resets itself before beginning the reload sequence. A reload motor feeds the next ping pong ball into position. An IR break beam sensor inside the reload system detects when the ball is ready and automatically stops the reload motor.

Additional IR break beam sensors are placed inside scoring cups. When a ping pong ball passes through a sensor, the Arduino adds points to the player’s score and updates the LED matrix display.

The system also includes:

* A 2:30 countdown timer
* “HURRY UP” warning animations near the end of the game
* A restart countdown sequence
* High score storage during gameplay sessions

At the end of a round, players can press the white arcade button to restart the game. The LCD display performs a 3…2…1 countdown before the next round begins.

---

## Components Used

* Arduino Mega (main controller)
* IR break beam sensors (score + reload detection)
* Arcade push buttons (user input)
* Servo motors / continuous rotation motors
* Motor driver
* MAX7219 8x8 LED matrix displays
* I2C LCD display
* Power supply / battery pack
* Structural materials (metal frame, cardboard, 3D printed parts)

---

## Functional Categories

### Output Display

* I2C LCD display
* MAX7219 LED matrix score display

### Manual User Input

* Arcade push buttons for aiming and firing

### Automatic Sensors

* IR break beam sensors for score detection
* IR break beam sensor for reload ball detection

### Actuators, Mechanisms, & Hardware

* Motorized launcher system
* Automatic reload mechanism
* Rotating and adjustable aiming system
* Custom frame and mounting hardware

### Logic, Processing, & Control

* Arduino Mega programmed logic
* Timer system
* High score tracking
* Automated reload and firing sequence
* Real-time score updates

---

## Design Purpose

The purpose of this project was to create an interactive arcade-style system that combines mechanical motion, automation, sensors, and user interaction into one fully integrated device.

The project demonstrates engineering concepts including:

* Embedded programming
* Automation
* Sensor integration
* Real-time control systems
* Mechanical design
* Human-machine interaction

---

## Challenges

One of the main challenges was organizing the large amount of wiring required for the project. Multiple motors, sensors, displays, and buttons had to be connected to the Arduino Mega while maintaining reliable operation.

Another challenge was tuning the launcher and reload timing. The firing motor, reverse motion, and reload sequence all had to be carefully programmed so the system could operate consistently without jamming.

Sensor calibration was also difficult because the IR break beam sensors needed to reliably detect ping pong balls under different lighting conditions and angles.

---

## Future Improvements

* Add sound effects and speaker output
* Improve launcher accuracy
* Add multiplayer mode
* Store high scores permanently using EEPROM
* Add LED animations and lighting effects
* Design a fully enclosed arcade cabinet

---

## Inspiration

<img width="3296" height="2547" alt="Assembly 1" src="https://github.com/user-attachments/assets/2be41873-2920-4170-8a7b-ccae0528e6ac" />

---

## Contributors

* Miguel Pescador
* Jay Sanchez
* Christian Chaidez

---

## Resources Used

* Arduino Documentation
* LedControl Library Documentation
* LiquidCrystal_I2C Library
* Arduino Servo Library
* MAX7219 Tutorials and Examples
* Online Arduino forums and debugging resources
