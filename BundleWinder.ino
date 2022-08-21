#include "StepperDriver.h"
#include "StateMachine.h"
#include "Hardware.h"

                                                                                                    

void setup() {
  delay(1000);
  Serial.begin(115200);
  // put your setup code here, to run once:

  Serial.println("Hello Mother Hello Father");
  delay(500);
  DisplayManager::init();
  Serial.println("Here I am at ");
  ButtonManager::init();

  BundleManager::init();
  Serial.println("Camp Granada");
  StateManager::init();

  StepperDriver::init();
  
  
  
//126 links 
//9.269

   //stepper.startMotor();
}

void loop() {
  // put your main code here, to run repeatedly:

  ButtonManager::getInstance()->update();
 
  StateManager::getInstance()->updateStates();

   DisplayManager::getInstance()->update();
  
   
  //StepperDriver::getInstance()->updateMotor();
}
