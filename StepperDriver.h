#include <AccelStepper.h>
#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>
#include "Hardware.h"


#ifndef H_STEPPERDRIVER
#define H_STEPPERDRIVER

#include<arduino.h>


class StepperDriver {
  public:

    static StepperDriver* getInstance() {
      return ourInstance;
    }
    
    static void init() {
      ourInstance = new StepperDriver();
    }
    

    void setup();

    void setTarget(int speed, int ratio, int wraps);
    void startMotor();

    void stopMotor();

    void updateMotor();

    bool isWinding();

  private:

    StepperDriver();
    static StepperDriver* ourInstance;

   

    unsigned int m_target_speed; // Time between pulses measured in 500nS clock cycles
    unsigned int m_target_wraps;


    


    AccelStepper windMotor;
    AccelStepper greaseMotor;


    
  
};






#endif
