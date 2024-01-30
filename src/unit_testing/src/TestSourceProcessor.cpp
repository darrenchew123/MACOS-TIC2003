#include "SourceProcessor.h"
#include "Database.h"

#include "catch.hpp"
#include <iostream>
using namespace std;

namespace TestSourceProcessor {
    void require(bool b) {
        REQUIRE(b);
    }

    TEST_CASE("CheckSourceProcessor") {
        // Create a SIMPLE program string
        string program = "procedure Sample { read x; y = 10; print y; }";

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

        vector<string> procedures;
        Database::getProcedures(procedures);
        vector<string> statements;
        Database::getStatements(statements);

        // Debugging: Print out the procedures and statements
        std::cout << "Debugging Information:" << std::endl; // Use std::cout and std::endl
        for (const auto& proc : procedures) {
            std::cout << "Procedure: " << proc << std::endl;
        }
        for (const auto& stmt : statements) {
            std::cout << "Statement: " << stmt << std::endl;
        }

        require(procedures.size() == 1);
        require(procedures[0] == "Sample");

        require(statements.size() == 3);
        require(statements[0].find("read") != string::npos && statements[0].find("x") != string::npos);
        require(statements[1].find("assign") != string::npos && statements[1].find("y = 10") != string::npos);
        require(statements[2].find("print") != string::npos && statements[2].find("y") != string::npos);

    }
}
