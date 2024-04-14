file(GLOB db "${CMAKE_CURRENT_SOURCE_DIR}/src/db/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/db/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/src/db/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/db/*.hpp")
file(GLOB pql "${CMAKE_CURRENT_SOURCE_DIR}/src/pql/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/pql/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/src/pql/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/pql/*.hpp")
file(GLOB simple "${CMAKE_CURRENT_SOURCE_DIR}/src/simple/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/simple/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/src/simple/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/simple/*.hpp")
file(GLOB utils "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/*.hpp")
add_library(spa ${db} ${pql} ${simple} ${utils}
        src/simple/TokenProcessing.cpp
        src/simple/TokenProcessing.h
        src/simple/StatementProcessing.cpp
        src/simple/StatementProcessing.h
        src/simple/ControlFlow.cpp
        src/simple/ControlFlow.h
        src/simple/ProcedureProcessing.cpp
        src/simple/ProcedureProcessing.h
        src/simple/ExpressionProcessing.cpp
        src/simple/ExpressionProcessing.h
        src/simple/VariableProcessing.cpp
        src/simple/VariableProcessing.h
        src/simple/ProcessConstant.cpp
        src/simple/ProcessConstant.h
        src/simple/ModifiesProcessing.cpp
        src/simple/ModifiesProcessing.h
        src/simple/UsesProcessing.cpp
        src/simple/UsesProcessing.h
        src/simple/StatementInfo.h
        src/pql/Query.h
        src/simple/CallsProcessing.h
        src/simple/AncestorProcessing.h
        src/pql/HandleSimpleQueries.cpp
        src/pql/HandleMultipleConditions.cpp
        src/pql/HandleMultipleSelect.h
        src/db/Parent_HelperFunctions.cpp)
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


