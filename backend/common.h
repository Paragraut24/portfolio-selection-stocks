#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <string>

// Stock data structure
struct Stock {
    int id;
    std::string name;
    int cost;
    double return_pct;
    double risk;
};

// Result structure for optimization algorithms
struct OptimizationResult {
    std::vector<int> selected_ids;
    std::vector<double> fractions;  // For fractional knapsack (1.0 = full stock, <1.0 = partial)
    int total_cost;
    double total_expected_amount;
    double total_expected_percent;
    double risk_summary;
    std::string algo_used;
    long runtime_us;  // Changed to microseconds for better precision
    std::string time_complexity;
    std::string space_complexity;
    size_t memory_used_bytes;
    int dp_table_rows;
    int dp_table_cols;
};

// MST Edge structure for diversification
struct Edge {
    int stock1_id;
    int stock2_id;
    double weight;  // Correlation or similarity measure
    
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// MST Result structure
struct MSTResult {
    std::vector<Edge> selected_edges;
    std::vector<int> diversified_stock_ids;
    double total_weight;
    long runtime_us;  // Changed to microseconds for better precision
    std::string algo_used;
};

// Utility function
double calculate_expected_return(const Stock& stock);

#endif
