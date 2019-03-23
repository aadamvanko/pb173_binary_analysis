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

    struct HexCharStruct
    {
        uint8_t c;
        HexCharStruct(uint8_t _c) : c(_c) { }
    };

    inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
    {
        return (o << std::hex << (int)hs.c);
    }

    inline HexCharStruct toHex(uint8_t _c)
    {
        return HexCharStruct(_c);
    }

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

    struct SIB {
        uint8_t scale;
        uint8_t index;
        uint8_t base;
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


        ModRM parseModRM(uint8_t byte) {
            return { .mod = static_cast<uint8_t>((byte & 0xc0) >> 6),
                     .reg = static_cast<uint8_t>((byte & 0x38) >> 3),
                     .rm = static_cast<uint8_t>(byte & 0x07) };
        }

        SIB parseSIB(uint8_t byte) {
            return { .scale = static_cast<uint8_t>((byte & 0xc0) >> 6),
                     .index = static_cast<uint8_t>((byte & 0x38) >> 3),
                     .base = static_cast<uint8_t>((byte & 0x07)) };
        }

        int64_t parseImmediate(uint8_t *startByte, int bytesCount) {
            uint64_t value = 0;
            for (int i = 0; i < bytesCount; i++, startByte++) {
                value <<= 8;
                value |= static_cast<uint64_t>(*startByte);
            }
            return static_cast<int64_t>(value);
        }

        string intToHex(int32_t value) {
            std::ostringstream os;
            os << hex << value;
            return os.str();
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

        string getRegisterName(uint8_t value) {
            unordered_map<uint8_t, string> registers {
                    {0b000, "%rax"},
                    {0b011, "%rbx"},
                    {0b001, "%rcx"},
                    {0b010, "%rdx"},
                    {0b100, "%rsp"},
                    {0b101, "%rbp"},
                    {0b110, "%rsi"},
                    {0b111, "%rdi"}
            };
            return registers[value];
        }

        string parseTwo64bitRegistersFromModRM(uint8_t byte) {
            ModRM modRM = parseModRM(byte);
            string leftOperand = getRegisterName(modRM.reg);
            string rightOperand = getRegisterName(modRM.rm);
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
                    decoded << "jmp $0x" << toHex(parseByteImmediate(opcode + 1)); // TODO
                    break;
                }

                // jmp rel32
                case 0xe9: {
                    decoded << "jmp $0x" << hex << parse4ByteImmediate(opcode + 1); // TODO
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

                // ret
                case 0xc3: {
                    decoded << "ret";
                    break;
                }

                // ret imm16
                case 0xc2: {
                    decoded << "ret $0x" << hex << parse2ByteImmediate(opcode + 1);
                    break;
                }

                // call rel32off
                case 0xe8: {
                    decoded << "call " << hex << parse4ByteImmediate(opcode + 1);
                    break;
                }

                // pop reg64
                case 0x58:
                case 0x5b:
                case 0x59:
                case 0x5a:
                case 0x5d:
                case 0x5c:
                case 0x5f:
                case 0x5e: {
                    decoded << "pop " << getRegisterName(*opcode & 0x07);
                    break;
                }

                // push reg/mem64
                case 0xff:
                // pop reg/mem64
                case 0x8f: {
                    uint8_t modRMByte = *(opcode + 1);
                    ModRM modRM = parseModRM(modRMByte);

                    // opcode, <mnemonic, reg field>
                    map<uint8_t, std::pair<string, uint8_t>> opcodes {
                            { 0xff, { "push", 6} },
                            { 0x8f, { "pop", 0} }
                    };

                    if (opcodes.find(*opcode)->second.second != modRM.reg) {
                        decoded << "uknown instruction";
                        break;
                    }

                    decoded << opcodes[*opcode].first << " ";

                    // register not memory address
                    if (modRM.mod == 0b11) {
                        decoded << getRegisterName(modRM.rm);
                    }
                    else if (modRM.mod == 0b00 && modRM.rm == 0b101) {
                        decoded << "0x" << hex << parse4ByteImmediate(opcode + 2) << "(%rip)";
                    }
                    else if (modRMByte == 0x04 || modRMByte == 0x34) { // pop, push
                        // SIB !!!
                        // modRM.mod == 00 thus SIB.index = 0, SIB.base = 0
                        // skip also SIB byte
                        SIB SIB = parseSIB(*(opcode + 2));
                        uint8_t SIBByte = *(opcode + 2);
                        if (modRM.mod == 0 && SIB.index == 0b100 && SIB.base != 0b101) {
                            decoded << '(' << getRegisterName(SIBByte & 0x07) << ')';
                        }
                        else {
                            decoded << "0x" << hex << parse4ByteImmediate(opcode + 3);
                        }
                    }
                    else {
                        // with displacement/offset
                        // pop, push
                        if ((modRMByte & 0xf0) == 0x80 || (modRMByte & 0xf0) == 0xb0) { // 4 byte offset
                            decoded << "0x" << hex << parse4ByteImmediate(opcode + 2);
                        } else if ((modRMByte & 0xf0) == 0x40 || (modRMByte & 0xf0) == 0x70) { // 1 byte offset
                            decoded << "0x" << toHex(parseByteImmediate(opcode + 2));
                        }
                        decoded << '(' << getRegisterName(modRMByte & 0x07) << ')';
                    }
                    break;
                }

                // push imm64
                case 0x68: {
                    decoded << "push $0x" << hex << static_cast<int64_t>(parse4ByteImmediate(opcode + 1));
                    break;
                }

                // push reg64
                case 0x50:
                case 0x51:
                case 0x52:
                case 0x53:
                case 0x54:
                case 0x55:
                case 0x56:
                case 0x57: {
                    decoded << "push " << getRegisterName(*opcode & 0x07);
                    break;
                }

                // mov reg/mem64, reg64
                case 0x89:
                // mov reg64, reg/mem64
                case 0x8b: {
                    decoded << "mov ";

                    ModRM modRM = parseModRM(*(opcode + 1));
                    // only registers
                    if (modRM.mod == 0b11) {
                        decoded << parseTwo64bitRegistersFromModRM(*(opcode + 1));
                    }
                    else {
                        string regOperand = getRegisterName(modRM.reg);
                        string memOperand;
                        if (modRM.mod == 0b00 && modRM.rm == 0b101) { // rip + offset
                            auto offset = parse4ByteImmediate(opcode + 2);
                            memOperand = "0x" + intToHex(offset) + "(%rip)";
                        } else if (modRM.mod != 0b11 && modRM.rm == 0b101) { // rbp
                            auto offset = parse4ByteImmediate(opcode + 2);
                            memOperand = "0x" + intToHex(offset) + '(' + getRegisterName(modRM.rm) + ')';
                        } else {
                            memOperand = '(' + getRegisterName(modRM.reg) + ')';
                        }

                        if (*opcode == 0x89) {
                            decoded << regOperand << ", " << memOperand;
                        } else { // 0x8b
                            decoded << memOperand << ", " << regOperand;
                        }
                    }
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
