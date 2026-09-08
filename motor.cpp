#include "motor.h"

ExpAccelStepper::ExpAccelStepper(uint8_t step_pin, uint8_t dir_pin, int stepsPerRev)
                : AccelStepper(AccelStepper::DRIVER, step_pin, dir_pin),
                stepsPerRev(stepsPerRev)
{

}

//-------------

void ExpAccelStepper::moveDeg(float Deg)
{
  move(lround(Deg/360 * stepsPerRev));
}

//-------------

void ExpAccelStepper::moveToDegFancy(float Deg)
{
   // Normalize target to [0, 360)
    Deg = fmod(Deg, 360.0f);
    if (Deg < 0)
        Deg += 360.0f;

    // Current angle in degrees
    long currentSteps = currentPosition();
    float currentDeg = fmod((currentSteps * 360.0f) / stepsPerRev, 360.0f);
    if (currentDeg < 0)
        currentDeg += 360.0f;

    // Smallest signed angle difference (-180, 180]
    float delta = Deg - currentDeg;
    if (delta > 180.0f)
        delta -= 360.0f;
    else if (delta <= -180.0f)
        delta += 360.0f;

    // Move relative by the required amount
    move(lround(delta * stepsPerRev / 360.0f));
}

//-------------

void ExpAccelStepper::moveToDeg(float Deg)
{
  moveTo(lround(Deg/360 * stepsPerRev));
}

//-------------

void ExpAccelStepper::moveRevs(float Revs)
{
  move(lround(Revs * stepsPerRev));
}

//-------------

void ExpAccelStepper::moveToRevs(float Revs)
{
    moveTo(lround(Revs * stepsPerRev));
}

//-------------

void ExpAccelStepper::setMaxRPM(float RPM)
{
  float stepspersec = RPM * stepsPerRev / 60;
  setMaxSpeed(stepspersec);
}

//-------------

void ExpAccelStepper::setStepsPerRev(int steps)
{
  stepsPerRev = steps;
}