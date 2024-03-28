#include <MiniPID.h>
#include <PPMReader.h>
#include <elapsedMillis.h>
#include <ESP32Servo.h>
#include <Encoder.h>

//Pins on arduino
#define RC_INPUT_PIN 1//Placeholder
#define LEFT_DRIVE_MOTOR_PIN 2//Placeholder
#define LEFT_ENCODER_PIN_1 10// placeholder
#define LEFT_ENCODER_PIN_2 11// placeholder
#define FORWARD_CHANNEL 1//placeholder
#define TURN_CHANNEL 2//placeholder
//PPM is on range of 1000-2000, 1000 full reverse 2000 full forwards
#define NEUTRAL_OUTPUT 1500
#define FORWARD_OUTPUT 500
#define REVERSE_OUTPUT -500

//PID
MiniPID leftDrivePID = MiniPID(1,0,0);//TUNE

//RC Input, takes in two channels from transmitter
PPMReader leftDriveInput(RC_INPUT_PIN,2);//Adjust second term upwards if you want more channels, only forward + turn needed currently

//Motor
Servo leftDriveMotor;
double leftMotorSpeed;

//Encoder
Encoder leftDriveEncoder(LEFT_ENCODER_PIN_1,LEFT_ENCODER_PIN_2);//Placeholder pins, need to be interrupt

void setup() {
  //Debug usb setup
  Serial.begin(9600);

  //Setup motor
  leftDriveMotor.attach(LEFT_DRIVE_MOTOR_PIN);
  leftDriveMotor.write(NEUTRAL_OUTPUT);
}

void loop() {
  double forwardInput = leftDriveInput.latestValidChannelValue(FORWARD_CHANNEL,NEUTRAL_OUTPUT);
  double turnInput = leftDriveInput.latestValidChannelValue(TURN_CHANNEL,NEUTRAL_OUTPUT);//placeholder channels

  //Account for out of bound inputs and convert turning
  forwardInput = map(forwardInput,1000,2000,1000,2000);//Flip second set to invert direction, not negative lmao
  turnInput = map(turnInput,1000,2000,-500,500);
  leftMotorSpeed= forwardInput + turnInput;//Flip sign to invert, positive on left negative on right

  


  double target =leftMotorSpeed; 
  double position = leftDriveEncoder.read();
  long double output = NEUTRAL_OUTPUT + leftDrivePID.getOutput(position,target);
  leftDriveMotor.writeMicroseconds(output);
}
