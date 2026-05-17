#include "compiler.h"
#include<stdexcept>

Chunk Compiler::compile(const std::vector<NodePtr>& statements){
    for(const auto& statement:statements){
        compileStatement(statement.get());
    }
    chunk.emit(OpCode::OP_RETURN);
    return chunk;
}

void Compiler::compileStatement(ASTnode* node){
    if(auto* n=dynamic_cast<VariableDeclarationStatement*>(node)){
        compileVariableDeclaration(n);
        return;
    }
    if(auto* n=dynamic_cast<PrintStatement*>(node)){
        compilePrint(n);
        return;
    }
    if(auto* n=dynamic_cast<InputStatement*>(node)){
        compileInput(n);
        return;
    }
    if(auto* n=dynamic_cast<IfStatement*>(node)){
        compileIf(n);
        return;
    }
    if(auto* n=dynamic_cast<WhileStatement*>(node)){
        compileWhile(n);
        return;
    }

    throw std::runtime_error("Compiler: Unknown statement type");
}

void Compiler::compileExpression(ASTnode* node){
    if(auto* n=dynamic_cast<LiteralExpression*>(node)){
        int index=chunk.addConstant(n->value);
        chunk.emit(OpCode::OP_CONSTANT, (uint8_t)index);
        return;
    }
    if(auto* n=dynamic_cast<BoolExpression*>(node)){
        chunk.emit(n->value? OpCode::OP_TRUE:OpCode::OP_FALSE);
        return;
    }
    if(auto* n=dynamic_cast<VariableExpression*>(node)){
        int index=chunk.addVariableName(n->name);
        chunk.emit(OpCode::OP_LOAD, (uint8_t)index);
        return;
    }
    if(auto* n=dynamic_cast<BinaryExpression*>(node)){
        compileBinary(n);
        return;
    }

    throw std::runtime_error("Compiler: Unknown statement type");
}

void Compiler::compileBinary(BinaryExpression* node){
    compileExpression(node->left.get());
    compileExpression(node->right.get());
    if(node->op=="+"){
        chunk.emit(OpCode::OP_ADD);
    }
    else if(node->op=="-"){
        chunk.emit(OpCode::OP_SUBTRACT);
    }
    else if(node->op=="*"){
        chunk.emit(OpCode::OP_MULTIPLY);
    }
    else if(node->op=="/"){
        chunk.emit(OpCode::OP_DIVIDE);
    }
    else if(node->op=="=="){
        chunk.emit(OpCode::OP_EQUAL);
    }
    else if(node->op=="<"){
        chunk.emit(OpCode::OP_LESS);
    }
    else if(node->op==">"){
        chunk.emit(OpCode::OP_GREATER);
    }
    else{
        throw std::runtime_error("Compiler: Unknown operator "+node->op);
    }
}

void Compiler::compileVariableDeclaration(VariableDeclarationStatement* node){
    compileExpression(node->initializer.get());
    int index=chunk.addVariableName(node->name);
    chunk.emit(OpCode::OP_STORE, (uint8_t)index);
}

void Compiler::compilePrint(PrintStatement* node){
    compileExpression(node->expression.get());
    chunk.emit(OpCode::OP_OUTPUT);
}

void Compiler::compileInput(InputStatement* node){
    chunk.emit(OpCode::OP_INPUT);
    int index=chunk.addVariableName(node->variableName);
    chunk.emit(OpCode::OP_STORE, (uint8_t)index);
}

void Compiler::compileIf(IfStatement* node){
    compileExpression(node->condition.get());
    chunk.emit(OpCode::OP_JUMP_IF_FALSE, 0);
    int jumpIfFalsePosition=(int)chunk.code.size()-1;
    for(const auto& statement:node->thenBlock){
        compileStatement(statement.get());
    }
    chunk.emit(OpCode::OP_JUMP, 0);
    int jumpPosition=(int)chunk.code.size()-1;
    chunk.code[jumpIfFalsePosition]=(uint8_t)chunk.code.size();
    for(const auto& statement:node->elseBlock){
        compileStatement(statement.get());
    }
    chunk.code[jumpPosition]=(uint8_t)chunk.code.size();
}

void Compiler::compileWhile(WhileStatement* node){
    int loopStart=(int)chunk.code.size();
    compileExpression(node->condition.get());
    chunk.emit(OpCode::OP_JUMP_IF_FALSE, 0);
    int jumpOutPosition=(int)chunk.code.size()-1;
    for(const auto& statement:node->body){
        compileStatement(statement.get());
    }
    chunk.emit(OpCode::OP_JUMP, (uint8_t)loopStart);
    chunk.code[jumpOutPosition]=(uint8_t)chunk.code.size();
}