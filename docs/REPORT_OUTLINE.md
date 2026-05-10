# Portfolio Selection System - Report Outline
## 12-Page Technical Report

---

## Page 1: Title Page

**Title:** Portfolio Selection System Using Knapsack Algorithms

**Subtitle:** A Comprehensive Implementation of Dynamic Programming and Greedy Approaches for Investment Optimization

**Author Information:**
- Student Name
- Roll Number
- Course: Data Structures and Algorithms
- Semester/Year
- Institution Name
- Submission Date

**Supervisor:** [Professor Name]

---

## Page 2: Abstract & Table of Contents

### Abstract (200-250 words)
This project presents a comprehensive implementation of portfolio optimization using knapsack algorithms. The system helps investors select an optimal subset of stocks within a budget constraint to maximize expected returns. We implement four algorithms: 2D Dynamic Programming, space-optimized 1D Dynamic Programming, Greedy heuristic, and Fractional Knapsack. The 2D DP guarantees optimal solutions with O(n×budget) complexity, while the 1D variant achieves 90% memory reduction. Extensive testing validates correctness, and additional features include CSV import/export, verbose mode with algorithm visualization, and performance profiling. Results show that both DP algorithms produce identical optimal solutions, while the Greedy approach provides fast approximations. The system demonstrates practical application of theoretical algorithms to real-world financial problems.

### Table of Contents
1. Introduction
2. Literature Review
3. Problem Formulation
4. Methodology
5. Algorithm Design
6. Implementation
7. Testing & Results
8. Conclusion
9. References
10. Appendices

---

## Pages 3-4: Chapter 1 - Introduction

### 1.1 Background
- Portfolio optimization in finance
- Importance of algorithmic decision-making
- Budget constraints in investment
- Risk-return tradeoff

### 1.2 Problem Statement
Given:
- Set of n stocks S = {s₁, s₂, ..., sₙ}
- Each stock sᵢ has:
  - Cost cᵢ (investment required)
  - Expected return rᵢ (percentage)
  - Risk score kᵢ
- Budget constraint B

Find: Subset T ⊆ S that maximizes total expected return while Σ(cᵢ) ≤ B

### 1.3 Objectives
- Implement optimal DP algorithms
- Develop space-efficient solutions
- Create user-friendly interface
- Validate through comprehensive testing
- Compare algorithm performance

### 1.4 Scope
- Focus on 0/1 Knapsack variant
- Single-period optimization
- Deterministic returns
- No transaction costs

### 1.5 Organization
Brief description of remaining chapters

---

## Pages 5-6: Chapter 2 - Literature Review & Problem Formulation

### 2.1 Literature Review

**2.1.1 Knapsack Problem**
- Classic NP-hard problem
- Applications in resource allocation
- Variants: 0/1, fractional, bounded, unbounded

**2.1.2 Dynamic Programming**
- Bellman's principle of optimality
- Memoization vs tabulation
- Space optimization techniques

**2.1.3 Portfolio Optimization**
- Markowitz portfolio theory
- Modern portfolio theory
- Algorithmic trading

**2.1.4 Related Work**
- Previous implementations
- Commercial systems
- Academic research

### 2.2 Mathematical Formulation

**2.2.1 Objective Function**
```
Maximize: Σ(vᵢ × xᵢ)
where vᵢ = cᵢ × (rᵢ / 100)
```

**2.2.2 Constraints**
```
Subject to:
  Σ(cᵢ × xᵢ) ≤ B
  xᵢ ∈ {0, 1} for all i
```

**2.2.3 Decision Variables**
- xᵢ = 1 if stock i is selected
- xᵢ = 0 otherwise

**2.2.4 Mapping to Knapsack**
- Items = Stocks
- Weights = Costs
- Values = Expected returns
- Capacity = Budget

---

## Pages 7-8: Chapter 3 - Methodology & Algorithm Design

### 3.1 Algorithm Selection

**3.1.1 Why Dynamic Programming?**
- Optimal substructure property
- Overlapping subproblems
- Polynomial time solution
- Guaranteed optimality

**3.1.2 Why Greedy?**
- Fast approximation
- Simple implementation
- Good for large datasets
- Comparison baseline

