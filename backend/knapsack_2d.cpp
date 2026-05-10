#include "knapsack_2d.h"
#include <algorithm>
#include <chrono>
#include <cmath>

OptimizationResult knapsack_2d(const std::vector<Stock>& stocks, int budget) {
    auto start = std::chrono::high_resolution_clock::now();
    
    OptimizationResult result;
    result.algo_used = "2D DP";
    result.time_complexity = "O(n * budget)";
    result.space_complexity = "O(n * budget)";
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
    
    // Scale expected returns to integers (multiply by 100 to preserve 2 decimal places)
    std::vector<int> scaled_returns(n);
    for (int i = 0; i < n; i++) {
        scaled_returns[i] = static_cast<int>(std::round(calculate_expected_return(stocks[i]) * 100));
    }
    
    // DP table: dp[i][w] = max scaled return using first i items with budget w
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(budget + 1, 0));
    
    // Calculate memory usage
    result.memory_used_bytes = (n + 1) * (budget + 1) * sizeof(int);
    result.dp_table_rows = n + 1;
    result.dp_table_cols = budget + 1;
    
    // Fill DP table (0/1 Knapsack - each stock at most once)
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= budget; w++) {
            // Don't take item i-1
            dp[i][w] = dp[i-1][w];
            
            // Take item i-1 if possible (only once)
            if (stocks[i-1].cost <= w) {
                int take_value = dp[i-1][w - stocks[i-1].cost] + scaled_returns[i-1];
                dp[i][w] = std::max(dp[i][w], take_value);
            }
        }
    }
    
    // Traceback to find selected items
    int w = budget;
    for (int i = n; i > 0; i--) {
        if (dp[i][w] != dp[i-1][w]) {
            result.selected_ids.push_back(stocks[i-1].id);
            result.fractions.push_back(1.0);
            result.total_cost += stocks[i-1].cost;
            result.total_expected_amount += calculate_expected_return(stocks[i-1]);
            result.risk_summary += stocks[i-1].risk;
            w -= stocks[i-1].cost;
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
