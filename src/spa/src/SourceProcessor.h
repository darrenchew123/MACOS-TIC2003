#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include<iostream>
#include<stack>
#include "Database.h"
#include "Tokenizer.h"
#include "ControlFlowGraph.cpp"

using namespace std;

class SourceProcessor {
private:
    ControlFlowGraph cfg;
    CFGNode* currentProcedureNode = nullptr;
    stack<CFGNode*> contextStack; // Stack to manage nested blocks
    int lineCount = 0; // Keeps track of line numbers for database entries

    bool isInteger(const string& intString);

    void processProcedure(bool &inProcedure, string &procedureName, int &i, const vector<string> &tokens);

    void processInProcedure(const string& token, const string& procedureName, int& i, int& lineCount, const vector<string>& tokens, stack<string>& statementTypes, stack<int>& parentStack);

    void processVariable(const string& varName, const int& lineCount);

    void processStatement(const string& procedureName, const string& token, int& i, int& lineCount, const vector<string>& tokens, stack<string>& statementTypes, stack<int>& parentStack);

    void processConstant(const string& constantString, int& lineCount);

    void processReadPrintAssignment(const string& token, stack<string>& statementTypes);

    void processControlFlow(const string& token, stack<string>& statementTypes, stack<int>& parentStack, int& lineCount);
    void flushCurrentStatementToCFG(string& currentStatement, std::shared_ptr<CFGNode>& lastNode, ControlFlowGraph& cfg, int& statementCodeLine);

public:
    void process(string& process);

};