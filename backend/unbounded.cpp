#include "unbounded.h"
#include <algorithm>
#include <chrono>
#include <cmath>

OptimizationResult unbounded_knapsack(const std::vector<Stock>& stocks, int budget) {
    auto start = std::chrono::high_resolution_clock::now();
    
    OptimizationResult result;
    result.algo_used = "Unbounded";
    result.time_complexity = "O(n * budget)";
    result.space_complexity = "O(budget)";
    result.total_cost = 0;
    result.total_expected_amount = 0.0;
    result.total_expected_percent = 0.0;
    result.risk_summary = 0.0;
    result.memory_used_bytes = 0;
    result.dp_table_rows = 0;
    result.dp_table_cols = 0;
    
    if (stocks.empty() || budget <= 0) {
        auto end = std::chrono::high_resolution_clock::now();
        result.runtime_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        return result;
    }
    
    int n = stocks.size();
    
    // Scale expected returns to integers
    std::vector<int> scaled_returns(n);
    for (int i = 0; i < n; i++) {
        scaled_returns[i] = static_cast<int>(std::round(calculate_expected_return(stocks[i]) * 100));
    }
    
    // 1D DP array for unbounded knapsack
    std::vector<int> dp(budget + 1, 0);
    
    // Calculate memory usage
    result.memory_used_bytes = (budget + 1) * sizeof(int);
    result.dp_table_rows = 1;
    result.dp_table_cols = budget + 1;
    
    // Fill DP array (Unbounded - allows multiple purchases)
    for (int i = 0; i < n; i++) {
        // Traverse forwards to allow using same item multiple times
        for (int w = stocks[i].cost; w <= budget; w++) {
            int new_value = dp[w - stocks[i].cost] + scaled_returns[i];
            if (new_value > dp[w]) {
                dp[w] = new_value;
            }
        }
    }
    
    // Traceback to find selected items (with quantities)
    int w = budget;
    std::vector<int> quantities(n, 0);
    
    // Work backwards to reconstruct solution
    while (w > 0) {
        bool found = false;
        // Try each item to see which one was used
        for (int i = 0; i < n; i++) {
            if (stocks[i].cost <= w) {
                int value_with_item = dp[w - stocks[i].cost] + scaled_returns[i];
                if (value_with_item == dp[w]) {
                    // This item was used
                    quantities[i]++;
                    w -= stocks[i].cost;
                    found = true;
                    break;
                }
            }
        }
        if (!found) break;  // No more items can be added
    }
    
    // Build result from quantities
    for (int i = 0; i < n; i++) {
        for (int q = 0; q < quantities[i]; q++) {
            result.selected_ids.push_back(stocks[i].id);
            result.fractions.push_back(1.0);
            result.total_cost += stocks[i].cost;
            result.total_expected_amount += calculate_expected_return(stocks[i]);
            result.risk_summary += stocks[i].risk;
        }
    }
    
    // Calculate average return percentage
    if (result.total_cost > 0) {
        result.total_expected_percent = (result.total_expected_amount / result.total_cost) * 100.0;
    }
    
    // Calculate average risk
    if (!result.selected_ids.empty()) {
        result.risk_summary /= result.selected_ids.size();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    result.runtime_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    return result;
}
