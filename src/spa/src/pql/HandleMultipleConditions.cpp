#include "HandleMultipleConditions.h"


void HandleMultipleConditions::processSingleSelectMultiCond(string selectVar, string selectType, string conditionType, bool isT, string leftArg, string rightArg, string patternType, string patternLeftArg, string patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query queryToExecute) {
    vector<string> results;
    vector<string> curr;

    // Process conditions
    for (int i = 0; i < queryToExecute.conditions.size(); ++i) {
        cout << "how many times condition " <<endl;
        processCondition(queryToExecute.conditions[i], selectVar, selectType, patternType, patternLeftArg, patternRightArg, isSubexpression, curr, queryToExecute);

        if (i == 0) {
            results.swap(curr);
        } else {
            intersectResults(results, curr);
        }
        curr.clear();
    }
    for(auto s : results){
        cout  << "conditions result " << s << endl;
    }

    // Process patterns
    for (const auto& pattern : queryToExecute.patterns) {
        cout << "how many times pattern " <<endl;
        processPattern(pattern, selectVar, selectType, isSubexpression, curr, queryToExecute);
        intersectResults(results, curr);
        curr.clear();
    }
    for(auto s : results){
        cout  << "pattern result " << s << endl;
    }

    databaseResults = results;
}
void HandleMultipleConditions::intersectResults(vector<string>& results, vector<string> curr) {
    vector<string> intersection;
    sort(curr.begin(), curr.end());
    sort(results.begin(), results.end());
    set_intersection(curr.begin(), curr.end(), results.begin(), results.end(), back_inserter(intersection));
    results = intersection;

    cout << "results: ";
    for (const auto& a : results) {
        cout << a << " ";
    }
    cout << endl;
}

void HandleMultipleConditions::processCondition(Condition condition, string selectVar, string selectType, string patternType, string patternLeftArg, string patternRightArg, bool isSubexpression, vector<string>& curr, Query queryToExecute) {
    cout << "processing condition\n";
    HandleSimpleQueries::processSimpleQuery(selectVar, selectType, condition.conditionType, condition.isT, condition.leftArg, condition.rightArg, "", patternLeftArg, patternRightArg, isSubexpression, curr, queryToExecute);
    cout << "curr: ";
    for (const auto& a : curr) {
        cout << a << " ";
    }
    cout << endl;
}

void HandleMultipleConditions::processPattern(Pattern pattern, string selectVar, string selectType, bool isSubexpression, vector<string>& curr, Query queryToExecute) {
    cout << "Processing pattern\n";
    string emptyConditionType = "";
    HandleSimpleQueries::processSimpleQuery(selectVar, selectType, emptyConditionType, false, "", "", pattern.patternType, pattern.patternLeftArg, pattern.patternRightArg, isSubexpression, curr, queryToExecute);
    cout << "curr: ";
    for (const auto& a : curr) {
        cout << a << " ";
    }
    cout << endl;
}
