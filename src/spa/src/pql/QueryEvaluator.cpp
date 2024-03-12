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

    //check for combo
    if (conditionType != "" && patternType != "") {

        if ((selectType == "w" || selectType == "i") && conditionType == "Parent" && isT && patternType == "pattern") {
            QueryProcessor::getParentT_Pattern_OutputParentT(leftArg, patternLeftArg, patternRightArg, isSubexpression, selectType, databaseResults);
        }
        else if (selectType == "a" && conditionType == "Parent" && isT && patternType == "pattern") {
            QueryProcessor::getParentT_Pattern_OutputAssign(leftArg, patternLeftArg, patternRightArg, isSubexpression, databaseResults);
        }
        else if (selectType == "p" && conditionType == "Modifies" && patternType == "pattern") {
            QueryProcessor::getModifies_Pattern_OutputProcedure(rightArg,patternLeftArg,patternRightArg,isSubexpression,databaseResults);
        }
    }

    else {
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
    }
    for (string res : databaseResults)
        output.push_back(res);

}

