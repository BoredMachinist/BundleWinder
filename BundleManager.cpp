#include "BundleManager.h"

BundleManager* BundleManager::ourInstance = nullptr;
SetWrapsCallback* SetWrapsCallback::ourInstance = nullptr;
SetSpeedCallback* SetSpeedCallback::ourInstance = nullptr;
SetRatioCallback* SetRatioCallback::ourInstance = nullptr;

Bundle::Bundle(int wraps, float speed, int ratio) {
  m_wraps = wraps;
  m_speed = speed;
  m_greeseRatio = ratio;
}


BundleManager::BundleManager() {
  defaultBundle();
}

void BundleManager::defaultBundle() {
  currentBundle = new Bundle(600, 100.0, 10);
}
