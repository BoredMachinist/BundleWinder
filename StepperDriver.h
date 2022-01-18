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
    
    void setPPR(int ppr);

    void setup();
    void toggle();

    void setTarget(double rpm, double revolutions);
    void startMotor();

    void stopMotor();

    void updateMotor();

  private:

    StepperDriver();
    static StepperDriver* ourInstance;

    int m_pulsePerRevolution;
    int m_pulsePerSecond;

    unsigned int m_target_speed; // Time between pulses measured in 500nS clock cycles
    unsigned int m_target_count;

    bool m_pulse;
    unsigned int m_current_speed;
    unsigned int m_current_count;

    int startTime;
  
    byte driver1Pulse;
    byte driver1Dir;
    byte driver1Enable;

    byte driver2Pulse;
    byte driver2Enable;
  
};




#endif
