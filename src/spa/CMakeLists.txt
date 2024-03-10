file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers}
        src/QueryParser.cpp
        src/QueryParser.h
        src/utils/InfixToPostfix.cpp
        src/utils/InfixToPostfix.h
        src/utils/SyntaxValidator.cpp
        src/utils/SyntaxValidator.h
        src/utils/StringUtil.cpp
        src/utils/StringUtil.h)
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


