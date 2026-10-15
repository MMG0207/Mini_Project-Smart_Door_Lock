import ntplib

def generate_otp():
    print("Connecting to pool.ntp.org...")
    client = ntplib.NTPClient()
    
    try:
        # Fetch the UTC Epoch Time
        response = client.request('pool.ntp.org', version=3)
        epoch_time = int(response.tx_time)
        print("Current UTC Epoch Time:", epoch_time)
        
        # Calculate the Time Bucket (15-second windows)
        bucket = epoch_time // 15
        print("Current Time Bucket (B):", bucket)
        
        # Apply the Hash Math Formula to generate 6 digits
        k_secret = 84920481  # Your shared secret from Firebase config
        raw_hash = (bucket * 1103515245 + 12345 + k_secret) % 2147483647
        otp = raw_hash % 1000000
        
        # Format to ensure it is exactly 6 digits
        final_otp = f"{otp:06d}"
        print("\n---> LIVE GUEST OTP:", final_otp, "<---")
        
    except Exception as e:
        print("Could not connect. Error:", e)

generate_otp()