#pragma once
#include <sstream>
#include <vector>
#include<iostream>
#include<queue>
#include <unordered_map>
#include <unordered_set>
#include "db/Database.h"
#include  "StatementInfo.h"


class AncestorProcessing {
public:
    static void processAncestor(unordered_map<int,int> parentChildMapping);

};


