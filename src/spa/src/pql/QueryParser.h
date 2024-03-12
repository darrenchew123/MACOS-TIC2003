#pragma once

#include <string>
#include <vector>
#include <pql/Query.h>
#include "db/Database.h"
#include "utils/Tokenizer.h"
#include "utils/InfixToPostfix.h"
#include <unordered_set>
using namespace std;


class QueryParser {
public:

    static bool isT(const string& token);

    static void initSelectType(string token, Query& query);

    static string checkQuotationMarks_returnArg(int& currIdx, const vector<string>& tokens, Query& query);

    static Query parser(const vector<string>& tokens);

};




