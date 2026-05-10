# Backend Architecture

## System Overview

```
┌─────────────────────────────────────────────────────────────┐
│                    Portfolio Backend System                  │
└─────────────────────────────────────────────────────────────┘

Input: stocks.csv
    │
    ▼
┌─────────────────┐
│   CSV Reader    │  (csv_io.cpp)
│  load_stocks()  │
└────────┬────────┘
         │
         ▼
┌─────────────────────────────────────────────────────────────┐
│              Stock Data (vector<Stock>)                      │
└─────────────────────────────────────────────────────────────┘
         │
         ├──────────────┬──────────────┬──────────────┬────────────────┐
         ▼              ▼              ▼              ▼                ▼
┌──────────────┐ ┌──────────────┐ ┌──────────────┐ ┌──────────────┐ ┌──────────────┐
│  2D Knapsack │ │  1D Knapsack │ │    Greedy    │ │  Fractional  │ │     MST      │
│knapsack_2d() │ │knapsack_1d() │ │   greedy()   │ │ fractional() │ │portfolio_mst()│
└──────┬───────┘ └──────┬───────┘ └──────┬───────┘ └──────┬───────┘ └──────┬───────┘
       │                │                │                │                │
       ▼                ▼                ▼                ▼                ▼
┌──────────────┐ ┌──────────────┐ ┌──────────────┐ ┌──────────────┐ ┌──────────────┐
│Optimization  │ │Optimization  │ │Optimization  │ │Optimization  │ │  MSTResult   │
│   Result     │ │   Result     │ │   Result     │ │   Result     │ │              │
└──────┬───────┘ └──────┬───────┘ └──────┬───────┘ └──────┬───────┘ └──────┬───────┘
       │                │                │                │                │
       └────────────────┴────────────────┴────────────────┴────────────────┘
                                    │
                                    ▼
                        ┌─────────────────────┐
                        │    CSV Writers      │
                        │    (csv_io.cpp)     │
                        └──────────┬──────────┘
                                   │
         ┌─────────────────────────┼─────────────────────────┐
         │                         │                         │
         ▼                         ▼                         ▼
┌──────────────────┐  ┌──────────────────────┐  ┌──────────────────────┐
│portfolio_results │  │algorithm_comparison  │  │diversification_output│
│   _[algo].csv    │  │       .csv           │  │       .csv           │
└──────────────────┘  └──────────────────────┘  └──────────────────────┘
```

## Module Dependencies

```
main.cpp
  │
  ├─→ csv_io.h
  │     └─→ common.h
  │
  ├─→ knapsack_2d.h
  │     └─→ common.h
  │
  ├─→ knapsack_1d.h
  │     └─→ common.h
  │
  ├─→ greedy.h
  │     └─→ common.h
  │
  ├─→ fractional.h
  │     └─→ common.h
  │
  └─→ mst.h
        └─→ common.h

common.h (no dependencies)
```

## Data Flow

### 1. Input Phase
```
stocks.csv → load_stocks_csv() → vector<Stock>
```

### 2. Processing Phase
```
vector<Stock> + budget → Algorithm → OptimizationResult
vector<Stock>          → MST       → MSTResult
```

### 3. Output Phase
```
OptimizationResult → save_portfolio_results_csv() → portfolio_results_[algo].csv
vector<OptimizationResult> → save_algorithm_comparison_csv() → algorithm_comparison.csv
MSTResult → save_diversification_output_csv() → diversification_output.csv
```

## Algorithm Execution Flow

### Knapsack Algorithms (2D, 1D, Greedy, Fractional)

```
Start
  │
  ├─→ Initialize result structure
  │
  ├─→ Validate input (stocks not empty, budget > 0)
  │
  ├─→ Start timer
  │
  ├─→ Execute algorithm logic
  │     │
  │     ├─→ 2D DP: Build 2D table, traceback
  │     ├─→ 1D DP: Build 1D array with parent tracking
  │     ├─→ Greedy: Sort by ratio, select greedily
  │     └─→ Fractional: Sort by ratio, allow fractions
  │
  ├─→ Calculate metrics
  │     ├─→ Total cost
  │     ├─→ Total expected return
  │     ├─→ Average return %
  │     └─→ Average risk
  │
  ├─→ Stop timer
  │
  └─→ Return OptimizationResult
```

