# Power Pong Pals Machine 🎮

## Overview
The Power Pong Pals Machine is an interactive arcade-style game that launches ping pong balls into cups using a motorized cannon system. Players control the launcher using arcade buttons to aim and fire. The system automatically reloads balls and tracks scoring using sensors, creating a fully automated and engaging experience.

This project was designed as a transportable mini game that can be easily moved and powered anywhere.

---

## Features
- 🎯 Arcade-style aiming and shooting system  
- 🔁 Automatic ball reload mechanism  
- 📊 Score tracking using IR break beam sensors  
- ⏱ Display system for score and timer  
- 🎮 Real arcade push buttons for user input  
- 🤖 Fully controlled by Arduino  

---

## How It Works
Players use arcade buttons to control the cannon’s movement. The left and right buttons rotate the launcher, while the up and down buttons adjust the angle. When the fire button is pressed, the system launches a ping pong ball toward the cups.

After firing, the system automatically reloads the next ball. An ultrasonic sensor detects when a ball is in position and stops the reload. IR break beam sensors detect when a ball enters a cup and update the score. The Arduino processes all inputs and controls the system to ensure smooth and consistent performance.

---

## Components Used
- Arduino Mega (main controller)  
- IR break beam sensors (scoring detection)  
- Ultrasonic sensor (reload detection)  
- Arcade push buttons (user input)  
- DC motors / servo motors (cannon + reload system)  
- Motor driver  
- 7-segment display  
- Power supply / battery pack  
- Structural materials (metal frame, cardboard, 3D printed parts)  

---

## Design Purpose
The purpose of this project was to create an interactive arcade-style system that integrates mechanical motion, sensors, and user input into one automated device. The goal was to apply engineering concepts such as control systems, automation, and real-time feedback in a fun and functional design.

---

## Challenges
One of the main challenges was managing the wiring of the system. With multiple components connected to the Arduino, the wiring became complex and difficult to organize, making troubleshooting more time-consuming.

Another challenge was achieving precise measurements and control. Detecting ball position accurately and controlling the launcher angle required careful calibration to ensure reliable performance.

---

## Future Improvements
- Add sound effects for better user feedback  
- Increase range accuracy
- Add automatic reload chamber
- Implement high score tracking  

---

## Inspo
<img width="3296" height="2547" alt="Assembly 1" src="https://github.com/user-attachments/assets/2be41873-2920-4170-8a7b-ccae0528e6ac" />

---

## Contributors
- Miguel Pescador  
- Jay Sanchez  
- Christian Chaidez  
