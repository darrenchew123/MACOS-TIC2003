#include "SourceProcessor.h"

//Skip token
bool SourceProcessor::skipTokenCheck(const string& token, int& i, const vector<string>& tokens) {
    return token == "{" || token == ";" || token == "+" || token == "-" || token == "*" ||
           token == "/" || token == "then" ||
           (token == "\n" && tokens.at(i-1) == "\n") ||
           (token == "\n" && tokens.at(i-1) == "{" && tokens.at(i-2) == "else") ||
           (token == "\n" && tokens.at(i-1) == "}");
}

// Process procedure
void SourceProcessor::processProcedure(bool &inProcedure, string &procedureName, int &i, const vector<string> &tokens) {
    cout << "Entering processProcedure with token: " << tokens[i] << endl;
    inProcedure = true;
    procedureName = tokens[++i]; // Move to the next token to get the procedure name

    if (!SyntaxValidator::checkName(procedureName)) {
        cerr << "Procedure name does not adhere to naming convention: " << procedureName << endl;
        throw runtime_error("Procedure name error");
    }

    Database::insertProcedure(procedureName);
}

// Process statement and insert into statement table
void SourceProcessor::processStatement(const string& procedureName, const string& token, int& i, int& lineCount, const vector<string>& tokens, stack<string>& statementTypes, stack<int>& parentStack, vector<StatementInfo> &statementInfo, bool& pendingParentPush ) {
    if(statementTypes.size() == 0) return;
    cout << "Processing statement type :" << statementTypes.top() << " at line count: " << lineCount;
    if(!parentStack.empty()){
        cout << " Parent line code is :" << parentStack.top() << endl;
    }
    string statementContent;
    int counter = i - 2;
    while(counter>=0 && tokens[counter]!= "\n"){
        statementContent = tokens[counter] + " " + statementContent;
        counter--;
    }
    statementInfo.push_back({lineCount, statementContent, statementTypes.top()});
    Database::insertStatement(procedureName, statementTypes.top(), statementContent, lineCount);
    statementTypes.pop();
    if (!parentStack.empty() && parentStack.top()!=lineCount) {
        cout << "Inserting parentChild parent :" << parentStack.top() << " child: " << lineCount << endl;
        int parentLine = parentStack.top();
        Database::insertParentChildRelation(parentLine, lineCount);
    }
    if (pendingParentPush ) {
        cout << "Pushing into parent stack: " << lineCount << endl;
        parentStack.push(lineCount );
        pendingParentPush = false;
    }

}

// Process variable and insert into variable table
void SourceProcessor::processVariable(const string& varName, const int& lineCount){
    if(!SyntaxValidator::checkName(varName)) return;
    Database::insertVariable(varName,lineCount);
}

//Process constant and insert into constant table
void SourceProcessor::processConstant(const string& constantString, int& lineCount) {
    int constantValue = stoi(constantString);
    Database::insertConstant(lineCount, constantValue);
}

//Process read print assignment logic and add into statement type
void SourceProcessor::processReadPrintAssignment(const string& token, stack<string>& statementTypes) {
    if(token == "=") statementTypes.push("assign");
    else statementTypes.push(token);
    cout << "Pushed to statementTypes: " << token << endl;
}

// Process control flow for if/else/while
void SourceProcessor::processControlFlow(const string& token, stack<string>& statementTypes, stack<int>& parentStack, int& lineCount, stack<int>& ifStack, bool& pendingParentPush) {
    if (token == "if" || token == "while") {
        statementTypes.push(token);
        cout << "Pushed to statementTypes: " << token << endl;
        pendingParentPush = true;
        if (token == "if") {
            ifStack.push(lineCount);
        }
    } else if (token == "else") {
        if (ifStack.empty()) {
            throw runtime_error("Syntax error: 'else' without preceding 'if'");
        }
        // Handling else; popping the ifStack indicates closing the if block
        cout << "Else push: " <<ifStack.top() <<endl;
        parentStack.push(ifStack.top());
        ifStack.pop();
    } else if (token == "}") {
        if (!parentStack.empty()) {
            cout << "Deleting parentChild parent :" << parentStack.top() << " child: " << lineCount << endl;
            parentStack.pop();
        }
    }
}

// Process "()"
void SourceProcessor::handleExpressionStack(const string& token, stack<bool>& expressionStack) {
    if (token == "(") {
        expressionStack.push(true);
    } else if (token == ")" && !expressionStack.empty()) {
        expressionStack.pop();
    }
}

