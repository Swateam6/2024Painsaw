#include <MiniPID.h>
#include <PPMReader.h>
#include <elapsedMillis.h>
#include <ESP32Servo.h>
#include <Encoder.h>

//Pins on arduino
#define RC_INPUT_PIN 1//Placeholder
#define RIGHT_DRIVE_MOTOR_PIN 2//Placeholder
#define RIGHT_ENCODER_PIN_1 10// placeholder
#define RIGHT_ENCODER_PIN_2 11// placeholder
#define FORWARD_CHANNEL 1//placeholder
#define TURN_CHANNEL 2//placeholder
//PPM is on range of 1000-2000, 1000 full reverse 2000 full forwards
#define NEUTRAL_OUTPUT 1500
#define FORWARD_OUTPUT 500
#define REVERSE_OUTPUT -500

//PID
MiniPID rightDrivePID = MiniPID(1,0,0);//TUNE

//RC Input, takes in two channels from transmitter
PPMReader rightDriveInput(RC_INPUT_PIN,2);//Adjust second term upwards if you want more channels, only forward + turn needed currently

//Motor
Servo rightDriveMotor;
double rightMotorSpeed;

//Encoder
Encoder rightDriveEncoder(RIGHT_ENCODER_PIN_1,RIGHT_ENCODER_PIN_2);//Placeholder pins, need to be interrupt

void setup() {
  //Debug usb setup
  Serial.begin(9600);

  //Setup motor
  rightDriveMotor.attach(RIGHT_DRIVE_MOTOR_PIN);
  rightDriveMotor.write(NEUTRAL_OUTPUT);
}

void loop() {
  double forwardInput = rightDriveInput
.latestValidChannelValue(FORWARD_CHANNEL,NEUTRAL_OUTPUT);
  double turnInput = rightDriveInput
.latestValidChannelValue(TURN_CHANNEL,NEUTRAL_OUTPUT);//placeholder channels
  
 

  //Account for out of bound inputs and convert turning
  forwardInput = map(forwardInput,1000,2000,1000,2000);//Flip second set to invert direction, not negative lmao
  turnInput = map(turnInput,1000,2000,-500,500);
  rightMotorSpeed= forwardInput - turnInput;//Flip sign to invert, positive on left negative on right

  


  double target =rightMotorSpeed; 
  double position = rightDriveEncoder.read();
  long double output = NEUTRAL_OUTPUT + rightDrivePID.getOutput(position,target);
  rightDriveMotor.writeMicroseconds(output);
}
