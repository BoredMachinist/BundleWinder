#include "StepperDriver.h"
#include <Arduino.h>


#define INIT_STEP_DELAY     10000           //#     uS = N*500nS    5mS initial step delay
#define STEP_PULSE_WIDTH    40             //#     uS = N*500nS    50uS step pulse width


#define ACCELERATION_TIME_MILLIS 8000
#define STARTING_TICKS 6500


StepperDriver* StepperDriver::ourInstance = nullptr;


StepperDriver::StepperDriver() {
  driver1Pulse = B00000001;
  driver1Dir = B00000010;
  driver1Enable = B00000100;
  
  driver2Pulse = B00001000;
  driver2Enable = B00010000;
}

void StepperDriver::setPPR(int ppr) {
  m_pulsePerRevolution = ppr;
}

void StepperDriver::setTarget(double rpm, double revolutions) {
  double RPS = rpm/60.0;
  unsigned int clicksPerSecond = RPS * m_pulsePerRevolution;


  unsigned int newSteps = 16000000/(8*clicksPerSecond) - 1;

  m_target_speed = newSteps; // PRoblems if this exceeds 65536. On 200 pulses per revolution this means no slower than 9rpma
  Serial.print("TargetSpeed");
  Serial.print(m_target_speed);
  m_current_speed = STARTING_TICKS;

  unsigned int nSteps = revolutions * m_pulsePerRevolution;

  m_target_count = nSteps;
  m_current_count = 0;
 
}

void StepperDriver::setup() {
  pinMode(8,OUTPUT);



  TCCR1A = 0; //WGM 0, OC1A disconnected from PB1 to start
  TCCR1B = 0;
  TIMSK1 = 0;

  //set timer 1 to tick at 2MHz
  TCCR1B = (1 << CS11);   ///8 = 2MHz clock rate (500nS per tick)



}

void StepperDriver::startMotor() {
  startTime = millis();
  
  noInterrupts();
  //grab current count and add an arbitrary count to give initial step delay
  OCR1A = TCNT1 + INIT_STEP_DELAY;

  //
    TCCR1A |= (1 << COM1A0 );        //connect pin PB1 to OC1A; set pin on next compare
    TIFR1 |= (1 << OCF1A);          //clear any pending interrupt
    TIMSK1 |= (1 << OCIE1A);         //enable OC1A interrupt
   

    PORTB |= 0b10000000;
    
    interrupts();
}

void StepperDriver::stopMotor() {
  noInterrupts();
    TCCR1A &= ~(1 << COM1A0 );        //connect pin PB1 to OC1A; set pin on next compare
    TIFR1 &= ~(1 << OCF1A);          //clear any pending interrupt
    TIMSK1 &= ~(1 << OCIE1A);         //enable OC1A interrupt 
    interrupts();
}

void StepperDriver::updateMotor() {
  int now = millis();

  if ((now - startTime) > ACCELERATION_TIME_MILLIS) {
    return;
  }

  int adjust = (float)(now - startTime) / (float)ACCELERATION_TIME_MILLIS * (65000 - m_target_speed);
  m_current_speed = 65000 - adjust;
  Serial.println(m_current_speed);
}

void StepperDriver::toggle() {
  static unsigned int
        oldOC;
       
    //OC1A occurred; is pin high or low now?
    if(!m_pulse)
    {
        m_pulse = true;
        PORTB = PORTB | driver1Pulse;

        
        //digitalWrite( LED_BUILTIN, HIGH );

        //Serial.println("High");
        
        //save time of OC
        oldOC = OCR1A;
        //set up for width of step pulse
        OCR1A = oldOC + STEP_PULSE_WIDTH;       
        //set to clear the pin on the next compare
        TCCR1A |= (1 << COM1A1 );
        TCCR1A &= ~(1 << COM1A0 );        
        
   
    }//if
    else
    {
        m_pulse = false;
        PORTB = PORTB & ~driver1Pulse;

        m_current_count++;

        if (m_current_count >= m_target_count) {
          stopMotor();
          return;
        }
        
        //digitalWrite(LED_BUILTIN, LOW);

        //Serial.println("Low");
        
        if (m_current_speed > m_target_speed) {
          m_current_speed -= 80;
         
          
        }
        //no, use constant
        OCR1A = oldOC + m_current_speed;       
       
        //set pin on next compare
        TCCR1A |= (1 << COM1A0 );       
        TCCR1A &= ~(1 << COM1A1 );           

    }//else
}
