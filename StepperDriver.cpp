#include "StepperDriver.h"
#include <Arduino.h>


#define WINDER_STEPS_PER_LOOP 10500
#define DEFAULT_WINDER_LOOPS 400
#define WINDER_MAX_SPEED 400
#define WINDER_ACCELERATION 50.0

#define GREASER_ACCELERATION 50.0

// Create the motor shield object with the default I2C address
    Adafruit_MotorShield AFMS;
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

    // Connect a stepper motor with 200 steps per revolution (1.8 degree)
    // to motor port #2 (M3 and M4)
    Adafruit_StepperMotor *myStepper1 = AFMS.getStepper(200,1);

    Adafruit_StepperMotor *myStepper2 = AFMS.getStepper(200,2);

    void forwardstep1() {
      myStepper1->quickstep(FORWARD);
    }
    void backwardstep1() {
      myStepper1->quickstep(BACKWARD);
    }
    void forwardstep2() {
      myStepper2->quickstep(FORWARD);
    }
    void backwardstep2() {
      myStepper2->quickstep(BACKWARD);
    }


StepperDriver* StepperDriver::ourInstance = nullptr;


StepperDriver::StepperDriver():
  windMotor(forwardstep2, backwardstep2),
  greaseMotor(forwardstep1, backwardstep1)
  {

  setup();
}


void StepperDriver::setTarget(int speed, int ratio, int wraps) {
  windMotor.setMaxSpeed(speed);
  greaseMotor.setMaxSpeed(speed/ratio);

  DisplayManager::getInstance()->print("targeted" + (String)wraps);
  windMotor.moveTo((long)(wraps) * (long)WINDER_STEPS_PER_LOOP);
  greaseMotor.moveTo((long)(wraps) * (long)WINDER_STEPS_PER_LOOP);
  
 
}

void StepperDriver::setup() {

  AFMS.begin();

  myStepper1->onestep(FORWARD,DOUBLE);
  myStepper2->onestep(FORWARD,DOUBLE);
  
  windMotor.setAcceleration(WINDER_ACCELERATION);
  greaseMotor.setAcceleration(GREASER_ACCELERATION);
}

void StepperDriver::startMotor() {
  
}

void StepperDriver::stopMotor() {
  windMotor.stop();
  greaseMotor.stop();
}

void StepperDriver::updateMotor() {
  windMotor.run();
  greaseMotor.run();
}

boolean StepperDriver::isWinding() {
  Serial.println(windMotor.distanceToGo());
  return windMotor.distanceToGo() != 0;
}
