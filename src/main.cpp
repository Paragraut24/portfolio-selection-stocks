#include <iostream>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include "io.h"
#include "utils.h"
#include "knapsack.h"

std::vector<Stock> portfolio;
KnapsackResult last_result;
bool has_last_result = false;
bool verbose_mode = false;

void displayMenu();
void print_result(const KnapsackResult& result, const std::vector<Stock>& portfolio, int budget);
void print_allocation_chart(const KnapsackResult& result, const std::vector<Stock>& portfolio, int budget);
void handleLoadCSV();
void handleAddStock();
void handleEditStock();
void handleDeleteStock();
void handleExportCSV();
void handleRunOptimization();
void handleShowLastResult();
void handleExportSolution();
void handleShowComplexity();
void handleRunTests();
void handleToggleVerbose();
void handleCompareAlgorithms(const std::vector<Stock>& portfolio);

void displayMenu() {
    std::cout << "\n========================================\n";
    std::cout << "   Portfolio Selection System\n";
    std::cout << "========================================\n";
    std::cout << "1.  Load portfolio data from CSV\n";
    std::cout << "2.  Display all stocks\n";
    std::cout << "3.  Add new stock\n";
    std::cout << "4.  Edit stock\n";
    std::cout << "5.  Delete stock\n";
    std::cout << "6.  Export portfolio to CSV\n";
    std::cout << "7.  Run optimization (choose algorithm)\n";
    std::cout << "8.  Show last optimization result\n";
    std::cout << "9.  Export solution to CSV\n";
    std::cout << "10. Show complexity & explanation\n";
    std::cout << "11. Compare all algorithms\n";
    std::cout << "12. Run unit tests\n";
    std::cout << "13. Toggle verbose mode [" << (verbose_mode ? "ON" : "OFF") << "]\n";
    std::cout << "14. Exit\n";
    std::cout << "========================================\n";
    std::cout << "Enter your choice: ";
}

void print_allocation_chart(const KnapsackResult& result, const std::vector<Stock>& portfolio, int budget) {
    if (result.selected_ids.empty()) return;
    
    std::cout << "\nBudget Allocation Chart:\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    
    for (int id : result.selected_ids) {
        auto it = std::find_if(portfolio.begin(), portfolio.end(),
                              [id](const Stock& s) { return s.id == id; });
        if (it != portfolio.end()) {
            double percentage = (static_cast<double>(it->cost) / budget) * 100.0;
            int bar_length = static_cast<int>((percentage / 100.0) * 40);
            
            std::cout << std::left << std::setw(12) << it->name << " [";
            for (int i = 0; i < 40; i++) {
                if (i < bar_length) std::cout << "#";
                else std::cout << ".";
            }
            std::cout << "] " << std::fixed << std::setprecision(1) << percentage 
                     << "% (Rs." << it->cost << ")\n";
        }
    }
    
    double used_percentage = (static_cast<double>(result.total_cost) / budget) * 100.0;
    std::cout << "--------------------------------------------------------------------------------\n";
    std::cout << "Total Used:  " << result.total_cost << " / " << budget 
             << " (" << std::fixed << std::setprecision(1) << used_percentage << "%)\n";
    std::cout << "Remaining:   " << (budget - result.total_cost) << "\n";
}

