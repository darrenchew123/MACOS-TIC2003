#include <iostream>
#include <vector>
#include <string>
#include <memory> // Include for smart pointers
#include <Database.h>

using namespace std;

struct CFGNode {
    int id;
    string content;
    vector<std::shared_ptr<CFGNode>> next; // Correctly named to 'next' for edges

    CFGNode(int id, string content) : id(id), content(std::move(content)) {}
};

class ControlFlowGraph {
public:
    vector<std::shared_ptr<CFGNode>> nodes;
    std::shared_ptr<CFGNode> entry = nullptr;
    int nextNodeId = 0;

    std::shared_ptr<CFGNode> createNode(const string& content) {
        // Use make_shared for efficient smart pointer creation
        auto node = std::make_shared<CFGNode>(nextNodeId++, content);
        nodes.push_back(node);
        if (!entry) entry = node; // Set entry if not already set
        return node;
    }

    void addEdge(const std::shared_ptr<CFGNode>& from, const std::shared_ptr<CFGNode>& to) {
        if (from && to) {
            from->next.push_back(to);
            Database::insertParentChildRelation(from->id, to->id); // Automatically record the relationship
        }
    }

    void display() const {
        for (const auto& node : nodes) {
            cout << "Node " << node->id << ": " << node->content << " -> ";
            for (const auto& next : node->next) {
                cout << next->id << " ";
            }
            cout << endl;
        }
    }
};
