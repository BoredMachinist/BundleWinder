#include "StepperDriver.h"
#include "StateMachine.h"
#include "Hardware.h"

                                                                                                    

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

  
  DisplayManager::init();
  ButtonManager::init();

  BundleManager::init();
  
  StateManager::init();
  Serial.println(4);
  
  
//126 links 
//9.269

   //stepper.startMotor();
}

void loop() {
  // put your main code here, to run repeatedly:


  StateManager::getInstance()->updateStates();
  ButtonManager::getInstance()->update();
}

ISR(TIMER1_COMPA_vect)
{
    StepperDriver::getInstance()->toggle();
   
}//ISR(TIMER1_COMPA_vect)
