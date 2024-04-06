#include "ModifiesProcessing.h"

void ModifiesProcessing::processModifies(vector<StatementInfo>& statementInfo, multimap<int,int> parentChildMapping) {
    map<int, string> lineToTypeMapping;
    set<int> insertedParents;
    for (auto info : statementInfo) {
        lineToTypeMapping[info.lineCount] = info.statementType;
    }

    for (const auto& info : statementInfo) {
        std::string variableName = extractModifiesVariable(info.statementContent, info.statementType);
        if (!variableName.empty()) {
            for (const auto& pair : parentChildMapping) {
                if (pair.second == info.lineCount) {
                    int parentLine = pair.first;
                    string parentStatementType = lineToTypeMapping[parentLine];
                    if (parentStatementType == "if" || parentStatementType == "while") {
                        Database::insertModifies(parentLine, variableName);
                    }
                }
            }
            Database::insertModifies(info.lineCount, variableName);
        }
    }
}

// Extract variable
string ModifiesProcessing::extractModifiesVariable(const string& statement, const string& statementType) {
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