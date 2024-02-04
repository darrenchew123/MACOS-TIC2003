#include "catch.hpp"
#include "QueryProcessor.h"
#include "Tokenizer.h"

TEST_CASE("QueryProcessor Type Validation") {
    QueryProcessor qp;
    REQUIRE(qp.typeValidator("procedure") == true);
    REQUIRE(qp.typeValidator("variable") == true);
    REQUIRE(qp.typeValidator("constant") == true);
    REQUIRE(qp.typeValidator("assign") == true);
    REQUIRE(qp.typeValidator("print") == true);
    REQUIRE(qp.typeValidator("read") == true);
    REQUIRE(qp.typeValidator("stmt") == true);
    REQUIRE(qp.typeValidator("nonexistentType") == false);
}

TEST_CASE("QueryProcessor Declaration Processing") {
    QueryProcessor qp;
    vector<string> tokens = {"procedure", "p", ";", "variable", "v", ";"};
    unordered_map<string, string> declaredObjects;
    qp.processObjects(tokens, declaredObjects);

    REQUIRE(declaredObjects.size() == 2);
    REQUIRE(declaredObjects["p"] == "procedure");
    REQUIRE(declaredObjects["v"] == "variable");
}

TEST_CASE("QueryProcessor Select Processing") {
    QueryProcessor qp;
    vector<string> tokens = {"Select", "p"};
    vector<pair<string, string>> selectObjects;
    unordered_map<string, string> declaredObjects = {{"p", "procedure"}};

    qp.processSelect(tokens, declaredObjects, selectObjects);

    REQUIRE(selectObjects.size() == 1);
    REQUIRE(selectObjects[0] == std::make_pair(std::string("procedure"), std::string("p")));
}

TEST_CASE("QueryProcessor Invalid Select") {
    QueryProcessor qp;
    vector<string> tokens = {"Select", "undefined"};
    vector<pair<string, string>> selectObjects;
    unordered_map<string, string> declaredObjects = {{"p", "procedure"}};

    REQUIRE_THROWS_AS(qp.processSelect(tokens, declaredObjects, selectObjects), runtime_error);
}

TEST_CASE("QueryProcessor Full Parsing") {
    QueryProcessor qp;
    string query = "procedure p; variable v; Select p";
    vector<string> output;
    vector<pair<string, string>> selectObjects;
    Tokenizer tk;
    vector<string> tokens;
    tk.tokenize(query, tokens);

    qp.parser(tokens, selectObjects);

    REQUIRE(selectObjects.size() == 1);

    REQUIRE(selectObjects[0] == std::make_pair(std::string("procedure"), std::string("p")));
}

