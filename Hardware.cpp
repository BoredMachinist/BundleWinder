#include "Hardware.h"

#define I2C_ADDR 0x27

#define MENU_ARROW 0

#define SELECT_PIN 2
#define UP_PIN 3
#define DOWN_PIN 4


// DisplayManager

DisplayManager* DisplayManager::ourInstance = nullptr;

static const uint8_t menuArrowData[] = {0x04,0x0E,0x1F,0x00,0x00,0x1F,0x0E,0x04};
uint8_t* DisplayManager::menuArrow = menuArrowData;

DisplayManager::DisplayManager():
  lcd(I2C_ADDR, 20, 4)
  {
    Serial.println("Doing things");
     lcd.init();
    lcd.backlight();

  lcd.createChar(MENU_ARROW, menuArrow);

  lcd.setCursor(1,0);
  lcd.print("Bundle Winder");
}

void DisplayManager::print(String s) {
  lcd.print(s);
}

void DisplayManager::clear() {
  lcd.clear();
  lcd.setCursor(0,0);
}

void DisplayManager::printMenuArrow() {
  lcd.write(MENU_ARROW);
}

void DisplayManager::cursor(int x, int y) {
  lcd.setCursor(x, y);
}


// Button Manager

ButtonManager* ButtonManager::ourInstance = nullptr;

ButtonManager::ButtonManager():
  m_selectPin(SELECT_PIN),
  m_upPin(UP_PIN),
  m_downPin(DOWN_PIN) 
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
