#ifndef CSV_IO_H
#define CSV_IO_H

#include "common.h"
#include <string>

// CSV Input/Output functions
std::vector<Stock> load_stocks_csv(const std::string& filename);
void save_portfolio_results_csv(const std::string& filename, const OptimizationResult& result, const std::vector<Stock>& all_stocks);
void save_algorithm_comparison_csv(const std::string& filename, const std::vector<OptimizationResult>& results);
void save_diversification_output_csv(const std::string& filename, const MSTResult& result, const std::vector<Stock>& all_stocks);

#endif
