#pragma once
#include<string>
#include<vector>
using namespace std;

enum class TokenType{

    NUMBER,
    BOOL_TRUE,
    BOOL_FALSE,
    IDENTIFIER,

    PLUS,
    MINUS,
    STAR,
    SLASH,
    PERCENT,
    EQUAL_EQUAL,
    LESS,
    GREATER,
    LESS_EQUAL,
    GREATER_EQUAL,
    EQUALS,
    COLON,

    LET,
    PRINT,
    INPUT,
    IF,
    ELSE,
    WHILE,

    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,

    NEWLINE,
    END,
    END_OF_FILE

};

struct Token{
    TokenType type;
    string lexeme;
    int line;
};

class Lexer{
    public:
        Lexer(const string& source);
        vector<Token> tokenize();
    private:
        string src;
        int pos=0;
        int line=1;
        
        char peek();
        char advance();
        void skipWhiteSpace();
        Token scanNumber();
        Token scanIdentifierOrKeyword();
};