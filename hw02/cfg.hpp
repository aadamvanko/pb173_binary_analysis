#include "../hw01/decode.hpp"

#include <unordered_map>
#include <unordered_set>
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
    using InstructionDecoding::AddressableInstruction;
    using InstructionDecoding::AddressType;
    using std::string;
    using std::unordered_map;
    using std::unordered_set;
    using std::map;
    using std::set;
    using std::cout;
    using std::endl;
    using std::pair;
    using std::to_string;

    using BasicBlock = vector<Instruction>;
    using BasicBlockAddressable = vector<AddressableInstruction>;
    using SplitPoint = pair<AddressType, bool>;

    class ControlFlowGraph {
    private:
        const string LABEL_PREFIX = ".label_";

        bool isControlFlowInstruction(const Instruction &instruction) const {
            const std::set<string> mnemonics = { "jmp", "je", "jb", "jne" };
            return mnemonics.find(instruction.mnemonic) != mnemonics.end();
        }

        bool isControlFlowInstruction(const AddressableInstruction &instruction) const {
            const std::set<string> mnemonics = { "jmp", "je", "jb", "jne", "call" };
            return mnemonics.find(instruction.ins.mnemonic) != mnemonics.end();
        }

        unordered_map<AddressType, bool> findSplitPoints(const vector<Instruction> &instructions) const {
            unordered_map<AddressType, bool> splitPoints;

            for (int i = 0; i < instructions.size(); i++) {
                const Instruction& instruction = instructions[i];
                if (isControlFlowInstruction(instruction)) {
                    splitPoints[i] = true;
                    AddressType destinationAddress = i + instruction.operandA.value;
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
/*
        int calculateInstructionIndex(const vector<AddressableInstruction>& instructions, const AddressType from, int64_t offset) const {
            AddressType index = from;
            if (offset < 0) {
                offset += instructions[index].ins.length;
                while (index > 0 && offset < 0) {
                    index--;
                    if (labs(offset) < instructions[index].ins.length) {
                        // destination is inside instruction !
                    }
                    offset += instructions[index].ins.length;
                }
                if (offset < 0 && index == 0) {
                    // destination is outside of instruction addresses range
                }
            } else {
                while (index < instructions.size() - 1 && offset > 0) {
                    index++;
                    if (offset < instructions[index].ins.length) {
                        // destination is inside instruction !
                    }
                    offset -= instructions[index].ins.length;
                }
                if (offset > 0 && index == instructions.size() - 1) {
                    // destination is outside of instruction addresses range
                }
                index++;
            }
            return index;
        }
*/
        map<AddressType, BasicBlock> createBasicBlocks(const vector<Instruction>& instructions, const vector<SplitPoint>& orderedSplitPoints) const {
            map<AddressType, BasicBlock> basicBlocks;
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

        unordered_set<AddressType> findDestinationIndexes(const vector<AddressableInstruction>& instructions) const {
            unordered_set<AddressType> destinationIndexes;
            for (int i = 0; i < instructions.size(); i++) {
                const auto& instruction = instructions[i];
                if (isControlFlowInstruction(instruction)) {
                    auto destinationIndex = instruction.destination;
                    destinationIndexes.insert(destinationIndex);
                }
            }
            return destinationIndexes;
        }

        map<AddressType, BasicBlockAddressable> createBasicBlocks(const vector<AddressableInstruction>& instructions) const {
            const auto destinationIndexes = findDestinationIndexes(instructions);
            vector<BasicBlockAddressable> blocks;
            BasicBlockAddressable block;
            for (size_t i = 0; i < instructions.size(); i++) {
                const auto& instruction = instructions[i];
                if (destinationIndexes.find(instruction.address) != destinationIndexes.end() && isControlFlowInstruction(instruction)) {
                    blocks.push_back(block);
                    block.clear();
                    blocks.push_back({ instruction });
                }
                else if (destinationIndexes.find(instruction.address) != destinationIndexes.end()) {
                    blocks.push_back(block);
                    block.clear();
                    block.push_back(instruction);
                }
                else if (isControlFlowInstruction(instruction) || instruction.ins.mnemonic == "ret") {
                    block.push_back(instruction);
                    blocks.push_back(block);
                    block.clear();
                }
                else {
                    block.push_back(instruction);
                }
            }

            map<AddressType, BasicBlockAddressable> basicBlocks;
            for (const auto& block : blocks) {
                if (block.empty()) {
                    continue;
                }
                basicBlocks[block.front().address] = block;
            }
            return basicBlocks;
        }

        string createEdge(AddressType from, const Instruction& instruction) const {
            int destinationAddress = from + instruction.operandA.value;
            return createEdge(from, destinationAddress, instruction.toStr() + " # label_" + to_string(destinationAddress));
        }

        string toHex(AddressType address) const {
            std::ostringstream os;
            os << std::hex << address;
            return os.str();
        }

        string wrappedHex(AddressType address) const {
            return "\"" + toHex(address) + "\"";
        }

        string createEdge(AddressType from, AddressType to, const string& edgeLabel) const {

            return wrappedHex(from) + " -> " + wrappedHex(to) + " [ label=\"" + edgeLabel + "\"]\n";
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

        string generateDotSource(const vector<AddressableInstruction>& instructions) const {
            auto basicBlocks = createBasicBlocks(instructions);

            // add boxes
            string dotSource = "digraph G {\n";
            for (const auto& basicBlock : basicBlocks) {
                string basicBlockRectangle = wrappedHex(basicBlock.first) +
                        " [ shape = rectangle label=\"";
                basicBlockRectangle += LABEL_PREFIX + toHex(basicBlock.first) + ":\\l";
                for (const auto& instruction : basicBlock.second) {
                    string targetLabel = "";
                    if (isControlFlowInstruction(instruction)) {
                        targetLabel = " # " + LABEL_PREFIX + toHex(instruction.destination);
                    }
                    basicBlockRectangle += "    " + instruction.toStrWithoutAddress() + targetLabel + "\\l";
                }
                basicBlockRectangle += "\"]\n";
                dotSource += basicBlockRectangle;
            }

            vector<AddressType> orderedBasicBlocksAddresses;
            orderedBasicBlocksAddresses.reserve(basicBlocks.size());
            for(auto const& basicBlock: basicBlocks) {
                orderedBasicBlocksAddresses.push_back(basicBlock.first);
            }
            std::sort(orderedBasicBlocksAddresses.begin(), orderedBasicBlocksAddresses.end());

            // add edges
            for (size_t i = 0; i < orderedBasicBlocksAddresses.size(); i++) {
                const auto& basicBlock = basicBlocks[orderedBasicBlocksAddresses[i]];
                const auto basicBlockAddress = orderedBasicBlocksAddresses[i];

                const auto& lastInstruction = basicBlock.back();
                if (isControlFlowInstruction(lastInstruction)) {
                    const auto& mnemonic = lastInstruction.ins.mnemonic;
                    auto label = lastInstruction.toStrWithoutAddress() + " # " + LABEL_PREFIX + toHex(lastInstruction.destination);
                    dotSource += createEdge(basicBlockAddress, lastInstruction.destination, label);

                    if (mnemonic == "call" || mnemonic == "je" || mnemonic == "jne" || mnemonic == "jb") {
                        if (i < orderedBasicBlocksAddresses.size() - 1) {
                            dotSource += createEdge(basicBlockAddress, orderedBasicBlocksAddresses[i + 1], "");
                        }
                    }
                }
                else {
                    if (i < orderedBasicBlocksAddresses.size() - 1) {
                        dotSource += createEdge(basicBlockAddress, orderedBasicBlocksAddresses[i + 1], "");
                    }
                }
            }

            dotSource += "}";
            return dotSource;
        }


    };

}
