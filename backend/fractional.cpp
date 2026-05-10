#include "fractional.h"
#include <algorithm>
#include <chrono>

OptimizationResult fractional_knapsack(const std::vector<Stock>& stocks, int budget) {
    auto start = std::chrono::high_resolution_clock::now();
    
    OptimizationResult result;
    result.algo_used = "Fractional";
    result.time_complexity = "O(n log n)";
    result.space_complexity = "O(n)";
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
    
    // Create pairs of (stock_index, ratio)
    std::vector<std::pair<int, double>> ratios;
    for (size_t i = 0; i < stocks.size(); i++) {
        double expected_return = calculate_expected_return(stocks[i]);
        double ratio = expected_return / stocks[i].cost;
        ratios.push_back({i, ratio});
    }
    
    result.memory_used_bytes = ratios.size() * sizeof(std::pair<int, double>);
    
    // Sort by ratio in descending order
    std::sort(ratios.begin(), ratios.end(), 
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    // Greedily select stocks (allowing fractions)
    int remaining_budget = budget;
    for (size_t i = 0; i < ratios.size(); i++) {
        int idx = ratios[i].first;
        if (remaining_budget <= 0) break;
        
        if (stocks[idx].cost <= remaining_budget) {
            // Take full stock
            result.selected_ids.push_back(stocks[idx].id);
            result.fractions.push_back(1.0);  // Full stock
            result.total_cost += stocks[idx].cost;
            result.total_expected_amount += calculate_expected_return(stocks[idx]);
            result.risk_summary += stocks[idx].risk;
            remaining_budget -= stocks[idx].cost;
        } else {
            // Take fraction
            double fraction = static_cast<double>(remaining_budget) / stocks[idx].cost;
            result.selected_ids.push_back(stocks[idx].id);
            result.fractions.push_back(fraction);  // Partial stock
            result.total_cost += remaining_budget;
            result.total_expected_amount += calculate_expected_return(stocks[idx]) * fraction;
            result.risk_summary += stocks[idx].risk * fraction;
            remaining_budget = 0;
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