void print_result(const KnapsackResult& result, const std::vector<Stock>& portfolio, int budget) {
    std::cout << "\n";
    std::cout << "================================================================================\n";
    std::cout << "Algorithm: " << result.algo_used << "\n";
    std::cout << "Time Complexity: " << result.time_complexity << "\n";
    std::cout << "Space Complexity: " << result.space_complexity << "\n";
    std::cout << "Runtime: " << result.runtime_ms << " ms\n";
    
    // Profiling information
    if (result.memory_used_bytes > 0) {
        std::cout << "Memory Used: ";
        if (result.memory_used_bytes < 1024) {
            std::cout << result.memory_used_bytes << " bytes\n";
        } else if (result.memory_used_bytes < 1024 * 1024) {
            std::cout << std::fixed << std::setprecision(2) 
                     << (result.memory_used_bytes / 1024.0) << " KB\n";
        } else {
            std::cout << std::fixed << std::setprecision(2) 
                     << (result.memory_used_bytes / (1024.0 * 1024.0)) << " MB\n";
        }
    }
    
    if (result.dp_table_rows > 0 && result.dp_table_cols > 0) {
        std::cout << "DP Table Size: " << result.dp_table_rows << " x " 
                 << result.dp_table_cols << " = " 
                 << (result.dp_table_rows * result.dp_table_cols) << " cells\n";
    }
    
    std::cout << "--------------------------------------------------------------------------------\n";
    
    if (result.selected_ids.empty()) {
        std::cout << "No stocks selected (budget too low or no stocks available)\n";
    } else {
        std::cout << "Selected Stocks:\n";
        for (int id : result.selected_ids) {
            auto it = std::find_if(portfolio.begin(), portfolio.end(),
                                  [id](const Stock& s) { return s.id == id; });
            if (it != portfolio.end()) {
                double exp_return = calculate_expected_return(*it);
                std::cout << "  - " << it->name << " (ID: " << it->id 
                         << ", Cost: Rs." << it->cost 
                         << ", Return: " << std::fixed << std::setprecision(2) 
                         << it->return_pct << "% = Rs." << exp_return << ")\n";
            }
        }
        std::cout << "\nSummary:\n";
        std::cout << "  Total Cost: Rs." << result.total_cost << "\n";
        std::cout << "  Total Expected Return: Rs." << std::fixed << std::setprecision(2) 
                 << result.total_expected_amount << "\n";
        std::cout << "  Average Return %: " << std::fixed << std::setprecision(2) 
                 << result.total_expected_percent << "%\n";
        std::cout << "  Average Risk Score: " << std::fixed << std::setprecision(2) 
                 << result.risk_summary << "\n";
        std::cout << "  Number of Stocks: " << result.selected_ids.size() << "\n";
        
        if (budget > 0) {
            print_allocation_chart(result, portfolio, budget);
        }
    }
    std::cout << "================================================================================\n";
}

void handleRunOptimization() {
    if (portfolio.empty()) {
        std::cout << "Error: No stocks loaded. Please load a portfolio first.\n";
        return;
    }
    
    std::cout << "Enter budget: ";
    int budget;
    if (!(std::cin >> budget) || budget <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid budget. Must be positive.\n";
        return;
    }
    
    std::cout << "\nChoose algorithm:\n";
    std::cout << "  A - 2D Dynamic Programming (optimal, more memory)\n";
    std::cout << "  B - 1D Dynamic Programming (optimal, less memory)\n";
    std::cout << "  C - Greedy (fast approximation)\n";
    std::cout << "  D - Fractional Knapsack (theoretical demo)\n";
    std::cout << "Choice: ";
    
    char choice;
    std::cin >> choice;
    choice = toupper(choice);
    
    KnapsackResult result;
    switch (choice) {
        case 'A':
            result = run_knapsack_2d(portfolio, budget, verbose_mode);
            break;
        case 'B':
            result = run_knapsack_1d(portfolio, budget, verbose_mode);
            break;
        case 'C':
            result = run_greedy(portfolio, budget, verbose_mode);
            break;
        case 'D':
            result = run_fractional(portfolio, budget, verbose_mode);
            break;
        default:
            std::cout << "Invalid choice.\n";
            return;
    }
    
    last_result = result;
    has_last_result = true;
    print_result(result, portfolio, budget);
}

void handleShowLastResult() {
    if (!has_last_result) {
        std::cout << "No optimization has been run yet.\n";
        return;
    }
    
    std::cout << "\nLast Optimization Result:\n";
    print_result(last_result, portfolio, 0);
}

