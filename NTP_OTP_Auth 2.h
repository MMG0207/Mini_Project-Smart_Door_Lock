#ifndef NTP_OTP_AUTH_H
#define NTP_OTP_AUTH_H

#include <Arduino.h>

class NTP_OTP_Auth {
  public:
    // Core Setup
    void begin(const char* ssid, const char* password, const char* firebase_url, const char* firebase_api_key);
    
    // OTP & Time Functions (From your Week 2 math)
    unsigned long fetchEpochTime();
    String generateGuestOTP(unsigned long epochTime);

    // Firebase Driver Integration (From your Week 2 write verification)
    void pushLockStatus(String state);
    void pushFailedAttempt(int count);
    void triggerLockdown();

  private:
    unsigned long k_secret = 84920481;
};

#endif