#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

struct Stock {
    int id;
    std::string name;
    int cost;
    double return_pct;
    double risk;
};

void add_stock(std::vector<Stock>& stocks, const Stock& stock);
bool edit_stock(std::vector<Stock>& stocks, int id);
bool delete_stock(std::vector<Stock>& stocks, int id);
void print_stock_table(const std::vector<Stock>& stocks);
bool has_duplicate_name(const std::vector<Stock>& stocks, const std::string& name, int exclude_id = -1);
bool has_duplicate_id(const std::vector<Stock>& stocks, int id);

#endif
