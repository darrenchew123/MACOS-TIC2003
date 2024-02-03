#include "SourceProcessor.h"
#include "Database.h"

#include "catch.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>

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
//        string program = "procedure echo {\n"
//                         "    read num1;\n"
//                         "    index = num1;\n"
//                         "\n"
//                         "    print num1;\n"
//                         "    print index;\n"
//                         "}";
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


        // Debugging: Print out the procedures and statements
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
}
