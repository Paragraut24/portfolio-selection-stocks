# Stock Data Fetcher Scripts

## Overview

Python scripts for fetching real-time stock data from various sources and saving to CSV format.

## Files

- `fetch_stocks.py` - Main fetcher script
- `config.json` - Configuration file
- `requirements.txt` - Python dependencies

## Installation

```bash
pip install -r requirements.txt
```

## Usage

### Mock Data (Default)
```bash
python fetch_stocks.py --source mock
```

### Alpha Vantage API
```bash
# Set API key as environment variable
export ALPHA_VANTAGE_API_KEY="your_api_key_here"

# Run fetcher
python fetch_stocks.py --source alpha_vantage
```

### Custom Configuration
```bash
python fetch_stocks.py --config custom_config.json --output custom_output.csv
```

## Configuration

Edit `config.json` to customize:

```json
{
  "symbols": ["AAPL", "GOOGL", "MSFT"],
  "output_path": "../backend/stocks.csv",
  "data_source": "mock"
}
```

## API Keys

### Alpha Vantage
1. Get free API key from: https://www.alphavantage.co/support/#api-key
2. Set environment variable:
   - Windows: `set ALPHA_VANTAGE_API_KEY=your_key`
   - Linux/Mac: `export ALPHA_VANTAGE_API_KEY=your_key`

### Yahoo Finance
- Uses yfinance library (optional)
- No API key required
- Install: `pip install yfinance`

## Output Format

CSV file with columns:
- `id` - Stock ID
- `name` - Stock symbol/name
- `cost` - Current price
- `expected_return_percent` - Expected return percentage
- `risk_score` - Risk score (1-15)

## Integration with Web App

The web application can trigger this script via API endpoint:
```
POST /api/stocks/fetch
```

## Error Handling

- Falls back to mock data if API fails
- Logs errors to stderr
- Returns JSON status with success/error information

## Notes

- Free API keys have rate limits (5 calls/minute for Alpha Vantage)
- Mock data is randomly generated for testing
- Configure symbols in config.json
- Never commit API keys to version control
