#include <iostream>
#include "lexer.h"
#include "parser.h"
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

int main(){
    string source="let x = 10 + 20\n"
    "let y = x + 5\n"
    "output y\n"
    "if x < 20 :\n"
    "output x\n"
    "while x < 15 :\n"
    "let x = x + 1\n";
    Lexer lexer(source);
    auto tokens=lexer.tokenize();
    for(const Token& t:tokens){
        cout<<"Line "<<t.line<<" | lexeme: "<<t.lexeme<<" | type: "<<(int)t.type<<"\n";
    }
    Parser parser(tokens);
    auto ast=parser.parse();
    cout<<"AST has "<<ast.size()<<" top-level statements:\n\n";
    printAST(ast);
    return 0;
}