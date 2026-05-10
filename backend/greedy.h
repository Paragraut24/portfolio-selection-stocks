#ifndef GREEDY_H
#define GREEDY_H

#include "common.h"

// Greedy heuristic (by return/cost ratio)
// Time: O(n log n), Space: O(n)
OptimizationResult greedy_knapsack(const std::vector<Stock>& stocks, int budget);

#endif