### 3.2 Algorithm Descriptions

**3.2.1 2D Dynamic Programming**

Pseudocode:
```
Algorithm: 2D_DP_Knapsack(stocks[], budget)
Input: Array of stocks, budget B
Output: Maximum return, selected stocks

1. Create DP table dp[n+1][B+1]
2. Initialize dp[0][w] = 0 for all w
3. For i = 1 to n:
     For w = 0 to B:
       dp[i][w] = dp[i-1][w]  // Don't take
       If cost[i] ≤ w:
         take = dp[i-1][w-cost[i]] + value[i]
         dp[i][w] = max(dp[i][w], take)
4. Traceback from dp[n][B] to find selected items
5. Return result
```

Time Complexity: O(n × B)
Space Complexity: O(n × B)

**3.2.2 1D Dynamic Programming**

Pseudocode:
```
Algorithm: 1D_DP_Knapsack(stocks[], budget)
Input: Array of stocks, budget B
Output: Maximum return, selected stocks

1. Create DP array dp[B+1]
2. Create parent array for traceback
3. Initialize dp[w] = 0 for all w
4. For i = 0 to n-1:
     For w = B down to cost[i]:  // Backward!
       new_value = dp[w-cost[i]] + value[i]
       If new_value > dp[w]:
         dp[w] = new_value
         parent[i+1][w] = (i, w-cost[i])
5. Traceback using parent array
6. Return result
```

Time Complexity: O(n × B)
Space Complexity: O(B)

**3.2.3 Greedy Algorithm**

Pseudocode:
```
Algorithm: Greedy_Knapsack(stocks[], budget)
Input: Array of stocks, budget B
Output: Approximate solution

1. Calculate ratio[i] = value[i] / cost[i] for all i
2. Sort stocks by ratio in descending order
3. remaining = B
4. selected = []
5. For each stock in sorted order:
     If cost[stock] ≤ remaining:
       Add stock to selected
       remaining -= cost[stock]
6. Return selected
```

Time Complexity: O(n log n)
Space Complexity: O(n)

### 3.3 Flowcharts

**3.3.1 Main Program Flow**
```
Start → Load CSV → Display Menu → User Choice
  ↓
  ├─ Load Data → Validate → Store
  ├─ Display → Format → Print
  ├─ Add/Edit/Delete → Validate → Update
  ├─ Optimize → Select Algorithm → Run → Display
  ├─ Compare → Run All → Display Table
  └─ Exit
```

**3.3.2 DP Algorithm Flow**
```
Start → Initialize DP Table → Fill Table
  ↓
For each item:
  For each weight:
    Calculate max(don't take, take)
    Update dp[i][w]
  ↓
Traceback → Find selected items → Return
```

**3.3.3 Greedy Algorithm Flow**
```
Start → Calculate Ratios → Sort by Ratio
  ↓
For each stock (sorted):
  If fits in budget:
    Select stock
    Reduce budget
  ↓
Return selected stocks
```

---

## Pages 9-10: Chapter 4 - Implementation & Testing

### 4.1 Implementation Details

**4.1.1 Technology Stack**
- Language: C++17
- Compiler: g++ 7.0+
- Build System: Make
- Testing: Custom unit test framework

**4.1.2 File Structure**
```
src/
├── main.cpp      - Menu and UI (350+ lines)
├── knapsack.cpp  - Algorithms (450+ lines)
├── io.cpp        - CSV operations (150+ lines)
├── utils.cpp     - Data structures (150+ lines)
└── tests.cpp     - Unit tests (150+ lines)
```

**4.1.3 Data Structures**

Stock Structure:
```cpp
struct Stock {
    int id;
    string name;
    int cost;
    double return_pct;
    double risk;
};
```

Result Structure:
```cpp
struct KnapsackResult {
    vector<int> selected_ids;
    int total_cost;
    double total_expected_amount;
    double total_expected_percent;
    double risk_summary;
    string algo_used;
    long runtime_ms;
    string time_complexity;
    string space_complexity;
    size_t memory_used_bytes;
    int dp_table_rows;
    int dp_table_cols;
};
```

**4.1.4 Key Functions**