void handleExportSolution() {
    if (!has_last_result) {
        std::cout << "No optimization result to export. Run an optimization first.\n";
        return;
    }
    
    std::cout << "Enter output CSV file path: ";
    std::string path;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, path);
    
    // Create solution portfolio with only selected stocks
    std::vector<Stock> solution_portfolio;
    for (int id : last_result.selected_ids) {
        auto it = std::find_if(portfolio.begin(), portfolio.end(),
                              [id](const Stock& s) { return s.id == id; });
        if (it != portfolio.end()) {
            solution_portfolio.push_back(*it);
        }
    }
    
    save_csv(path, solution_portfolio);
    
    // Also save a summary file
    std::string summary_path = path.substr(0, path.find_last_of('.')) + "_summary.txt";
    std::ofstream summary(summary_path);
    if (summary.is_open()) {
        summary << "Portfolio Optimization Summary\n";
        summary << "==============================\n\n";
        summary << "Algorithm: " << last_result.algo_used << "\n";
        summary << "Runtime: " << last_result.runtime_ms << " ms\n";
        summary << "Total Cost: ₹" << last_result.total_cost << "\n";
        summary << "Total Expected Return: ₹" << last_result.total_expected_amount << "\n";
        summary << "Average Return %: " << last_result.total_expected_percent << "%\n";
        summary << "Average Risk: " << last_result.risk_summary << "\n";
        summary << "Number of Stocks: " << last_result.selected_ids.size() << "\n";
        summary.close();
        std::cout << "Summary saved to: " << summary_path << "\n";
    }
}

void handleShowComplexity() {
    if (!has_last_result) {
        std::cout << "No optimization has been run yet.\n";
        return;
    }
    
    std::cout << "\n" << std::string(80, '=') << "\n";
    std::cout << "Complexity Analysis: " << last_result.algo_used << "\n";
    std::cout << std::string(80, '=') << "\n\n";
    
    explain_algorithm(last_result.algo_used);
    
    std::cout << "\nActual Performance:\n";
    std::cout << "  Runtime: " << last_result.runtime_ms << " ms\n";
    std::cout << "  Time Complexity: " << last_result.time_complexity << "\n";
    std::cout << "  Space Complexity: " << last_result.space_complexity << "\n";
    
    if (last_result.memory_used_bytes > 0) {
        std::cout << "  Memory Used: ";
        if (last_result.memory_used_bytes < 1024) {
            std::cout << last_result.memory_used_bytes << " bytes\n";
        } else if (last_result.memory_used_bytes < 1024 * 1024) {
            std::cout << std::fixed << std::setprecision(2) 
                     << (last_result.memory_used_bytes / 1024.0) << " KB\n";
        } else {
            std::cout << std::fixed << std::setprecision(2) 
                     << (last_result.memory_used_bytes / (1024.0 * 1024.0)) << " MB\n";
        }
    }
    
    if (last_result.dp_table_rows > 0 && last_result.dp_table_cols > 0) {
        std::cout << "  DP Table: " << last_result.dp_table_rows << " rows × " 
                 << last_result.dp_table_cols << " cols = " 
                 << (last_result.dp_table_rows * last_result.dp_table_cols) << " cells\n";
    }
    
    std::cout << std::string(80, '=') << "\n";
}

void handleRunTests() {
    std::cout << "\nRunning unit tests...\n";
    std::cout << "Note: Tests are in a separate executable. Please run:\n";
    std::cout << "  g++ -std=c++17 -O2 -Wall -o test_runner src/tests.cpp src/knapsack.cpp src/io.cpp src/utils.cpp\n";
    std::cout << "  ./test_runner\n";
}

void handleToggleVerbose() {
    verbose_mode = !verbose_mode;
    std::cout << "Verbose mode " << (verbose_mode ? "enabled" : "disabled") << "\n";
    if (verbose_mode) {
        std::cout << "Note: Verbose mode shows DP tables and step-by-step execution.\n";
        std::cout << "      Best for small datasets (n ≤ 8, budget ≤ 5000).\n";
    }
}

