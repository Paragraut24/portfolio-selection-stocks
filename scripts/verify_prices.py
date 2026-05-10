#!/usr/bin/env python3
"""Verify stock prices from Yahoo Finance"""

import urllib.request
import json
import time

def fetch_stock_price(symbol):
    """Fetch and display detailed stock information"""
    url = f"https://query1.finance.yahoo.com/v8/finance/chart/{symbol}?interval=1d&range=5d"
    
    req = urllib.request.Request(url)
    req.add_header('User-Agent', 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36')
    req.add_header('Accept', 'application/json')
    
    try:
        with urllib.request.urlopen(req, timeout=10) as response:
            data = json.loads(response.read().decode())
            
            if "chart" in data and "result" in data["chart"] and data["chart"]["result"]:
                result = data["chart"]["result"][0]
                meta = result.get("meta", {})
                
                print(f"\n{'='*60}")
                print(f"Symbol: {symbol}")
                print(f"{'='*60}")
                print(f"Currency: {meta.get('currency', 'N/A')}")
                print(f"Exchange: {meta.get('exchangeName', 'N/A')}")
                print(f"Regular Market Price: ₹{meta.get('regularMarketPrice', 0):.2f}")
                print(f"Previous Close: ₹{meta.get('previousClose', 0):.2f}")
                print(f"Chart Previous Close: ₹{meta.get('chartPreviousClose', 0):.2f}")
                
                # Get the latest close price from historical data
                timestamps = result.get('timestamp', [])
                indicators = result.get('indicators', {})
                quote = indicators.get('quote', [{}])[0]
                closes = quote.get('close', [])
                
                if closes:
                    # Filter out None values and get the last valid close
                    valid_closes = [c for c in closes if c is not None]
                    if valid_closes:
                        print(f"Latest Close from History: ₹{valid_closes[-1]:.2f}")
                
                print(f"{'='*60}\n")
                
                return meta.get('regularMarketPrice', 0)
    except Exception as e:
        print(f"Error fetching {symbol}: {e}")
        return None

# Test all Indian stocks
indian_stocks = {
    "INFY": "INFY.NS",
    "TCS": "TCS.NS",
    "RELIANCE": "RELIANCE.NS",
    "WIPRO": "WIPRO.NS",
    "HCL": "HCLTECH.NS"
}

print("Fetching real-time stock prices from Yahoo Finance...")
print("Note: Market may be closed, showing last traded prices\n")

for name, symbol in indian_stocks.items():
    price = fetch_stock_price(symbol)
    time.sleep(0.6)  # Rate limiting

print("\nUS Stocks (converted to INR at 1 USD = 83 INR):")
us_stocks = {
    "AAPL": "AAPL",
    "TSLA": "TSLA",
    "GOOGL": "GOOGL"
}

for name, symbol in us_stocks.items():
    price = fetch_stock_price(symbol)
    if price:
        print(f"{name} in INR: ₹{int(price * 83)}")
    time.sleep(0.6)
