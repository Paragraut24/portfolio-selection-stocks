#include <iostream>
#include <iomanip>
#include <vector>
#include "common.h"
#include "csv_io.h"
#include "knapsack_2d.h"
#include "knapsack_1d.h"
#include "greedy.h"
#include "fractional.h"
#include "unbounded.h"
#include "mst.h"

void print_summary(const OptimizationResult& result) {
    std::cout << "\n========================================\n";
    std::cout << "Algorithm: " << result.algo_used << "\n";
    std::cout << "========================================\n";
    std::cout << "Selected Stocks: " << result.selected_ids.size() << "\n";
    std::cout << "Total Investment: Rs." << result.total_cost << "\n";
    std::cout << "Total Cost: Rs." << result.total_cost << "\n";
    std::cout << "Total Expected Return: Rs." << result.total_expected_amount << "\n";
    std::cout << "Average Return %: " << result.total_expected_percent << "%\n";
    std::cout << "Average Risk: " << result.risk_summary << "\n";
    
    // Display runtime with appropriate precision
    double runtime_ms = result.runtime_us / 1000.0;
    if (runtime_ms < 0.01) {
        std::cout << "Runtime: " << result.runtime_us << " μs (0.00" << (result.runtime_us / 10) << " ms)\n";
    } else if (runtime_ms < 1.0) {
        std::cout << "Runtime: " << std::fixed << std::setprecision(3) << runtime_ms << " ms (" << result.runtime_us << " μs)\n";
    } else {
        std::cout << "Runtime: " << std::fixed << std::setprecision(2) << runtime_ms << " ms (" << result.runtime_us << " μs)\n";
    }
    
    std::cout << "Time Complexity: " << result.time_complexity << "\n";
    std::cout << "Space Complexity: " << result.space_complexity << "\n";
    std::cout << "========================================\n";
}

void print_mst_summary(const MSTResult& result) {
    std::cout << "\n========================================\n";
    std::cout << "Portfolio Diversification (MST)\n";
    std::cout << "========================================\n";
    std::cout << "Algorithm: " << result.algo_used << "\n";
    std::cout << "Diversified Stocks: " << result.diversified_stock_ids.size() << "\n";
    std::cout << "Total MST Weight: " << result.total_weight << "\n";
    
    // Display runtime with appropriate precision
    double runtime_ms = result.runtime_us / 1000.0;
    if (runtime_ms < 0.01) {
        std::cout << "Runtime: " << result.runtime_us << " μs (0.00" << (result.runtime_us / 10) << " ms)\n";
    } else if (runtime_ms < 1.0) {
        std::cout << "Runtime: " << std::fixed << std::setprecision(3) << runtime_ms << " ms (" << result.runtime_us << " μs)\n";
    } else {
        std::cout << "Runtime: " << std::fixed << std::setprecision(2) << runtime_ms << " ms (" << result.runtime_us << " μs)\n";
    }
    
    std::cout << "========================================\n";
}

int main(int argc, char* argv[]) {
    std::cout << "Portfolio Selection Backend System\n";
    std::cout << "===================================\n\n";
    
    // Configuration
    std::string input_file = "stocks.csv";
    int budget = 5000;
    
    // Parse command line arguments
    if (argc >= 2) {
        input_file = argv[1];
    }
    if (argc >= 3) {
        budget = std::stoi(argv[2]);
    }
    
    std::cout << "Input File: " << input_file << "\n";
    std::cout << "Budget: Rs." << budget << "\n\n";
    
    // Step 1: Load stocks from CSV
    std::cout << "Step 1: Loading stocks...\n";
    std::vector<Stock> stocks = load_stocks_csv(input_file);
    
    if (stocks.empty()) {
        std::cerr << "Error: No stocks loaded. Exiting.\n";
        return 1;
    }
    
    // Step 2: Run all knapsack algorithms
    std::cout << "\nStep 2: Running optimization algorithms...\n";
    
    std::cout << "\nRunning 2D Knapsack (0/1)...\n";
    OptimizationResult result_2d = knapsack_2d(stocks, budget);
    print_summary(result_2d);
    save_portfolio_results_csv("portfolio_results_2d.csv", result_2d, stocks);
    
    std::cout << "\nRunning 1D Knapsack (0/1)...\n";
    OptimizationResult result_1d = knapsack_1d(stocks, budget);
    print_summary(result_1d);
    save_portfolio_results_csv("portfolio_results_1d.csv", result_1d, stocks);
    
    std::cout << "\nRunning Unbounded Knapsack...\n";
    OptimizationResult result_unbounded = unbounded_knapsack(stocks, budget);
    print_summary(result_unbounded);
    save_portfolio_results_csv("portfolio_results_unbounded.csv", result_unbounded, stocks);
    
    std::cout << "\nRunning Greedy Algorithm...\n";
    OptimizationResult result_greedy = greedy_knapsack(stocks, budget);
    print_summary(result_greedy);
    save_portfolio_results_csv("portfolio_results_greedy.csv", result_greedy, stocks);
    
    std::cout << "\nRunning Fractional Knapsack...\n";
    OptimizationResult result_fractional = fractional_knapsack(stocks, budget);
    print_summary(result_fractional);
    save_portfolio_results_csv("portfolio_results_fractional.csv", result_fractional, stocks);
    
    // Step 3: Save algorithm comparison
    std::cout << "\nStep 3: Generating algorithm comparison...\n";
    std::vector<OptimizationResult> all_results = {result_2d, result_1d, result_unbounded, result_greedy, result_fractional};
    save_algorithm_comparison_csv("algorithm_comparison.csv", all_results);
    
    // Step 4: Run MST for portfolio diversification
    std::cout << "\nStep 4: Running portfolio diversification (MST)...\n";
    MSTResult mst_result = portfolio_diversification_mst(stocks);
    print_mst_summary(mst_result);
    save_diversification_output_csv("diversification_output.csv", mst_result, stocks);
    
    std::cout << "\n===================================\n";
    std::cout << "Processing Complete!\n";
    std::cout << "===================================\n";
    
    return 0;
}