// Process token to respective calls
void SourceProcessor::delegateTokenProcessing(const string& token, const string& procedureName, int& i, int& lineCount, const vector<string>& tokens, stack<string>& statementTypes, stack<int>& parentStack, stack<bool>& expressionStack, vector<StatementInfo>& statementInfo, stack<int>& ifStack, bool& pendingParentPush) {
    cout << "Delegating token: " << token << ", lineCount: " << lineCount << endl;
    if (token == "read" || token == "print" || token == "=") {
        if (expressionStack.empty() || !expressionStack.top()) {
            processReadPrintAssignment(token, statementTypes);
        }
    } else if (SyntaxValidator::isInteger(token)) {
        processConstant(token, lineCount);
    } else if (token == "\n") {
        processStatement(procedureName, token, i, lineCount, tokens, statementTypes, parentStack, statementInfo,pendingParentPush);
        lineCount++;
    } else if (token == "if" || token == "else" || token == "while" || token == "}") {
        processControlFlow(token, statementTypes, parentStack, lineCount, ifStack,pendingParentPush);
    } else {
        processVariable(token, lineCount);
    }
}

// Process expression to LHS RHS
void SourceProcessor::processExpression(vector<StatementInfo> &statementInfo) {
    for (const auto& info : statementInfo) {
        if(info.statementType != "assign") continue;
        size_t equalsPosition = info.statementContent.find('=');
        if (equalsPosition != string::npos) {
            string lhs = info.statementContent.substr(0, equalsPosition);
            string rhs = info.statementContent.substr(equalsPosition + 1);

            rhs = InfixToPostfix::infixToPostfix(rhs);

            lhs = StringUtil::trim(lhs);
            rhs = StringUtil::trim(rhs);

            Database::insertPattern(info.lineCount, lhs, rhs);
        } else {
            cerr << "Assignment operator not found in statement: " << info.statementContent << endl;
        }
    }
}

// Extract variable
string SourceProcessor::extractVariableName(const string& statement, const string& statementType) {
    istringstream iss(statement);
    string word;
    if (statementType == "read") {
        iss >> word >> word;
        return word;
    } else if (statementType == "assign") {
        getline(iss, word, '=');
        word.erase(remove_if(word.begin(), word.end(), ::isspace), word.end());
        return word;
    }
    return "";
}

void SourceProcessor::processModifies(vector<StatementInfo>& statementInfo) {
    for (const auto& info : statementInfo) {
        string variableName = extractVariableName(info.statementContent, info.statementType);
        if (!variableName.empty()) {
            Database::insertModifies(info.lineCount, variableName);
        }
    }
}

// Process SIMPLE code inside the procedure
void SourceProcessor::processInProcedure(const string& token, const string& procedureName, int& i, int& lineCount, const vector<string>& tokens, stack<string>& statementTypes, stack<int>& parentStack, stack<bool>& expressionStack, vector<StatementInfo>& statementInfo, stack<int>& ifStack, bool& pendingParentPush) {
    if (skipTokenCheck(token, i, tokens)) {
        return;
    }

    handleExpressionStack(token, expressionStack);
    delegateTokenProcessing(token, procedureName, i, lineCount, tokens, statementTypes, parentStack, expressionStack, statementInfo, ifStack,pendingParentPush);
}

//Parse source program
void SourceProcessor::process(string &program) {
    // Initialize the database
    Database::initialize();

    // Tokenize the program
    Tokenizer tk;
    vector<string> tokens;
    tk.tokenize(program, tokens);
    stack<string> statementTypes;
    stack<bool> expressionStack;
    stack<int> parentStack, ifStack;
    string lhs, rhs;
    vector<StatementInfo> statementInfo;
    bool pendingParentPush = false;


    int lineCount = 0;
    int blockDepth = 0;
    bool inProcedure = false;
    string procedureName;

    for (int i = 0; i < tokens.size(); i++) {
        string token = tokens[i];
        cout << "Processing token: " << token << " at index: " << i << endl;

        if (token == "procedure") {
            blockDepth = 0;
            processProcedure(inProcedure, procedureName, i, tokens);
            inProcedure = true;
        }

        if (inProcedure) {
            if (token != "procedure") {
                processInProcedure(token, procedureName, i, lineCount, tokens, statementTypes, parentStack, expressionStack, statementInfo, ifStack, pendingParentPush);
            }
            if (token == "{") {
                blockDepth++;
            } else if (token == "}") {
                if (blockDepth > 0) {
                    blockDepth--;
                } else {
                    inProcedure = false;
                }
            }
        }
    }

    processExpression(statementInfo);
    processModifies(statementInfo);
}