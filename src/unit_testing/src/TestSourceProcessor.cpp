#include "SourceProcessor.h"
#include "Database.h"

#include "catch.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <set>

using namespace std;

namespace TestSourceProcessor {
    void require(bool b) {
        REQUIRE(b);
    }

    TEST_CASE("CheckSourceProcessor") {
//         Create a SIMPLE program string
        std::string filePath = "../../../src/unit_testing/testfile/test.txt";
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != nullptr) {
            std::cout << "Current working directory: " << cwd << std::endl;
        } else {
            std::cerr << "getcwd() error" << std::endl;
            return;
        }

        // Open the file
        std::ifstream fileStream(filePath);
        if (!fileStream) {
            std::cerr << "Error opening file: " << filePath << std::endl;
            return;
        }

        // Read the file into a string
        std::stringstream buffer;
        buffer << fileStream.rdbuf();
        std::string program = buffer.str();

        Tokenizer tk;
        vector<string> tokens;
        tk.tokenize(program, tokens);

        std::cout << "Tokens: ";
        for (const auto& token : tokens) {
            std::cout << token << endl;
        }
        std::cout << std::endl;

        // Process the program
        SourceProcessor sp;
        sp.process(program);
        cout << "Processing completed." << endl;


        vector<string> procedures;
        Database::getProcedures(procedures);
        cout << "Procedures fetched: " << procedures.size() << endl;

        vector<string> statements;
        Database::getStatements(statements);
        cout << "Statement fetched: " << procedures.size() << endl;


        // Debugging
        std::cout << "Debugging Information:" << std::endl; // Use std::cout and std::endl
        for (const auto& proc : procedures) {
            std::cout << "Procedure: " << proc << std::endl;
        }
        for (const auto& stmt : statements) {
            std::cout << "Statement: " << stmt << std::endl;
        }

        require(procedures.size() == 3);
        cout << "Checking procedure size: " << procedures.size() << endl;

        // Check the names of the procedures
        require(find(procedures.begin(), procedures.end(), "input") != procedures.end());
        require(find(procedures.begin(), procedures.end(), "output") != procedures.end());
        require(find(procedures.begin(), procedures.end(), "findGCFLCM") != procedures.end());

    }
    TEST_CASE("TestVariableInsertion") {
        string program = "procedure testProcedure {\n"
                         "    read x;\n"
                         "    y = 5;\n"
                         "    print y;\n"
                         "}";
        SourceProcessor sp;
        sp.process(program);

        vector<string> variables;
        Database::getVariables(variables);
        cout << "Variables fetched: " << variables.size() << endl;

        // Check the number of variables inserted
        require(variables.size() == 2);

        for (const auto& variable : variables) {
            std::cout << variable << endl;
        }

        // Check the presence of specific variables
        require(find(variables.begin(), variables.end(), "x") != variables.end());
        require(find(variables.begin(), variables.end(), "y") != variables.end());

        cout << "Variable insertion test passed." << endl;
    }

    TEST_CASE("TestConstantInsertion") {
        string program = "procedure calculate {\n"
                         "    constValue = 10;\n"
                         "    const2 = 123;\n"
                         "}";
        SourceProcessor sp;
        sp.process(program);

        vector<string> constants;
        Database::getConstants(constants); // Assuming getConstants() returns all constants' values
        for (const auto& constant : constants) {
            std::cout <<"Constant Line: " << constant << endl;
        }
        // Check the number of constants inserted
        require(constants.size() == 2);


        // Check the value of the constant
        require(constants[0] == "10");
        require(constants[1] == "123");

        cout << "Constant insertion test passed." << endl;
    }

    TEST_CASE("TestStatementInsertion") {
        string program = "procedure actions {\n"
                         "    read inputValue;\n"
                         "    print result;\n"
                         "}";
        SourceProcessor sp;
        sp.process(program);

        vector<string> statements;
        Database::getStatements(statements); // Assuming this method returns all statement strings

        for (const auto& statement : statements) {
            std::cout <<"Statement: " << statement << endl;
        }
        // Check the number of statements inserted
        require(statements.size() == 2);

        // Check for specific statement types
        require(find(statements.begin(), statements.end(), "1") != statements.end());
        require(find(statements.begin(), statements.end(), "2") != statements.end());

        cout << "Statement insertion test passed." << endl;
    }

    TEST_CASE("TestReadPrint") {
        string program = "procedure sample {\n"
                         "    read num1;\n"
                         "    print num2;\n"
                         "    print num1;\n"
                         "    read num2;\n"
                         "}";
        SourceProcessor sp;
        sp.process(program);

        vector<string> readLines;
        vector<string> printLines;
        Database::getStatementType("read", readLines);
        Database::getStatementType("print", printLines);

        for (const auto& readLine : readLines) {
            std::cout <<"Read Line: " << readLine << endl;
        }
        for (const auto& printLine : printLines) {
            std::cout <<"Print Line: " << printLine << endl;
        }

        // Check the presence of variable in procedure
        require(readLines.size() == 2);
        require(printLines.size() == 2);
        require(readLines[0] == "1");
        require(readLines[1] == "4");
        require(printLines[0] == "2");
        require(printLines[1] == "3");

        cout << "TestReadPrint test passed." << endl;
    }

    TEST_CASE("TestAssignmentAndPrintOperations") {
        string program = "procedure echo {\n"
                         "    read num1;\n"
                         "    index = num1;\n"
                         "\n"
                         "    print num1;\n"
                         "    print index;\n"
                         "}";
        SourceProcessor sp;
        sp.process(program);

        vector<string> variables;
        Database::getVariables(variables);

        for (const auto& variable : variables) {
            std::cout <<"variables "<< variable << endl;
        }
        require(find(variables.begin(), variables.end(), "num1") != variables.end());
        require(find(variables.begin(), variables.end(), "index") != variables.end());

        vector<string> printLines;
        Database::getStatementType("print", printLines);


        require(printLines[0] == "3");
        require(printLines[1] == "4");
        for (const auto& printLine : printLines) {
            std::cout <<"print lines:  "<< printLine << endl;
        }
        vector<string> assignmentLines;
        Database::getStatementType("assign", assignmentLines);
        for (const auto& assignmentLine : assignmentLines) {
            std::cout <<"assignment lines:  "<< assignmentLine << endl;
        }
        require(assignmentLines[0] == "2");

        cout << "Assignment and print operations test passed." << endl;
    }

    TEST_CASE("TestControlFlowProcessing") {
        string program = "procedure ControlFlow {\n"
                         "    if (x == 1) then {\n"
                         "        print x;\n"
                         "    } else {\n"
                         "        print y;\n"
                         "    }\n"
                         "    z = 0;\n"
                         "    while (z < 5) {\n"
                         "        z = z + 1;\n"
                         "        print z;\n"
                         "    }\n"
                         "}";

        SourceProcessor sp;
        sp.process(program);

        vector<string> ifLines, whileLines;
        Database::getStatementType("if", ifLines);
        Database::getStatementType("while", whileLines);

        REQUIRE(ifLines.size() == 1);
        REQUIRE(whileLines.size() == 1);

    }
}
