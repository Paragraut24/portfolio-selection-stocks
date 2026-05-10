#include "knapsack.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>

double calculate_expected_return(const Stock& stock) {
    return stock.cost * (stock.return_pct / 100.0);
}

std::string get_time_complexity(const std::string& algo) {
    if (algo == "2D DP" || algo == "1D DP") {
        return "O(n * budget)";
    } else if (algo == "Greedy" || algo == "Fractional") {
        return "O(n log n)";
    }
    return "Unknown";
}

std::string get_space_complexity(const std::string& algo) {
    if (algo == "2D DP") {
        return "O(n * budget)";
    } else if (algo == "1D DP") {
        return "O(budget)";
    } else if (algo == "Greedy" || algo == "Fractional") {
        return "O(n)";
    }
    return "Unknown";
}

void explain_algorithm(const std::string& algo) {
    std::cout << "Algorithm: " << algo << "\n\n";
    
    if (algo == "2D DP") {
        std::cout << "Approach:\n";
        std::cout << "  - Uses a 2D table dp[i][w] where:\n";
        std::cout << "    * i = number of items considered (0 to n)\n";
        std::cout << "    * w = budget available (0 to budget)\n";
        std::cout << "    * dp[i][w] = maximum return using first i items with budget w\n\n";
        std::cout << "  - Recurrence relation:\n";
        std::cout << "    dp[i][w] = max(dp[i-1][w],                    // don't take item i\n";
        std::cout << "                   dp[i-1][w-cost[i]] + value[i]) // take item i\n\n";
        std::cout << "  - Traceback: Walk backwards through table to find selected items\n\n";
        std::cout << "Guarantees: Optimal solution\n";
        
    } else if (algo == "1D DP") {
        std::cout << "Approach:\n";
        std::cout << "  - Space-optimized version using single array dp[w]\n";
        std::cout << "  - Iterates backwards through budget to avoid reusing items\n";
        std::cout << "  - Uses parent tracking array to reconstruct solution\n\n";
        std::cout << "  - Key optimization: Only stores current row, not entire table\n";
        std::cout << "  - Memory: O(budget) instead of O(n × budget)\n\n";
        std::cout << "Guarantees: Optimal solution (same as 2D DP)\n";
        
    } else if (algo == "Greedy") {
        std::cout << "Approach:\n";
        std::cout << "  - Calculate return/cost ratio for each stock\n";
        std::cout << "  - Sort stocks by ratio in descending order\n";
        std::cout << "  - Greedily select stocks until budget exhausted\n\n";
        std::cout << "  - Fast but not guaranteed optimal for 0/1 knapsack\n";
        std::cout << "  - Works well when ratios vary significantly\n\n";
        std::cout << "Guarantees: Approximation (may not be optimal)\n";
        
    } else if (algo == "Fractional") {
        std::cout << "Approach:\n";
        std::cout << "  - Similar to Greedy but allows fractional items\n";
        std::cout << "  - Always optimal for fractional knapsack variant\n";
        std::cout << "  - Provides upper bound for 0/1 knapsack\n\n";
        std::cout << "  - Note: Real stocks cannot be fractional\n";
        std::cout << "  - Used for theoretical analysis and comparison\n\n";
        std::cout << "Guarantees: Optimal for fractional variant\n";
    }
}

