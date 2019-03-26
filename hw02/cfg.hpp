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
    using std::set;
    using std::cout;
    using std::endl;
    using std::pair;

    using BasicBlock = vector<Instruction>;
    using SplitPoint = pair<int, bool>;
    using Edge = pair<int, int>;

    class ControlFlowGraph {
    private:
        /*
        enum class SplitPointType { DestinationAddress,  ControlFlowInstruction };

        struct SplitPoint {
            int address;
            int destinationAddress;
            SplitPointType type;
        };
        */

        bool isControlFlowInstruction(const Instruction &instruction) {
            const std::set<string> mnemonics = { "jmp", "je", "jb", "jne" };
            return mnemonics.find(instruction.mnemonic) != mnemonics.end();
        }

    public:
        /*std::set<SplitPoint> findSplitPoints(const vector<Instruction> &instructions) {
            std::unordered_map<SplitPoint> splitPoints;

            for (int i = 0; i < instructions.size(); i++) {
                const Instruction& instruction = instructions[i];
                if (isControlFlowInstruction(instruction)) {
                    splitPoints.insert({ SplitPoint{ i, -1, SplitPointType::ControlFlowInstruction } });
                    int destinationAddress = i + instruction.operandA.value;
                    splitPoints.insert({ SplitPoint{ i, destinationAddress, SplitPointType::DestinationAddress } });
                }
            }

            return splitPoints;
        }

        vector<SplitPoint> orderSplitPoints(const set<SplitPoint>& splitPoints) {
            vector<SplitPoint> orderedSplitPoints = vector<SplitPoint>(splitPoints.begin(), splitPoints.end());
            std::sort(orderedSplitPoints.begin(), orderedSplitPoints.end(), [](const SplitPoint& left, const SplitPoint& right) {
                return left.address > right.address;
            });
            return orderedSplitPoints;
        }

        vector<BasicBlock> generateBasicBlocks(const vector<Instruction>& instructions, const vector<SplitPoint>& orderedSplitPoints) {

        }*/

        string generateDotSource(const vector<Instruction>& instructions) {
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

            vector<pair<int, bool>> orderedSplitPoints = vector<pair<int, bool>>(splitPoints.begin(), splitPoints.end());
            std::sort(orderedSplitPoints.begin(), orderedSplitPoints.end());

            for (auto splitPoint : orderedSplitPoints) {
                cout << splitPoint.first << " " << splitPoint.second << endl;
            }




            return {};
        }

    };

}