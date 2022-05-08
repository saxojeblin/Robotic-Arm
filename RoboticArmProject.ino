/* ---------------------------------------------
 * |                ROBOTIC ARM                |
 * |           Author: Nicholas Rubio          |
 * |              Date: 05/07/2022             |
 * --------------------------------------------- */

/*
 * This is the code for simple object movement for my Arudino controlled robotic arm.
 * The arm is controlled by 6 servo motors: Waist, Shoulder, Elbow, Wrist, GripperRotate and GripperClose.
 * Each motor is responsible for a joint, and the arm is assembled together using custom
 * made 3D printed parts. All part models were made by me using Fusion 360.
 * 
 * The Servos are hooked up to a servo motor driver. The motor driver is connected to a 
 * breadboard, which is connected to the Arduino.
 * I am using a 5V 2.2A battery to power the motor driver to give enough current to the 
 * motors for optimal performance.
 */

/* I'm using a PCA9685 servo driver to drive the motors. In order to program the motor driver,
 * I will be using the HCPA9685.h library. Credit for the library goes to the maker.
*/

#include "HCPCA9685.h"
#define  I2CAdd 0x40

/* We first want to establish each motor's number label and its limits of rotation.
 * Below is the reference number and maximum/minimum positions for each servo motor. 
 * It is important to note that the servos rotate based on a given position, not an angle.
*/

int waist = 0;
int waistMin = 50;
int waistMax = 450;

int shoulder = 1;
int shoulderMin = 170;
int shoulderMax = 450;

int elbow = 2;
int elbowMin = 50;
int elbowMax = 450;

int wrist = 3;
int wristMin = 50;
int wristMax = 450;

int gripperRotate = 4;
int gripperRotateMin = 50;
int gripperRotateMax = 350;

int gripperClose = 5;
int gripClose = 170;
int gripOpen = 300;

/* We also want to declare all the pick up/drop off positions for each motor during
 * the movement actions. The positions are all fixed values. */
int waistCenter = 270;
int shoulderCenter = 450;
int elbowCenter = 150;
int wristCenter = 250;
int gripperRotateCenter = 150;

int waistPickUp = waistCenter;
int shoulderPickUp = 350;
int elbowPickUp = 165;
int wristPickUp = wristCenter;
int gripperRotatePickUp = 200;

int waistDropOff = 460;
int shoulderDropOff = 350;
int elbowDropOff = 130;
int wristDropOff = wristCenter;
int gripperRotateDropOff = 220;

/* Lastly, we need to create an obeject of HCPCA9685 */
 
HCPCA9685 HCPCA9685(I2CAdd);

/*  The set up is fairly simple.
 *  We want to initialize the HCPA9685 library and turn on the motor driver.
 *  We also start the serial monitor in case we want to display motor information.
 *  Finally, we should orient the arm to be in its default position.
 */

void setup() 
{
  Serial.begin(9600);
  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false);
  defaultPosition();
}

/* This program is designed to pick up a small object and drop it into a container.
 * Because there are currently no sensors on the robot, the object and container
 * need to be in fixed locations that the robot knows, which have already been declared
 * from the dropOff/pickUp variables. 
 * 
 * The movement cycles as follows:
 * (Start) Center Position -> Move and Pick Up Object -> Return to Center -> Move and Drop Off Object -> Return to Center (End)
 * 
 * The movements to and from the center are nice because it makes the robot move much 
 * more methodically and keeps the movements gentle enough so that there aren't any 
 * jerks that could lead to the object being dropped.
 * 
 * Throughout the movements, there are also a few gradual changes to the desired positions,
 * because the leverage of the arm can sometimes be too great for the shoulder joint to handle
 * reliably.
 */
 

void loop() 
{
  pickUpObject();
  delay(100);
  returnToCenter(0);
  delay(100);
  dropOffObject();
  returnToCenter(1);
  delay(2000);
}

