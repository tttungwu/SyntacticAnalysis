//
// Created by Tung Wu on 2024/5/20.
//

#ifndef CODE_NONTERMINAL_H
#define CODE_NONTERMINAL_H

#include <string>


class NonTerminal {
public:
    std::string code;

    static std::string newlabel();
    static std::string newtemp();
};

class P : public NonTerminal{

};

class K : public NonTerminal{

};

class S : public NonTerminal{
public:
    std::string begin, next;
};

class A : public NonTerminal{

};

class C : public NonTerminal{
public:
    std::string t_label, f_label;
};

class E : public NonTerminal{
public:
    std::string place;
};

class B : public NonTerminal{
public:
    std::string place;
    std::string t_label, f_label;
};

class T : public NonTerminal{
public:
    std::string place;
};

class D : public NonTerminal{
public:
    std::string iplace, place, icode;
};

class F : public NonTerminal{
public:
    std::string place;
};

class G : public NonTerminal{
public:
    std::string iplace, place, icode;
};


#endif //CODE_NONTERMINAL_H
