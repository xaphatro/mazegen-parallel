#include "kruskal.h"
#include <algorithm>
#include <numeric>
#include <random>
#include <omp.h>

std::vector<std::pair<int, int>> Kruskal::MinimumSpanningTree(int vertices, const Graph& adjacencylist) {
    std::vector<std::pair<int, int>> edges;
    //parallelizing this actually makes it slower, since the overhead is too high
    #pragma omp parallel for
    for (int i = 0; i < vertices; ++i) {
        for (const auto& edge : adjacencylist[i]) {
            //there is only one small line executed serially, thus giving no benefit
            #pragma omp critical
            {
                if (edge.first > i) edges.push_back({i, edge.first});
            }
        }
    }
    shuffle(edges.begin(), edges.end(), generator);

    parent_ = std::vector<int>(vertices);
    std::iota(parent_.begin(), parent_.end(), 0);

    minimumspanningtree.clear();
    //parallelizing this for ends up in a segementation fault
    for (const auto& edge : edges) {
        int u = GetParent(edge.first), v = GetParent(edge.second);
        if (u == v) continue;
        parent_[u] = v;
        minimumspanningtree.push_back(edge);
    }
    return minimumspanningtree;
}

int Kruskal::GetParent(int u) {
  return (parent_[u] == u) ? u : (parent_[u] = GetParent(parent_[u]));
}
