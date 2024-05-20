//
// Created by Tung Wu on 2024/5/19.
//

#include "../include/LL1Parser.h"


std::vector<Token> LL1Parser::tokens;
Token LL1Parser::cur_token = Token("", Type::EMPTY);
long long LL1Parser::token_idx = 0;
int LL1Parser::line_cnt = 1;

void LL1Parser::match(Type expectedType, std::string expectedValue)
{
    if (LL1Parser::cur_token.getType() == expectedType && (expectedValue.empty() || LL1Parser::cur_token.getValue() == expectedValue))
    {
        if (LL1Parser::token_idx < LL1Parser::tokens.size() - 1)
            LL1Parser::cur_token = LL1Parser::tokens[++ LL1Parser::token_idx];
        else
        {
            ++ LL1Parser::token_idx;
            LL1Parser::cur_token = Token("", Type::EMPTY);
            if (LL1Parser::token_idx > LL1Parser::tokens.size())
                Error::printErrors(ErrorType::SyntaxError, "match: Missing tokens", true, LL1Parser::line_cnt);
        }
    }
    else Error::printErrors(ErrorType::SyntaxError, "match: Ungrammatical", true, LL1Parser::line_cnt);
}

void LL1Parser::parse(std::vector<Token> tokens)
{
    if (tokens.empty()) return;

    // 初始化数据
    LL1Parser::tokens = tokens;
    LL1Parser::token_idx = 0;
    LL1Parser::cur_token = LL1Parser::tokens[LL1Parser::token_idx];
    LL1Parser::line_cnt = 0;

    // 进行语法分析
#ifndef USE_VISUALIZATION
    LL1Parser::parseP();
#else
    P *p = new P();
    NTree* root = LL1Parser::parseP(p);
    std::vector<bool> flag(30, true);
    NTree::printNTree(root, flag);
    auto formatPrint = [](std::string str) -> std::string {
        std::istringstream codeStream(str);
        std::string line;
        std::vector<std::string> lines;
        std::string indent = "    ";  // 4个空格的缩进
        while (std::getline(codeStream, line)) {
            lines.push_back(line);
        }
        std::ostringstream formattedCode;
        auto startsWithLabel = [](const std::string& line) -> bool {
            std::regex labelRegex("^L[0-9]+:");
            return std::regex_search(line, labelRegex);
        };
        for (const auto& line : lines) {
            if (startsWithLabel(line)) formattedCode << line << "\n";
            else formattedCode << indent << line << "\n";
        }
        return formattedCode.str();
    };
    std::cout << "IR: \n" << formatPrint(p->code) << std::endl;
#endif
}

ReturnType LL1Parser::parseP(P *p)
{
#ifndef USE_VISUALIZATION
    LL1Parser::parseK();
#else
    NTree* root = new NTree("P");
    K *k = new K();
    NTree* son = parseK(k);
    p->code = k->code;
    root->append(son);
    return root;
#endif
}

ReturnType LL1Parser::parseK(K *k, S *pre_s)
{
#ifndef USE_VISUALIZATION
    if (LL1Parser::cur_token.getType() == Type::EMPTY) return;
    LL1Parser::parseS();
    LL1Parser::match(Type::PERIOD, ";");
    ++ LL1Parser::line_cnt;
    LL1Parser::parseK();
#else
    NTree* root = new NTree("K"), *son;
    if (LL1Parser::cur_token.getType() == Type::EMPTY)
    {
        k->code = "";
        if (pre_s != nullptr) k->code = pre_s->next + ": \n";
        son = new NTree("ε");
        root->append(son);
    }
    else
    {
        S *s = new S();
        s->next = NonTerminal::newlabel();
        if (pre_s != nullptr) s->begin = pre_s->next;
        son = LL1Parser::parseS(s);
        root->append(son);
        LL1Parser::match(Type::PERIOD, ";");
        son = new NTree(";");
        root->append(son);
        ++ LL1Parser::line_cnt;
        K *k1 = new K();
        son = LL1Parser::parseK(k1, s);
        k->code = s->code + k1->code;
        root->append(son);
    }
    return root;
#endif
}

