cmake_minimum_required(VERSION 3.26)
project(code)

set(CMAKE_CXX_STANDARD 17)

add_executable(code main.cpp
        LexicalAnalysis/include/Lexer.h
        LexicalAnalysis/src/Lexer.cpp
        LexicalAnalysis/include/Statechart.h
        LexicalAnalysis/src/Statechart.cpp
        LexicalAnalysis/src/Token.cpp
        Error/Error.h
        Error/Error.cpp
        SyntacticAnalysis/include/Parser.h
        SyntacticAnalysis/src/Parser.cpp
        SyntacticAnalysis/include/LL1Parser.h
        SyntacticAnalysis/src/LL1Parser.cpp
)
