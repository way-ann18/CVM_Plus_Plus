#pragma once
#include "lexer.h"
#include<memory>
#include<vector>

struct ASTnode{
    virtual ~ASTnode()=default;
};

using NodePtr=std::unique_ptr<ASTnode>;

struct LiteralExpression:ASTnode{
    int value;
    LiteralExpression(int v):value(v){}
};

struct BoolExpression:ASTnode{
    bool value;
    BoolExpression(bool v):value(v){}
};

struct VariableExpression:ASTnode{
    std::string name;
    VariableExpression(const std::string& n):name(n){}
};

struct BinaryExpression:ASTnode{
    std::string op;
    NodePtr left;
    NodePtr right;
    BinaryExpression(const std::string& op, NodePtr l, NodePtr r):op(op), left(std::move(l)), right(std::move(r)){}
};

struct VariableDeclarationStatement:ASTnode{
    std::string name;
    NodePtr initializer;
    VariableDeclarationStatement(const std::string& n, NodePtr init):name(n), initializer(std::move(init)){}
};

struct PrintStatement:ASTnode{
    NodePtr expression;
    PrintStatement(NodePtr e):expression(std::move(e)){}
};

struct InputStatement:ASTnode{
    std::string variableName;
    InputStatement(const std::string& n):variableName(n){}
};

struct IfStatement:ASTnode{
    NodePtr condition;
    std::vector<NodePtr> thenBlock;
    std::vector<NodePtr> elseBlock;
    IfStatement(NodePtr cond, std::vector<NodePtr> tb, std::vector<NodePtr> eb):condition(std::move(cond)), thenBlock(std::move(tb)), elseBlock(std::move(eb)){}
};

struct WhileStatement:ASTnode{
    NodePtr condition;
    std::vector<NodePtr> body;
    WhileStatement(NodePtr cond, std::vector<NodePtr> b):condition(std::move(cond)), body(std::move(b)){}
};

class Parser{
    public:
        Parser(const std::vector<Token>& tokens);
        std::vector<NodePtr> parse();
    private:
        std::vector<Token> tokens;
        int pos=0;

        Token peek();
        Token advance();
        Token expect(TokenType type, const std::string& errorMessage);
        bool check(TokenType type);
        bool match(TokenType type);
        void skipNewLines();

        std::vector<NodePtr> parseBlock();
        NodePtr parseStatement();
        NodePtr parseVariableDeclaration();
        NodePtr parsePrint();
        NodePtr parseInput();
        NodePtr parseIf();
        NodePtr parseWhile();

        NodePtr parseExpression();
        NodePtr parseComparison();
        NodePtr parseAddSubtract();
        NodePtr parseMultiplyDivide();
        NodePtr parsePrimary();

};