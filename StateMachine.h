

#ifndef H_STATEMACHINE
#define H_STATEMACHINE

#include <StackArray.h>
#include "Hardware.h"
#include "BundleManager.h"
#include "StepperDriver.h"


class State {
  public:
    virtual void onEntry() = 0;
    virtual void onUpdate() = 0;
    virtual void onExit() = 0;
    virtual bool shouldExit() = 0;

  
};

class MenuState: public State {
  public:
    MenuState();
    ~MenuState();
    void onEntry();
    void onUpdate();
    void onExit();
    bool shouldExit();

 private:
    enum MenuItem {
      INVALID = -1,
      START = 0,
      SPEED = 1,
      WRAPS = 2,
      RATIO = 3,
      RESET = 4,
      TOTAL = 5
    };

    MenuItem currentItem;
    void updateDisplay();
};

class CounterState : public State {
  public:
    void onEntry();
    void onUpdate();
    void onExit();
    bool shouldExit();

    CounterState(String m_varName, int startVal, int scale, IntSetCallback* setFunc);
    ~CounterState();

  private:
    int m_holdStart;
    bool m_isCountingUp;
    int m_nextTick;
  
    String m_varName;
    int m_var;
    int m_useScaling;
    IntSetCallback* m_setFunc;

    void updateDisplay();

    static int* scalingRate;
    
};

class WindingState : public State {
  public:
    void onEntry();
    void onUpdate();
    void onExit();
    bool shouldExit();

    WindingState();
};

class StateManager {
  public:
    static StateManager* getInstance() {
      return ourInstance;
    }
    
    static void init() {
      menuState = new MenuState();
      ourInstance = new StateManager();
    }

    ~StateManager();

    void pushState(State* newState);
    void popState();

    void updateStates();

  private:
    StateManager();
    static StateManager* ourInstance;

    StackArray <State*> stateStack;

    static MenuState* menuState;
};


#endif
