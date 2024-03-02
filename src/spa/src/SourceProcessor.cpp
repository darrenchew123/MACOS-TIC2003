#include "SourceProcessor.h"

bool SourceProcessor::isInteger(const string& intString) {
    if (intString.empty()) return false;
    int startIndex = 0;
    return all_of(intString.begin() + startIndex, intString.end(), ::isdigit);
}

void SourceProcessor::flushCurrentStatementToCFG(std::string& currentStatement, std::shared_ptr<CFGNode>& lastNode, ControlFlowGraph& cfg, int& statementCodeLine) {
    if (!currentStatement.empty()) {
        auto node = cfg.createNode("Stmt: " + currentStatement);
        if (lastNode) {
            cfg.addEdge(lastNode, node); // Connect the last node to this new statement node
            // Assuming each node's ID corresponds to a statement code line for simplicity
            Database::insertParentChildRelation(lastNode->id, node->id);
        }
        lastNode = node; // Update the last node
        currentStatement.clear();
    }
}

//Parse source program
void SourceProcessor::process(string &program) {
    // Initialize the database
    Database::initialize();

    // Tokenize the program
    Tokenizer tk;
    vector<string> tokens;
    tk.tokenize(program, tokens);

    stack<std::shared_ptr<CFGNode>> contextStack; // Use this to manage block and control flow context
    std::shared_ptr<CFGNode> lastNode = nullptr; // Keep track of the last node for connecting edges

    ControlFlowGraph cfg;

    string currentStatement;
    int statementCodeLine = 1; // Initialize statement code line counter

    for (int i = 0; i < tokens.size(); ++i) {
        const string& token = tokens[i];

        // Handle procedure
        if (token == "procedure") {
            flushCurrentStatementToCFG(currentStatement, lastNode, cfg, statementCodeLine);
            string procedureName = tokens[++i]; // Assuming the next token is the procedure name
            auto node = cfg.createNode("Procedure: " + procedureName);
            contextStack.push(node); // New scope starts
            lastNode = node; // This node is now the last node for connections
            Database::insertProcedure(procedureName); // Database insert for the procedure
        }
            // Control structures - start new block
        else if (token == "if" || token == "while" || token == "else" || token == "{") {
            flushCurrentStatementToCFG(currentStatement, lastNode, cfg, statementCodeLine);
            string content = (token == "{") ? "Block Start" : "Control: " + token;
            auto node = cfg.createNode(content);
            if (lastNode != nullptr) cfg.addEdge(lastNode, node); // Connect control structure
            contextStack.push(node); // New scope starts
            lastNode = node; // Update last node
        }
            // End of control structure or block
        else if (token == "}") {
            flushCurrentStatementToCFG(currentStatement, lastNode, cfg, statementCodeLine);
            contextStack.pop(); // Exiting current block or control structure
            lastNode = contextStack.empty() ? nullptr : contextStack.top(); // Update lastNode to parent block
        }
            // Accumulate tokens into statements
        else {
            currentStatement += token + " ";
            // End of statement
            if (token.back() == ';') {
                auto node = cfg.createNode("Stmt: " + currentStatement);
                if (lastNode != nullptr) cfg.addEdge(lastNode, node); // Connect statements sequentially
                lastNode = node; // Update lastNode
                // Database insert for the statement
                Database::insertStatement("procedureName", "statementType", currentStatement, statementCodeLine++);
                currentStatement.clear(); // Reset current statement for next accumulation
            }
        }
    }

    cfg.display(); // For demonstration
}