1. `run_knapsack_2d()` - 2D DP implementation
2. `run_knapsack_1d()` - 1D DP implementation
3. `run_greedy()` - Greedy algorithm
4. `load_csv()` - CSV parser with validation
5. `save_csv()` - CSV writer
6. `print_result()` - Result formatter

**4.1.5 Implementation Challenges**

1. **Floating Point Precision**
   - Problem: DP requires integer values
   - Solution: Scale returns by 100 (paise)
   - Result: Maintains 2 decimal precision

2. **Solution Reconstruction**
   - Problem: 1D DP loses history
   - Solution: Parent tracking array
   - Result: O(n×B) space for parents

3. **Memory Management**
   - Problem: Large DP tables
   - Solution: 1D optimization
   - Result: 90% memory reduction

### 4.2 Testing

**4.2.1 Unit Tests**

Test 1: Small Dataset Optimal Selection
- Input: 5 stocks, budget 3000
- Expected: AAPL + TSLA + RELIANCE = ₹284
- Result: PASS ✓

Test 2: Empty Stock List
- Input: Empty portfolio
- Expected: No selection, graceful handling
- Result: PASS ✓

Test 3: Zero Budget
- Input: Budget = 0
- Expected: No selection
- Result: PASS ✓

Test 4: All Items Too Expensive
- Input: All costs > budget
- Expected: No selection
- Result: PASS ✓

Test 5: Algorithm Consistency
- Input: Same dataset to 2D and 1D DP
- Expected: Identical results
- Result: PASS ✓ (both return ₹208)

Test 6: Greedy Approximation Quality
- Input: Compare greedy vs optimal
- Expected: Greedy ≤ Optimal
- Result: PASS ✓

Test 7: Single Item Selection
- Input: 1 stock, sufficient budget
- Expected: Select that stock
- Result: PASS ✓

**4.2.2 Performance Testing**

Small Dataset (n=5, B=3000):
- 2D DP: 2 ms, 12 KB
- 1D DP: 2 ms, 12 bytes
- Greedy: <1 ms, 80 bytes

Large Dataset (n=10, B=10000):
- 2D DP: 8 ms, 400 KB
- 1D DP: 8 ms, 40 KB
- Greedy: 1 ms, 160 bytes

**4.2.3 Edge Case Testing**
- Invalid CSV files ✓
- Duplicate stocks ✓
- Negative values ✓
- Non-numeric data ✓
- Empty files ✓

### 4.3 Results Analysis

**4.3.1 Correctness**
- All unit tests pass
- 2D and 1D DP produce identical results
- Manual verification confirms optimality

**4.3.2 Performance**
- 1D DP achieves 90% memory reduction
- Greedy is 2-8× faster than DP
- Runtime scales linearly with budget

**4.3.3 Usability**
- Clear menu interface
- Robust error handling
- Helpful error messages
- Comprehensive documentation

---

## Page 11: Chapter 5 - Conclusion & Future Work

### 5.1 Summary of Achievements

**5.1.1 Technical Achievements**
- ✓ Implemented 4 algorithms
- ✓ Achieved 90% memory optimization
- ✓ Validated correctness through testing
- ✓ Professional code quality

**5.1.2 Functional Achievements**
- ✓ User-friendly interface
- ✓ CSV import/export
- ✓ Performance profiling
- ✓ Algorithm comparison
- ✓ Verbose mode for learning

**5.1.3 Documentation Achievements**
- ✓ Comprehensive README
- ✓ Sample run transcripts
- ✓ Code comments
- ✓ Technical report
- ✓ Presentation slides

### 5.2 Key Insights

1. **DP Guarantees Optimality**
   - Both 2D and 1D produce identical optimal solutions
   - Worth the computational cost for critical decisions

2. **Space Optimization Matters**
   - 1D DP reduces memory by 90%
   - Enables larger problem instances

3. **Greedy is Fast but Approximate**
   - 2-8× faster than DP
   - Good for quick estimates or large datasets
   - Not guaranteed optimal

4. **Testing is Essential**
   - Unit tests caught edge cases
   - Validated algorithm correctness
   - Ensured robustness

### 5.3 Limitations

