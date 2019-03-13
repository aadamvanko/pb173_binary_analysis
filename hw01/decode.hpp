#ifndef DECODE_DECODE_HPP
#define DECODE_DECODE_HPP

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <cassert>
#include <unordered_map>

namespace InstructionDecoding {

    using std::string;
    using std::cout;
    using std::endl;
    using std::vector;
    using std::hex;
    using std::unordered_map;

    struct REX {
        bool W;
        bool R;
        bool X;
        bool B;
    };

    struct ModRM {
        uint8_t mod;
        uint8_t reg;
        uint8_t rm;
    };

    class Decoder {
    private:
        vector<string> split(const string& str, char delim)  {
            vector<string> tokens;
            std::stringstream mySstream(str);
            string token;

            while(getline(mySstream, token, delim)) {
                tokens.push_back(token);
            }

            return tokens;
        }

        REX parseRex(uint8_t byte) {
            return { .W = static_cast<bool>(byte & 0x8),
                     .R = static_cast<bool>(byte & 0x4),
                     .X = static_cast<bool>(byte & 0x2),
                     .B = static_cast<bool>(byte & 0x1) };
        }

        int64_t parseImmediate(uint8_t *startByte, int bytesCount) {
            uint64_t value = 0;
            for (int i = 0; i < bytesCount; i++, startByte++) {
                value <<= 8;
                value |= static_cast<uint64_t>(*startByte);
            }
            return static_cast<int64_t>(value);
        }

    public:
        int8_t parseByteImmediate(uint8_t *immediateStart) {
            return static_cast<int8_t>(parseImmediate(immediateStart, 1));
        }

        int16_t parse2ByteImmediate(uint8_t *immediateStart) {
            return static_cast<int16_t>(parseImmediate(immediateStart, 2));
        }

        int32_t parse4ByteImmediate(uint8_t *immediateStart) {
            return static_cast<int32_t>(parseImmediate(immediateStart, 4));
        }

        int64_t parse8ByteImmediate(uint8_t *immediateStart) {
            return static_cast<int64_t>(parseImmediate(immediateStart, 8));
        }

        ModRM parseModRM(uint8_t byte) {
            return { byte & 0xc0, byte & 0x38, byte & 0x07 };
        }

        string parseTwo64bitRegistersFromModRM(uint8_t byte) {
            unordered_map<uint8_t, string> registers {
                    {0b000, "%rax"},
                    {0b011, "%rbx"},
                    {0b001, "%rcx"},
                    {0b010, "%rdx"}
            };

            ModRM modRM = parseModRM(byte);
            string leftOperand = registers[modRM.reg];
            string rightOperand = registers[modRM.rm];
            return leftOperand + ", " + rightOperand;
        }

        string decodeBytes(vector <uint8_t> bytes) {
            if (bytes.empty()) {
                return "unknown instruction";
            }

            REX rex{};
            uint8_t* opcode = &bytes[0];

            if (bytes[0] >= 0x40 && bytes[0] <= 0x4f) {
                rex = parseRex(bytes[0]);
                opcode++;
            }

            std::ostringstream decoded;
            switch (*opcode) {
                // add
                case 0x05:
                // xor
                case 0x35:{
                    decoded << (*opcode == 0x05 ? "add" : "xor");
                    int32_t value = parse4ByteImmediate(opcode + 1);
                    if (rex.W) {
                        decoded << " $0x" << hex << static_cast<int64_t>(value);
                        decoded << ", %rax";
                    } else {

                        decoded << " $0x" << hex << value;
                        decoded << ", %eax";
                    }
                    break;
                }

                // imul
                case 0x0F: {
                    if (*(opcode + 1) != 0xAF || !rex.W) {
                        decoded << "unknown instruction";
                        break;
                    }
                    decoded << "mul " << parseTwo64bitRegistersFromModRM(*(opcode + 2));
                    break;
                }


                default:
                    decoded << "unknown instruction";
                    break;
            }

            return decoded.str();
        }

        string decode(const string& concatenatedBytes) {
            if (concatenatedBytes.empty()) {
                return "unknown instruction";
            }

            vector<uint8_t> bytes;
            for (const auto& byteStr : split(concatenatedBytes, ' ')) {
                assert(!byteStr.empty());
                uint8_t byte = static_cast<uint8_t>(std::stoul(byteStr, 0, 16));
                bytes.push_back(byte);
            }

            return decodeBytes(bytes);
        }
    };

}

#endif //DECODE_DECODE_H