ReturnType LL1Parser::parseS(S *s)
{
#ifndef USE_VISUALIZATION
    if (LL1Parser::cur_token.getType() == Type::IDN)
    {
        LL1Parser::match(Type::IDN);
        LL1Parser::match(Type::SIGN, "=");
        parseE();
    }
    else if (LL1Parser::cur_token.getValue() == "if")
    {
        LL1Parser::match(Type::KEYWORDS, "if");
        parseC();
        LL1Parser::match(Type::KEYWORDS, "then");
        parseS();
        parseA();
    }
    else if (LL1Parser::cur_token.getValue() == "while")
    {
        LL1Parser::match(Type::KEYWORDS, "while");
        LL1Parser::parseC();
        LL1Parser::match(Type::KEYWORDS, "do");
        LL1Parser::parseS();
    }
    else Error::printErrors(ErrorType::SyntaxError, "parseS: Ungrammatical", true,LL1Parser::line_cnt);
#else
    NTree* root = new NTree("S"), *son;
    if (LL1Parser::cur_token.getType() == Type::IDN)
    {
        std::string idPlace = cur_token.getValue();
        son = new NTree(LL1Parser::cur_token.getValue());
        root->append(son);
        LL1Parser::match(Type::IDN);
        son = new NTree(LL1Parser::cur_token.getValue());
        LL1Parser::match(Type::SIGN, "=");
        E *e = new E();
        son = parseE(e);
        root->append(son);
        s->code = (!s->begin.empty() ? s->begin + ": " : "") + e->code + idPlace + ":=" + e->place + "\n";
    }
    else if (LL1Parser::cur_token.getValue() == "if")
    {
        if (s->begin.empty()) s->begin = NonTerminal::newlabel();
        son = new NTree(LL1Parser::cur_token.getValue());
        root->append(son);
        LL1Parser::match(Type::KEYWORDS, "if");
        C *c = new C();
        c->t_label = NonTerminal::newlabel();
        c->f_label = "L_";
        son = parseC(c);
        root->append(son);
        son = new NTree(LL1Parser::cur_token.getValue());
        root->append(son);
        LL1Parser::match(Type::KEYWORDS, "then");
        S *s1 = new S();
        s1->next = s->next;
        s1->begin = c->t_label;
        son = parseS(s1);
        root->append(son);
        A *a = new A();
        son = parseA(a, s, c, s1);
        s->code = a->code;
        root->append(son);
    }
    else if (LL1Parser::cur_token.getValue() == "while")
    {
        if (s->begin.empty()) s->begin = NonTerminal::newlabel();
        son = new NTree(LL1Parser::cur_token.getValue());
        root->append(son);
        LL1Parser::match(Type::KEYWORDS, "while");
        C *c = new C();
        c->t_label = NonTerminal::newlabel();
        c->f_label = s->next;
        son = LL1Parser::parseC(c);
        root->append(son);
        son = new NTree(LL1Parser::cur_token.getValue());
        root->append(son);
        LL1Parser::match(Type::KEYWORDS, "do");
        S *s1 = new S();
        s1->next = s->begin;
        s1->begin = c->t_label;
        son = LL1Parser::parseS(s1);
        root->append(son);
        s->code = s->begin + ": " + c->code + s1->code + "goto " + s1->next + "\n";
    }
    else Error::printErrors(ErrorType::SyntaxError, "parseS: Ungrammatical", true,LL1Parser::line_cnt);
    return root;
#endif
}

ReturnType LL1Parser::parseA(A *a, S *s, C *c, S *s1)
{
#ifndef USE_VISUALIZATION
    if (LL1Parser::cur_token.getValue() == "else")
    {
        LL1Parser::match(Type::KEYWORDS, "else");
        parseS();
    }
    else return;
#else
    const static std::string toReplace = "L_";
    NTree *root = new NTree("A"), *son;
    std::size_t pos = c->code.find(toReplace);
    if (LL1Parser::cur_token.getValue() == "else")
    {
        c->f_label = NonTerminal::newlabel();
        son = new NTree("else");
        root->append(son);
        LL1Parser::match(Type::KEYWORDS, "else");
        S *s2 = new S();
        s2->next = s->next;
        son = parseS(s2);
        root->append(son);
        if (pos != std::string::npos) { // 如果找到了
            c->code.replace(pos, toReplace.length(), c->f_label); // 进行替换
        }
        a->code = s->begin + ": " + c->code + s1->code + "goto " + s1->next + "\n";
        a->code = a->code + c->f_label + ": " + s2->code;
    }
    else
    {
        son = new NTree("ε");
        root->append(son);
        if (pos != std::string::npos) { // 如果找到了
            c->code.replace(pos, toReplace.length(), s->next); // 进行替换
        }
        a->code = s->begin + ": " + c->code + s1->code;
    }
    return root;
#endif
}

