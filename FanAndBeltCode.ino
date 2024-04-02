// MSE 2202 MSEBot base code for Lab 3
// Language: Arduino
// Authors: Eugen Porter and Michael Naish

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_TCS34725.h>

// Function declarations
void setMotor(int dir, int pwm, int in1, int in2);
void encoderISR();

// Port pin constants
#define LEFT_MOTOR_A        35
#define LEFT_MOTOR_B        36
#define RIGHT_MOTOR_A       37
#define RIGHT_MOTOR_B       38
#define ENCODER_LEFT_A      15
#define ENCODER_LEFT_B      16
#define ENCODER_RIGHT_A     11
#define ENCODER_RIGHT_B     12

// Constants
const int cPWMRes = 8;
const int cPWMFreq = 20000;
const int cNumMotors = 2;
const int cIN1Pin[] = {LEFT_MOTOR_A, RIGHT_MOTOR_A};
const int cIN1Chan[] = {0, 1};
const int c2IN2Pin[] = {LEFT_MOTOR_B, RIGHT_MOTOR_B};
const int cIN2Chan[] = {2, 3};

// Variables
unsigned char driveSpeed;
unsigned char driveIndex;

// Encoder structure
struct Encoder {
   const int chanA;
   const int chanB;
   long pos;
} encoder[] = {{ENCODER_LEFT_A, ENCODER_LEFT_B, 0}, {ENCODER_RIGHT_A, ENCODER_RIGHT_B, 0}};

void setup() {
   for (int k = 0; k < cNumMotors; k++) {
      ledcAttachPin(cIN1Pin[k], cIN1Chan[k]);
      ledcSetup(cIN1Chan[k], cPWMFreq, cPWMRes);
      ledcAttachPin(c2IN2Pin[k], cIN2Chan[k]);
      ledcSetup(cIN2Chan[k], cPWMFreq, cPWMRes);
      pinMode(encoder[k].chanA, INPUT);
      pinMode(encoder[k].chanB, INPUT);
      attachInterrupt(digitalPinToInterrupt(encoder[k].chanA), encoderISR, RISING);
   }
}

void loop() {
   // constantly spin motors
   setMotor(-1, 245, cIN1Chan[0], cIN2Chan[0]);
   setMotor(1, 245, cIN1Chan[1], cIN2Chan[1]);
}

// send motor control signals
void setMotor(int dir, int pwm, int in1, int in2) {
   if (dir == 1) {
      ledcWrite(in1, pwm);
      ledcWrite(in2, 0);
   }
   else if (dir == -1) {
      ledcWrite(in1, 0);
      ledcWrite(in2, pwm);
   }
   else {
      ledcWrite(in1, 0);
      ledcWrite(in2, 0);
   }
}

// encoder interrupt service routine
void encoderISR() {
   for (int k = 0; k < cNumMotors; k++) {
      int b = digitalRead(encoder[k].chanB);
      if (b > 0) {
         encoder[k].pos++;
      }
      else {
         encoder[k].pos--;
      }
   }
}