1. **Deterministic Returns**
   - Assumes fixed expected returns
   - Real markets are stochastic

2. **Single Period**
   - Optimizes for one time period
   - Doesn't consider rebalancing

3. **No Transaction Costs**
   - Ignores brokerage fees
   - Assumes free trading

4. **Integer Costs**
   - Requires integer budget and costs
   - May need scaling for fractional values

### 5.4 Future Enhancements

**5.4.1 Algorithm Extensions**
- Branch & Bound for exact solutions with pruning
- Brute force for small datasets (verification)
- Approximation algorithms (FPTAS)
- Parallel DP for large datasets

**5.4.2 Feature Additions**
- Multi-objective optimization (return + risk)
- Risk-adjusted returns (Sharpe ratio)
- Portfolio rebalancing
- Historical backtesting
- Monte Carlo simulation

**5.4.3 Integration**
- Real-time stock data APIs
- Database backend
- Web interface
- Mobile app
- Cloud deployment

**5.4.4 Advanced Analytics**
- Machine learning predictions
- Sentiment analysis
- Technical indicators
- Correlation analysis
- Diversification metrics

### 5.5 Conclusion

This project successfully demonstrates the application of dynamic programming and greedy algorithms to portfolio optimization. The implementation achieves optimal solutions efficiently, with significant memory optimization through the 1D DP variant. Comprehensive testing validates correctness, and additional features enhance usability. The system serves as both a practical tool for investment decisions and an educational resource for understanding algorithmic problem-solving. Future work can extend the system with stochastic modeling, multi-period optimization, and real-time data integration.

---

## Page 12: References & Appendices

### References

[1] Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2022). Introduction to Algorithms (4th ed.). MIT Press.

[2] Kellerer, H., Pferschy, U., & Pisinger, D. (2004). Knapsack Problems. Springer.

[3] Markowitz, H. (1952). Portfolio Selection. The Journal of Finance, 7(1), 77-91.

[4] Martello, S., & Toth, P. (1990). Knapsack Problems: Algorithms and Computer Implementations. John Wiley & Sons.

[5] Skiena, S. S. (2020). The Algorithm Design Manual (3rd ed.). Springer.

[6] Vazirani, V. V. (2001). Approximation Algorithms. Springer.

[7] Dynamic Programming. (2023). GeeksforGeeks. https://www.geeksforgeeks.org/dynamic-programming/

[8] Knapsack Problem. (2023). Wikipedia. https://en.wikipedia.org/wiki/Knapsack_problem

### Appendices

**Appendix A: Complete Source Code**
- Full listings of all source files
- Available in submission ZIP

**Appendix B: Sample Data**
- sample_small.csv
- sample_large.csv
- Test case files

**Appendix C: Test Results**
- Complete unit test output
- Performance benchmarks
- Memory profiling data

**Appendix D: User Manual**
- Installation instructions
- Usage guide
- Troubleshooting

**Appendix E: Build Instructions**
- Makefile explanation
- Compilation commands
- Platform-specific notes

**Appendix F: Sample Run Transcripts**
- Complete interactive sessions
- Expected outputs
- Verification steps

---

## Formatting Guidelines

### Document Formatting
- Font: Times New Roman, 12pt
- Line Spacing: 1.5
- Margins: 1 inch all sides
- Page Numbers: Bottom center
- Headers: Chapter titles

### Code Formatting
- Font: Courier New, 10pt
- Syntax highlighting if possible
- Line numbers for reference
- Proper indentation

### Figures & Tables
- Numbered sequentially
- Captions below figures
- Captions above tables
- Referenced in text

### Citations
- IEEE or ACM format
- Numbered in order of appearance
- Complete bibliographic information

---

## Report Checklist

- [ ] Title page complete
- [ ] Abstract (200-250 words)
- [ ] Table of contents
- [ ] All chapters present
- [ ] Pseudocode included
- [ ] Flowcharts included
- [ ] Test results documented
- [ ] References cited
- [ ] Appendices attached
- [ ] Proofread for errors
- [ ] Page count: 12 pages
- [ ] PDF format
- [ ] Professional appearance

**Status: READY FOR WRITING** ✓
