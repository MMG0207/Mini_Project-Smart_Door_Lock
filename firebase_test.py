import requests
import json

# Your Firebase Database URL (Notice the .json added to the end for the API)
url = "https://smart-door-lock-mvp-default-rtdb.firebaseio.com/status.json"

# The test data we are pushing to your /status node
test_data = {
    "lockState": "UNLOCKED",
    "failedAttempts": 0,
    "lockdownMode": False
}

print("Connecting to Firebase...")

try:
    # We use PATCH to update only these specific fields without deleting anything else
    response = requests.patch(url, data=json.dumps(test_data))
    
    if response.status_code == 200:
        print("✅ SUCCESS! Cloud Write Verification complete.")
        print("Firebase saved this data:", response.json())
    else:
        print("❌ FAILED. Error code:", response.status_code)
        
except Exception as e:
    print("Could not connect. Error:", e)