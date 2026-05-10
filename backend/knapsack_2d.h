#ifndef KNAPSACK_2D_H
#define KNAPSACK_2D_H

#include "common.h"

// 2D Dynamic Programming (0/1 Knapsack)
// Time: O(n * budget), Space: O(n * budget)
OptimizationResult knapsack_2d(const std::vector<Stock>& stocks, int budget);

#endif
