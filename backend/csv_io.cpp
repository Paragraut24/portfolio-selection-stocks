#include "csv_io.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

std::vector<Stock> load_stocks_csv(const std::string& filename) {
    std::vector<Stock> stocks;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'\n";
        return stocks;
    }
    
    std::string line;
    bool first_line = true;
    
    while (std::getline(file, line)) {
        if (first_line) {
            first_line = false;
            continue; // Skip header
        }
        
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string token;
        Stock stock;
        
        try {
            // Parse ID
            if (!std::getline(ss, token, ',')) continue;
            stock.id = std::stoi(token);
            
            // Parse name
            if (!std::getline(ss, token, ',')) continue;
            stock.name = token;
            
            // Parse cost
            if (!std::getline(ss, token, ',')) continue;
            stock.cost = std::stoi(token);
            
            // Parse return_pct
            if (!std::getline(ss, token, ',')) continue;
            stock.return_pct = std::stod(token);
            
            // Parse risk
            if (!std::getline(ss, token, ',')) continue;
            stock.risk = std::stod(token);
            
            stocks.push_back(stock);
            
        } catch (const std::exception& e) {
            std::cerr << "Warning: Skipping malformed line\n";
            continue;
        }
    }
    
    file.close();
    std::cout << "Loaded " << stocks.size() << " stocks from '" << filename << "'\n";
    return stocks;
}

void save_portfolio_results_csv(const std::string& filename, const OptimizationResult& result, const std::vector<Stock>& all_stocks) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not create file '" << filename << "'\n";
        return;
    }
    
    // Write header
    file << "algorithm,stock_id,stock_name,cost,return_pct,risk,expected_return\n";
    
    // Write selected stocks
    for (size_t i = 0; i < result.selected_ids.size(); i++) {
        int id = result.selected_ids[i];
        double fraction = (i < result.fractions.size()) ? result.fractions[i] : 1.0;
        
        auto it = std::find_if(all_stocks.begin(), all_stocks.end(),
                              [id](const Stock& s) { return s.id == id; });
        if (it != all_stocks.end()) {
            double fractional_cost = it->cost * fraction;
            double exp_return = calculate_expected_return(*it) * fraction;
            file << result.algo_used << ","
                 << it->id << ","
                 << it->name << ","
                 << std::fixed << std::setprecision(2) << fractional_cost << ","
                 << std::fixed << std::setprecision(2) << it->return_pct << ","
                 << std::fixed << std::setprecision(2) << it->risk << ","
                 << std::fixed << std::setprecision(2) << exp_return << "\n";
        }
    }
    
    file.close();
    std::cout << "Saved portfolio results to '" << filename << "'\n";
}

void save_algorithm_comparison_csv(const std::string& filename, const std::vector<OptimizationResult>& results) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not create file '" << filename << "'\n";
        return;
    }
    
    // Write header
    file << "algorithm,total_cost,total_expected_return,avg_return_pct,avg_risk,num_stocks,runtime_ms,time_complexity,space_complexity,memory_bytes\n";
    
    // Write comparison data
    for (const auto& result : results) {
        double runtime_ms = result.runtime_us / 1000.0;  // Convert microseconds to milliseconds
        file << result.algo_used << ","
             << result.total_cost << ","
             << std::fixed << std::setprecision(2) << result.total_expected_amount << ","
             << std::fixed << std::setprecision(2) << result.total_expected_percent << ","
             << std::fixed << std::setprecision(2) << result.risk_summary << ","
             << result.selected_ids.size() << ","
             << std::fixed << std::setprecision(3) << runtime_ms << ","
             << result.time_complexity << ","
             << result.space_complexity << ","
             << result.memory_used_bytes << "\n";
    }
    
    file.close();
    std::cout << "Saved algorithm comparison to '" << filename << "'\n";
}

void save_diversification_output_csv(const std::string& filename, const MSTResult& result, const std::vector<Stock>& all_stocks) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not create file '" << filename << "'\n";
        return;
    }
    
    // Write header
    file << "stock_id,stock_name,cost,return_pct,risk\n";
    
    // Write diversified stocks
    for (int id : result.diversified_stock_ids) {
        auto it = std::find_if(all_stocks.begin(), all_stocks.end(),
                              [id](const Stock& s) { return s.id == id; });
        if (it != all_stocks.end()) {
            file << it->id << ","
                 << it->name << ","
                 << it->cost << ","
                 << std::fixed << std::setprecision(2) << it->return_pct << ","
                 << std::fixed << std::setprecision(2) << it->risk << "\n";
        }
    }
    
    file.close();
    std::cout << "Saved diversification output to '" << filename << "'\n";
}
