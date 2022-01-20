#include "StateMachine.h"
#include <math.h>

StateManager* StateManager::ourInstance = nullptr;
MenuState* StateManager::menuState = nullptr;

StateManager::StateManager() :
  stateStack()
  {
    stateStack.push(menuState);
    menuState->onEntry();
  
  
}

void StateManager::updateStates() {
  if (stateStack.peek()->shouldExit()) {
    stateStack.pop()->onExit();
    stateStack.peek()->onEntry();
  }
  
  stateStack.peek()->onUpdate();

  
}

void StateManager::pushState(State* newState) {
  stateStack.peek()->onExit();
  stateStack.push(newState);
  newState->onEntry();
}


// MenuState

MenuState::MenuState() {
  
}

void MenuState::onEntry() {
  currentItem = START;
  updateDisplay();
}

void MenuState::onUpdate() {
  if (ButtonManager::getInstance()->upReleased()) {
    currentItem = static_cast<MenuItem>(static_cast<int>(currentItem) - 1);
    if (currentItem == INVALID) {
      currentItem = TOTAL - 1;
    }
    updateDisplay();
  }

  if (ButtonManager::getInstance()->downReleased()) {
    currentItem = static_cast<MenuItem>(static_cast<int>(currentItem) + 1);
    if (currentItem == TOTAL) {
      currentItem = INVALID + 1;
    }
    updateDisplay();
  }

  if (ButtonManager::getInstance()->selectReleased()) {
    switch(currentItem) {
      case START:
        StateManager::getInstance()->pushState(new WindingState());
        break; 
      case SPEED:
        StateManager::getInstance()->pushState(new CounterState(String("Speed"), BundleManager::getInstance()->getBundle()->getTargetSpeed(), 3, SetSpeedCallback::instance()));
        break;
      case WRAPS:
        StateManager::getInstance()->pushState(new CounterState(String("Wraps"), BundleManager::getInstance()->getBundle()->getTargetWraps(), 5, SetWrapsCallback::instance()));
        break;
      case RATIO:
        StateManager::getInstance()->pushState(new CounterState(String("Ratio"), BundleManager::getInstance()->getBundle()->getRatio(), 0, SetRatioCallback::instance()));
        break;
      case RESET:
        BundleManager::getInstance()->defaultBundle();
        currentItem = START;
        updateDisplay();
    }
  }
}

void MenuState::onExit() {
  
}

bool MenuState::shouldExit() {
  return false;
}

void MenuState::updateDisplay() {
  DisplayManager::getInstance()->clear();
  DisplayManager::getInstance()->printMenuArrow();
  DisplayManager::getInstance()->print(" ");
  switch(currentItem) {
    case START:
      DisplayManager::getInstance()->print("Start");
      break; 
    case SPEED:
      DisplayManager::getInstance()->print("Speed = " + String(BundleManager::getInstance()->getBundle()->getTargetSpeed()));
      break;
    case WRAPS:
      DisplayManager::getInstance()->print("Wraps = " + String(BundleManager::getInstance()->getBundle()->getTargetWraps()));
      break;
    case RATIO:
      DisplayManager::getInstance()->print("Ratio = " + String(BundleManager::getInstance()->getBundle()->getRatio()));
      break;
    case RESET:
      DisplayManager::getInstance()->print("Reset?");
      break;
  }
  
}

#define COUNTER_RATE 500
#define COUNTER_INCREMENT 3000

static const int scalingRateData[] = {1, 5, 10, 25, 100, 200};
int* CounterState::scalingRate = scalingRateData;

CounterState::CounterState(String varName, int startVal, int scale, IntSetCallback* setFunc) :
    m_varName(varName),
    m_var(startVal),
    m_useScaling(scale),
    m_setFunc(setFunc),
    m_holdStart(-1)
{
  
}

void CounterState::onEntry() {
  updateDisplay();
}

void CounterState::onUpdate() {
  if(ButtonManager::getInstance()->upPressed()) {
    m_holdStart = millis();
    m_isCountingUp = true;
    m_nextTick = m_holdStart + COUNTER_RATE;
  } else if (ButtonManager::getInstance()->downPressed()) {
    m_holdStart = millis();
    m_isCountingUp = false;
    m_nextTick = m_holdStart + COUNTER_RATE;
  }

  if (m_holdStart != -1) {
    if (ButtonManager::getInstance()->upReleased() || ButtonManager::getInstance()->downReleased()) {

      if (m_nextTick == m_holdStart + COUNTER_RATE) {
        if (m_isCountingUp) {
          m_var += 1;
        } else {
          m_var -= 1;
        }
        updateDisplay();
      }
      
      m_holdStart = -1;
    } else {
      int now = millis();
      if (now >= m_nextTick) {
        m_nextTick += COUNTER_RATE;

        int index = floor(((float)now - (float)m_holdStart)/(float)COUNTER_INCREMENT);
        int increment = scalingRate[min(m_useScaling,index)];
        if (m_isCountingUp) {
          m_var += increment;
        } else {
          m_var -= increment;
        }
        
        updateDisplay();
      }
    }
  }
}

void CounterState::onExit() {
  m_setFunc->set(m_var);
}

bool CounterState::shouldExit() {
  return ButtonManager::getInstance()->selectReleased();
}

void CounterState::updateDisplay() {
  DisplayManager::getInstance()->clear();
  DisplayManager::getInstance()->print("Edit: ");
  DisplayManager::getInstance()->cursor(0,1);
  DisplayManager::getInstance()->print(m_varName + ": " + String(m_var) + " ");
  DisplayManager::getInstance()->printMenuArrow();
}


/// Winding State

WindingState::WindingState() {
  
}

void WindingState::onEntry() {

  DisplayManager::getInstance()->clear();
  DisplayManager::getInstance()->print("Winding");

  Bundle* bundle = BundleManager::getInstance()->getBundle();
  DisplayManager::getInstance()->print("1");
  StepperDriver::getInstance()->setTarget(bundle->getTargetSpeed(), bundle->getTargetSpeed());
  DisplayManager::getInstance()->print("2");
  StepperDriver::getInstance()->startMotor();
  DisplayManager::getInstance()->print("3");
}

void WindingState::onUpdate() {
  StepperDriver::getInstance()->updateMotor();
}

void WindingState::onExit() {
  StepperDriver::getInstance()->stopMotor();
}

bool WindingState::shouldExit() {
  return ButtonManager::getInstance()->selectReleased();
}