/* Function to turn all motors to default position from start up. */
void defaultPosition() {
  HCPCA9685.Servo(waist, waistCenter);
  delay(200);
  HCPCA9685.Servo(shoulder, shoulderCenter);
  delay(200);
  HCPCA9685.Servo(elbow, elbowCenter);
  delay(200);
  HCPCA9685.Servo(wrist, wristCenter);
  delay(200);
  HCPCA9685.Servo(gripperRotate, gripperRotateCenter);
  delay(200);
  HCPCA9685.Servo(gripperClose, gripOpen);
  delay(1000);
}

/* Function to move a joint from a given starting position to a given ending position. */
void moveJoint(int joint, int startPos, int endPos) {
  if (startPos < endPos) {
    for (int i = startPos; i <= endPos; i++) {
        HCPCA9685.Servo(joint, i);
        delay(10);
    }
  }
  else if (startPos > endPos) {
    for (int i = startPos; i >= endPos; i--) {
        HCPCA9685.Servo(joint, i);
        delay(10);
    }
  }
  else {
    HCPCA9685.Servo(joint, endPos);
  }
  delay(100);
}

/* Function to open and close the gripping mechanism, depending on parameter given. */
void openCloseGrip(int funct) {
  //We are going to open the grip
  int i;
  if (funct == 0) {
    for (i=gripClose; i < gripOpen; i++) {
      HCPCA9685.Servo(gripperClose, i);
      delay(10);
    }
  }
  //We are going to close the grip
  else if (funct == 1) {
    for (i=gripOpen; i > gripClose; i--) {
      HCPCA9685.Servo(gripperClose, i);
      delay(10);
    }
  }
}

/* Function for moving the arm to pick up the object, starting from the center/default position. */
void pickUpObject() {
  moveJoint(shoulder, shoulderCenter, 400);
  delay(100);
  moveJoint(gripperRotate, gripperRotateCenter, gripperRotatePickUp);
  delay(100);
  moveJoint(elbow, elbowCenter, elbowPickUp);
  delay(100);
  moveJoint(shoulder, 400, shoulderPickUp);
  delay(100);
  openCloseGrip(1);
}

/* Function for moving the arm to drop off the object, starting from center */
void dropOffObject() {
  moveJoint(waist, waistCenter, waistDropOff);
  delay(100);
  moveJoint(shoulder, shoulderCenter, 400);
  delay(100);
  moveJoint(elbow, elbowCenter, elbowDropOff);
  delay(100);
  moveJoint(shoulder, 400, shoulderDropOff); 
  delay(100);
  moveJoint(gripperRotate, gripperRotateCenter, gripperRotateDropOff);
  delay(100);
  openCloseGrip(0);
}

/* Function to return the arm to its center postiion 
 * In between the pick up/drop off steps, we want to return the arm
 * to its center postiion for smoother, consistent movement. Action is
 * determined from command parameter.
 */
void returnToCenter(int command) {
  //If we are returning from the pick up
  if(command == 0) {
    moveJoint(gripperRotate, gripperRotatePickUp, 300);
    delay(100);
    moveJoint(elbow, elbowPickUp, 200);
    delay(100);
    moveJoint(shoulder, shoulderPickUp, shoulderCenter);
    delay(100);
    moveJoint(elbow, 200, elbowCenter);
    delay(100);
    moveJoint(gripperRotate, 300, gripperRotateCenter);
    delay(100);
    
  }
  //If we are returning from the drop off
  else if (command == 1) {
    moveJoint(elbow, elbowDropOff, 180);
    delay(100);
    moveJoint(gripperRotate, gripperRotateDropOff, 250);
    delay(100);
    moveJoint(shoulder, shoulderDropOff, shoulderCenter);
    delay(100);
    moveJoint(elbow, 180, elbowCenter);
    delay(100);
    moveJoint(gripperRotate, 250, gripperRotateCenter);
    delay(100);
    moveJoint(waist, waistDropOff, waistCenter);
    delay(100);
  }
}
