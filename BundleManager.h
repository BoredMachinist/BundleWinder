#ifndef H_BUNDLEMANAGER
#define H_BUNDLEMANAGER

struct IntSetCallback
{
  public:
    virtual void set(int num) = 0;
};



class Bundle {

  public:
    Bundle(int wraps, float speed, int greeseRatio);

    float getRemainingRevs();
    float getCurrentWraps();

    void setTargetWraps(int wraps) {m_wraps = wraps;}
    int getTargetWraps() {return m_wraps;}

    void setTargetSpeed(int speed) {m_speed = speed;}
    int getTargetSpeed() {return m_speed;}

    void setRatio(int ratio) {m_greeseRatio = ratio;}
    int getRatio() {return m_greeseRatio;}

  private:
    int m_wraps; // Number of wraps to complete
    int m_speed; //Winding wraps per minute
    int m_greeseRatio; //Ratio between winder rotations and greese rotations

    
    float m_currentRevs;
  
};

class BundleManager {
  public:
    static BundleManager* getInstance() {
      return ourInstance;
    }
    
    static void init() {
      ourInstance = new BundleManager();
    }

    ~BundleManager();

    Bundle* getBundle() {
      return currentBundle;
    }

    void defaultBundle();

  private:
    BundleManager();
    static BundleManager* ourInstance;

    Bundle* currentBundle;

    
};


class SetWrapsCallback : public IntSetCallback
{
  public:
    static SetWrapsCallback* instance() {
      if (ourInstance == nullptr) {
        ourInstance = new SetWrapsCallback();
      }
      return ourInstance;
     }
      
    void set(int num) {
      BundleManager::getInstance()->getBundle()->setTargetWraps(num);
    }
  private:
    SetWrapsCallback(){};
    static SetWrapsCallback* ourInstance;
};

class SetSpeedCallback : public IntSetCallback
{
  public:
    static SetSpeedCallback* instance() {
      if (ourInstance == nullptr) {
        ourInstance = new SetSpeedCallback();
      }
      return ourInstance;
     }
      
    void set(int num) {
      BundleManager::getInstance()->getBundle()->setTargetSpeed(num);
    }
  private:
    SetSpeedCallback(){};
    static SetSpeedCallback* ourInstance;
};

class SetRatioCallback : public IntSetCallback
{
  public:
    static SetRatioCallback* instance() {
      if (ourInstance == nullptr) {
        ourInstance = new SetRatioCallback();
      }
      return ourInstance;
     }
      
    void set(int num) {
      BundleManager::getInstance()->getBundle()->setRatio(num);
    }
  private:
    SetRatioCallback(){};
    static SetRatioCallback* ourInstance;
};

#endif
