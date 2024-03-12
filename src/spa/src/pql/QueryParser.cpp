#include "QueryParser.h"


bool QueryParser::isT(const string& token) {
    if (token == "*")
        return 1;
    return 0;
}

void QueryParser::initSelectType(string token, Query& query) {

    query.declaredVar = token;
    query.selectType = token[0];
}

string QueryParser::checkQuotationMarks_returnArg(int& currIdx, const vector<string>& tokens, Query& query) {

    string res = "";

    //check for isSubexpression for pattern
    if (tokens[currIdx] == "_" && tokens[currIdx + 1] == "\"") {

        currIdx += 2;

        query.pattern.isSubexpression = true;

        while (currIdx < tokens.size() && tokens[currIdx] != "\"") {
            res += tokens[currIdx];
            currIdx++;
        }
        currIdx++;
        res = InfixToPostfix::infixToPostfix(res); // convert to postfix
    }
        // tokens withing quotations marks
    else if (tokens[currIdx] == "\"") {

        currIdx++;

        while (currIdx < tokens.size() && tokens[currIdx] != "\"") {
            res += tokens[currIdx];
            currIdx++;
        }
        currIdx++;
        res = InfixToPostfix::infixToPostfix(res); // convert to postfix
    }
        //single token
    else {
        res = tokens[currIdx];
        currIdx++;
    }
    return res;
}

Query QueryParser::parser(const vector<string>& tokens) {
    Query query;

    unordered_set<string> conditionTypes{ "Follows", "Modifies", "Uses", "Parent" };
    unordered_set<string> patternTypes{ "pattern" };


    for (int i = 0; i < tokens.size(); ++i) {

        if (tokens[i] == "Select") {

            initSelectType(tokens[i + 1], query);
            i++;
        }
        else if (tokens[i] == "such" && tokens[i + 1] == "that") {

            i += 2;

            bool flag_isT = isT(tokens[i + 1]);

            if (conditionTypes.count(tokens[i]) && flag_isT) {

                query.condition.type = tokens[i];
                query.condition.isT = true;

                i += 3;
                //check for quotation marks on left/right args
                query.condition.leftArg = checkQuotationMarks_returnArg(i, tokens, query);

                i++;
                query.condition.rightArg = checkQuotationMarks_returnArg(i, tokens, query);

            }
            else if (conditionTypes.count(tokens[i])) {

                query.condition.type = tokens[i];

                i += 2;
                query.condition.leftArg = checkQuotationMarks_returnArg(i, tokens, query);
                i++;
                query.condition.rightArg = checkQuotationMarks_returnArg(i, tokens, query);
            }
        }
            //check for pattern
        else if (patternTypes.count(tokens[i])) {

            query.pattern.patternType = tokens[i];
            query.pattern.var = tokens[i + 1];
            i += 3;
            query.pattern.patternLeftArg = checkQuotationMarks_returnArg(i, tokens, query);
            i++;
            query.pattern.patternRightArg = checkQuotationMarks_returnArg(i, tokens, query);
        }
    }

    return query;
}