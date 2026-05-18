#include "vm.h"
#include <iostream>
#include<stdexcept>
#ifdef __EMSCRIPTEN__
#include<emscripten.h>

EM_ASYNC_JS(int, inputFromUser, (), {
        return new Promise(function(resolve){
            var inputRow=document.getElementById("input-row");
            var inlineInput=document.getElementById("inline-input");
            var display=document.getElementById("output-display");

            display.textContent+="input > ";
            inputRow.classList.add("visible");
            inlineInput.value="";
            inlineInput.focus();

            function onSubmit(){
                var val=parseInt(inlineInput.value.trim()) || 0;
                display.textContent+=val+"\n";
                inputRow.classList.remove("visible");
                inlineInput.removeEventListener("keydown", onKey);
                document.getElementById("submit-input").removeEventListener("click", onSubmit);
                resolve(val);
            }

            function onKey(e){
                if(e.key=="Enter"){
                    onSubmit();
                }
            }

            inlineInput.addEventListener("keydown", onKey);
            document.getElementById("submit-input").addEventListener("click", onSubmit);
            
        });
    });

#endif

void VM::runInput(const Chunk& chunk){
#ifdef __EMSCRIPTEN__
    

    int value=inputFromUser();
    push(value);

#else
    int value;
    std::cout<<"Input: ";
    std::cin>>value;
    push(value);
#endif
}


void VM::push(int value){
    stack.push_back(value);
}

int VM::pop(){
    if(stack.empty()){
        throw std::runtime_error("VM: Stack underflow- too many operations on empty stack");
    }
    int value=stack.back();
    stack.pop_back();
    return value;
}

int VM::peek(){
    if(stack.empty()){
        throw std::runtime_error("VM: stack is empty- nothing to peek at");
    }
    return stack.back();
}

void VM::run(const Chunk& chunk){
    ip=0;
    stack.clear();
    variables.clear();
    while(ip<(int)chunk.code.size()){
        OpCode op=(OpCode)chunk.code[ip++];
    
        switch (op){
            case OpCode::OP_CONSTANT:
                runConstant(chunk);
                break;
            case OpCode::OP_TRUE:
                push(1);
                break;
            case OpCode::OP_FALSE:
                push(0);
                break;
            case OpCode::OP_ADD:
                runAdd();
                break;
            case OpCode::OP_SUBTRACT:
                runSubtract();
                break;
            case OpCode::OP_MULTIPLY:
                runMultiply();
                break;
            case OpCode::OP_DIVIDE:
                runDivide();
                break;
            case OpCode::OP_MODULO:
                runModulo();
                break;
            case OpCode::OP_EQUAL:
                runEqual();
                break;
            case OpCode::OP_LESS:
                runLess();
                break;
            case OpCode::OP_GREATER:
                runGreater();
                break;
            case OpCode::OP_LESS_EQUAL:
                runLessEqual();
                break;
            case OpCode::OP_GREATER_EQUAL:
                runGreaterEqual();
                break;
            case OpCode::OP_STORE:
                runStore(chunk);
                break;
            case OpCode::OP_LOAD:
                runLoad(chunk);
                break;
            case OpCode::OP_OUTPUT:
                runOutput();
                break;
            case OpCode::OP_INPUT:
                runInput(chunk);
                break;
            case OpCode::OP_JUMP:
                runJump(chunk);
                break;
            case OpCode::OP_JUMP_IF_FALSE:
                runJumpIfFalse(chunk);
                break;
            case OpCode::OP_RETURN:
                return;
            default:
                throw std::runtime_error("VM: Unknown opcode");
        }
    }
}

void VM::runConstant(const Chunk& chunk){
    uint8_t index=chunk.code[ip++];
    push(chunk.constants[index]);
}

void VM::runStore(const Chunk& chunk){
    uint8_t index=chunk.code[ip++];
    std::string name=chunk.variableNames[index];
    variables[name]=pop();
}

void VM::runLoad(const Chunk& chunk){
    uint8_t index=chunk.code[ip++];
    std::string name=chunk.variableNames[index];
    if(variables.find(name)==variables.end()){
        throw std::runtime_error("VM: Undefined variable '"+name+"'");
    }
    push(variables[name]);
}

void VM::runAdd(){
    int b=pop();
    int a=pop();
    push(a+b);
}

void VM::runSubtract(){
    int b=pop();
    int a=pop();
    push(a-b);
}

void VM::runMultiply(){
    int b=pop();
    int a=pop();
    push(a*b);
}

void VM::runDivide(){
    int b=pop();
    int a=pop();
    if(b==0){
        throw std::runtime_error("VM: Division by 0");
    }
    push(a/b);
}

void VM::runModulo(){
    int b=pop();
    int a=pop();
    if(b==0){
        throw std::runtime_error("VM: Modulo by 0");
    }
    push(a%b);
}

void VM::runEqual(){
    int b=pop();
    int a=pop();
    push(a==b? 1:0);
}

void VM::runLess(){
    int b=pop();
    int a=pop();
    push(a<b? 1:0);
}

void VM::runGreater(){
    int b=pop();
    int a=pop();
    push(a>b? 1:0);
}

void VM::runLessEqual(){
    int b=pop();
    int a=pop();
    push(a<=b? 1:0);
}

void VM::runGreaterEqual(){
    int b=pop();
    int a=pop();
    push(a>=b? 1:0);
}

void VM::runOutput(){
    int value=pop();
    std::cout<<value<<"\n";
}

void VM::runJump(const Chunk& chunk){
    uint8_t target=chunk.code[ip++];
    ip=target;
}

void VM::runJumpIfFalse(const Chunk& chunk){
    uint8_t target=chunk.code[ip++];
    int condition=pop();
    if(condition==0){
        ip=target;
    }
}