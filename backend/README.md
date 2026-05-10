# Portfolio Selection Backend System

A modular C++ backend system for portfolio optimization using multiple algorithms including knapsack variants and graph-based diversification.

## Architecture

### Modular Design

The system is organized into separate, reusable components:

```
backend/
├── common.h/cpp           # Shared data structures and utilities
├── csv_io.h/cpp           # CSV input/output operations
├── knapsack_2d.h/cpp      # 2D Dynamic Programming
├── knapsack_1d.h/cpp      # 1D Space-Optimized DP
├── greedy.h/cpp           # Greedy heuristic algorithm
├── fractional.h/cpp       # Fractional knapsack algorithm
├── mst.h/cpp              # Minimum Spanning Tree for diversification
├── main.cpp               # Main orchestration logic
├── Makefile               # Build configuration
├── stocks.csv             # Sample input data
└── README.md              # This file
```

## Algorithms

### 1. 2D Knapsack (knapsack_2d.cpp)
- **Algorithm**: 2D Dynamic Programming (0/1 Knapsack)
- **Time Complexity**: O(n × budget)
- **Space Complexity**: O(n × budget)
- **Optimal**: Yes
- **Use Case**: Small to medium datasets where memory is not constrained

### 2. 1D Knapsack (knapsack_1d.cpp)
- **Algorithm**: 1D Space-Optimized Dynamic Programming
- **Time Complexity**: O(n × budget)
- **Space Complexity**: O(budget)
- **Optimal**: Yes
- **Use Case**: Memory-constrained environments, large budgets

### 3. Greedy (greedy.cpp)
- **Algorithm**: Greedy by return/cost ratio
- **Time Complexity**: O(n log n)
- **Space Complexity**: O(n)
- **Optimal**: No (approximation)
- **Use Case**: Large datasets, quick results needed

### 4. Fractional Knapsack (fractional.cpp)
- **Algorithm**: Fractional knapsack with greedy approach
- **Time Complexity**: O(n log n)
- **Space Complexity**: O(n)
- **Optimal**: Yes (for fractional variant)
- **Use Case**: Theoretical analysis, upper bound calculation

### 5. MST Diversification (mst.cpp)
- **Algorithm**: Kruskal's Minimum Spanning Tree
- **Time Complexity**: O(E log E) where E = n(n-1)/2
- **Space Complexity**: O(V + E)
- **Purpose**: Portfolio diversification based on stock similarity
- **Use Case**: Selecting diverse stocks to minimize correlation risk

## Data Structures

### Stock
```cpp
struct Stock {
    int id;
    std::string name;
    int cost;
    double return_pct;
    double risk;
};
```

### OptimizationResult
```cpp
struct OptimizationResult {
    std::vector<int> selected_ids;
    int total_cost;
    double total_expected_amount;
    double total_expected_percent;
    double risk_summary;
    std::string algo_used;
    long runtime_ms;
    // ... complexity and profiling data
};
```

### MSTResult
```cpp
struct MSTResult {
    std::vector<Edge> selected_edges;
    std::vector<int> diversified_stock_ids;
    double total_weight;
    long runtime_ms;
    std::string algo_used;
};
```

## CSV Pipeline

### Input: stocks.csv
```csv
id,name,cost,expected_return_percent,risk_score
1,INFY,1400,10.0,6
2,TCS,3200,11.0,5
```

### Outputs

#### 1. portfolio_results_[algorithm].csv
Selected stocks for each algorithm:
```csv
algorithm,stock_id,stock_name,cost,return_pct,risk,expected_return
2D DP,3,AAPL,150,12.00,8.00,18.00
```

#### 2. algorithm_comparison.csv
Performance comparison across all algorithms:
```csv
algorithm,total_cost,total_expected_return,avg_return_pct,avg_risk,num_stocks,runtime_ms,time_complexity,space_complexity,memory_bytes
2D DP,2800,284.00,10.14,10.00,3,2,O(n * budget),O(n * budget),60024
```

#### 3. diversification_output.csv
Diversified stock selection from MST:
```csv
stock_id,stock_name,cost,return_pct,risk
1,INFY,1400,10.00,6.00
```

## Build and Run

### Prerequisites
- C++17 compatible compiler (g++ 7.0+, clang++ 5.0+)
- Make (optional, for using Makefile)

### Compilation

Using Makefile:
```bash
make
```

