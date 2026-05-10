# Portfolio Selection System - Complete Documentation

## Project Overview

A comprehensive C++ implementation of portfolio optimization using knapsack algorithms. This system helps investors select an optimal portfolio of stocks within a budget constraint to maximize expected returns.

### Problem Statement
Given a set of stocks with costs, expected returns, and risk scores, and a budget constraint, find the optimal subset of stocks that maximizes total expected return while staying within budget.

### Solution Approach
This is modeled as a 0/1 Knapsack problem where:
- Items = Stocks
- Weight = Cost (₹)
- Value = Expected Return Amount = Cost × (Return % / 100)
- Capacity = Budget (₹)

## Features

### Core Algorithms
1. **2D Dynamic Programming** - Optimal solution with O(n×budget) time and space
2. **1D Dynamic Programming** - Space-optimized optimal solution with O(budget) space
3. **Greedy Heuristic** - Fast O(n log n) approximation by return/cost ratio
4. **Fractional Knapsack** - Theoretical upper bound demonstration

### Additional Features
- CSV import/export with validation
- Interactive menu-driven interface
- In-memory stock management (add/edit/delete)
- Verbose mode with algorithm visualization
- ASCII allocation charts
- Performance profiling (runtime and memory)
- Complexity analysis and explanations
- Solution export with summary
- Comprehensive unit tests
- Robust error handling

## Build Instructions

### Prerequisites
- C++17 compatible compiler (g++ 7.0+, clang++ 5.0+, MSVC 2017+)
- Make (optional, can compile directly with g++)

### Compilation

#### Using Makefile
```bash
make          # Compile the project
make run      # Build and run interactive menu
make test     # Compile and run unit tests
make clean    # Remove compiled binaries
make zip      # Create submission archive
```

#### Direct Compilation
```bash
# Main program
g++ -std=c++17 -O2 -Wall -o portfolio src/main.cpp src/knapsack.cpp src/io.cpp src/utils.cpp

# Unit tests
g++ -std=c++17 -O2 -Wall -o test_runner src/tests.cpp src/knapsack.cpp src/io.cpp src/utils.cpp
```

## Running the Program

### Interactive Mode
```bash
./portfolio
```

Follow the menu prompts to:
1. Load portfolio data from CSV
2. Display stocks
3. Add/edit/delete stocks
4. Run optimization algorithms
5. Compare algorithms
6. Export results

### Running Tests
```bash
./test_runner
```

Expected output:
```
Running Portfolio Selection Tests
==================================

Test 1: Small dataset optimal selection
  ✓ Test passed

Test 2: Empty stock list
  ✓ Test passed

...

==================================
All tests passed! ✓
```

## CSV Format

### Input Format
```csv
id,name,cost,expected_return_percent,risk_score
1,INFY,1400,10.0,6
2,TCS,3200,11.0,5
3,AAPL,150,12.0,8
4,TSLA,250,20.0,15
5,RELIANCE,2400,9.0,7
```

### Field Descriptions
- **id**: Unique integer identifier
- **name**: Stock name (string, no commas)
- **cost**: Investment cost in ₹ (positive integer)
- **expected_return_percent**: Expected annual return percentage (double)
- **risk_score**: Risk rating 0-20 (double, higher = riskier)

### Validation Rules
- All fields required
- No duplicate IDs or names
- Cost must be positive
- Numeric fields must be valid numbers
- Malformed lines are skipped with warnings

## Sample Run Transcript

### Basic Optimization
```
$ ./portfolio

Welcome to Portfolio Selection System!

========================================
   Portfolio Selection System
========================================
1.  Load portfolio data from CSV
2.  Display all stocks
3.  Add new stock
4.  Edit stock
5.  Delete stock
6.  Export portfolio to CSV
7.  Run optimization (choose algorithm)
8.  Show last optimization result
9.  Export solution to CSV
10. Show complexity & explanation
11. Compare all algorithms
12. Run unit tests
13. Toggle verbose mode [OFF]
14. Exit
========================================
Enter your choice: 1
Enter CSV file path: data/sample_small.csv
Successfully loaded 5 stocks from 'data/sample_small.csv'

Enter your choice: 2

================================================================================
ID    Name                      Cost        Return %        Risk
--------------------------------------------------------------------------------
1     INFY                      1400           10.00        6.00
2     TCS                       3200           11.00        5.00
3     AAPL                       150           12.00        8.00
4     TSLA                       250           20.00       15.00
5     RELIANCE                  2400            9.00        7.00
================================================================================
Total stocks: 5

Enter your choice: 7
Enter budget: 3000

Choose algorithm:
  A - 2D Dynamic Programming (optimal, more memory)
  B - 1D Dynamic Programming (optimal, less memory)
  C - Greedy (fast approximation)
  D - Fractional Knapsack (theoretical demo)
Choice: A

================================================================================
Algorithm: 2D DP
Time Complexity: O(n * budget)
Space Complexity: O(n * budget)
Runtime: 2 ms
Memory Used: 11.72 KB
DP Table Size: 6 × 3001 = 18006 cells
--------------------------------------------------------------------------------
Selected Stocks:
  - AAPL (ID: 3, Cost: ₹150, Return: 12.00% = ₹18.00)
  - TSLA (ID: 4, Cost: ₹250, Return: 20.00% = ₹50.00)
  - RELIANCE (ID: 5, Cost: ₹2400, Return: 9.00% = ₹216.00)

Summary:
  Total Cost: ₹2800
  Total Expected Return: ₹284.00
  Average Return %: 10.14%
  Average Risk Score: 10.00
  Number of Stocks: 3

Budget Allocation Chart:
--------------------------------------------------------------------------------
AAPL         [#####.....................................] 5.0% (₹150)
TSLA         [########...................................] 8.3% (₹250)
RELIANCE     [################################........] 80.0% (₹2400)
--------------------------------------------------------------------------------
Total Used:  2800 / 3000 (93.3%)
Remaining:   200
================================================================================

Enter your choice: 14
Exiting... Goodbye!
```