void handleCompareAlgorithms(const std::vector<Stock>& portfolio) {
    if (portfolio.empty()) {
        std::cout << "Error: No stocks loaded. Please load a portfolio first.\n";
        return;
    }
    
    std::cout << "Enter budget: ";
    int budget;
    if (!(std::cin >> budget) || budget <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid budget. Must be positive.\n";
        return;
    }
    
    std::cout << "\nRunning all algorithms...\n";
    
    auto result_2d = run_knapsack_2d(portfolio, budget, false);
    print_result(result_2d, portfolio, budget);
    
    auto result_1d = run_knapsack_1d(portfolio, budget, false);
    print_result(result_1d, portfolio, budget);
    
    auto result_greedy = run_greedy(portfolio, budget, false);
    print_result(result_greedy, portfolio, budget);
    
    auto result_fractional = run_fractional(portfolio, budget, false);
    print_result(result_fractional, portfolio, budget);
    
    std::cout << "\nComparison Summary:\n";
    std::cout << "================================================================================\n";
    std::cout << std::left << std::setw(20) << "Algorithm" 
              << std::right << std::setw(15) << "Return (Rs.)" 
              << std::setw(15) << "Runtime (ms)" 
              << std::setw(15) << "Stocks" << "\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    
    std::cout << std::left << std::setw(20) << result_2d.algo_used
              << std::right << std::setw(15) << std::fixed << std::setprecision(2) 
              << result_2d.total_expected_amount
              << std::setw(15) << result_2d.runtime_ms
              << std::setw(15) << result_2d.selected_ids.size() << "\n";
    
    std::cout << std::left << std::setw(20) << result_1d.algo_used
              << std::right << std::setw(15) << std::fixed << std::setprecision(2) 
              << result_1d.total_expected_amount
              << std::setw(15) << result_1d.runtime_ms
              << std::setw(15) << result_1d.selected_ids.size() << "\n";
    
    std::cout << std::left << std::setw(20) << result_greedy.algo_used
              << std::right << std::setw(15) << std::fixed << std::setprecision(2) 
              << result_greedy.total_expected_amount
              << std::setw(15) << result_greedy.runtime_ms
              << std::setw(15) << result_greedy.selected_ids.size() << "\n";
    
    std::cout << std::left << std::setw(20) << result_fractional.algo_used
              << std::right << std::setw(15) << std::fixed << std::setprecision(2) 
              << result_fractional.total_expected_amount
              << std::setw(15) << result_fractional.runtime_ms
              << std::setw(15) << result_fractional.selected_ids.size() << "\n";
    
    std::cout << "================================================================================\n";
}

void handleLoadCSV() {
    std::cout << "Enter CSV file path: ";
    std::string path;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, path);
    
    auto loaded_stocks = load_csv(path);
    if (!loaded_stocks.empty()) {
        portfolio = loaded_stocks;
    }
}

void handleAddStock() {
    Stock stock;
    std::cout << "Enter stock ID: ";
    if (!(std::cin >> stock.id)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid ID.\n";
        return;
    }

    std::cout << "Enter stock name: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, stock.name);

    std::cout << "Enter cost: ";
    if (!(std::cin >> stock.cost) || stock.cost <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid cost. Must be positive.\n";
        return;
    }

    std::cout << "Enter expected return %: ";
    if (!(std::cin >> stock.return_pct) || stock.return_pct < 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid return percentage.\n";
        return;
    }

    std::cout << "Enter risk score: ";
    if (!(std::cin >> stock.risk) || stock.risk < 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid risk score.\n";
        return;
    }

    add_stock(portfolio, stock);
}

void handleEditStock() {
    std::cout << "Enter stock ID to edit: ";
    int id;
    if (!(std::cin >> id)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid ID.\n";
        return;
    }
    edit_stock(portfolio, id);
}

void handleDeleteStock() {
    std::cout << "Enter stock ID to delete: ";
    int id;
    if (!(std::cin >> id)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid ID.\n";
        return;
    }
    delete_stock(portfolio, id);
}

void handleExportCSV() {
    std::cout << "Enter output CSV file path: ";
    std::string path;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, path);
    save_csv(path, portfolio);
}

int main() {
    int choice;
    bool running = true;

    std::cout << "Welcome to Portfolio Selection System!\n";

    while (running) {
        displayMenu();
        
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1:
                handleLoadCSV();
                break;
            case 2:
                print_stock_table(portfolio);
                break;
            case 3:
                handleAddStock();
                break;
            case 4:
                handleEditStock();
                break;
            case 5:
                handleDeleteStock();
                break;
            case 6:
                handleExportCSV();
                break;
            case 7:
                handleRunOptimization();
                break;
            case 8:
                handleShowLastResult();
                break;
            case 9:
                handleExportSolution();
                break;
            case 10:
                handleShowComplexity();
                break;
            case 11:
                handleCompareAlgorithms(portfolio);
                break;
            case 12:
                handleRunTests();
                break;
            case 13:
                handleToggleVerbose();
                break;
            case 14:
                std::cout << "Exiting... Goodbye!\n";
                running = false;
                break;
            default:
                std::cout << "Invalid option. Please choose 1-14.\n";
                break;
        }
    }

    return 0;
}
