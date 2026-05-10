#!/usr/bin/env python3
"""
Stock Data Fetcher
Fetches real-time stock data from API and saves to CSV
Supports multiple data sources with fallback to mock data
"""

import csv
import json
import os
import sys
from datetime import datetime
from typing import List, Dict, Optional
import urllib.request
import urllib.error

class StockDataFetcher:
    def __init__(self, config_path: str = "config.json"):
        """Initialize fetcher with configuration"""
        self.config = self.load_config(config_path)
        self.output_path = self.config.get("output_path", "../backend/stocks.csv")
        
    def load_config(self, config_path: str) -> Dict:
        """Load configuration from JSON file"""
        if os.path.exists(config_path):
            with open(config_path, 'r') as f:
                return json.load(f)
        return {}
    
    def fetch_from_alpha_vantage(self, symbols: List[str]) -> List[Dict]:
        """Fetch data from Alpha Vantage API"""
        api_key = os.environ.get('ALPHA_VANTAGE_API_KEY')
        if not api_key:
            raise ValueError("ALPHA_VANTAGE_API_KEY not set in environment")
        
        stocks = []
        base_url = "https://www.alphavantage.co/query"
        
        for idx, symbol in enumerate(symbols, 1):
            try:
                # Fetch quote data
                url = f"{base_url}?function=GLOBAL_QUOTE&symbol={symbol}&apikey={api_key}"
                with urllib.request.urlopen(url, timeout=10) as response:
                    data = json.loads(response.read().decode())
                    
                if "Global Quote" in data:
                    quote = data["Global Quote"]
                    price = float(quote.get("05. price", 0))
                    change_pct = float(quote.get("10. change percent", "0").replace("%", ""))
                    
                    # Estimate risk based on volatility (simplified)
                    risk = abs(change_pct) * 0.5 + 5
                    
                    stocks.append({
                        "id": idx,
                        "name": symbol,
                        "cost": int(price),
                        "expected_return_percent": max(5, min(20, 10 + change_pct)),
                        "risk_score": round(risk, 1)
                    })
                    
            except Exception as e:
                print(f"Warning: Failed to fetch {symbol}: {e}", file=sys.stderr)
                continue
        
        return stocks
    
    def fetch_from_yahoo_finance(self, symbols: List[str]) -> List[Dict]:
        """Fetch real-time data from Yahoo Finance with improved risk/return calculations"""
        import time
        
        stocks = []
        
        # Symbol mapping for Indian stocks (add .NS suffix for NSE)
        symbol_map = {
            "INFY": "INFY.NS",
            "TCS": "TCS.NS",
            "RELIANCE": "RELIANCE.NS",
            "WIPRO": "WIPRO.NS",
            "HCL": "HCLTECH.NS",
            "AAPL": "AAPL",
            "TSLA": "TSLA",
            "GOOGL": "GOOGL",
            "MSFT": "MSFT",
            "AMZN": "AMZN"
        }
        
        # Known beta values and sector characteristics for better risk calculation
        beta_map = {
            "INFY": 0.85,    # IT sector - moderate volatility
            "TCS": 0.40,     # IT sector - low volatility (stable)
            "RELIANCE": 1.10, # Conglomerate - higher volatility
            "WIPRO": 0.90,   # IT sector - moderate volatility
            "HCL": 0.75,     # IT sector - moderate-low volatility
            "AAPL": 1.20,    # Tech - higher volatility
            "TSLA": 2.00,    # EV/Tech - very high volatility
            "GOOGL": 1.15,   # Tech - higher volatility
            "MSFT": 0.95,    # Tech - moderate volatility
            "AMZN": 1.30     # E-commerce/Tech - higher volatility
        }
        
        for idx, symbol in enumerate(symbols, 1):
            try:
                yahoo_symbol = symbol_map.get(symbol, symbol)
                
                # Fetch quote data from Yahoo Finance with proper headers
                url = f"https://query1.finance.yahoo.com/v8/finance/chart/{yahoo_symbol}?interval=1d&range=5d"
                
                # Add headers to avoid rate limiting
                req = urllib.request.Request(url)
                req.add_header('User-Agent', 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36')
                req.add_header('Accept', 'application/json')
                
                with urllib.request.urlopen(req, timeout=10) as response:
                    data = json.loads(response.read().decode())
                    
                    if "chart" in data and "result" in data["chart"] and data["chart"]["result"]:
                        result = data["chart"]["result"][0]
                        meta = result.get("meta", {})
                        
                        # Get current price
                        current_price = meta.get("regularMarketPrice", 0)
                        
                        # Get previous close for calculating change
                        # Use chartPreviousClose if previousClose is 0 (market closed)
                        prev_close = meta.get("previousClose", 0)
                        if prev_close == 0:
                            prev_close = meta.get("chartPreviousClose", current_price)
                        
                        # Calculate percentage change
                        if prev_close > 0 and prev_close != current_price:
                            change_pct = ((current_price - prev_close) / prev_close) * 100
                        else:
                            change_pct = 0
                        
                        # Convert USD to INR for US stocks (approximate rate: 1 USD = 83 INR)
                        if not yahoo_symbol.endswith(".NS"):
                            cost = int(current_price * 83)
                        else:
                            cost = int(current_price)
                        
                        # Get beta value for this stock
                        beta = beta_map.get(symbol, 1.0)
                        
                        # Calculate Expected Return using improved formula
                        # Formula: Expected Return = Risk-Free Rate + Beta * Market Risk Premium + Alpha
                        # Risk-Free Rate (India): ~7%, Market Risk Premium: ~8%
                        # For simplicity: Base Return + (Beta * Market Factor) + Recent Performance Adjustment
                        
                        risk_free_rate = 7.0  # India government bonds ~7%
                        market_premium = 6.0  # Expected market return above risk-free
                        
                        # Base expected return from CAPM
                        base_expected_return = risk_free_rate + (beta * market_premium)
                        
                        # Adjust based on recent performance (momentum factor)
                        momentum_adjustment = change_pct * 0.2  # 20% weight to recent momentum
                        
                        expected_return = round(base_expected_return + momentum_adjustment, 1)
                        expected_return = max(5.0, min(25.0, expected_return))  # Clamp between 5-25%
                        
                        # Calculate Risk Score based on Beta and Volatility
                        # Risk Score = Beta * 5 + abs(Recent Change) * 0.3
                        # This gives a score roughly between 3-15
                        
                        base_risk = beta * 6.0  # Beta contribution to risk
                        volatility_risk = abs(change_pct) * 0.4  # Recent volatility contribution
                        
                        risk_score = round(base_risk + volatility_risk, 1)
                        risk_score = max(3.0, min(15.0, risk_score))  # Clamp between 3-15
                        
                        stocks.append({
                            "id": idx,
                            "name": symbol,
                            "cost": cost,
                            "expected_return_percent": expected_return,
                            "risk_score": risk_score
                        })
                        
                        print(f"✓ Fetched {symbol}: ₹{cost} (Change: {change_pct:+.2f}%, Beta: {beta}, Return: {expected_return}%, Risk: {risk_score})")
                        
                        # Add delay to avoid rate limiting (0.5 seconds between requests)
                        time.sleep(0.5)
                        
            except urllib.error.HTTPError as e:
                if e.code == 429:
                    print(f"⚠ Rate limited on {symbol}, waiting 2 seconds...", file=sys.stderr)
                    time.sleep(2)
                    # Retry once
                    try:
                        req = urllib.request.Request(url)
                        req.add_header('User-Agent', 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36')
                        with urllib.request.urlopen(req, timeout=10) as response:
                            data = json.loads(response.read().decode())
                            if "chart" in data and "result" in data["chart"] and data["chart"]["result"]:
                                result = data["chart"]["result"][0]
                                meta = result.get("meta", {})
                                current_price = meta.get("regularMarketPrice", 0)
                                prev_close = meta.get("previousClose", current_price)
                                change_pct = ((current_price - prev_close) / prev_close) * 100 if prev_close > 0 else 0
                                
                                if not yahoo_symbol.endswith(".NS"):
                                    cost = int(current_price * 83)
                                else:
                                    cost = int(current_price)
                                
                                expected_return = round(10.0 + (change_pct * 0.5), 1)
                                expected_return = max(5.0, min(20.0, expected_return))
                                risk_score = round(abs(change_pct) * 0.8 + 7.0, 1)
                                risk_score = max(3.0, min(15.0, risk_score))
                                
                                stocks.append({
                                    "id": idx,
                                    "name": symbol,
                                    "cost": cost,
                                    "expected_return_percent": expected_return,
                                    "risk_score": risk_score
                                })
                                print(f"✓ Fetched {symbol} (retry): ₹{cost}")
                    except:
                        print(f"✗ Failed to fetch {symbol} after retry", file=sys.stderr)
                else:
                    print(f"✗ Failed to fetch {symbol}: HTTP {e.code}", file=sys.stderr)
            except Exception as e:
                print(f"✗ Failed to fetch {symbol}: {e}", file=sys.stderr)
        
        return stocks if stocks else []
    
    def generate_mock_data(self, symbols: List[str]) -> List[Dict]:
        """Generate consistent mock stock data for testing (fallback only)"""
        # Static mock data - returns same values every time for consistency
        # Based on realistic Indian and US stock market prices
        mock_data_map = {
            "INFY": {"cost": 1526, "return": 12.5, "risk": 8.2},
            "TCS": {"cost": 3492, "return": 11.8, "risk": 7.5},
            "AAPL": {"cost": 17500, "return": 15.2, "risk": 9.8},
            "TSLA": {"cost": 24800, "return": 18.5, "risk": 14.2},
            "RELIANCE": {"cost": 2456, "return": 10.5, "risk": 8.9},
            "GOOGL": {"cost": 14200, "return": 14.8, "risk": 9.2},
            "MSFT": {"cost": 37500, "return": 13.5, "risk": 8.5},
            "AMZN": {"cost": 16800, "return": 16.2, "risk": 10.5},
            "WIPRO": {"cost": 445, "return": 9.8, "risk": 7.8},
            "HCL": {"cost": 1234, "return": 11.2, "risk": 8.0}
        }
        
        stocks = []
        for idx, symbol in enumerate(symbols, 1):
            # Use predefined data if available, otherwise use default values
            if symbol in mock_data_map:
                data = mock_data_map[symbol]
                cost = data["cost"]
                return_pct = data["return"]
                risk = data["risk"]
            else:
                # Default values for unknown symbols
                cost = 1000 + (idx * 500)
                return_pct = 10.0 + (idx * 0.5)
                risk = 8.0 + (idx * 0.3)
            
            stocks.append({
                "id": idx,
                "name": symbol,
                "cost": cost,
                "expected_return_percent": return_pct,
                "risk_score": risk
            })
        
        return stocks
    
    def fetch_stocks(self, source: str = "mock") -> List[Dict]:
        """Fetch stocks from specified source"""
        symbols = self.config.get("symbols", [
            "INFY", "TCS", "AAPL", "TSLA", "RELIANCE",
            "GOOGL", "MSFT", "AMZN", "WIPRO", "HCL"
        ])
        
        print(f"Fetching stock data from {source}...")
        
        if source == "alpha_vantage":
            try:
                return self.fetch_from_alpha_vantage(symbols)
            except Exception as e:
                print(f"Error with Alpha Vantage: {e}", file=sys.stderr)
                print("Falling back to mock data...")
                return self.generate_mock_data(symbols)
        
        elif source == "yahoo":
            stocks = self.fetch_from_yahoo_finance(symbols)
            if not stocks:
                print("Falling back to mock data...")
                return self.generate_mock_data(symbols)
            return stocks
        
        else:  # mock or default
            return self.generate_mock_data(symbols)
    
    def save_to_csv(self, stocks: List[Dict]) -> str:
        """Save stocks to CSV file"""
        if not stocks:
            raise ValueError("No stock data to save")
        
        # Ensure output directory exists
        os.makedirs(os.path.dirname(self.output_path), exist_ok=True)
        
        with open(self.output_path, 'w', newline='') as f:
            writer = csv.DictWriter(f, fieldnames=[
                'id', 'name', 'cost', 'expected_return_percent', 'risk_score'
            ])
            writer.writeheader()
            writer.writerows(stocks)
        
        return self.output_path
    
    def run(self, source: str = "mock") -> Dict:
        """Main execution method"""
        try:
            stocks = self.fetch_stocks(source)
            
            if not stocks:
                return {
                    "success": False,
                    "error": "No stocks fetched",
                    "timestamp": datetime.now().isoformat()
                }
            
            output_file = self.save_to_csv(stocks)
            
            return {
                "success": True,
                "stocks_count": len(stocks),
                "output_file": output_file,
                "source": source,
                "timestamp": datetime.now().isoformat()
            }
            
        except Exception as e:
            return {
                "success": False,
                "error": str(e),
                "timestamp": datetime.now().isoformat()
            }


def main():
    """Command line interface"""
    import argparse
    
    parser = argparse.ArgumentParser(description='Fetch stock data and save to CSV')
    parser.add_argument('--source', choices=['mock', 'alpha_vantage', 'yahoo'],
                       default='mock', help='Data source to use')
    parser.add_argument('--config', default='config.json',
                       help='Path to configuration file')
    parser.add_argument('--output', help='Output CSV file path')
    
    args = parser.parse_args()
    
    fetcher = StockDataFetcher(args.config)
    
    if args.output:
        fetcher.output_path = args.output
    
    result = fetcher.run(args.source)
    
    # Print result as JSON for easy parsing
    print(json.dumps(result, indent=2))
    
    # Exit with appropriate code
    sys.exit(0 if result["success"] else 1)


if __name__ == "__main__":
    main()
