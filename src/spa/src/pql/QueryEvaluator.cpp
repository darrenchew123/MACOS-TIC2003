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

    if (!queryToExecute.conditions.empty()) {
            conditionType = queryToExecute.conditions[0].conditionType;
            isT = queryToExecute.conditions[0].isT;
            leftArg = queryToExecute.conditions[0].leftArg;
            rightArg = queryToExecute.conditions[0].rightArg;
    }
    cout << "left arg " << leftArg << " right arg " <<  rightArg << " select-type " << selectType << endl;
    if (!queryToExecute.patterns.empty()) {
        patternType = queryToExecute.patterns[0].patternType;
        patternLeftArg = queryToExecute.patterns[0].patternLeftArg;
        patternRightArg = queryToExecute.patterns[0].patternRightArg;
        isSubexpression = queryToExecute.patterns[0].isSubexpression;
    }

    //Determine if query is a combo or simple type and process
    int condSize = queryToExecute.conditions.size();
    int patternSize = queryToExecute.patterns.size();
    int totalSize = condSize + patternSize;

    if (totalSize > 1) {
        isMultipleCond = true;
    }
    if (!queryToExecute.multiSelectVar.empty()) {
        isMultiSelect = true;
    }
    cout << "isMultipleCond= " << isMultipleCond << " isMultipleSelect= " << isMultiSelect << endl;

    if (isMultiSelect) {
        cout << "multi select" << endl;
//        process_multiSelect( queryToExecute, isMultipleCond);
    }
    else if (isMultipleCond) {
        cout << "single select + multicond" << endl;
        processSingleSelectMultiCond(selectVar, selectType, conditionType, isT, leftArg, rightArg, patternType, patternLeftArg, patternRightArg, isSubexpression, databaseResults, queryToExecute);
    } else {
        cout << "single select + single cond" << endl;
        processSimpleQuery(selectVar, selectType, conditionType, isT, leftArg, rightArg, patternType, patternLeftArg, patternRightArg, isSubexpression, databaseResults, queryToExecute);
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



void QueryEvaluator::processSingleSelectMultiCond(string selectVar, string selectType, string conditionType, bool isT, string leftArg, string rightArg, string patternType, string patternLeftArg, string patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query queryToExecute) {

    vector<string> results;
    vector<string> curr;

    vector<Condition> conditions = queryToExecute.conditions;

    for (int i = 0; i < conditions.size(); i++) {

        cout << "processing condition " << i+1 << endl;

        string conditionType;
        bool isT = 0;
        string leftArg;
        string rightArg;

        if (!queryToExecute.conditions.empty()) {
            conditionType = queryToExecute.conditions[i].conditionType;
            isT = queryToExecute.conditions[i].isT;
            leftArg = queryToExecute.conditions[i].leftArg;
            rightArg = queryToExecute.conditions[i].rightArg;
        }

        processSimpleQuery(selectVar, selectType, conditionType, isT, leftArg, rightArg, patternType, patternLeftArg, patternRightArg, isSubexpression, curr, queryToExecute);

        cout << "curr: ";
        for (auto a : curr) {
            cout << a << " ";
        }
        cout << endl;

        if (i == 0) {
            results = curr;
        }
        else {
            //intersect curr and results
            sort(curr.begin(), curr.end()); // set_intersection requires sorted ranges
            sort(results.begin(), results.end()); // set_intersection requires sorted ranges

            vector<string> intersection;
            set_intersection(results.begin(), results.end(),
                             curr.begin(), curr.end(),
                             back_inserter(intersection));
            results = intersection;

        }
        curr.clear();

        cout << "results: ";
        for (auto a : results) {
            cout << a << " ";
        }
        cout << endl;
    }

    vector<Pattern> patterns = queryToExecute.patterns;

    for (int i = 0; i < patterns.size(); i++) {

        cout << "processing pattern " << i+1 << endl;

        string patternType;
        string patternLeftArg;
        string patternRightArg;
        bool isSubexpression = 0;

        if (!queryToExecute.patterns.empty()) {
            patternType = queryToExecute.patterns[i].patternType;
            patternLeftArg = queryToExecute.patterns[i].patternLeftArg;
            patternRightArg = queryToExecute.patterns[i].patternRightArg;
            isSubexpression = queryToExecute.patterns[i].isSubexpression;
        }
        conditionType = "";

        processSimpleQuery(selectVar, selectType, conditionType, isT, leftArg, rightArg, patternType, patternLeftArg, patternRightArg, isSubexpression, curr, queryToExecute);

        cout << "curr: ";
        for (auto a : curr) {
            cout << a << " ";
        }
        cout << endl;


        //intersect curr and results
        sort(curr.begin(), curr.end()); // set_intersection requires sorted ranges
        sort(results.begin(), results.end()); // set_intersection requires sorted ranges

        vector<string> intersection;
        set_intersection(
                results.begin(), results.end(),
                curr.begin(), curr.end(),
                back_inserter(intersection));

        results = intersection;


        curr.clear();

        cout << "results: ";
        for (auto a : results) {
            cout << a << " ";
        }
        cout << endl;

    }

    databaseResults = results;
}


// process simple queries
void QueryEvaluator::processSimpleQuery(string selectVar, string selectType, string conditionType, bool isT, string leftArg, string rightArg, string patternType, string patternLeftArg, string patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query queryToExecute) {
    if (selectType == "procedure") {
        if (conditionType == "Modifies") {
            Database::getModifies_OutputProcedures(rightArg, databaseResults, queryToExecute);
        }
        else if (conditionType == "Uses") {
            Database::getUses_OutputProcedures(leftArg, databaseResults, queryToExecute);
        }
        else if (conditionType == "calls") {
            if(isT){
                Database::getCallsT_OutputProcedures(leftArg,rightArg ,databaseResults, queryToExecute);
            }else {
                Database::getCalls_OutputProcedures(leftArg,rightArg ,databaseResults, queryToExecute);
            }
        }
        else
            Database::getProcedures(databaseResults);
    }
    else if (selectType == "print") {
        if (conditionType == "Uses") {
            Database::getUses_OutputType(leftArg, rightArg,databaseResults,queryToExecute);
        }
        else if (conditionType == "Parent") {
            if (isT) {
                cout << "ParentT" << endl;
                Database::getParentT(selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
            }

            else {
                cout << "Parent" << endl;
                Database::getParent(selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
            }
        }
        else {
            Database::getStatementType(selectType, databaseResults);
        }
    }
    else if (selectType == "variable") {
        if (conditionType == "Modifies") {
            Database::getModifies_OutputVar(leftArg, databaseResults, queryToExecute);
        }
        else if (conditionType == "Parent") {
            if (isT) {
                Database::getParentT(selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
            }
            else {
                Database::getParent(selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
            }
        }
        else if(patternType == "pattern"){
            Database::getVariablesPattern(databaseResults,patternRightArg,isSubexpression);
        }
        else if (conditionType == "Uses"){
            Database::getUses_OutputVar(leftArg, databaseResults, queryToExecute);
        }
        else{
            Database::getVariables(databaseResults);
        }
    }
    else if (selectType == "constant") {
        if (conditionType == "Parent") {
            if (isT) {
                Database::getParentT(selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
            }

            else {
                Database::getParent(selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
            }
        }
        else
            Database::getConstants(databaseResults);
    }
    else if (selectType == "stmt") {
        if (conditionType == "Parent") {
            if (isT) {
                Database::getParentT(selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
            }

            else {
                Database::getParent(selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
            }
        }
        else if (conditionType == "Uses"){
            Database::getUses_OutputStmt(leftArg, rightArg, databaseResults, queryToExecute);
        }
        else if (conditionType == "Modifies") {
            Database::getModifies_OutputStmt(rightArg, databaseResults,queryToExecute);
        }
        else {
            Database::getStatements(databaseResults);
        }
    }

    else if (selectType == "assign") {
        if (conditionType == "Parent") {
            if (isT) {
                Database::getParentT(selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
            } else {
                Database::getParent(selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
            }
        }
        else if (conditionType == "Uses"){
            Database::getUses_OutputType(leftArg,rightArg,databaseResults,queryToExecute);
        }
        else if (patternType == "pattern") {
            Database::getPattern_OutputStmt(patternLeftArg, patternRightArg, isSubexpression, databaseResults, queryToExecute);
        }
        else
            Database::getStatementType(selectType, databaseResults);
    }
    else if (selectType == "read") {
        if (conditionType == "Parent") {
            if (isT) {
                Database::getParentT(selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
            }
            else {
                Database::getParent(selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
            }
        }
        else {
            Database::getStatementType(selectType, databaseResults);
        }
    }
    else if (selectType == "while" || selectType == "if") {
        if (conditionType == "Modifies") {
            Database::getModifies_OutputParents(selectType, databaseResults);
        }
        else if (conditionType == "Parent") {
            if (isT) {
                cout << "ParentT" << endl;
                Database::getParentT(selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
            }

            else {
                cout << "Parent" << endl;
                Database::getParent(selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
            }
        }
        else if (patternType == "pattern") {
            Database::getPattern_OutputStmt(patternLeftArg, patternRightArg, isSubexpression, databaseResults, queryToExecute);
        }
        else {
            Database::getStatementType(selectType, databaseResults);
        }
    }
}



