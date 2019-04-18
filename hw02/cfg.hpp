#include "../hw01/decode.hpp"

#include <unordered_map>
#include <set>
#include <set>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <utility>

namespace
{
    using std::vector;
    using InstructionDecoding::Instruction;
    using std::string;
    using std::unordered_map;
    using std::map;
    using std::set;
    using std::cout;
    using std::endl;
    using std::pair;
    using std::to_string;

    using BasicBlock = vector<Instruction>;
    using SplitPoint = pair<int, bool>;
    using Edge = pair<int, int>;

    class ControlFlowGraph {
    private:
        /*
        enum class SplitPointType { DestinationAddress,  ControlFlowInstruction };

        struct SplitPoint {
            int address;
            SplitPointType type;
        };
        */

        bool isControlFlowInstruction(const Instruction &instruction) const {
            const std::set<string> mnemonics = { "jmp", "je", "jb", "jne" };
            return mnemonics.find(instruction.mnemonic) != mnemonics.end();
        }

        unordered_map<int, bool> findSplitPoints(const vector<Instruction> &instructions) const {
            unordered_map<int, bool> splitPoints;

            for (int i = 0; i < instructions.size(); i++) {
                const Instruction& instruction = instructions[i];
                if (isControlFlowInstruction(instruction)) {
                    splitPoints[i] = true;
                    int destinationAddress = i + instruction.operandA.value;
                    if (splitPoints.find(destinationAddress) == splitPoints.end()) {
                        splitPoints[destinationAddress] = false;
                    }
                }
            }

            // add start and end point for nicer code later
            if (splitPoints.find(0) == splitPoints.end()) {
                splitPoints[0] = false;
            }
            if (splitPoints.find(instructions.size() - 1) == splitPoints.end()) {
                splitPoints[instructions.size()] = false;
            }
            return splitPoints;
        }

        map<int, BasicBlock> createBasicBlocks(const vector<Instruction>& instructions, const vector<SplitPoint>& orderedSplitPoints) const {
            map<int, BasicBlock> basicBlocks;
            for (int i = 0; i < orderedSplitPoints.size() - 1; i++) {
                const auto& splitPoint = orderedSplitPoints[i];
                const auto& nextSplitPoint = orderedSplitPoints[i + 1];
                int blockBeginIndex = splitPoint.second ? splitPoint.first + 1 : splitPoint.first;
                int blockEndIndex = nextSplitPoint.first;

                if (splitPoint.second && basicBlocks.find(splitPoint.first) == basicBlocks.end()) {
                    basicBlocks[splitPoint.first] = {};
                }

                if (nextSplitPoint.second && basicBlocks.find(nextSplitPoint.first) == basicBlocks.end()) {
                    basicBlocks[nextSplitPoint.first] = {};
                }

                if (!splitPoint.second || !nextSplitPoint.second) {
                    BasicBlock basicBlock = vector<Instruction>(
                            instructions.begin() + blockBeginIndex,
                            instructions.begin() + blockEndIndex);
                    basicBlocks[blockBeginIndex] = basicBlock;
                }
            }
            return basicBlocks;
        }

        string createEdge(int from, const Instruction& instruction) const {
            int destinationAddress = from + instruction.operandA.value;
            return createEdge(from, destinationAddress, instruction.toStr() + " # label_" + to_string(destinationAddress));
        }

        string createEdge(int from, int to, const string& edgeLabel) const {
            return to_string(from) + " -> " + to_string(to) + " [ label=\"" + edgeLabel + "\"]\n";
        }

    public:
        string generateDotSource(const vector<Instruction>& instructions) const {
            const auto splitPoints = findSplitPoints(instructions);
            auto orderedSplitPoints = vector<SplitPoint>(splitPoints.begin(), splitPoints.end());
            std::sort(orderedSplitPoints.begin(), orderedSplitPoints.end());
            const auto basicBlocks = createBasicBlocks(instructions, orderedSplitPoints);

            // add boxes
            string dotSource = "digraph G {\n";
            for (const auto& basicBlock : basicBlocks) {
                string basicBlockRectangle = to_string(basicBlock.first) +
                        " [ shape = " + (basicBlock.second.empty() ? "diamond" : "rectangle") + " label=\"";
                for (auto instruction : basicBlock.second) {
                    basicBlockRectangle += instruction.mnemonic + "\\l";
                }

                basicBlockRectangle += "\"]\n";
                dotSource += basicBlockRectangle;
            }

            // add jump edges
            for (const auto& splitPoint : orderedSplitPoints) {
                if (!splitPoint.second) {
                    continue;
                }

                const auto& instruction = instructions[splitPoint.first];
                dotSource += createEdge(splitPoint.first, instruction);
            }

            // add fallthrough edges
            vector<int> orderedBasicBlocksIndices;
            orderedBasicBlocksIndices.reserve(basicBlocks.size());
            for(auto const& basicBlock: basicBlocks)
                orderedBasicBlocksIndices.push_back(basicBlock.first);
            std::sort(orderedBasicBlocksIndices.begin(), orderedBasicBlocksIndices.end());

            for (int i = 0; i < orderedBasicBlocksIndices.size() - 1; i++) {
                const Instruction& instruction = instructions[orderedBasicBlocksIndices[i]];
                if (instruction.mnemonic != "jmp") {
                    dotSource += createEdge(orderedBasicBlocksIndices[i], orderedBasicBlocksIndices[i + 1], "");
                }
            }

            dotSource += "}";
            return dotSource;
        }

    };

}