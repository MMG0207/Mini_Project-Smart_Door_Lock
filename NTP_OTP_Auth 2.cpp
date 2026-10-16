#include "NTP_OTP_Auth.h"
#include <time.h>
#include <HTTPClient.h>

// 1. Setup function to be called once by Member 2
void NTP_OTP_Auth::begin(const char* ssid, const char* password, const char* firebase_url, const char* firebase_api_key) {
    // We will add Wi-Fi and Firebase connection logic here
}
// 2. The Time Fetcher
unsigned long NTP_OTP_Auth::fetchEpochTime() {
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    time_t now = time(nullptr);
    int retries = 0;
    
    while (now < 1000000000 && retries < 20) {
        delay(500);
        now = time(nullptr);
        retries++;
    }
    return (unsigned long)now;
}
// 3. The OTP Math Engine
String NTP_OTP_Auth::generateGuestOTP(unsigned long epochTime) {
    unsigned long bucket = epochTime / 15;
    
    // 64-bit integer prevents ESP32 memory overflow
    uint64_t raw_hash = ((uint64_t)bucket * 1103515245ULL + 12345ULL + k_secret) % 2147483647ULL;
    unsigned long otp = raw_hash % 1000000;
    
    char otpString[7];
    sprintf(otpString, "%06lu", otp);
    
    return String(otpString);
}

// 4. Firebase Drivers 
void NTP_OTP_Auth::pushLockStatus(String state) {
    HTTPClient http;
    
    // Your exact Firebase URL from the Python script
    String url = "https://smart-door-lock-mvp-default-rtdb.firebaseio.com/status.json";
    
    http.begin(url);
    
    // Tell Firebase we are sending a JSON package
    http.addHeader("Content-Type", "application/json");
    
    // Package up the exact same test_data format from your Python script
    String payload = "{\"lockState\":\"" + state + "\"}";
    
    // Send the PATCH request to update just this field without deleting others
    int httpResponseCode = http.PATCH(payload);
    
    if (httpResponseCode == 200) {
        Serial.println("✅ SUCCESS! Firebase lockState updated.");
    } else {
        Serial.print("❌ FAILED. Error code: ");
        Serial.println(httpResponseCode);
    }
    
    http.end(); // Free up the ESP32's memory
}
// 5. Update Failed Attempts
void NTP_OTP_Auth::pushFailedAttempt(int count) {
    HTTPClient http;
    String url = "https://smart-door-lock-mvp-default-rtdb.firebaseio.com/status.json";
    
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    
    // Package the integer count into the JSON payload
    String payload = "{\"failedAttempts\":" + String(count) + "}";
    
    http.PATCH(payload);
    http.end();
}

// 6. Trigger Lockdown Mode
void NTP_OTP_Auth::triggerLockdown() {
    HTTPClient http;
    String url = "https://smart-door-lock-mvp-default-rtdb.firebaseio.com/status.json";
    
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    
    // Set lockdownMode to true (boolean, no quotes)
    String payload = "{\"lockdownMode\":true}";
    
    http.PATCH(payload);
    http.end();
}