Manual compilation:
```bash
g++ -std=c++17 -O2 -Wall -Wextra -o portfolio_backend \
    main.cpp common.cpp csv_io.cpp knapsack_2d.cpp knapsack_1d.cpp \
    greedy.cpp fractional.cpp mst.cpp
```

### Running

Default parameters (stocks.csv, budget=5000):
```bash
make run
```

Or directly:
```bash
./portfolio_backend
```

Custom parameters:
```bash
./portfolio_backend input.csv 10000
```

Using Makefile with custom parameters:
```bash
make run-custom INPUT=mydata.csv BUDGET=8000
```

### Cleaning

Remove all build artifacts and outputs:
```bash
make clean
```

Remove only CSV outputs:
```bash
make clean-output
```

## Usage Example

```bash
$ make run

Portfolio Selection Backend System
===================================

Input File: stocks.csv
Budget: Rs.5000

Step 1: Loading stocks...
Loaded 10 stocks from 'stocks.csv'

Step 2: Running optimization algorithms...

Running 2D Knapsack...
========================================
Algorithm: 2D DP
========================================
Selected Stocks: 4
Total Cost: Rs.4950
Total Expected Return: Rs.567.50
Average Return %: 11.47%
Average Risk: 9.25
Runtime: 3 ms
Time Complexity: O(n * budget)
Space Complexity: O(n * budget)
========================================
Saved portfolio results to 'portfolio_results_2d.csv'

[... similar output for other algorithms ...]

Step 3: Generating algorithm comparison...
Saved algorithm comparison to 'algorithm_comparison.csv'

Step 4: Running portfolio diversification (MST)...
========================================
Portfolio Diversification (MST)
========================================
Algorithm: Kruskal MST
Diversified Stocks: 10
Total MST Weight: 2.45
Runtime: 1 ms
========================================
Saved diversification output to 'diversification_output.csv'

===================================
Processing Complete!
===================================
```

## Key Features

### Modularity
- Each algorithm in separate file
- Clean interfaces with header files
- Reusable components
- Easy to extend with new algorithms

### CSV-Driven
- Reads input from CSV
- Generates multiple CSV outputs
- Easy integration with other tools
- No interactive UI (backend only)

### Performance Profiling
- Runtime measurement for each algorithm
- Memory usage tracking
- Complexity analysis
- Comparison metrics

### Diversification
- MST-based stock selection
- Minimizes correlation between stocks
- Uses risk and return similarity
- Kruskal's algorithm with Union-Find

## Algorithm Details

### Expected Return Calculation
```cpp
expected_return = cost × (return_pct / 100)
```

### Stock Similarity (for MST)
```cpp
similarity = 1.0 / (1.0 + risk_diff + return_diff)
```
Lower similarity = more diverse = better for MST

### Union-Find (for MST)
- Path compression optimization
- Union by rank
- O(α(n)) amortized time per operation

## Extending the System

### Adding a New Algorithm

1. Create header file: `new_algo.h`
```cpp
#ifndef NEW_ALGO_H
#define NEW_ALGO_H
#include "common.h"
OptimizationResult new_algorithm(const std::vector<Stock>& stocks, int budget);
#endif
```

2. Implement: `new_algo.cpp`
3. Add to `main.cpp`
4. Update `Makefile` SOURCES

### Adding New Output Format

1. Add function to `csv_io.h/cpp`
2. Call from `main.cpp`

## Testing

Create test input files with edge cases:
- Empty portfolio
- Single stock
- All stocks too expensive
- Budget = 0
- Large datasets

Run and verify outputs match expected results.

## Performance Benchmarks

Typical performance on modern hardware:

| Dataset Size | Budget | 2D DP | 1D DP | Greedy | MST |
|--------------|--------|-------|-------|--------|-----|
| 10 stocks    | 5000   | 3 ms  | 3 ms  | <1 ms  | 1 ms |
| 50 stocks    | 10000  | 45 ms | 42 ms | 2 ms   | 8 ms |
| 100 stocks   | 20000  | 180 ms| 175 ms| 5 ms   | 35 ms |

## License

MIT License - Free to use, modify, and distribute

## Future Enhancements

- Branch & Bound algorithm
- Genetic algorithm for large-scale optimization
- Parallel processing support
- JSON output format
- Configuration file support
- Logging system
- Unit test suite
- Benchmark suite
