#include "BundleManager.h"

BundleManager* BundleManager::ourInstance = nullptr;
SetWrapsCallback* SetWrapsCallback::ourInstance = nullptr;
SetSpeedCallback* SetSpeedCallback::ourInstance = nullptr;
SetRatioCallback* SetRatioCallback::ourInstance = nullptr;

Bundle::Bundle(int wraps, float speed, int ratio) {
  m_wraps = wraps;
  m_speed = speed;
  m_greeseRatio = ratio;
  m_currentRevs = 0.0f;
}

float Bundle::getRemainingRevs() {
  return m_wraps * 9.269f - m_currentRevs;
}


BundleManager::BundleManager() {
  defaultBundle();
}

void BundleManager::defaultBundle() {
  currentBundle = new Bundle(400, 100.0, 10);
}
