#pragma once
#include "parser.h"
#include<vector>
#include<string>
#include<unordered_map>
using namespace std;

enum class OpCode:uint8_t{
    OP_CONSTANT,
    OP_TRUE,
    OP_FALSE,

    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,

    OP_EQUAL,
    OP_LESS,
    OP_GREATER,

    OP_STORE,
    OP_LOAD,

    OP_JUMP,
    OP_JUMP_IF_FALSE,

    OP_OUTPUT,
    OP_INPUT,

    OP_RETURN
};

struct Chunk{
    vector<uint8_t> code;
    vector<int> constants;
    vector<string> variableNames;

    void emit(OpCode op){
        code.push_back((uint8_t)op);
    }
    void emit(OpCode op, uint8_t argument){
        code.push_back((uint8_t)op);
        code.push_back(argument);
    }

    int addConstant(int value){
        constants.push_back(value);
        return (int)constants.size()-1;
    }

    int addVariableName(const string& name){
        for(int i=0; i<(int)variableNames.size();i++){
            if(variableNames[i]==name){
                return i;
            }
        }
        variableNames.push_back(name);
        return (int)variableNames.size()-1;
    }
};

class Compiler{
    public:
        Chunk compile(const vector<NodePtr>& statements);
    private:
        Chunk chunk;
        void compileStatement(ASTnode* node);
        void compileExpression(ASTnode* node);
        void compileVariableDeclaration(VariableDeclarationStatement* node);
        void compilePrint(PrintStatement* node);
        void compileInput(InputStatement* node);
        void compileIf(IfStatement* node);
        void compileWhile(WhileStatement* node);
        void compileBinary(BinaryExpression* node);
};