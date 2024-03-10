#pragma once

#include <string>
#include <vector>
#include "db/Database.h"
#include "utils/Tokenizer.h"
#include "QueryParser.h"



using namespace std;

class QueryEvaluator {

public:
	QueryEvaluator();

    ~QueryEvaluator();

	void evaluate(string query, vector<string>& results);
};