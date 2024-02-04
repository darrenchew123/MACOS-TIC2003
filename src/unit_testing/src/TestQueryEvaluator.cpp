#include "catch.hpp"
#include "QueryEvaluator.h"
#include "SourceProcessor.h"

TEST_CASE("Evaluate simple query to select procedures") {
    string program = "procedure calculate {\n"
                     "    constValue = 10;\n"
                     "    const2 = 123;\n"
                     "}";
    SourceProcessor sp;
    sp.process(program);

    QueryEvaluator qp;
    string query = "procedure p ; Select p";
    vector<string> output;

    qp.evaluate(query, output);

    REQUIRE(output.size() == 1);
    REQUIRE(std::find(output.begin(), output.end(), "calculate") != output.end());
}

TEST_CASE("Evaluate simple query to select Variable") {
    string program = "procedure calculate {\n"
                     "    constValue = 10;\n"
                     "    const2 = 123;\n"
                     "}";
    SourceProcessor sp;
    sp.process(program);

    QueryEvaluator qp;
    string query = "variable v ; Select v";
    vector<string> output;

    qp.evaluate(query, output);

    REQUIRE(output.size() == 2);
    REQUIRE(std::find(output.begin(), output.end(), "constValue") != output.end());
    REQUIRE(std::find(output.begin(), output.end(), "const2") != output.end());
}

TEST_CASE("Evaluate simple query to select Print") {
    string program = "procedure calculate {\n"
                     "    read p;\n"
                     "    print p2;\n"
                     "}";
    SourceProcessor sp;
    sp.process(program);

    QueryEvaluator qp;
    string query = "print p ; Select p";
    vector<string> output;

    qp.evaluate(query, output);

    REQUIRE(output.size() == 1);
    REQUIRE(output[0] == "2");
}

TEST_CASE("Evaluate simple query to select Read") {
    string program = "procedure calculate {\n"
                     "    read p;\n"
                     "    print p2;\n"
                     "}";
    SourceProcessor sp;
    sp.process(program);

    QueryEvaluator qp;
    string query = "read r ; Select r";
    vector<string> output;

    qp.evaluate(query, output);

    REQUIRE(output.size() == 1);
    REQUIRE(output[0] == "1");
}

TEST_CASE("Evaluate simple query to select statement") {
    string program = "procedure calculate {\n"
                     "    read p;\n"
                     "    print p2;\n"
                     "}";
    SourceProcessor sp;
    sp.process(program);

    QueryEvaluator qp;
    string query = "stmt s ; Select s";
    vector<string> output;

    qp.evaluate(query, output);

    REQUIRE(output.size() == 2);
}