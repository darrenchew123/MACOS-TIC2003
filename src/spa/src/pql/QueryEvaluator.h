#pragma once

#include <string>
#include <vector>
#include "db/Database.h"
#include "utils/Tokenizer.h"
#include "QueryParser.h"
#include "QueryProcessor.h"


using namespace std;

class QueryEvaluator {

public:

    QueryEvaluator();

    ~QueryEvaluator();

    static void evaluate(string query, vector<string>& output);

};