### MST Algorithm

```
Start
  │
  ├─→ Initialize result structure
  │
  ├─→ Validate input (at least 2 stocks)
  │
  ├─→ Start timer
  │
  ├─→ Build complete graph
  │     └─→ For each pair: calculate similarity weight
  │
  ├─→ Sort edges by weight (ascending)
  │
  ├─→ Initialize Union-Find
  │
  ├─→ Kruskal's algorithm
  │     └─→ For each edge: if no cycle, add to MST
  │
  ├─→ Extract unique stock IDs from MST edges
  │
  ├─→ Stop timer
  │
  └─→ Return MSTResult
```

## Data Structures

### Stock
```cpp
struct Stock {
    int id;              // Unique identifier
    string name;         // Stock name
    int cost;            // Investment cost
    double return_pct;   // Expected return %
    double risk;         // Risk score
};
```

### OptimizationResult
```cpp
struct OptimizationResult {
    vector<int> selected_ids;        // Selected stock IDs
    int total_cost;                  // Total investment
    double total_expected_amount;    // Total return amount
    double total_expected_percent;   // Average return %
    double risk_summary;             // Average risk
    string algo_used;                // Algorithm name
    long runtime_ms;                 // Execution time
    string time_complexity;          // Big-O time
    string space_complexity;         // Big-O space
    size_t memory_used_bytes;        // Memory usage
    int dp_table_rows;               // DP table rows
    int dp_table_cols;               // DP table cols
};
```

### Edge (for MST)
```cpp
struct Edge {
    int stock1_id;       // First stock
    int stock2_id;       // Second stock
    double weight;       // Similarity measure
};
```

### MSTResult
```cpp
struct MSTResult {
    vector<Edge> selected_edges;           // MST edges
    vector<int> diversified_stock_ids;     // Unique stocks
    double total_weight;                   // Sum of weights
    long runtime_ms;                       // Execution time
    string algo_used;                      // "Kruskal MST"
};
```

## Algorithm Complexity

| Algorithm | Time Complexity | Space Complexity | Optimal |
|-----------|----------------|------------------|---------|
| 2D DP     | O(n × W)       | O(n × W)         | Yes     |
| 1D DP     | O(n × W)       | O(W)             | Yes     |
| Greedy    | O(n log n)     | O(n)             | No      |
| Fractional| O(n log n)     | O(n)             | Yes*    |
| MST       | O(E log E)     | O(V + E)         | N/A     |

Where:
- n = number of stocks
- W = budget
- E = n(n-1)/2 (complete graph edges)
- V = n (vertices)
- *Optimal for fractional variant only

## MST Similarity Calculation

```
For two stocks s1 and s2:

risk_diff = |s1.risk - s2.risk|
return_diff = |s1.return_pct - s2.return_pct|

similarity = 1.0 / (1.0 + risk_diff + return_diff)

Lower similarity → More diverse → Better for MST
```

### Example
```
Stock A: risk=5, return=10%
Stock B: risk=15, return=20%

risk_diff = |5 - 15| = 10
return_diff = |10 - 20| = 10
similarity = 1.0 / (1.0 + 10 + 10) = 0.0476

Stock C: risk=6, return=11%
Stock D: risk=7, return=12%

risk_diff = |6 - 7| = 1
return_diff = |11 - 12| = 1
similarity = 1.0 / (1.0 + 1 + 1) = 0.333

A-B more diverse (lower similarity) than C-D
```

## Union-Find Operations

