//Created by: Wojciech Boncela

#include "MPU9250.h"
#include <Servo.h>

#define MIN_PWM 105.0
#define VEL_COEFFICIENT 355.0

#define CHANNEL_1_FORWARD 8
#define CHANNEL_1_BACKWARD 4
#define CHANNEL_2_FORWARD 7
#define CHANNEL_2_BACKWARD 2
#define PWM_1 5
#define PWM_2 6

#define SERVO 3
#define POSITION_FWD 0
#define POSTION_MID 90
#define POSITON_BWD 180

enum Direction{
  FORWARD,
  BACKWARD
};

MPU9250 imu(Wire,0x68);
Servo dir_servo;

int speed = 0;
float data = 0.0;

void setDriveDirection(Direction);

void setup() {

  Serial.begin(9600);
  
  dir_servo.attach(SERVO);
  imu.setAccelRange(MPU9250::ACCEL_RANGE_2G);
  if(imu.begin() < 0){
    Serial.println("Gyroscope initalization failed");
  }
  
  //CHANNEL 1 INPUT
  pinMode(CHANNEL_1_FORWARD,OUTPUT);
  pinMode(CHANNEL_1_BACKWARD,OUTPUT);
  //CHANNEL 2 INPUT
  pinMode(CHANNEL_2_FORWARD,OUTPUT);
  pinMode(CHANNEL_2_BACKWARD,OUTPUT);
  //PWM 1, 2
  pinMode(PWM_1 ,OUTPUT);
  pinMode(PWM_2 ,OUTPUT);

  digitalWrite(CHANNEL_1_FORWARD,HIGH);
  digitalWrite(CHANNEL_2_FORWARD,HIGH);
}

void loop() {

  imu.readSensor();
  data = imu.getAccelY_mss();
  if(data >= 0){
    setDriveDirection(Direction::FORWARD);
  }
  else{
    setDriveDirection(Direction::BACKWARD);
  }

  speed = abs(data)/10.0 * (VEL_COEFFICIENT - MIN_PWM) + MIN_PWM;
  Serial.println(speed);
  if(speed <= 255){
    analogWrite(PWM_1,speed);
    analogWrite(PWM_2,speed);
  }
  else{
    analogWrite(PWM_1,255);
    analogWrite(PWM_2,255);
  }
}

void setDriveDirection(Direction dir){
  switch(dir){
    case FORWARD:
      digitalWrite(CHANNEL_1_FORWARD,HIGH);
      digitalWrite(CHANNEL_2_FORWARD,HIGH);
      digitalWrite(CHANNEL_1_BACKWARD,LOW);
      digitalWrite(CHANNEL_2_BACKWARD,LOW);
      break;
    case BACKWARD:
      digitalWrite(CHANNEL_1_BACKWARD,HIGH);
      digitalWrite(CHANNEL_2_BACKWARD,HIGH);
      digitalWrite(CHANNEL_1_FORWARD,LOW);
      digitalWrite(CHANNEL_2_FORWARD,LOW);
      break;
  }
}