ReturnType LL1Parser::parseC(C *c)
{
#ifndef USE_VISUALIZATION
    LL1Parser::parseE();
    LL1Parser::parseB();
#else
    NTree* root = new NTree("C"), *son;
    E *e = new E();
    son = LL1Parser::parseE(e);
    root->append(son);
    B *b = new B();
    b->code = e->code, b->place = e->place;
    b->t_label = c->t_label, b->f_label = c->f_label;
    son = LL1Parser::parseB(b);
    c->code = b->code;
    root->append(son);
    return root;
#endif
}

ReturnType LL1Parser::parseB(B *b)
{
    if (LL1Parser::cur_token.getValue() == "=" || LL1Parser::cur_token.getValue() == "<" || LL1Parser::cur_token.getValue() == ">")
    {
#ifndef USE_VISUALIZATION
        LL1Parser::match(Type::SIGN);
        parseE();
#else
        NTree* root = new NTree("B"), *son;
        std::string symbo = LL1Parser::cur_token.getValue();
        son = new NTree(LL1Parser::cur_token.getValue());
        LL1Parser::match(Type::SIGN);
        root->append(son);
        E *e = new E();
        son = parseE(e);
        b->code = b->code + e->code + "if " + b->place + " " + symbo + " " + e->place + " goto " + b->t_label + "\n";
        b->code = b->code + "goto " + b->f_label + "\n";
        root->append(son);
        return root;
#endif
    }
    else Error::printErrors(ErrorType::SyntaxError, "parseB: Ungrammatical", true,LL1Parser::line_cnt);
}

ReturnType LL1Parser::parseE(E *e)
{
#ifndef USE_VISUALIZATION
    LL1Parser::parseT();
    LL1Parser::parseD();
#else
    NTree* root = new NTree("E"), *son;
    T *t = new T();
    son = LL1Parser::parseT(t);
    root->append(son);
    D *d = new D();
    d->icode = t->code, d->iplace = t->place;
    son = LL1Parser::parseD(d);
    e->code = d->code, e->place = d->place;
    root->append(son);
    return root;
#endif
}

ReturnType LL1Parser::parseD(D *d)
{
#ifndef USE_VISUALIZATION
    if (LL1Parser::cur_token.getValue() == "-" || LL1Parser::cur_token.getValue() == "+")
    {
        LL1Parser::match(Type::SIGN, LL1Parser::cur_token.getValue());
        LL1Parser::parseT();
        LL1Parser::parseD();
    }
    else return;
#else
    NTree* root = new NTree("D"), *son;
    if (LL1Parser::cur_token.getValue() == "-" || LL1Parser::cur_token.getValue() == "+")
    {
        d->place = NonTerminal::newtemp();
        std::string symbo = LL1Parser::cur_token.getValue();
        son = new NTree(LL1Parser::cur_token.getValue());
        root->append(son);
        LL1Parser::match(Type::SIGN, LL1Parser::cur_token.getValue());
        T *t1 = new T();
        son = LL1Parser::parseT(t1);
        root->append(son);
        D *d1 = new D();
        d1->icode = d->icode + t1->code + d->place + " := " + d->iplace + " " + symbo + " " + t1->place + "\n";
        d1->iplace = d->place;
        son = LL1Parser::parseD(d1);
        root->append(son);
        d->code = d1->code, d->place = d1->place;
    }
    else
    {
        son = new NTree("ε");
        root->append(son);
        d->code = d->icode, d->place = d->iplace;
    }
    return root;
#endif
}

ReturnType LL1Parser::parseT(T *t)
{
#ifndef USE_VISUALIZATION
    LL1Parser::parseF();
    LL1Parser::parseG();
#else
    NTree* root = new NTree("T"), *son;
    F *f = new F();
    son = LL1Parser::parseF(f);
    root->append(son);
    G *g = new G();
    g->icode = f->code, g->iplace = f->place;
    son = LL1Parser::parseG(g);
    t->code = g->code, t->place = g->place;
    root->append(son);
    return root;
#endif
}

