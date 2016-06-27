#include <Servo.h>

int throttle, yaw, modifyer = 1000;
float yaw_pct;
byte throttle_pin = 2;
byte yaw_pin = 3;
byte l_motor_pin = 9;
byte r_motor_pin = 10;

int reverse_yaw = -1; // 1 or -1 to reverse the yaw channel
int yaw_deadband = 20; // ignore this much jitter on yaw channel

Servo l_motor, r_motor;


void setup() {
  // put your setup code here, to run once:

  pinMode(throttle_pin, INPUT);
  pinMode(yaw_pin, INPUT);

  l_motor.attach(l_motor_pin);
  r_motor.attach(r_motor_pin);
  
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
//  delay(100);
  
  // read input pwm from throttle and yaw
  throttle = pulseIn(throttle_pin, HIGH);
  yaw = pulseIn(yaw_pin,HIGH);

  
  if(yaw > 1500 + yaw_deadband || yaw < 1500 - yaw_deadband){
    yaw_pct = ((yaw-1500)/(float)500) * reverse_yaw;
  }else{
    yaw_pct = 0;
  }
  
  // calculate transformation
  modifyer = abs(yaw_pct) * (throttle - 1000);
//  Serial.println("yaw pct"+(String) yaw_pct);
  
  // calculate & write left and right motor mixed throttle/yaw values
  if(yaw_pct < 0){
    l_motor.writeMicroseconds(throttle - modifyer);
    r_motor.writeMicroseconds(throttle);
//    Serial.println("yaw left: "+ (String) (modifyer));
  }else{
    l_motor.writeMicroseconds(throttle);
    r_motor.writeMicroseconds(throttle - modifyer);
//    Serial.println("yaw right: "+ (String) (modifyer));
  }

  String t;
  Serial.println("T: "+(String) throttle + "  Y: "+ (String) yaw + "  LM: " + (String) l_motor.readMicroseconds() + "  RM: "+ r_motor.readMicroseconds());
}




