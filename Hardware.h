
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>


#ifndef H_HARDWARE
#define H_HARDWARE


class DisplayManager {
  public:
    static DisplayManager* getInstance() {
      return ourInstance;
    }
    
    static void init() {
      ourInstance = new DisplayManager();
    }

    ~DisplayManager();

    void update();

    void print(String s);
    void printCentered(String s);
    void clear();
    void cursor(int x, int y);

    void printMenuArrow();


  private:
    DisplayManager();
    static DisplayManager* ourInstance;


    
    Adafruit_SH1107 display;
};

class ButtonManager {
  public:
    static ButtonManager* getInstance() {
      return ourInstance;
    }

    static void init() {
      ourInstance = new ButtonManager();
    }

    ~ButtonManager();

    bool selectPressed(){
      return m_select && !m_lastSelect;
    }
    bool upPressed() {
      return m_up && !m_lastUp;
    }
    bool downPressed(){
      return m_down && !m_lastDown;
    }

    bool select() {
      return m_select;
    }
    bool up() {
      return m_up;
    }
    bool down() {
      return m_down;
    }

    bool selectReleased(){
      return !m_select && m_lastSelect;
    }
    bool upReleased() {
      return !m_up && m_lastUp;
    }
    bool downReleased() {
      return !m_down && m_lastDown;
    }

    void update();

  private:
    ButtonManager();
    static ButtonManager* ourInstance;
  
    int m_selectPin;
    int m_upPin;
    int m_downPin;
  
    bool m_select;
    bool m_up;
    bool m_down;

    bool m_lastSelect;
    bool m_lastUp;
    bool m_lastDown;
};

#endif
