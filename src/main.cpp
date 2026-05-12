#include <iostream>
#include "lexer.h"
using namespace std;

int main(){
    string source="let x = 10 + 20\noutput x";
    Lexer lexer(source);
    vector<Token> tokens=lexer.tokensize();
    for(const Token& t:tokens){
        cout<<"Line "<<t.line<<" | lexeme: "<<t.lexeme<<" | type: "<<(int)t.type<<"\n";
    }
    return 0;
}