KnapsackResult run_knapsack_2d(const std::vector<Stock>& stocks, int budget, bool verbose) {
    auto start = std::chrono::high_resolution_clock::now();
    
    KnapsackResult result;
    result.algo_used = "2D DP";
    result.time_complexity = get_time_complexity(result.algo_used);
    result.space_complexity = get_space_complexity(result.algo_used);
    result.total_cost = 0;
    result.total_expected_amount = 0.0;
    result.total_expected_percent = 0.0;
    result.risk_summary = 0.0;
    result.memory_used_bytes = 0;
    result.dp_table_rows = 0;
    result.dp_table_cols = 0;
    
    if (stocks.empty() || budget <= 0) {
        auto end = std::chrono::high_resolution_clock::now();
        result.runtime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
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
    
    if (verbose && n <= 8 && budget <= 5000) {
        std::cout << "\n[Verbose Mode] Building 2D DP table...\n";
        std::cout << "Table size: " << (n + 1) << " × " << (budget + 1) << "\n";
    }
    
    // Fill DP table
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= budget; w++) {
            // Don't take item i-1
            dp[i][w] = dp[i-1][w];
            
            // Take item i-1 if possible
            if (stocks[i-1].cost <= w) {
                int take_value = dp[i-1][w - stocks[i-1].cost] + scaled_returns[i-1];
                dp[i][w] = std::max(dp[i][w], take_value);
            }
        }
    }
    
    // Print DP table if verbose and small enough
    if (verbose && n <= 8 && budget <= 100) {
        std::cout << "\nDP Table (showing every 10th column for readability):\n";
        std::cout << std::setw(8) << "Item\\W";
        for (int w = 0; w <= budget; w += 10) {
            std::cout << std::setw(8) << w;
        }
        std::cout << "\n";
        
        for (int i = 0; i <= n; i++) {
            if (i == 0) {
                std::cout << std::setw(8) << "None";
            } else {
                std::cout << std::setw(8) << stocks[i-1].name.substr(0, 7);
            }
            for (int w = 0; w <= budget; w += 10) {
                std::cout << std::setw(8) << (dp[i][w] / 100.0);
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
    
    // Traceback to find selected items
    if (verbose && n <= 8) {
        std::cout << "[Verbose Mode] Traceback:\n";
    }
    
    int w = budget;
    for (int i = n; i > 0; i--) {
        if (dp[i][w] != dp[i-1][w]) {
            if (verbose && n <= 8) {
                std::cout << "  Selected: " << stocks[i-1].name 
                         << " (cost: " << stocks[i-1].cost 
                         << ", return: " << (scaled_returns[i-1] / 100.0) << ")\n";
            }
            result.selected_ids.push_back(stocks[i-1].id);
            result.total_cost += stocks[i-1].cost;
            result.total_expected_amount += calculate_expected_return(stocks[i-1]);
            result.risk_summary += stocks[i-1].risk;
            w -= stocks[i-1].cost;
        }
    }
    
    if (verbose && n <= 8) {
        std::cout << "\n";
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
    result.runtime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    return result;
}

KnapsackResult run_knapsack_1d(const std::vector<Stock>& stocks, int budget, bool verbose) {
    auto start = std::chrono::high_resolution_clock::now();
    
    KnapsackResult result;
    result.algo_used = "1D DP";
    result.time_complexity = get_time_complexity(result.algo_used);
    result.space_complexity = get_space_complexity(result.algo_used);
    result.total_cost = 0;
    result.total_expected_amount = 0.0;
    result.total_expected_percent = 0.0;
    result.risk_summary = 0.0;
    result.memory_used_bytes = 0;
    result.dp_table_rows = 0;
    result.dp_table_cols = 0;
    
    if (stocks.empty() || budget <= 0) {
        auto end = std::chrono::high_resolution_clock::now();
        result.runtime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
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
    
    // Parent tracking for reconstruction: parent[w] = {item_index, previous_weight}
    std::vector<std::vector<std::pair<int, int>>> parent(n + 1, std::vector<std::pair<int, int>>(budget + 1, {-1, -1}));
    
    // Calculate memory usage
    result.memory_used_bytes = (budget + 1) * sizeof(int) + 
                               (n + 1) * (budget + 1) * sizeof(std::pair<int, int>);
    result.dp_table_rows = 1;
    result.dp_table_cols = budget + 1;
    
    if (verbose && n <= 8 && budget <= 5000) {
        std::cout << "\n[Verbose Mode] Building 1D DP array...\n";
        std::cout << "Array size: " << (budget + 1) << " elements\n";
        std::cout << "Memory saved vs 2D: " << (n * (budget + 1) * sizeof(int)) << " bytes\n\n";
    }
    
    // Fill DP array
    for (int i = 0; i < n; i++) {
        // Traverse backwards to avoid using same item multiple times
        for (int w = budget; w >= stocks[i].cost; w--) {
            int new_value = dp[w - stocks[i].cost] + scaled_returns[i];
            if (new_value > dp[w]) {
                dp[w] = new_value;
                parent[i+1][w] = {i, w - stocks[i].cost};
            }
        }
    }
    
    // Traceback using parent array
    int w = budget;
    
    // Find which items were selected by checking parent array
    for (int i = n; i > 0; i--) {
        if (parent[i][w].first != -1) {
            int item_idx = parent[i][w].first;
            result.selected_ids.push_back(stocks[item_idx].id);
            result.total_cost += stocks[item_idx].cost;
            result.total_expected_amount += calculate_expected_return(stocks[item_idx]);
            result.risk_summary += stocks[item_idx].risk;
            w = parent[i][w].second;
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
    result.runtime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    return result;
}

KnapsackResult run_greedy(const std::vector<Stock>& stocks, int budget, bool verbose) {
    auto start = std::chrono::high_resolution_clock::now();
    
    KnapsackResult result;
    result.algo_used = "Greedy";
    result.time_complexity = get_time_complexity(result.algo_used);
    result.space_complexity = get_space_complexity(result.algo_used);
    result.total_cost = 0;
    result.total_expected_amount = 0.0;
    result.total_expected_percent = 0.0;
    result.risk_summary = 0.0;
    result.memory_used_bytes = 0;
    result.dp_table_rows = 0;
    result.dp_table_cols = 0;
    
    if (stocks.empty() || budget <= 0) {
        auto end = std::chrono::high_resolution_clock::now();
        result.runtime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
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
    
    if (verbose && stocks.size() <= 20) {
        std::cout << "\n[Verbose Mode] Greedy selection by return/cost ratio:\n";
        std::cout << std::left << std::setw(15) << "Stock" 
                 << std::right << std::setw(10) << "Cost" 
                 << std::setw(12) << "Return" 
                 << std::setw(12) << "Ratio" 
                 << std::setw(12) << "Selected" << "\n";
        std::cout << std::string(60, '-') << "\n";
    }
    
    // Greedily select stocks
    for (size_t i = 0; i < ratios.size(); i++) {
        int idx = ratios[i].first;
        double ratio = ratios[i].second;
        bool selected = false;
        
        if (result.total_cost + stocks[idx].cost <= budget) {
            result.selected_ids.push_back(stocks[idx].id);
            result.total_cost += stocks[idx].cost;
            result.total_expected_amount += calculate_expected_return(stocks[idx]);
            result.risk_summary += stocks[idx].risk;
            selected = true;
        }
        
        if (verbose && stocks.size() <= 20) {
            std::cout << std::left << std::setw(15) << stocks[idx].name.substr(0, 14)
                     << std::right << std::setw(10) << stocks[idx].cost
                     << std::setw(12) << std::fixed << std::setprecision(2) 
                     << calculate_expected_return(stocks[idx])
                     << std::setw(12) << std::fixed << std::setprecision(4) << ratio
                     << std::setw(12) << (selected ? "YES" : "NO") << "\n";
        }
    }
    
    if (verbose && stocks.size() <= 20) {
        std::cout << "\n";
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
    result.runtime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    return result;
}

KnapsackResult run_fractional(const std::vector<Stock>& stocks, int budget, bool verbose) {
    auto start = std::chrono::high_resolution_clock::now();
    
    KnapsackResult result;
    result.algo_used = "Fractional";
    result.time_complexity = get_time_complexity(result.algo_used);
    result.space_complexity = get_space_complexity(result.algo_used);
    result.total_cost = 0;
    result.total_expected_amount = 0.0;
    result.total_expected_percent = 0.0;
    result.risk_summary = 0.0;
    result.memory_used_bytes = 0;
    result.dp_table_rows = 0;
    result.dp_table_cols = 0;
    
    if (stocks.empty() || budget <= 0) {
        auto end = std::chrono::high_resolution_clock::now();
        result.runtime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
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
    
    if (verbose && stocks.size() <= 20) {
        std::cout << "\n[Verbose Mode] Fractional knapsack selection:\n";
        std::cout << "Note: This is theoretical - real stocks cannot be fractional\n\n";
    }
    
    // Greedily select stocks (allowing fractions)
    int remaining_budget = budget;
    for (size_t i = 0; i < ratios.size(); i++) {
        int idx = ratios[i].first;
        if (remaining_budget <= 0) break;
        
        if (stocks[idx].cost <= remaining_budget) {
            // Take full stock
            result.selected_ids.push_back(stocks[idx].id);
            result.total_cost += stocks[idx].cost;
            result.total_expected_amount += calculate_expected_return(stocks[idx]);
            result.risk_summary += stocks[idx].risk;
            remaining_budget -= stocks[idx].cost;
        } else {
            // Take fraction (for demonstration - in reality stocks can't be fractional)
            double fraction = static_cast<double>(remaining_budget) / stocks[idx].cost;
            result.selected_ids.push_back(stocks[idx].id); // Mark as partially selected
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
    result.runtime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    return result;
}