```
make_set(x):
    parent[x] = x
    rank[x] = 0

find(x):
    if parent[x] ≠ x:
        parent[x] = find(parent[x])  // Path compression
    return parent[x]

union(x, y):
    root_x = find(x)
    root_y = find(y)
    
    if root_x = root_y:
        return false  // Already in same set
    
    // Union by rank
    if rank[root_x] < rank[root_y]:
        parent[root_x] = root_y
    else if rank[root_x] > rank[root_y]:
        parent[root_y] = root_x
    else:
        parent[root_y] = root_x
        rank[root_x]++
    
    return true
```

## CSV Format Specifications

### Input: stocks.csv
```
Header: id,name,cost,expected_return_percent,risk_score
Data:   integer,string,integer,double,double
```

### Output: portfolio_results_[algo].csv
```
Header: algorithm,stock_id,stock_name,cost,return_pct,risk,expected_return
Data:   string,integer,string,integer,double,double,double
```

### Output: algorithm_comparison.csv
```
Header: algorithm,total_cost,total_expected_return,avg_return_pct,avg_risk,
        num_stocks,runtime_ms,time_complexity,space_complexity,memory_bytes
Data:   string,integer,double,double,double,integer,long,string,string,size_t
```

### Output: diversification_output.csv
```
Header: stock_id,stock_name,cost,return_pct,risk
Data:   integer,string,integer,double,double
```

## Execution Sequence

```
1. main() starts
2. Parse command line arguments (input file, budget)
3. Load stocks from CSV
4. Run 2D Knapsack → Save results
5. Run 1D Knapsack → Save results
6. Run Greedy → Save results
7. Run Fractional → Save results
8. Collect all results → Save comparison
9. Run MST → Save diversification
10. Print summary
11. Exit
```

## Error Handling

```
Input Validation:
  ├─→ File not found → Error message, return empty
  ├─→ Malformed CSV → Skip line, warning
  ├─→ Invalid numbers → Skip line, warning
  ├─→ Empty portfolio → Return empty result
  └─→ Budget ≤ 0 → Return empty result

Algorithm Execution:
  ├─→ No stocks → Return empty result
  ├─→ All stocks too expensive → Return empty result
  └─→ Single stock for MST → Return that stock

Output:
  ├─→ Cannot create file → Error message
  └─→ Write successful → Confirmation message
```

## Performance Characteristics

### Memory Usage
```
2D DP:     (n+1) × (W+1) × sizeof(int) ≈ 4nW bytes
1D DP:     (W+1) × sizeof(int) ≈ 4W bytes
Greedy:    n × sizeof(pair<int,double>) ≈ 16n bytes
Fractional: n × sizeof(pair<int,double>) ≈ 16n bytes
MST:       n(n-1)/2 × sizeof(Edge) ≈ 12n² bytes
```

### Typical Runtime (10 stocks, budget 5000)
```
2D DP:     < 1 ms
1D DP:     < 1 ms
Greedy:    < 1 ms
Fractional: < 1 ms
MST:       < 1 ms
Total:     < 5 ms
```

## Extension Points

### Adding New Algorithm
1. Create `new_algo.h` with function signature
2. Implement in `new_algo.cpp`
3. Include in `main.cpp`
4. Add to compilation command
5. Call in main execution sequence

### Adding New Output Format
1. Add function to `csv_io.h`
2. Implement in `csv_io.cpp`
3. Call from `main.cpp` after algorithm execution

### Modifying Similarity Metric
1. Edit `calculate_stock_similarity()` in `mst.cpp`
2. Experiment with different formulas
3. Test impact on diversification

## Testing Strategy

### Unit Tests
- Test each algorithm with known inputs
- Verify optimal solutions for small cases
- Check edge cases (empty, single stock, etc.)

### Integration Tests
- Run full pipeline with sample data
- Verify all output files created
- Check CSV format correctness

### Performance Tests
- Measure runtime for various dataset sizes
- Track memory usage
- Compare against theoretical complexity

## Conclusion

The modular architecture provides:
- Clear separation of concerns
- Easy to understand and maintain
- Simple to extend with new algorithms
- Efficient execution pipeline
- Comprehensive output generation
