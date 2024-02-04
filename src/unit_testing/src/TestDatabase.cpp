
#include "Database.h"
#include<iostream>

#include "catch.hpp"
using namespace std;

// Each cpp is a set of test cases for a specific component.
namespace TestDatabase {
    void require(bool b) {
        REQUIRE(b);
    }

    // The name of a test case should be unique and meaningful.
    TEST_CASE("CheckDatabaseProcedure") {
        // initialize the database and insert a procedure
        Database::initialize();
        Database::insertProcedure("echo1");
        Database::insertProcedure("echo2");

        // retrieve the procedures from the database
        vector<string> dbResults;
        Database::getProcedures(dbResults);

        // create the test output string from the procedures retrieved
        string testOutput;
        for (unsigned int i = 0; i < dbResults.size(); i++)
        {
            testOutput.append(dbResults.at(i) + "$");
        }

        // create the expected output string
        string expectedOutput = "echo1$echo2$";

        // compare the test output with expected output
        require(testOutput == expectedOutput);

        // The test output should match with the expected output
        // and hence the assertion would be true.
    }

    TEST_CASE("CheckDatabaseStatements") {
        Database::initialize();
        string procedureName = "testProcedure";
        Database::insertProcedure(procedureName);
        Database::insertStatement(procedureName, "read", "var1", 1);
        Database::insertStatement(procedureName, "print", "var2", 2);

        vector<string> dbResults;
        Database::getStatements(dbResults);

        string testOutput;
        for (const auto& statement : dbResults) {
            testOutput.append(statement + "$");
        }

        string expectedOutput = "Code Line: 1, Procedure: testProcedure, Type: read, Content: var1$Code Line: 2, Procedure: testProcedure, Type: print, Content: var2$";

        require(testOutput == expectedOutput);
    }

    TEST_CASE("CheckDatabaseVariables") {
        Database::initialize();
        Database::insertVariable("var1", 1);
        Database::insertVariable("var2", 2);

        vector<string> dbResults;
        Database::getVariables(dbResults);

        string testOutput;
        for (const auto& variable : dbResults) {
            testOutput.append(variable + ",");
        }

        string expectedOutput = "var1,var2,";
        cout << testOutput << endl;
        require(testOutput == expectedOutput);
    }

    TEST_CASE("CheckDatabaseConstants") {
        Database::initialize();
        Database::insertConstant(1,100);
        Database::insertConstant(2,200);

        vector<string> dbResults;
        Database::getConstants(dbResults);

        string testOutput;
        for (const auto& constant : dbResults) {
            testOutput.append(constant + "$");
        }

        string expectedOutput = "Constant Value: 100$Constant Value: 200$";

        require(testOutput == expectedOutput);
    }

}



