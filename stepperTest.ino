#include "StepperDriver.h"
#include "StateMachine.h"
#include "Hardware.h"

                                                                                                    

void setup() {
  StepperDriver::init();
  Serial.begin(9600);
  // put your setup code here, to run once:

  
  DisplayManager::init();
  ButtonManager::init();

  BundleManager::init();
  
  StateManager::init();

  
  
  
//126 links 
//9.269

   //stepper.startMotor();
}

void loop() {
  // put your main code here, to run repeatedly:

  ButtonManager::getInstance()->update();
  StateManager::getInstance()->updateStates();
  
}

ISR(TIMER1_COMPA_vect)
{
    DisplayManager::getInstance()->print("A");
    StepperDriver::getInstance()->toggle();
    DisplayManager::getInstance()->print("B");
   
}//ISR(TIMER1_COMPA_vect)
