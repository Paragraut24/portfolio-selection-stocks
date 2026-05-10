#include "knapsack_1d.h"
#include <algorithm>
#include <chrono>
#include <cmath>

OptimizationResult knapsack_1d(const std::vector<Stock>& stocks, int budget) {
    auto start = std::chrono::high_resolution_clock::now();
    
    OptimizationResult result;
    result.algo_used = "1D DP";
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
    
    // 1D DP array
    std::vector<int> dp(budget + 1, 0);
    
    // Calculate memory usage
    result.memory_used_bytes = (budget + 1) * sizeof(int);
    result.dp_table_rows = 1;
    result.dp_table_cols = budget + 1;
    
    // Fill DP array (0/1 Knapsack - each stock at most once)
    std::vector<std::vector<int>> parent(n + 1, std::vector<int>(budget + 1, -1));
    
    for (int i = 0; i < n; i++) {
        // Traverse backwards to prevent using same item multiple times
        for (int w = budget; w >= stocks[i].cost; w--) {
            int new_value = dp[w - stocks[i].cost] + scaled_returns[i];
            if (new_value > dp[w]) {
                dp[w] = new_value;
                parent[i][w] = i;  // Mark that item i was used for weight w
            }
        }
    }
    
    // Traceback to find selected items
    int w = budget;
    std::vector<bool> used(n, false);
    
    // Find which items were selected
    for (int i = n - 1; i >= 0; i--) {
        if (parent[i][w] == i && !used[i]) {
            result.selected_ids.push_back(stocks[i].id);
            result.fractions.push_back(1.0);
            result.total_cost += stocks[i].cost;
            result.total_expected_amount += calculate_expected_return(stocks[i]);
            result.risk_summary += stocks[i].risk;
            w -= stocks[i].cost;
            used[i] = true;
        }
    }
    
    // Reverse to get original order
    std::reverse(result.selected_ids.begin(), result.selected_ids.end());
    
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
