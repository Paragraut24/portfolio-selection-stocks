#include "io.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

std::vector<Stock> load_csv(const std::string& path) {
    std::vector<Stock> stocks;
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cout << "Error: Could not open file '" << path << "'\n";
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
            if (!std::getline(ss, token, ',')) {
                std::cout << "Warning: Skipping malformed line\n";
                continue;
            }
            stock.id = std::stoi(token);

            // Parse name
            if (!std::getline(ss, token, ',')) {
                std::cout << "Warning: Skipping malformed line\n";
                continue;
            }
            stock.name = token;

            // Parse cost
            if (!std::getline(ss, token, ',')) {
                std::cout << "Warning: Skipping malformed line\n";
                continue;
            }
            stock.cost = std::stoi(token);
            if (stock.cost <= 0) {
                std::cout << "Warning: Invalid cost for stock " << stock.name << ", skipping\n";
                continue;
            }

            // Parse return_pct
            if (!std::getline(ss, token, ',')) {
                std::cout << "Warning: Skipping malformed line\n";
                continue;
            }
            stock.return_pct = std::stod(token);

            // Parse risk
            if (!std::getline(ss, token, ',')) {
                std::cout << "Warning: Skipping malformed line\n";
                continue;
            }
            stock.risk = std::stod(token);

            // Check for duplicates
            if (has_duplicate_id(stocks, stock.id)) {
                std::cout << "Warning: Duplicate ID " << stock.id << " found, skipping\n";
                continue;
            }
            if (has_duplicate_name(stocks, stock.name)) {
                std::cout << "Warning: Duplicate name '" << stock.name << "' found, skipping\n";
                continue;
            }

            stocks.push_back(stock);

        } catch (const std::invalid_argument& e) {
            std::cout << "Warning: Invalid numeric value in line, skipping\n";
            continue;
        } catch (const std::out_of_range& e) {
            std::cout << "Warning: Numeric value out of range in line, skipping\n";
            continue;
        }
    }

    file.close();

    if (stocks.empty()) {
        std::cout << "Warning: No valid stocks loaded from file\n";
    } else {
        std::cout << "Successfully loaded " << stocks.size() << " stocks from '" << path << "'\n";
    }

    return stocks;
}

void save_csv(const std::string& path, const std::vector<Stock>& stocks) {
    std::ofstream file(path);

    if (!file.is_open()) {
        std::cout << "Error: Could not create file '" << path << "'\n";
        return;
    }

    // Write header
    file << "id,name,cost,expected_return_percent,risk_score\n";

    // Write data
    for (const auto& stock : stocks) {
        file << stock.id << ","
             << stock.name << ","
             << stock.cost << ","
             << stock.return_pct << ","
             << stock.risk << "\n";
    }

    file.close();
    std::cout << "Successfully saved " << stocks.size() << " stocks to '" << path << "'\n";
}
