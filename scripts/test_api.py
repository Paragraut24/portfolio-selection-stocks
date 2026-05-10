import urllib.request
import json

symbols = ["INFY.NS", "TCS.NS", "RELIANCE.NS", "WIPRO.NS", "HCLTECH.NS"]

for symbol in symbols:
    url = f"https://query1.finance.yahoo.com/v8/finance/chart/{symbol}?interval=1d&range=1d"
    req = urllib.request.Request(url)
    req.add_header('User-Agent', 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36')
    
    try:
        with urllib.request.urlopen(req, timeout=10) as response:
            data = json.loads(response.read().decode())
            
            if "chart" in data and "result" in data["chart"] and data["chart"]["result"]:
                result = data["chart"]["result"][0]
                meta = result.get("meta", {})
                
                current_price = meta.get("regularMarketPrice", 0)
                prev_close = meta.get("previousClose", 0)
                
                print(f"{symbol}: Current=₹{current_price:.2f}, PrevClose=₹{prev_close:.2f}")
    except Exception as e:
        print(f"{symbol}: Error - {e}")
