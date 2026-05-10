#ifndef KNAPSACK_1D_H
#define KNAPSACK_1D_H

#include "common.h"

// 1D Space-Optimized Dynamic Programming with traceback
// Time: O(n * budget), Space: O(budget)
OptimizationResult knapsack_1d(const std::vector<Stock>& stocks, int budget);

#endif
