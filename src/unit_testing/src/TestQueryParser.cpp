#include "catch.hpp"
#include "pql/QueryEvaluator.h"
#include "pql/QueryParser.h"
#include "utils/Tokenizer.h"


TEST_CASE("QueryProcessor Declaration Processing") {
    vector<string> tokens = {"procedure", "p", ";", "variable", "v", ";"};
    unordered_map<string, string> declaredObjects;
    QueryParser::processObjects(tokens, declaredObjects);

    REQUIRE(declaredObjects.size() == 2);
    REQUIRE(declaredObjects["p"] == "procedure");
    REQUIRE(declaredObjects["v"] == "variable");
}

TEST_CASE("QueryProcessor Select Processing") {
    vector<string> tokens = {"Select", "p"};
    vector<pair<string, string>> selectObjects;
    unordered_map<string, string> declaredObjects = {{"p", "procedure"}};

    QueryParser::processSelect(tokens, declaredObjects, selectObjects);

    REQUIRE(selectObjects.size() == 1);
    REQUIRE(selectObjects[0] == std::make_pair(std::string("procedure"), std::string("p")));
}

TEST_CASE("QueryProcessor Invalid Select") {
    vector<string> tokens = {"Select", "undefined"};
    vector<pair<string, string>> selectObjects;
    unordered_map<string, string> declaredObjects = {{"p", "procedure"}};

    REQUIRE_THROWS_AS(QueryParser::processSelect(tokens, declaredObjects, selectObjects), runtime_error);
}

TEST_CASE("QueryProcessor Full Parsing") {
    QueryEvaluator qp;
    string query = "procedure p; variable v; Select p";
    vector<string> output;
    vector<pair<string, string>> selectObjects;
    Tokenizer tk;
    vector<string> tokens;
    tk.tokenize(query, tokens);

    QueryParser::parser(tokens, selectObjects);

    REQUIRE(selectObjects.size() == 1);

    REQUIRE(selectObjects[0] == std::make_pair(std::string("procedure"), std::string("p")));
}

