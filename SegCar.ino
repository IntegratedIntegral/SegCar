#include "MPU9250.h"

#define MIN_PWM 85.0
#define CHANNEL_1_FORWARD 7
#define CHANNEL_1_BACKWARD 8
#define CHANNEL_2_FORWARD 2
#define CHANNEL_2_BACKWARD 4
#define PWM_1 5
#define PWM_2 6

enum Direction{
  FORWARD,
  BACKWARD
};

MPU9250 imu(Wire,0x68);

int speed = 0;
float data = 0.0;

void setDriveDirection(Direction);

void setup() {

  Serial.begin(9600);

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

  speed = abs(data)/10.0 * (255.0 - MIN_PWM) + MIN_PWM;

  Serial.println(speed);
  
  analogWrite(PWM_1,speed);
  analogWrite(PWM_2,speed);
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
