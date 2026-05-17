#include <iostream>
#include "lexer.h"
#include "parser.h"
#include "compiler.h"
#include "vm.h"
using namespace std;

void printAST(const vector<NodePtr>& nodes, int indent=0){
    string pad(indent*2, ' ');
    for(const auto& node: nodes){
        if(auto* n=dynamic_cast<VariableDeclarationStatement*>(node.get())){
            cout<<pad<<"VariableDeclaration: "<<n->name<<"\n";
            printAST({}, indent+1);
        }
        else if(auto* n=dynamic_cast<PrintStatement*>(node.get())){
            cout<<pad<<"PrintStatement\n";
        }
        else if(auto* n=dynamic_cast<IfStatement*>(node.get())){
            cout<<pad<<"IfStatement\n";
            cout<<pad<<" then: "<<n->thenBlock.size()<<" statements\n";
            cout<<pad<<" else: "<<n->elseBlock.size()<<" statements\n";
        }
        else if(auto* n=dynamic_cast<WhileStatement*>(node.get())){
            cout<<pad<<"WhileStatement\n";
            cout<<pad<<" body: "<<n->body.size()<<" statements\n";
        }
        else{
            cout<<pad<<"unknown node\n";
        }
    }
}

void printBytecode(const Chunk& chunk){
    cout<<"\n--- Bytecode ---\n";
    int i=0;
    while(i<(int)chunk.code.size()){
        OpCode op=(OpCode)chunk.code[i];
        cout<<i<<" : ";
        switch(op){
            case OpCode::OP_CONSTANT:
                cout<<"OP_CONSTANT "<<(int)chunk.code[i+1]<<" (value="<<chunk.constants[chunk.code[i+1]]<<")";
                i+=2;
                break;
            case OpCode::OP_TRUE:
                cout<<"OP_TRUE";
                i++;
                break;
            case OpCode::OP_FALSE:
                cout<<"OP_FALSE";
                i++;
                break;
            case OpCode::OP_ADD:
                cout<<"OP_ADD";
                i++;
                break;
            case OpCode::OP_SUBTRACT:
                cout<<"OP_SUBTRACT";
                i++;
                break;
            case OpCode::OP_MULTIPLY:
                cout<<"OP_MULTIPLY";
                i++;
                break;
            case OpCode::OP_DIVIDE:
                cout<<"OP_DIVIDE";
                i++;
                break;
            case OpCode::OP_EQUAL:
                cout<<"OP_EQUAL";
                i++;
                break;
            case OpCode::OP_LESS:
                cout<<"OP_LESS";
                i++;
                break;
            case OpCode::OP_GREATER:
                cout<<"OP_GREATER";
                i++;
                break;
            case OpCode::OP_STORE:
                cout<<"OP_STORE "<<(int)chunk.code[i+1]<<" (name="<<chunk.variableNames[chunk.code[i+1]]<<")";
                i+=2;
                break;
            case OpCode::OP_LOAD:
                cout<<"OP_LOAD "<<(int)chunk.code[i+1]<<" (name="<<chunk.variableNames[chunk.code[i+1]]<<")";
                i+=2;
                break;
            case OpCode::OP_JUMP:
                cout<<"OP_JUMP "<<(int)chunk.code[i+1];
                i+=2;
                break;
            case OpCode::OP_JUMP_IF_FALSE:
                cout<<"OP_JUMP_IF_FALSE "<<(int)chunk.code[i+1];
                i+=2;
                break;
            case OpCode::OP_OUTPUT:
                cout<<"OP_OUTPUT";
                i++;
                break;
            case OpCode::OP_INPUT:
                cout<<"OP_INPUT";
                i++;
                break;
            case OpCode::OP_RETURN:
                cout<<"OP_RETURN";
                i++;
                break;
            default:
                cout<<"Unknown";
                i++;
                break;
        }
        cout<<"\n";
    }

    cout<<"\n--- CONSTANTS ---\n";
    for(int j=0;j<(int)chunk.constants.size();j++){
        cout<<j<<" : "<<chunk.constants[j]<<"\n";
    }

    cout<<"\n--- VARIABLES ---\n";
    for(int j=0;j<(int)chunk.variableNames.size();j++){
        cout<<j<<" : "<<chunk.variableNames[j]<<"\n";
    }
}

int main(){
    string source="let x = 10\n"
    "let y = x + 5\n"
    "output y\n"
    "if x < 20 :\n"
    "output x\n"
    "let y = x + 1\n"
    "output y\n"
    "end\n"
    "while x < 15 :\n"
    "let x = x + 1\n"
    "output x\n"
    "end\n";
    Lexer lexer(source);
    auto tokens=lexer.tokenize();
    for(const Token& t:tokens){
        cout<<"Line "<<t.line<<" | lexeme: "<<t.lexeme<<" | type: "<<(int)t.type<<"\n";
    }

    Parser parser(tokens);
    auto ast=parser.parse();
    cout<<"AST has "<<ast.size()<<" top-level statements:\n\n";
    printAST(ast);

    Compiler compiler;
    Chunk chunk=compiler.compile(ast);
    printBytecode(chunk);

    cout<<"\n--- Program Output ---\n";
    VM vm;
    vm.run(chunk);

    return 0;
}