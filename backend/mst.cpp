#include "mst.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <map>

// Union-Find (Disjoint Set Union) data structure
class UnionFind {
private:
    std::map<int, int> parent;
    std::map<int, int> rank;
    
public:
    void make_set(int x) {
        if (parent.find(x) == parent.end()) {
            parent[x] = x;
            rank[x] = 0;
        }
    }
    
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // Path compression
        }
        return parent[x];
    }
    
    bool union_sets(int x, int y) {
        int root_x = find(x);
        int root_y = find(y);
        
        if (root_x == root_y) {
            return false; // Already in same set
        }
        
        // Union by rank
        if (rank[root_x] < rank[root_y]) {
            parent[root_x] = root_y;
        } else if (rank[root_x] > rank[root_y]) {
            parent[root_y] = root_x;
        } else {
            parent[root_y] = root_x;
            rank[root_x]++;
        }
        
        return true;
    }
};

// Calculate similarity/correlation between two stocks
// Lower weight = more diverse (better for MST)
double calculate_stock_similarity(const Stock& s1, const Stock& s2) {
    // Similarity based on risk difference and return difference
    // Stocks with similar risk and return are more correlated
    double risk_diff = std::abs(s1.risk - s2.risk);
    double return_diff = std::abs(s1.return_pct - s2.return_pct);
    
    // Normalize and combine (lower = more similar)
    double similarity = 1.0 / (1.0 + risk_diff + return_diff);
    
    return similarity;
}

MSTResult portfolio_diversification_mst(const std::vector<Stock>& stocks) {
    auto start = std::chrono::high_resolution_clock::now();
    
    MSTResult result;
    result.algo_used = "Kruskal MST";
    result.total_weight = 0.0;
    
    if (stocks.size() < 2) {
        // Need at least 2 stocks for MST
        if (stocks.size() == 1) {
            result.diversified_stock_ids.push_back(stocks[0].id);
        }
        auto end = std::chrono::high_resolution_clock::now();
        result.runtime_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        return result;
    }
    
    // Build complete graph with all edges
    std::vector<Edge> edges;
    for (size_t i = 0; i < stocks.size(); i++) {
        for (size_t j = i + 1; j < stocks.size(); j++) {
            Edge e;
            e.stock1_id = stocks[i].id;
            e.stock2_id = stocks[j].id;
            e.weight = calculate_stock_similarity(stocks[i], stocks[j]);
            edges.push_back(e);
        }
    }
    
    // Sort edges by weight (ascending - Kruskal's algorithm)
    std::sort(edges.begin(), edges.end());
    
    // Initialize Union-Find
    UnionFind uf;
    for (const auto& stock : stocks) {
        uf.make_set(stock.id);
    }
    
    // Kruskal's algorithm: select edges that don't form cycles
    for (const auto& edge : edges) {
        if (uf.union_sets(edge.stock1_id, edge.stock2_id)) {
            result.selected_edges.push_back(edge);
            result.total_weight += edge.weight;
        }
    }
    
    // Extract unique stock IDs from selected edges
    std::map<int, bool> stock_set;
    for (const auto& edge : result.selected_edges) {
        stock_set[edge.stock1_id] = true;
        stock_set[edge.stock2_id] = true;
    }
    
    for (const auto& pair : stock_set) {
        result.diversified_stock_ids.push_back(pair.first);
    }
    
    // Sort for consistent output
    std::sort(result.diversified_stock_ids.begin(), result.diversified_stock_ids.end());
    
    auto end = std::chrono::high_resolution_clock::now();
    result.runtime_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    return result;
}
