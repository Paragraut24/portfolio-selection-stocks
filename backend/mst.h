#ifndef MST_H
#define MST_H

#include "common.h"

// Minimum Spanning Tree using Kruskal's algorithm for portfolio diversification
// Time: O(E log E), Space: O(V + E)
// Uses correlation/similarity between stocks as edge weights
MSTResult portfolio_diversification_mst(const std::vector<Stock>& stocks);

#endif
