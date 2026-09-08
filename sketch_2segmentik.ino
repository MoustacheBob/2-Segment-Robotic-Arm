#include <math.h>
#include <Servo.h>
#include <ServoEasing.hpp>
#include "motor.h"

#define StepPin 8
#define DirPin 2
#define servo1pin 9
#define servo2pin 10

ExpAccelStepper stepper(StepPin, DirPin, 1600);
ServoEasing servo1;
ServoEasing servo2;

const int l1;
const int l2 ;
String response;

int x ;
int y ;

 // ------------------------ Ik Solver ---------

struct ikresult{
  float base;
  float theta1;
  float theta2;
  bool reachable;
};

ikresult solveik(float l1, float l2, float x1, float y, float z){
  ikresult result;

  float x = sqrt(x1*x1 + y*y);

  float base = atan2(y, x1);
  
  float r = sqrt(x*x + z*z);

  if(r > l1 + l2 || r < fabs(l1 - l2)){
    result.base = 0;
    result.theta1 = 0;
    result.theta2 = 0;
    result.reachable = false;
    return result;
  }
  
  result.reachable = true;

  float costheta2 = (x*x + z*z - l1*l1 -l2*l2) / (2 * l1 * l2);

  if (costheta2 > 1.0) costheta2 = 1.0;
  if (costheta2 < -1.0) costheta2 = -1.0;

  float theta2 = -acos(costheta2);

  float theta1 = atan2(z, x) - atan2(l2 * sin(theta2)     ,    l1 + l2 * cos(theta2));

  result.base = base;
  result.theta1 = theta1;
  result.theta2 = theta2;

  return result;
}


  


void setup() {
  // ------------------------ Motor Init ---------
  
  Serial.begin(9600);
  
  //servo setup
  stepper.setMaxSpeed(750);
  stepper.setAcceleration(250);

  servo1.attach(servo1pin);
  servo1.setSpeed(75);
  servo1.setEasingType(EASE_QUADRATIC_IN_OUT);
  servo1.write(0);

  servo2.attach(servo2pin);
  servo2.setSpeed(75);
  servo2.setEasingType(EASE_QUADRATIC_IN_OUT);
  servo2.write(0);

  delay(1000);
}

void loop() {
// ------------------------ Parsing input string ---------
  
  if (Serial.available()) {
  response = Serial.readStringUntil('\n');  // char terminator, not a string literal
  response.trim();

  int commaIndex1 = response.indexOf(',');
  int commaIndex2 = response.indexOf(',', commaIndex1 + 1);
  int commaIndex3 = response.indexOf(',', commaIndex2 + 1);
  int commaIndex4 = response.indexOf(',', commaIndex3 + 1);

  float l1 = response.substring(0, commaIndex1).toFloat();
  float l2 = response.substring(commaIndex1 + 1, commaIndex2).toFloat();
  float x  = response.substring(commaIndex2 + 1, commaIndex3).toFloat();
  float y  = response.substring(commaIndex3 + 1, commaIndex4).toFloat();
  float z  = response.substring(commaIndex4 + 1).toFloat();

  
//---------------------- calculating and moving ----------
  

  ikresult solved = solveik(l1,l2,x,y,z);
  
  float c = solved.base * (180/PI);
  float a = solved.theta1 * (180/PI);
  float b = solved.theta2 * (180/PI);
  
  
  Serial.print(c);
  Serial.print(" , ");
  Serial.print(a);
  Serial.print(" , ");
  Serial.print(b);
 
  servo1.easeTo(a);
  servo2.easeTo(static_cast<float>abs(b));
  
   
  stepper.moveToDeg(c);
  while(stepper.distanceToGo() != 0){
    stepper.run();
  } 
  delay(1000);
  

  }
}
