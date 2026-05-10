#include "utils.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>

void add_stock(std::vector<Stock>& stocks, const Stock& stock) {
    if (has_duplicate_id(stocks, stock.id)) {
        std::cout << "Error: Stock with ID " << stock.id << " already exists.\n";
        return;
    }
    if (has_duplicate_name(stocks, stock.name)) {
        std::cout << "Error: Stock with name '" << stock.name << "' already exists.\n";
        return;
    }
    stocks.push_back(stock);
    std::cout << "Stock added successfully.\n";
}

bool edit_stock(std::vector<Stock>& stocks, int id) {
    auto it = std::find_if(stocks.begin(), stocks.end(), 
                          [id](const Stock& s) { return s.id == id; });
    
    if (it == stocks.end()) {
        std::cout << "Error: Stock with ID " << id << " not found.\n";
        return false;
    }

    std::cout << "Editing stock: " << it->name << "\n";
    std::cout << "Enter new cost (current: " << it->cost << "): ";
    int new_cost;
    if (!(std::cin >> new_cost) || new_cost <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid cost. Edit cancelled.\n";
        return false;
    }

    std::cout << "Enter new expected return % (current: " << it->return_pct << "): ";
    double new_return;
    if (!(std::cin >> new_return) || new_return < 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid return. Edit cancelled.\n";
        return false;
    }

    std::cout << "Enter new risk score (current: " << it->risk << "): ";
    double new_risk;
    if (!(std::cin >> new_risk) || new_risk < 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid risk. Edit cancelled.\n";
        return false;
    }

    it->cost = new_cost;
    it->return_pct = new_return;
    it->risk = new_risk;
    std::cout << "Stock updated successfully.\n";
    return true;
}

bool delete_stock(std::vector<Stock>& stocks, int id) {
    auto it = std::find_if(stocks.begin(), stocks.end(), 
                          [id](const Stock& s) { return s.id == id; });
    
    if (it == stocks.end()) {
        std::cout << "Error: Stock with ID " << id << " not found.\n";
        return false;
    }

    std::string name = it->name;
    stocks.erase(it);
    std::cout << "Stock '" << name << "' deleted successfully.\n";
    return true;
}

void print_stock_table(const std::vector<Stock>& stocks) {
    if (stocks.empty()) {
        std::cout << "No stocks in portfolio.\n";
        return;
    }

    std::cout << "\n";
    std::cout << "================================================================================\n";
    std::cout << std::left << std::setw(6) << "ID" 
              << std::setw(20) << "Name" 
              << std::right << std::setw(10) << "Cost" 
              << std::setw(15) << "Return %" 
              << std::setw(12) << "Risk" << "\n";
    std::cout << "--------------------------------------------------------------------------------\n";

    for (const auto& stock : stocks) {
        std::cout << std::left << std::setw(6) << stock.id
                  << std::setw(20) << stock.name
                  << std::right << std::setw(10) << stock.cost
                  << std::setw(15) << std::fixed << std::setprecision(2) << stock.return_pct
                  << std::setw(12) << std::fixed << std::setprecision(2) << stock.risk << "\n";
    }
    std::cout << "================================================================================\n";
    std::cout << "Total stocks: " << stocks.size() << "\n";
}

bool has_duplicate_name(const std::vector<Stock>& stocks, const std::string& name, int exclude_id) {
    return std::any_of(stocks.begin(), stocks.end(), 
                      [&name, exclude_id](const Stock& s) { 
                          return s.name == name && s.id != exclude_id; 
                      });
}

bool has_duplicate_id(const std::vector<Stock>& stocks, int id) {
    return std::any_of(stocks.begin(), stocks.end(), 
                      [id](const Stock& s) { return s.id == id; });
}
