#include "QueryEvaluator.h"

QueryEvaluator::QueryEvaluator() {}

QueryEvaluator::~QueryEvaluator() {}


//Evalutate query to get result from DB
void QueryEvaluator::evaluate(string query, vector<string>& output) {
    output.clear();

    vector<string> databaseResults;

    Tokenizer tk;
    vector<string> tokens;
    tk.tokenize(query, tokens);

    Query queryToExecute = QueryParser::parser(tokens);

    string selectType = queryToExecute.selectType;
    string conditionType = queryToExecute.condition.type;
    bool isT = queryToExecute.condition.isT;
    string leftArg = queryToExecute.condition.leftArg;
    string rightArg = queryToExecute.condition.rightArg;

    string patternType = queryToExecute.pattern.patternType;
    string patternLeftArg = queryToExecute.pattern.patternLeftArg;
    string patternRightArg = queryToExecute.pattern.patternRightArg;
    bool isSubexpression = queryToExecute.pattern.isSubexpression;

    // Determine if query is a combo or simple type and process
    if (!conditionType.empty() && !patternType.empty()) {
        processComboQuery(selectType, conditionType, isT, leftArg, rightArg, patternType, patternLeftArg, patternRightArg, isSubexpression, databaseResults);
    } else {
        processSimpleQuery(selectType, conditionType, isT, leftArg, rightArg, patternType, patternLeftArg, patternRightArg, isSubexpression, databaseResults);
    }

    output.insert(output.end(), databaseResults.begin(), databaseResults.end());
}

// Process combo queries
void QueryEvaluator::processComboQuery(string selectType, string conditionType, bool isT, string leftArg, string rightArg, string patternType, string patternLeftArg, string patternRightArg, bool isSubexpression, vector<string>& databaseResults) {
    if ((selectType == "w" || selectType == "i") && conditionType == "Parent" && isT && patternType == "pattern") {
        QueryProcessor::getParentT_Pattern_OutputParentT(leftArg, patternLeftArg, patternRightArg, isSubexpression, selectType, databaseResults);
    }
    else if (selectType == "a" && conditionType == "Parent" && isT && patternType == "pattern") {
        QueryProcessor::getParentT_Pattern_OutputAssign(leftArg, patternLeftArg, patternRightArg, isSubexpression, databaseResults);
    }
    else if (selectType == "p" && conditionType == "Modifies" && patternType == "pattern") {
        QueryProcessor::getModifies_Pattern_OutputProcedure(rightArg,patternLeftArg,patternRightArg,isSubexpression,databaseResults);
    }
        // Select a such that Modifies (a, v) pattern a (v, _"n"_)
    else if (selectType == "a" && conditionType == "Modifies" && patternType == "pattern" && rightArg == patternLeftArg) {
        QueryProcessor::getModifies_Pattern_OutputAssign(patternRightArg, isSubexpression, databaseResults);
    }
        // Select v such that Modifies (a, v) pattern a1 (v, _"n"_)
    else if (selectType == "v" && conditionType == "Modifies" && patternType == "pattern" && rightArg == patternLeftArg) {
        QueryProcessor::getModifies_Pattern_OutputVar(patternRightArg, isSubexpression, databaseResults);
    }
}

// Process simple queries
void QueryEvaluator::processSimpleQuery(string selectType, string conditionType, bool isT, string leftArg, string rightArg, string patternType, string patternLeftArg, string patternRightArg, bool isSubexpression, vector<string>& databaseResults) {
    if (selectType == "p") {
        if (conditionType == "Modifies") {
            Database::getModifies_OutputProcedures(rightArg, databaseResults);
        }
        else
            Database::getProcedures(databaseResults);
    }
    else if (selectType == "v") {
        if (conditionType == "Modifies") {
            Database::getModifies_OutputVar(leftArg, databaseResults);
        }
        else
            Database::getVariables(databaseResults);
    }
    else if (selectType == "c") {
        Database::getConstants(databaseResults);
    }
    else if (selectType == "s") {
        if (conditionType == "Parent") {
            if (isT) {
                Database::getParentT_OutputStmt(leftArg, databaseResults);
            }
            else {
                Database::getParent_OutputStmt(rightArg, databaseResults);
            }
        }
        else {
            Database::getStatements(databaseResults);
        }
    }

    else if (selectType == "a") {
        if (conditionType == "Parent") {
            if (isT) {
                Database::getParentT_OutputAssign(leftArg, databaseResults);
            }
        }
        else if (patternType == "pattern") {
            Database::getPattern_OutputStmt(patternLeftArg, patternRightArg, isSubexpression, databaseResults);
        }
        else
            Database::getStatementType(selectType, databaseResults);
    }
    else if (selectType == "p") {
        Database::getStatementType(selectType, databaseResults);
    }
    else if (selectType == "r") {
        Database::getStatementType(selectType, databaseResults);
    }
    else if (selectType == "w" || selectType == "i") {
        if (conditionType == "Modifies") {
            QueryProcessor::getModifies_OutputParents(rightArg, selectType, databaseResults);
        }
    }
}



