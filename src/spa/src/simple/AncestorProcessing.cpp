#include "AncestorProcessing.h"

void AncestorProcessing::processAncestor(unordered_map<int,int> parentChildMapping) {
    std::unordered_set<int> visited; // Tracks visited nodes
    std::queue<int> q; // BFS queue

    // Initialize queue with all parent nodes to ensure all nodes are processed
    for (const auto& node : parentChildMapping) {
        if (visited.find(node.second) == visited.end()) {
            q.push(node.second);
            visited.insert(node.second);
        }
    }

    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        // Check if current node is a parent in the mapping
        auto it = parentChildMapping.find(curr);
        if (it != parentChildMapping.end()) {
            int child = it->second;

            // If this child hasn't been visited, add to queue and insert relation
            if (visited.find(child) == visited.end()) {
                q.push(child);
                visited.insert(child);
                Database::insertAncestorRelation(curr, child);
            }
        }
    }
}