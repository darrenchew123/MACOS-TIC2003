
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

        string expectedOutput = "1$2$";

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
        cout << "Test Output: "<<testOutput << endl;
        string expectedOutput = "var1,var2,";

        require(testOutput == expectedOutput);
    }

    TEST_CASE("CheckDatabaseConstants") {
        Database::initialize();
        Database::insertConstant(1,100);
        Database::insertConstant(2,200);

        vector<string> dbResults;
        Database::getConstants(dbResults);


        string testOutput;
        for (const auto& constantLine : dbResults) {
            testOutput.append(constantLine + "$");
        }
        cout <<"Test Output: "<<testOutput << endl;

        string expectedOutput = "100$200$";


        require(testOutput == expectedOutput);
    }

    TEST_CASE("CheckDatabaseParentChildRelation") {
        Database::initialize();
        Database::insertParentChildRelation(1, 2);
        Database::insertParentChildRelation(2, 3);

        vector<string> dbResults;
        Database::getParentChildRelations(dbResults);

        string testOutput;
        for (const auto& relation : dbResults) {
            testOutput.append(relation + "$");
        }
        cout <<"Test Output: "<<testOutput << endl;

        string expectedOutput = "1$2$"; // Assuming the format "parent-child"

        require(testOutput == expectedOutput);
    }

    TEST_CASE("CheckDatabaseNextRelation") {
        Database::initialize();
        Database::insertNextRelation(1, 2);
        Database::insertNextRelation(2, 3);

        vector<string> dbResults;
        Database::getNextRelations(dbResults);

        string testOutput;
        for (const auto& relation : dbResults) {
            testOutput.append(relation + "$");
        }
        cout <<"Test Output: "<<testOutput << endl;

        string expectedOutput = "1$2$";

        require(testOutput == expectedOutput);
    }

    TEST_CASE("CheckDatabaseUses") {
        Database::initialize();
        Database::insertUses(1, "varA");
        Database::insertUses(2, "varB");

        vector<string> dbResults;
        Database::getUses(dbResults);

        string testOutput;
        for (const auto& use : dbResults) {
            testOutput.append(use + "$");
        }
        cout <<"Test Output: "<<testOutput << endl;

        string expectedOutput = "1$2$";

        require(testOutput == expectedOutput);
    }

    TEST_CASE("CheckDatabaseModifies") {
        Database::initialize();
        Database::insertModifies(1, "varX");
        Database::insertModifies(2, "varY");

        vector<string> dbResults;
        Database::getModifies(dbResults);

        string testOutput;
        for (const auto& modify : dbResults) {
            testOutput.append(modify + "$");
        }
        cout <<"Test Output: "<<testOutput << endl;

        string expectedOutput = "1$2$";

        require(testOutput == expectedOutput);
    }

    TEST_CASE("CheckDatabasePattern") {
        Database::initialize();
        Database::insertPattern(1, "LHS1", "RHS1");
        Database::insertPattern(2, "LHS2", "RHS2");

        vector<string> dbResults;
        Database::getPatterns(dbResults);

        string testOutput;
        for (const auto& pattern : dbResults) {
            testOutput.append(pattern + "$");
        }
        cout <<"Test Output: "<<testOutput << endl;

        string expectedOutput = "RHS1$RHS2$";

        require(testOutput == expectedOutput);
    }

}



