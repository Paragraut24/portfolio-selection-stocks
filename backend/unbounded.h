#ifndef UNBOUNDED_H
#define UNBOUNDED_H

#include "common.h"
#include <vector>

// Unbounded Knapsack - allows buying same stock multiple times
OptimizationResult unbounded_knapsack(const std::vector<Stock>& stocks, int budget);

#endif
