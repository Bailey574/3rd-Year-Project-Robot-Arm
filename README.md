# 3rd-Year-Project-Robot-Arm
This repository contains all the code for my project, the code is for a 4-DOF robot arm to allow actuation and sorting. The main robot uses two states, one allows manual control, this setting

Diagram Image Video

User Installation. 
<img width="919" height="663" alt="image" src="https://github.com/user-attachments/assets/abdc5ff7-b11b-41de-aad5-6a1e3ffcabfd" />

The setup is shown above a arduino uno, motor driver board, ultrasonic and colour sensor is needed. As well as servos and the robot arm setup from acrylic. It needs to be connected to the correct pins in the diagram and code. The code can be uploaded to arduino uno via a usb b.

How to run:
Use code computer control other codes are used for testing.
Once the code is uploaded the user will enter a state machine will options servo control or scan mode, the block diagram belows show how each state works. Entering commands like changing each state, controlling servo angles and moving to preset location is done via the serial commands. The serial monitor also prints instructions on formatting commands on startup for users.
<img width="1093" height="508" alt="image" src="https://github.com/user-attachments/assets/d7860f68-99e5-4b9b-9bb7-61ff305d472f" />
The first state introduced control of each servo via computer using the serial monitor; the desired angle is typed, and the servo moves smoothly to this desired angle. Either a servo with angle is typed "servo1 76" or a location "red2. Type "servo" in serial to enter this state. By default robot moves to "home" state on startup and entering this state.

The other state of the software is scan mode, responsible for the main function of the robot to sort bricks by size and colour. Enter "scanmode" to enter this state. Place brick before entering this state. The robot will scan the the brick to identity size and colour, then the arm will pick up and place the correct location. Place another brick and enter "scanmode" to repeat.

Technical Details
Tutorial blogs [15] and [16] provided a reference for writing code effectively for TC3200 and HC-SR04.
[15] projecthub.arduino.cc. (n.d.). Color Detection Using TCS3200/230. [online] Available at:
https://projecthub.arduino.cc/SurtrTech/color-detection-using-tcs3200230-a1e463 [Accessed 5 Jan. 2026].
[16] projecthub.arduino.cc. (n.d.). Arduino Project Hub. [online] Available at: https://projecthub.arduino.cc/Isaac100/getting-started-with-the-hc-sr04-ultrasonic-sensor-7cabe1. [Accessed 5 Jan. 2026].

Known Issues/Future Improvements

