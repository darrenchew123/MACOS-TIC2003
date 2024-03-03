#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include<iostream>
#include<stack>
#include "Database.h"
#include "Tokenizer.h"

using namespace std;

class SourceProcessor {
private:
    bool isInteger(const string& intString);

    void processProcedure(bool &inProcedure, string &procedureName, int &i, const vector<string> &tokens);

    void processInProcedure(const string& token, const string& procedureName, int& i, int& lineCount, const vector<string>& tokens, stack<string>& statementTypes, stack<int>& parentStack, stack<bool>& expressionStack);

    void processVariable(const string& varName, const int& lineCount);

    void processStatement(const string& procedureName, const string& token, int& i, int& lineCount, const vector<string>& tokens, stack<string>& statementTypes, stack<int>& parentStack);

    void processConstant(const string& constantString, int& lineCount);

    void processReadPrintAssignment(const string& token, stack<string>& statementTypes);

    void processControlFlow(const string& token, stack<string>& statementTypes, stack<int>& parentStack, int& lineCount);

public:
    void process(string& process);

};