# Portfolio Selection Web Application

A modern web interface for the Portfolio Selection System that wraps the C++ backend with a Node.js/Express server.

## Features

- **6 Interactive Pages:**
  1. Home - System overview and statistics
  2. Stock Data - View and upload stock CSV files
  3. Optimizer - Run optimization algorithms with custom budget
  4. Results - View detailed portfolio results
  5. Analytics - Risk/return analysis and diversification
  6. Algorithm Analysis - Performance comparison and complexity metrics

- **Backend Integration:**
  - Wraps C++ backend executable
  - RESTful API endpoints
  - CSV file management
  - Real-time optimization execution

- **Visualizations:**
  - Portfolio distribution charts (Chart.js)
  - Risk vs return scatter plots
  - Algorithm performance comparison
  - Investment allocation pie charts

## Installation

1. Install Node.js dependencies:
```bash
cd portfolio-selection/web-app
npm install
```

2. Ensure C++ backend is compiled:
```bash
cd ../backend
make
```

## Usage

1. Start the server:
```bash
npm start
```

2. Open browser and navigate to:
```
http://localhost:3000
```

3. Use the web interface:
   - Upload stock data via Stock Data page
   - Set budget and run optimization via Optimizer page
   - View results and analytics

## API Endpoints

- `GET /api/stocks` - Get all stocks
- `POST /api/stocks/upload` - Upload stocks CSV
- `POST /api/optimize` - Run optimization (body: {budget, algorithm})
- `GET /api/results/:algorithm` - Get results for specific algorithm
- `GET /api/comparison` - Get algorithm comparison
- `GET /api/diversification` - Get MST diversification results

## Technology Stack

- **Backend:** Node.js, Express
- **Frontend:** HTML5, CSS3, JavaScript (ES6+)
- **Charts:** Chart.js
- **File Processing:** csv-parser, multer
- **C++ Backend:** Portfolio optimization algorithms

## Project Structure

```
web-app/
├── package.json          # Node.js dependencies
├── server.js             # Express server
├── frontend/
│   ├── index.html        # Home page
│   ├── stocks.html       # Stock data management
│   ├── optimizer.html    # Optimization interface
│   ├── results.html      # Results display
│   ├── analytics.html    # Analytics dashboard
│   ├── analysis.html     # Algorithm analysis
│   ├── css/
│   │   └── style.css     # Styling
│   └── js/
│       └── main.js       # API calls and utilities
└── README.md
```

## Notes

- Server runs on port 3000 by default
- C++ backend must be compiled before running
- Stock CSV format: id, name, cost, expected_return_percent, risk_score
- All algorithms run automatically when optimization is triggered
