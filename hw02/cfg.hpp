#include "../hw01/decode.hpp"

#include <unordered_set>
#include <string>
#include <vector>

namespace
{
    using std::vector;
    using InstructionDecoding::Instruction;
    using std::string;

    class ControlFlowGraph {
    private:
        bool isControlFlowInstruction(const Instruction &instruction) {
            const std::unordered_set<string> mnemonics = { "call", "jmp", "je", "jb", "jne" };
            return mnemonics.find(instruction.mnemonic) != mnemonics.end();
        }

    public:
        string generateDotSource(const vector<Instruction>& instructions) {
            using BasicBlock = vector<Instruction>;
            std::unordered_map<int, BasicBlock> basicBlocks;
            int labelIdCounter = 0;

            int labelAddress = 0;
            BasicBlock basicBlock;
            for (int i = 0; i < instructions.size(); i++) {
                const Instruction& instruction = instructions[i];
                if (isControlFlowInstruction(instruction)) {
                    basicBlocks[labelAddress] = basicBlock;

                    int destAddress = i + instruction.operandA.value;
                    auto it = basicBlocks.find(destAddress);
                    if (it == basicBlocks.end()) {

                    }
                    else {

                    }
                } else {
                    basicBlock.push_back(instruction);
                }
            }

            return {};
        }

    };

}