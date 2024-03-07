#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include<iostream>
#include<stack>
#include <algorithm>
#include "Database.h"
#include "Tokenizer.h"
#include "utils/InfixToPostfix.h"


using namespace std;

class SourceProcessor {
private:
    struct AssignmentInfo {
        int lineCount;
        string statementContent;
    };
    bool isInteger(const string& intString);

    void processProcedure(bool &inProcedure, string &procedureName, int &i, const vector<string> &tokens);

    void processInProcedure(const string& token, const string& procedureName, int& i, int& lineCount, const vector<string>& tokens, stack<string>& statementTypes, stack<int>& parentStack, stack<bool>& expressionStack, vector<AssignmentInfo> &assignmentInfo);

    void processVariable(const string& varName, const int& lineCount);

    void processStatement(const string& procedureName, const string& token, int& i, int& lineCount, const vector<string>& tokens, stack<string>& statementTypes, stack<int>& parentStack, vector<AssignmentInfo> &assignmentInfo);

    void processConstant(const string& constantString, int& lineCount);

    void processReadPrintAssignment(const string& token, stack<string>& statementTypes);

    void processControlFlow(const string& token, stack<string>& statementTypes, stack<int>& parentStack, int& lineCount);

    void processExpression(std::vector<AssignmentInfo> &assignmentInfo);

public:
    void process(string& process);

};