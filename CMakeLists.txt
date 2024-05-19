cmake_minimum_required(VERSION 3.26)
project(code)

set(CMAKE_CXX_STANDARD 17)

add_executable(code main.cpp
        LexicalAnalysis/include/Lexer.h
        LexicalAnalysis/src/Lexer.cpp
        LexicalAnalysis/include/Statechart.h
        LexicalAnalysis/src/Statechart.cpp
        LexicalAnalysis/src/Token.cpp
)
