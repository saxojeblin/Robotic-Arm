# Robotic-Arm
Program and models for an Arduino controlled robotic arm comprised of servo motors and 3D printed parts.

## How It Works
### Mechanical Work
-This project is an arm that is outfitted with a gripping mechanism to pick up and drop objects.
The arm is controlled by 6 servo motors: Waist, Shoulder, Elbow, Wrist, GripperRotate and GripperClose.
Each motor is responsible for a joint, and the arm is assembled together using custom made 3D printed 
parts. All part models were made by me using Fusion 360.

### Electrical Work
-The Servos are hooked up to a PCA9685 servo motor driver. The motor driver is connected to a breadboard, 
which is connected to the Arduino. The system is powered by a 5V 2.2A battery to give enough current to 
the motors for optimal performance.

### Software Work
-The program, which is in 'RoboticArmProject.ino', controls the arm to complete a cycle of picking up an 
object and dropping it in a different location. More information can be found with in the file throughout
the comments.