ReturnType LL1Parser::parseG(G *g)
{
#ifndef USE_VISUALIZATION
    if (LL1Parser::cur_token.getValue() == "*" || LL1Parser::cur_token.getValue() == "/")
    {
        LL1Parser::match(Type::SIGN, LL1Parser::cur_token.getValue());
        LL1Parser::parseF();
        LL1Parser::parseG();
    }
    else return;
#else
    NTree* root = new NTree("G"), *son;
    if (LL1Parser::cur_token.getValue() == "*" || LL1Parser::cur_token.getValue() == "/")
    {
        g->place = NonTerminal::newtemp();
        std::string symbo = LL1Parser::cur_token.getValue();
        son = new NTree(LL1Parser::cur_token.getValue());
        root->append(son);
        LL1Parser::match(Type::SIGN, LL1Parser::cur_token.getValue());
        F *f1 = new F();
        son = LL1Parser::parseF(f1);
        root->append(son);
        G *g1 = new G();
        g1->icode = g->icode + f1->code + g->place + " := " + g->iplace + " " + symbo + " " + f1->place + "\n";
        g1->iplace = g->place;
        son = LL1Parser::parseG(g1);
        root->append(son);
        g->code = g1->code, g->place = g1->place;
    }
    else
    {
        son = new NTree("ε");
        root->append(son);
        g->code = g->icode, g->place = g->iplace;
    }
    return root;
#endif
}

ReturnType LL1Parser::parseF(F* f)
{
#ifndef USE_VISUALIZATION
    if (LL1Parser::cur_token.getValue() == "(")
    {
        LL1Parser::match(Type::SIGN, "(");
        LL1Parser::parseE();
        LL1Parser::match(Type::SIGN, ")");
    }
    else if (LL1Parser::cur_token.getType() == Type::IDN)
    {
        LL1Parser::match(Type::IDN);
    }
    else if (LL1Parser::cur_token.getType() == Type::OCTAL)
    {
        LL1Parser::match(Type::OCTAL);
    }
    else if (LL1Parser::cur_token.getType() == Type::DECIMAL)
    {
        LL1Parser::match(Type::DECIMAL);
    }
    else if (LL1Parser::cur_token.getType() == Type::HEXADECIMAL)
    {
        LL1Parser::match(Type::HEXADECIMAL);
    }
    else Error::printErrors(ErrorType::SyntaxError, "parseF: Ungrammatical", true,LL1Parser::line_cnt);
#else
    NTree* root = new NTree("F"), *son;
    if (LL1Parser::cur_token.getValue() == "(")
    {
        son = new NTree("(");
        root->append(son);
        LL1Parser::match(Type::SIGN, "(");
        E *e = new E();
        son = LL1Parser::parseE(e);
        f->place = e->place, f->code = e->code;
        root->append(son);
        son = new NTree(")");
        root->append(son);
        LL1Parser::match(Type::SIGN, ")");
    }
    else if (LL1Parser::cur_token.getType() == Type::IDN)
    {
        f->place = cur_token.getValue(), f->code = "";
        son = new NTree(LL1Parser::cur_token.getValue());
        root->append(son);
        LL1Parser::match(Type::IDN);
    }
    else if (LL1Parser::cur_token.getType() == Type::OCTAL)
    {
        std::string temp = cur_token.getValue();
        temp.erase(1, 1);
        f->place = std::to_string(std::stoi(temp, nullptr, 8)), f->code = "";
        son = new NTree(LL1Parser::cur_token.getValue());
        root->append(son);
        LL1Parser::match(Type::OCTAL);
    }
    else if (LL1Parser::cur_token.getType() == Type::DECIMAL)
    {
        f->place = std::to_string(std::stoi(cur_token.getValue(), nullptr, 10)), f->code = "";
        son = new NTree(LL1Parser::cur_token.getValue());
        root->append(son);
        LL1Parser::match(Type::DECIMAL);
    }
    else if (LL1Parser::cur_token.getType() == Type::HEXADECIMAL)
    {
        f->place = std::to_string(std::stoi(cur_token.getValue(), nullptr, 16)), f->code = "";
        son = new NTree(LL1Parser::cur_token.getValue());
        root->append(son);
        LL1Parser::match(Type::HEXADECIMAL);
    }
    else Error::printErrors(ErrorType::SyntaxError, "parseF: Ungrammatical", true,LL1Parser::line_cnt);
    return root;
#endif

}