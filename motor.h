#pragma once
#include <AccelStepper.h>
#include <Arduino.h>
class ExpAccelStepper: public AccelStepper{
  public:
  //constructor
  ExpAccelStepper(uint8_t step_pin, 
                  uint8_t dir_pin, 
                  int StepsPerRev = 1600);


  //motion conversion
  void moveDeg(float Deg);
  void moveToDeg(float Deg);
  void moveToDegFancy(float Deg);
  
  void moveRevs(float Revs);
  void moveToRevs(float Revs);
  
  //settings

  void setMaxRPM(float RPM);
  //void setAngleOverflow(bool ...) //wether or not the motor is allowed to rotate a full rotation from 0 when using moveToDeg()
  void setStepsPerRev(int steps);

  private:
  
  int stepsPerRev;
};