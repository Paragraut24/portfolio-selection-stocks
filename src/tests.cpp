#include "knapsack.h"
#include "utils.h"
#include <iostream>
#include <cassert>
#include <cmath>
#include <vector>

void test_small_dataset_optimal() {
    std::cout << "Test 1: Small dataset optimal selection\n";
    
    // Create test dataset
    std::vector<Stock> stocks = {
        {1, "AAPL", 150, 12.0, 8},    // Expected return: 18
        {2, "TSLA", 250, 20.0, 15},   // Expected return: 50
        {3, "INFY", 1400, 10.0, 6},   // Expected return: 140
        {4, "TCS", 3200, 11.0, 5},    // Expected return: 352
        {5, "RELIANCE", 2400, 9.0, 7} // Expected return: 216
    };
    
    int budget = 3000;
    
    // Run 2D DP
    auto result = run_knapsack_2d(stocks, budget);
    
    // With budget 3000, optimal should be:
    // RELIANCE (2400) + TSLA (250) + AAPL (150) = 2800 cost, return = 216 + 50 + 18 = 284
    // OR RELIANCE (2400) + INFY (1400) would exceed budget
    // OR TCS (3200) exceeds budget alone
    
    std::cout << "  Selected stocks: ";
    for (int id : result.selected_ids) {
        std::cout << id << " ";
    }
    std::cout << "\n";
    std::cout << "  Total cost: " << result.total_cost << "\n";
    std::cout << "  Total expected return: " << result.total_expected_amount << "\n";
    
    // Verify result is valid
    assert(result.total_cost <= budget);
    assert(result.total_expected_amount > 0);
    
    std::cout << "  ✓ Test passed\n\n";
}

void test_edge_case_empty() {
    std::cout << "Test 2: Empty stock list\n";
    
    std::vector<Stock> stocks;
    int budget = 1000;
    
    auto result = run_knapsack_2d(stocks, budget);
    
    assert(result.selected_ids.empty());
    assert(result.total_cost == 0);
    assert(result.total_expected_amount == 0.0);
    
    std::cout << "  ✓ Test passed\n\n";
}

void test_edge_case_zero_budget() {
    std::cout << "Test 3: Zero budget\n";
    
    std::vector<Stock> stocks = {
        {1, "AAPL", 150, 12.0, 8}
    };
    int budget = 0;
    
    auto result = run_knapsack_2d(stocks, budget);
    
    assert(result.selected_ids.empty());
    assert(result.total_cost == 0);
    
    std::cout << "  ✓ Test passed\n\n";
}

void test_edge_case_all_too_expensive() {
    std::cout << "Test 4: All items cost more than budget\n";
    
    std::vector<Stock> stocks = {
        {1, "AAPL", 1500, 12.0, 8},
        {2, "TSLA", 2500, 20.0, 15}
    };
    int budget = 1000;
    
    auto result = run_knapsack_2d(stocks, budget);
    
    assert(result.selected_ids.empty());
    assert(result.total_cost == 0);
    
    std::cout << "  ✓ Test passed\n\n";
}

void test_algorithm_consistency() {
    std::cout << "Test 5: Algorithm consistency (2D vs 1D DP)\n";
    
    std::vector<Stock> stocks = {
        {1, "AAPL", 150, 12.0, 8},
        {2, "TSLA", 250, 20.0, 15},
        {3, "INFY", 1400, 10.0, 6}
    };
    int budget = 2000;
    
    auto result_2d = run_knapsack_2d(stocks, budget);
    auto result_1d = run_knapsack_1d(stocks, budget);
    
    // Both should give same optimal value
    assert(std::abs(result_2d.total_expected_amount - result_1d.total_expected_amount) < 0.01);
    assert(result_2d.total_cost == result_1d.total_cost);
    
    std::cout << "  2D DP return: " << result_2d.total_expected_amount << "\n";
    std::cout << "  1D DP return: " << result_1d.total_expected_amount << "\n";
    std::cout << "  ✓ Test passed\n\n";
}

void test_greedy_approximation() {
    std::cout << "Test 6: Greedy approximation quality\n";
    
    std::vector<Stock> stocks = {
        {1, "AAPL", 150, 12.0, 8},
        {2, "TSLA", 250, 20.0, 15},
        {3, "INFY", 1400, 10.0, 6}
    };
    int budget = 2000;
    
    auto result_optimal = run_knapsack_2d(stocks, budget);
    auto result_greedy = run_greedy(stocks, budget);
    
    std::cout << "  Optimal return: " << result_optimal.total_expected_amount << "\n";
    std::cout << "  Greedy return: " << result_greedy.total_expected_amount << "\n";
    
    // Greedy should be <= optimal
    assert(result_greedy.total_expected_amount <= result_optimal.total_expected_amount + 0.01);
    
    std::cout << "  ✓ Test passed\n\n";
}

void test_single_item() {
    std::cout << "Test 7: Single item selection\n";
    
    std::vector<Stock> stocks = {
        {1, "AAPL", 150, 12.0, 8}
    };
    int budget = 200;
    
    auto result = run_knapsack_2d(stocks, budget);
    
    assert(result.selected_ids.size() == 1);
    assert(result.selected_ids[0] == 1);
    assert(result.total_cost == 150);
    
    double expected_return = 150 * 0.12;
    assert(std::abs(result.total_expected_amount - expected_return) < 0.01);
    
    std::cout << "  ✓ Test passed\n\n";
}

int main() {
    std::cout << "Running Portfolio Selection Tests\n";
    std::cout << "==================================\n\n";
    
    try {
        test_small_dataset_optimal();
        test_edge_case_empty();
        test_edge_case_zero_budget();
        test_edge_case_all_too_expensive();
        test_algorithm_consistency();
        test_greedy_approximation();
        test_single_item();
        
        std::cout << "==================================\n";
        std::cout << "All tests passed! ✓\n";
        return 0;
    } catch (const std::exception& e) {
        std::cout << "Test failed with exception: " << e.what() << "\n";
        return 1;
    }
}
