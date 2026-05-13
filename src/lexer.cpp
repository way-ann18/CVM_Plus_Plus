#include "lexer.h"
#include<unordered_map>
#include<stdexcept>

Lexer::Lexer(const std::string& source){
    src=source;
}

char Lexer::peek(){
    if(pos>=src.size()){
        return '\0';
    }
    return src[pos];
}

char Lexer::advance(){
    return src[pos++];
}

void Lexer::skipWhiteSpace(){
    while(pos<src.size() && (src[pos]==' ' || src[pos]=='\t' || src[pos]=='\r')){
        pos++;
    }
}

Token Lexer::scanNumber(){
    std::string num;
    while(pos<src.size() && isdigit(src[pos])){
        num+=advance();
    }
    return{TokenType::NUMBER, num, line};
}

Token Lexer::scanIdentifierOrKeyword(){
    std::string word;
    while(pos<src.size() && (isalnum(src[pos]) || src[pos]=='_')){
        word+=advance();
    }
    static std::unordered_map<std::string, TokenType> keywords={
        {"let", TokenType::LET},
        {"output", TokenType::PRINT},
        {"input", TokenType::INPUT},
        {"if", TokenType::IF},
        {"else", TokenType::ELSE},
        {"while", TokenType::WHILE},
        {"right", TokenType::BOOL_TRUE},
        {"wrong", TokenType::BOOL_FALSE}
    };
    auto it=keywords.find(word);
    if(it!=keywords.end()){
        return {it->second, word, line};
    }
    return {TokenType::IDENTIFIER, word, line};
}

std::vector<Token> Lexer::tokenize(){
    std::vector<Token> tokens;
    while(pos<src.size()){
        skipWhiteSpace();
        if(pos>=src.size()){
            break;
        }
        char c=peek();
        if(isdigit(c)){
            tokens.push_back(scanNumber());
        }
        else if(isalpha(c) || c=='_'){
            tokens.push_back(scanIdentifierOrKeyword());
        }
        else if(c=='+'){
            advance();
            tokens.push_back({TokenType::PLUS, "+", line});
        }
        else if(c=='-'){
            advance();
            tokens.push_back({TokenType::MINUS, "-", line});
        }
        else if(c=='*'){
            advance();
            tokens.push_back({TokenType::STAR, "*", line});
        }
        else if(c=='/'){
            advance();
            tokens.push_back({TokenType::SLASH, "/", line});
        }
        else if(c=='<'){
            advance();
            tokens.push_back({TokenType::LESS, "<", line});
        }
        else if(c=='>'){
            advance();
            tokens.push_back({TokenType::GREATER, ">", line});
        }
        else if(c=='='){
            advance();
            if(peek()=='='){
                advance();
                tokens.push_back({TokenType::EQUAL_EQUAL, "==", line});
            }
            else{
                tokens.push_back({TokenType::EQUALS, "=", line});
            }
        }
        else if(c==':'){
            advance();
            tokens.push_back({TokenType::COLON, ":", line});
        }
        else if(c=='\n'){
            tokens.push_back({TokenType::NEWLINE, "\\n", line});
            advance();
            line++;
        }
        else if(c=='#'){
            while(pos<src.size() && src[pos]!='\n'){
                advance();
            }
        }
        else{
            advance();
        }
    }

    tokens.push_back({TokenType::END_OF_FILE, "", line});
    return tokens;
}