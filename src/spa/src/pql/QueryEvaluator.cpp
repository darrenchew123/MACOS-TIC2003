#include "QueryEvaluator.h"
#include "QueryParser.h"

QueryEvaluator::QueryEvaluator() {}

QueryEvaluator::~QueryEvaluator() {}


//Evalutate query to get result from DB
void QueryEvaluator::evaluate(string query, vector<string>& output) {
    output.clear();
    vector<string> databaseResults;
    Tokenizer tk;
    vector<string> tokens;
    string conditionType, leftArg, rightArg, selectType, selectVar, patternType, patternLeftArg, patternRightArg;
    bool isT = false, isSubexpression = false, isMultipleCond = false, isMultiSelect = false;

    tk.tokenize(query, tokens);
    Query queryToExecute = QueryParser::parser(tokens);

    selectType = queryToExecute.selectType;
    selectVar = queryToExecute.selectVar;

    extractConditions(queryToExecute, conditionType, isT, leftArg, rightArg);
    extractPatterns(queryToExecute, patternType, patternLeftArg, patternRightArg, isSubexpression);
    determineQueryComplexity(queryToExecute, isMultipleCond, isMultiSelect);

    if (isMultiSelect) {
        cout << "multi select" << endl;
//        process_multiSelect( queryToExecute, isMultipleCond);
    }
    else if (isMultipleCond) {
        cout << "single select + multicond" << endl;
        HandleMultipleConditions::processSingleSelectMultiCond(selectVar, selectType, conditionType, isT, leftArg, rightArg, patternType, patternLeftArg, patternRightArg, isSubexpression, databaseResults, queryToExecute);
    } else {
        cout << "single select + single cond" << endl;
        HandleSimpleQueries::processSimpleQuery(selectVar, selectType, conditionType, isT, leftArg, rightArg, patternType, patternLeftArg, patternRightArg, isSubexpression, databaseResults, queryToExecute);
    }
    output.insert(output.end(), databaseResults.begin(), databaseResults.end());
}


//void QueryEvaluator::process_multiSelect(Query queryToExecute, bool isMultiSelect ) {
//    vector<vector<string>> allResults;
//
//    // Example loop over conditions
//    for (auto& selectVar : queryToExecute.multiSelectVar) {
//        vector<string> result = evaluateSimpleQuery(selectVar,queryToExecute.declaredVariables[selectVar],queryToExecute.conditions,);
//        allResults.push_back(result);
//    }
//
//    // Now merge based on AND or OR logic
//    vector<string> mergedResults;
//    if (isAndLogic) {
//        // Start with the first result set
//        mergedResults = allResults[0];
//
//        // Intersect with each subsequent result set
//        for (size_t i = 1; i < allResults.size(); ++i) {
//            mergedResults = intersectResults(mergedResults, allResults[i]);
//        }
//    } else { // OR logic
//        // Simply take the union of all result sets
//        for (auto& result : allResults) {
//            mergedResults = unionResults(mergedResults, result);
//        }
//    }
//
//    // Now 'mergedResults' contains the final result according to the specified logic
//}


void QueryEvaluator::extractConditions(const Query& queryToExecute, string& conditionType, bool& isT, string& leftArg, string& rightArg) {
    if (!queryToExecute.conditions.empty()) {
        conditionType = queryToExecute.conditions[0].conditionType;
        isT = queryToExecute.conditions[0].isT;
        leftArg = queryToExecute.conditions[0].leftArg;
        rightArg = queryToExecute.conditions[0].rightArg;
        cout << "left arg " << leftArg << " right arg " << rightArg << " select-type " << queryToExecute.selectType << endl;
    }
}

void QueryEvaluator::extractPatterns(const Query& queryToExecute, string& patternType, string& patternLeftArg, string& patternRightArg, bool& isSubexpression) {
    if (!queryToExecute.patterns.empty()) {
        patternType = queryToExecute.patterns[0].patternType;
        patternLeftArg = queryToExecute.patterns[0].patternLeftArg;
        patternRightArg = queryToExecute.patterns[0].patternRightArg;
        isSubexpression = queryToExecute.patterns[0].isSubexpression;
    }
}

void QueryEvaluator::determineQueryComplexity(const Query& queryToExecute, bool& isMultipleCond, bool& isMultiSelect) {
    int condSize = queryToExecute.conditions.size();
    int patternSize = queryToExecute.patterns.size();
    int totalSize = condSize + patternSize;

    isMultipleCond = totalSize > 1;
    isMultiSelect = !queryToExecute.multiSelectVar.empty();

    cout << "isMultipleCond= " << isMultipleCond << " isMultiSelect= " << isMultiSelect << endl;
}

