#ifndef DECODE_DECODE_HPP
#define DECODE_DECODE_HPP

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <cassert>
#include <unordered_map>
#include <map>

namespace InstructionDecoding {

    using std::string;
    using std::cout;
    using std::endl;
    using std::vector;
    using std::hex;
    using std::unordered_map;
    using std::map;

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
            return { static_cast<uint8_t>(byte & 0xc0),
                     static_cast<uint8_t>((byte & 0x38) >> 3),
                     static_cast<uint8_t>(byte & 0x07)};
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
                // cmp eax | rax, imm32
                case 0x3D:
                // add
                case 0x05:
                // xor
                case 0x35:{
                    unordered_map<uint8_t, string> opcodesInstructions {
                            { 0x3D, "cmp" },
                            { 0x05, "add" },
                            { 0x35, "xor" }
                    };
                    decoded << opcodesInstructions[*opcode];
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

                // jmp rel8
                case 0xeb: {
                    decoded << "jmp $0x" << hex << static_cast<int>(parseByteImmediate(opcode + 1)); // TODO
                    break;
                }

                // jmp rel32
                case 0xe9: {
                    decoded << "jmp $0x" << hex << static_cast<int>(parse4ByteImmediate(opcode + 1)); // TODO
                    break;
                }

                // imul
                // rel32 je, jne, jb
                case 0x0f: {
                    uint8_t secondPart = *(opcode + 1);
                    map<uint8_t, string> secondParts {
                            { 0xaf, "mul" },
                            { 0x84, "je" },
                            { 0x85, "jne" },
                            { 0x82, "jb" }
                    };

                    if (secondParts.find(secondPart) == secondParts.end()) {
                        decoded << "uknown instruction";
                        break;
                    }

                    if (secondPart == 0xaf) {
                        decoded << "mul " << parseTwo64bitRegistersFromModRM(*(opcode + 2));
                    }
                    else {
                        decoded << secondParts[secondPart] << " $0x" << hex << parse4ByteImmediate(opcode + 2);
                    }

                   break;
                }



                // cmp r64, r64
                case 0x3B: {
                    if (!rex.W) {
                        decoded << "unknown instruction";
                        break;
                    }
                    decoded << "cmp " << parseTwo64bitRegistersFromModRM(*(opcode + 1));
                    break;
                }

                // nop
                case 0x90: {
                    decoded << "nop";
                    break;
                }

                // int3
                case 0xcc: {
                    decoded << "int3";
                    break;
                }

                // rel8
                // je
                case 0x74:
                // jne
                case 0x75:
                // jb
                case 0x72: {
                    unordered_map<uint8_t, string> opcodesInstructions{
                            {0x74, "je"},
                            {0x75, "jne"},
                            {0x72, "jb"}
                    };
                    decoded << opcodesInstructions[*opcode];
                    decoded << " $0x" << hex << static_cast<int>(parseByteImmediate(opcode + 1)); // TODO
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