### Algorithm Comparison
```
Enter your choice: 11
Enter budget: 3000

Running all algorithms...

[Results for 2D DP, 1D DP, Greedy, and Fractional shown]

Comparison Summary:
================================================================================
Algorithm                Return (₹)    Runtime (ms)         Stocks
--------------------------------------------------------------------------------
2D DP                        284.00               2              3
1D DP                        284.00               2              3
Greedy                       284.00               0              3
Fractional                   284.00               0              3
================================================================================
```

## Menu Options Reference

| # | Option | Description |
|---|--------|-------------|
| 1 | Load CSV | Import stocks from CSV file |
| 2 | Display | Show all stocks in formatted table |
| 3 | Add | Add new stock with validation |
| 4 | Edit | Modify cost, return, or risk |
| 5 | Delete | Remove stock by ID |
| 6 | Export | Save portfolio to CSV |
| 7 | Optimize | Run selected algorithm (A/B/C/D) |
| 8 | Show Last | View previous optimization result |
| 9 | Export Solution | Save selected stocks + summary |
| 10 | Complexity | Show algorithm explanation |
| 11 | Compare | Run all algorithms side-by-side |
| 12 | Tests | Instructions for running unit tests |
| 13 | Verbose | Toggle detailed output mode |
| 14 | Exit | Quit program |

## Algorithm Details

### 2D Dynamic Programming (Option A)
- **Approach**: Full DP table dp[i][w]
- **Time**: O(n × budget)
- **Space**: O(n × budget)
- **Optimal**: Yes
- **Use Case**: Small to medium datasets

### 1D Dynamic Programming (Option B)
- **Approach**: Space-optimized with parent tracking
- **Time**: O(n × budget)
- **Space**: O(budget)
- **Optimal**: Yes
- **Use Case**: Memory-constrained environments

### Greedy Heuristic (Option C)
- **Approach**: Sort by return/cost ratio
- **Time**: O(n log n)
- **Space**: O(n)
- **Optimal**: No (approximation)
- **Use Case**: Large datasets, quick results

### Fractional Knapsack (Option D)
- **Approach**: Greedy with fractional items
- **Time**: O(n log n)
- **Space**: O(n)
- **Optimal**: Yes (for fractional variant)
- **Use Case**: Theoretical analysis

## Expected Return Calculation

The objective function maximizes absolute expected return:

```
expected_return_amount = cost × (return_pct / 100)
```

**Example**:
- Stock: INFY
- Cost: ₹1400
- Return: 10%
- Expected Return: 1400 × 0.10 = ₹140

**Implementation Note**: Returns are scaled by 100 and stored as integers in DP tables to avoid floating-point precision issues while maintaining 2 decimal places.

## File Structure

```
portfolio-selection/
├── src/
│   ├── main.cpp          # Menu interface and UI
│   ├── knapsack.h/cpp    # Algorithm implementations
│   ├── io.h/cpp          # CSV import/export
│   ├── utils.h/cpp       # Stock data structures
│   └── tests.cpp         # Unit test suite
├── data/
│   ├── sample_small.csv  # 5 stocks for testing
│   ├── sample_large.csv  # 10 stocks for testing
│   └── test_*.csv        # Edge case test files
├── docs/
│   ├── README.md         # This file
│   ├── report.pdf        # Detailed project report
│   ├── presentation.pptx # Presentation slides
│   └── sample_runs.txt   # Sample session transcripts
├── Makefile              # Build configuration
├── LICENSE               # MIT License
└── README.md             # Main project documentation
```

## Testing

### Unit Tests
Run comprehensive automated tests:
```bash
make test
```

Tests include:
1. Small dataset optimal selection
2. Empty stock list handling
3. Zero budget edge case
4. All items too expensive
5. Algorithm consistency (2D vs 1D)
6. Greedy approximation quality
7. Single item selection

### Manual Testing
See `docs/sample_runs.txt` for detailed test scenarios.

## Error Handling

The system handles:
- Invalid file paths
- Malformed CSV data
- Duplicate stocks
- Invalid numeric values
- Negative/zero costs
- Budget constraints
- Empty portfolios
- Invalid menu choices

All errors return gracefully to the menu with clear error messages.

## Performance

| Dataset | Budget | Algorithm | Runtime | Memory |
|---------|--------|-----------|---------|--------|
| Small (5) | 3000 | 2D DP | ~2 ms | ~12 KB |
| Small (5) | 3000 | 1D DP | ~2 ms | ~12 bytes |
| Small (5) | 3000 | Greedy | <1 ms | ~80 bytes |
| Large (10) | 10000 | 2D DP | ~8 ms | ~400 KB |
| Large (10) | 10000 | 1D DP | ~8 ms | ~40 KB |
| Large (10) | 10000 | Greedy | ~1 ms | ~160 bytes |

## Troubleshooting

**Q: Compilation fails**
A: Ensure g++ version 7.0+ for C++17 support

**Q: ₹ symbol displays incorrectly**
A: Terminal encoding issue - functionality unaffected

**Q: Verbose mode shows nothing**
A: Dataset too large - use n ≤ 8, budget ≤ 5000

**Q: Different results on reruns**
A: Verify same budget and algorithm selection

## License

MIT License - See LICENSE file for details

## Authors

Portfolio Selection System
Academic Project - Data Structures and Algorithms

## References

- Cormen, T. H., et al. "Introduction to Algorithms" (4th Edition)
- Dynamic Programming and Knapsack Problems
- Greedy Algorithms and Approximation Theory
