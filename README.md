# 3rd-Year-Project-Robot-Arm
This repository contains all the code for my project, the code is for a 4-DOF robot arm to allow actuation and sorting. The main robot uses two states, one allows manual control, this setting

Diagram Image Video

User Installation. 
<img width="919" height="663" alt="image" src="https://github.com/user-attachments/assets/abdc5ff7-b11b-41de-aad5-6a1e3ffcabfd" />

The setup is shown above a arduino uno, motor driver board, ultrasonic and colour sensor is needed. As well as servos and the robot arm setup from acrylic. It needs to be connected to the correct pins in the diagram and code. The code can be uploaded to arduino uno via a usb b.

How to run:
Once the code is uploaded the user will enter a state machine will options servo control or scan mode, the block diagram belows show how each state works. Entering commands like changing each state, controlling servo angles and moving to preset location is done via the serial link, which posts instructions included on startup.
<img width="1093" height="508" alt="image" src="https://github.com/user-attachments/assets/d7860f68-99e5-4b9b-9bb7-61ff305d472f" />


Technical Details

Known Issues/Future Improvements

