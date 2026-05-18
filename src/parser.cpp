#include "parser.h"
#include<stdexcept>

Parser::Parser(const std::vector<Token>& tokens){
    this->tokens=tokens;
}

Token Parser::peek(){
    return tokens[pos];
}

Token Parser::advance(){
    return tokens[pos++];
}

bool Parser::check(TokenType type){
    return peek().type==type;
}

bool Parser::match(TokenType type){
    if(check(type)){
        advance();
        return true;
    }
    return false;
}

Token Parser::expect(TokenType type, const std::string& errorMessage){
    if(!check(type)){
        throw std::runtime_error("Line "+std::to_string(peek().line)+": "+errorMessage);
    }
    return advance();
}

void Parser::skipNewLines(){
    while(check(TokenType::NEWLINE)){
        advance();
    }
}

std::vector<NodePtr> Parser::parse(){
    std::vector<NodePtr> statements;
    skipNewLines();
    while(!check(TokenType::END_OF_FILE)){
        statements.push_back(parseStatement());
        skipNewLines();
    }
    return statements;
}

std::vector<NodePtr> Parser::parseBlock(){
    std::vector<NodePtr> statements;
    skipNewLines();
    while(!check(TokenType::END_OF_FILE) && !check(TokenType::ELSE) && !check(TokenType::END)){
        statements.push_back(parseStatement());
        skipNewLines();
    }
    return statements;
}

NodePtr Parser::parseStatement(){
    skipNewLines();
    TokenType t=peek().type;
    if(t==TokenType::LET){
        return parseVariableDeclaration();
    }
    if(t==TokenType::PRINT){
        return parsePrint();
    }
    if(t==TokenType::INPUT){
        return parseInput();
    }
    if(t==TokenType::IF){
        return parseIf();
    }
    if(t==TokenType::WHILE){
        return parseWhile();
    }

    throw std::runtime_error("Line "+std::to_string(peek().line)+": unexpected token '"+peek().lexeme+"'");
}

NodePtr Parser::parseVariableDeclaration(){
    advance();
    Token name=expect(TokenType::IDENTIFIER, "expected variable name after 'let'");
    expect(TokenType::EQUALS, "expected '=' after variable name");
    NodePtr init=parseExpression();
    return std::make_unique<VariableDeclarationStatement>(name.lexeme, std::move(init));
}

NodePtr Parser::parsePrint(){
    advance();
    NodePtr expression=parseExpression();
    if(!check(TokenType::NEWLINE) && !check(TokenType::END_OF_FILE)){
        throw std::runtime_error("Line "+std::to_string(peek().line)+": Unexpected token'"+peek().lexeme+"' after output statement");
    }
    match(TokenType::NEWLINE);
    return std::make_unique<PrintStatement>(std::move(expression));
}
NodePtr Parser::parseInput(){
    advance();
    Token name=expect(TokenType::IDENTIFIER, "expected variable name after 'input'");
    if(!check(TokenType::NEWLINE) && !check(TokenType::END_OF_FILE)){
        throw std::runtime_error("Line "+std::to_string(peek().line)+": Unexpected token '"+peek().lexeme+"' after input statement");
    }
    match(TokenType::NEWLINE);
    return std::make_unique<InputStatement>(name.lexeme);
}

NodePtr Parser::parseIf(){
    advance();
    NodePtr condition=parseExpression();
    expect(TokenType::COLON, "expected ':' after if condition");
    expect(TokenType::NEWLINE, "expected newline after ':'");
    std::vector<NodePtr> thenBlock=parseBlock();
    std::vector<std::pair<NodePtr, std::vector<NodePtr>>> elseIfBlocks;
    std::vector<NodePtr> elseBlock;
    skipNewLines();
    while(check(TokenType::ELSE)){
        advance();
        skipNewLines();
        if(check(TokenType::IF)){
            advance();
            NodePtr elseIfCondition=parseExpression();
            expect(TokenType::COLON, "expected ':' after else if condition");
            expect(TokenType::NEWLINE, "expected newline after':'");
            std::vector<NodePtr> elseIfBlock=parseBlock();
            elseIfBlocks.push_back({std::move(elseIfCondition), std::move(elseIfBlock)});
            skipNewLines();
        }
        else{
            match(TokenType::COLON);
            match(TokenType::NEWLINE);
            elseBlock=parseBlock();
            break;
        }
    }
    expect(TokenType::END, "expected 'end' to close if block");
    match(TokenType::NEWLINE);
    return std::make_unique<IfStatement>(std::move(condition), std::move(thenBlock), std::move(elseIfBlocks), std::move(elseBlock));
}

NodePtr Parser::parseWhile(){
    advance();
    NodePtr condition=parseExpression();
    expect(TokenType::COLON, "expected ':' after while condition");
    expect(TokenType::NEWLINE, "expected newline after ':'");
    std::vector<NodePtr> body=parseBlock();
    expect(TokenType::END, "expected 'end' to close while block");
    match(TokenType::NEWLINE);
    return std::make_unique<WhileStatement>(std::move(condition), std::move(body));
}

NodePtr Parser::parseExpression(){
    return parseComparison();
}

NodePtr Parser::parseComparison(){
    NodePtr left=parseAddSubtract();
    while(check(TokenType::EQUAL_EQUAL) || check(TokenType::LESS) || check(TokenType::GREATER) || check(TokenType::LESS_EQUAL) || check(TokenType::GREATER_EQUAL)){
        std::string op=advance().lexeme;
        NodePtr right=parseAddSubtract();
        left=std::make_unique<BinaryExpression>(op, std::move(left), std::move(right));
    }
    return left;
}

NodePtr Parser::parseAddSubtract(){
    NodePtr left=parseMultiplyDivide();
    while(check(TokenType::PLUS) || check(TokenType::MINUS)){
        std::string op=advance().lexeme;
        NodePtr right=parseMultiplyDivide();
        left=std::make_unique<BinaryExpression>(op, std::move(left), std::move(right));
    }
    return left;
}

NodePtr Parser::parseMultiplyDivide(){
    NodePtr left=parsePrimary();
    while(check(TokenType::STAR) || check(TokenType::SLASH) || check(TokenType::PERCENT)){
        std::string op=advance().lexeme;
        NodePtr right=parsePrimary();
        left=std::make_unique<BinaryExpression>(op, std::move(left), std::move(right));
    }
    return left;
}

NodePtr Parser::parsePrimary(){
    if(check(TokenType::MINUS)){
        advance();
        NodePtr operand=parsePrimary();
        return std::make_unique<BinaryExpression>("-", std::make_unique<LiteralExpression>(0), std::move(operand));
    }
    if(check(TokenType::NUMBER)){
        int value=std::stoi(advance().lexeme);
        return std::make_unique<LiteralExpression>(value);
    }
    if(check(TokenType::BOOL_TRUE)){
        advance();
        return std::make_unique<BoolExpression>(true);
    }
    if(check(TokenType::BOOL_FALSE)){
        advance();
        return std::make_unique<BoolExpression>(false);
    }
    if(check(TokenType::IDENTIFIER)){
        std::string name=advance().lexeme;
        return std::make_unique<VariableExpression>(name);
    }
    throw std::runtime_error("Line "+std::to_string(peek().line)+": unexpected token '"+peek().lexeme+"' in expression");
}