#include "Hardware.h"



#define I2C_ADDR 0x27

#define MENU_ARROW 0

#define BUTTON_A  9
#define BUTTON_B  6
#define BUTTON_C  5


// DisplayManager

DisplayManager* DisplayManager::ourInstance = nullptr;



DisplayManager::DisplayManager():
  display(64, 128, &Wire)
  {
     delay(250); // wait for the OLED to power up
  display.begin(0x3C, true); // Address 0x3C default

  Serial.println("OLED begun");

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setRotation(1);

  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);

  cursor(0,0);
}

void DisplayManager::print(String s) {
  display.print(s);
}

void DisplayManager::clear() {
  display.clearDisplay();
  cursor(0,0);
}

void DisplayManager::update() {
  display.display();
}

void DisplayManager::printMenuArrow() {
  
}

void DisplayManager::cursor(int x, int y) {
  display.setCursor(x, y);
}


// Button Manager

ButtonManager* ButtonManager::ourInstance = nullptr;

ButtonManager::ButtonManager():
  m_selectPin(BUTTON_B),
  m_upPin(BUTTON_A),
  m_downPin(BUTTON_C),
  m_lastSelect(false),
  m_lastUp(false),
  m_lastDown(false),
  m_select(false),
  m_up(false),
  m_down(false) 
  {
    
    pinMode(m_selectPin, INPUT_PULLUP);
    pinMode(m_upPin, INPUT_PULLUP);
    pinMode(m_downPin, INPUT_PULLUP);
  }

void ButtonManager::update() {
  m_lastSelect = m_select;
  m_lastUp = m_up;
  m_lastDown = m_down;

  m_select = !digitalRead(m_selectPin);
  m_up = !digitalRead(m_upPin);
  m_down = !digitalRead(m_downPin);
}
