# Quick Start Guide - Portfolio Selection Backend

## 1. Compile

```bash
cd portfolio-selection/backend

g++ -std=c++17 -O2 -Wall -Wextra -o portfolio_backend \
    main.cpp common.cpp csv_io.cpp knapsack_2d.cpp knapsack_1d.cpp \
    greedy.cpp fractional.cpp mst.cpp
```

## 2. Run

```bash
# Default: stocks.csv with budget 5000
./portfolio_backend

# Custom input and budget
./portfolio_backend mydata.csv 10000
```

## 3. Check Outputs

```bash
ls -l *.csv
```

You should see:
- `portfolio_results_2d.csv` - 2D DP results
- `portfolio_results_1d.csv` - 1D DP results
- `portfolio_results_greedy.csv` - Greedy results
- `portfolio_results_fractional.csv` - Fractional results
- `algorithm_comparison.csv` - Performance comparison
- `diversification_output.csv` - MST diversification

## Input Format

Create `stocks.csv`:
```csv
id,name,cost,expected_return_percent,risk_score
1,INFY,1400,10.0,6
2,TCS,3200,11.0,5
3,AAPL,150,12.0,8
```

## Algorithms

1. **2D Knapsack** - Optimal, O(n×budget) space
2. **1D Knapsack** - Optimal, O(budget) space
3. **Greedy** - Fast approximation, O(n log n)
4. **Fractional** - Theoretical upper bound
5. **MST** - Portfolio diversification

## Clean Up

```bash
# Remove outputs only
rm -f portfolio_results_*.csv algorithm_comparison.csv diversification_output.csv

# Remove everything including executable
rm -f *.o portfolio_backend *.csv
```

## Example Output

```
Portfolio Selection Backend System
===================================

Input File: stocks.csv
Budget: Rs.5000

Step 1: Loading stocks...
Loaded 10 stocks from 'stocks.csv'

Step 2: Running optimization algorithms...
[Results for each algorithm...]

Step 3: Generating algorithm comparison...
Saved algorithm comparison to 'algorithm_comparison.csv'

Step 4: Running portfolio diversification (MST)...
[MST results...]

===================================
Processing Complete!
===================================
```

## Troubleshooting

**Problem**: Compiler not found
```bash
# Install g++ on Ubuntu/Debian
sudo apt-get install g++

# Install on macOS
xcode-select --install
```

**Problem**: File not found
```bash
# Make sure stocks.csv exists
ls stocks.csv

# Or specify full path
./portfolio_backend /path/to/stocks.csv 5000
```

**Problem**: Permission denied
```bash
# Make executable
chmod +x portfolio_backend
```

## Next Steps

- Read `README.md` for detailed documentation
- Read `BACKEND_REFACTORING.md` for architecture details
- Modify `stocks.csv` with your own data
- Experiment with different budgets
- Integrate with your own tools/scripts
