#include <string>

using namespace std;

#ifndef QUERY_H
#define QUERY_H

struct Query {

    string selectType;
    string declaredVar;

    struct Condition {

        string type;
        string leftArg;
        string rightArg;
        bool isT = false;
    } condition;

    struct Pattern {

        string patternType;
        string var;
        string patternLeftArg;
        string patternRightArg;
        bool isSubexpression = false;

    } pattern;
};

#endif //QUERY_H
