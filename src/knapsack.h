#ifndef KNAPSACK_H
#define KNAPSACK_H

#include "utils.h"
#include <vector>
#include <string>

struct KnapsackResult {
    std::vector<int> selected_ids;
    int total_cost;
    double total_expected_amount;
    double total_expected_percent;
    double risk_summary;
    std::string algo_used;
    long runtime_ms;
    
    // Complexity information
    std::string time_complexity;
    std::string space_complexity;
    
    // Profiling information
    size_t memory_used_bytes;
    int dp_table_rows;
    int dp_table_cols;
};

// Calculate expected return amount for a stock
double calculate_expected_return(const Stock& stock);

// 2D Dynamic Programming (0/1 Knapsack)
KnapsackResult run_knapsack_2d(const std::vector<Stock>& stocks, int budget, bool verbose = false);

// 1D Space-Optimized Dynamic Programming with traceback
KnapsackResult run_knapsack_1d(const std::vector<Stock>& stocks, int budget, bool verbose = false);

// Greedy heuristic (by return/cost ratio)
KnapsackResult run_greedy(const std::vector<Stock>& stocks, int budget, bool verbose = false);

// Fractional knapsack (demonstration only)
KnapsackResult run_fractional(const std::vector<Stock>& stocks, int budget, bool verbose = false);

// Get complexity strings
std::string get_time_complexity(const std::string& algo);
std::string get_space_complexity(const std::string& algo);

// Explain algorithm complexity and approach
void explain_algorithm(const std::string& algo);

#endif
