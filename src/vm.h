#pragma once
#include "compiler.h"
#include<vector>
#include<unordered_map>
#include<string>

class VM{
    public:
        void run(const Chunk& chunk);
    private:
        std::vector<int> stack;
        std::unordered_map<std::string, int> variables;
        int ip=0;

        void push(int value);
        int pop();
        int peek();

        void runConstant(const Chunk& chunk);
        void runStore(const Chunk& chunk);
        void runLoad(const Chunk& chunk);
        void runAdd();
        void runSubtract();
        void runMultiply();
        void runDivide();
        void runEqual();
        void runLess();
        void runGreater();
        void runOutput();
        void runInput(const Chunk& chunk);
        void runJump(const Chunk& chunk);
        void runJumpIfFalse(const Chunk& chunk);
};