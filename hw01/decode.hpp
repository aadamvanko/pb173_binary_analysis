#ifndef DECODE_DECODE_HPP
#define DECODE_DECODE_HPP

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <cassert>
#include <unordered_map>
#include <map>

namespace InstructionDecoding
{
    namespace details
    {
        struct HexCharStruct
        {
            uint8_t c;
            HexCharStruct(uint8_t _c) : c(_c) { }
        };

        inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
        {
            return (o << std::hex << (int)hs.c);
        }

        inline std::string int8ToHexStr(uint8_t _c)
        {
            std::ostringstream os;
            os << HexCharStruct(_c);
            return os.str();
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

    }

    enum class OperandTypes { Immediate, MemoryAddress, RelativeOffset, Register };

    struct Operand {
        std::string representation;
        int64_t value = 0;
        bool isRegister = false;
    };

    struct Instruction {
        std::string mnemonic;
        Operand operandA;
        Operand operandB;
        int length = 0;

        std::string toStr() const {
            std::string str = mnemonic;
            if (!operandA.representation.empty()) {
                str += " " + operandA.representation;
            }
            if (!operandB.representation.empty()) {
                str += ", " + operandB.representation;
            }
            return str;// + ", len = " + std::to_string(length);
        }

        friend std::ostream& operator<<(std::ostream& os, const Instruction& instruction);
    };


    std::ostream& operator<<(std::ostream& os, const Instruction& instruction)
    {
        os << instruction.toStr();
        return os;
    }

    using std::string;
    using std::cout;
    using std::endl;
    using std::vector;
    using std::hex;
    using std::unordered_map;
    using std::map;

    using namespace details;
    using RegistersPair = std::pair<Operand, Operand>;

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

        int64_t parseImmediate(const uint8_t *startByte, int bytesCount) {
            uint64_t value = 0;
            for (int i = 0; i < bytesCount; i++, startByte++) {
                value |= static_cast<uint64_t>(*startByte) << (i * 8);
            }
            return static_cast<int64_t>(value);
        }

        string int64ToHex(int64_t value, bool hexPrefix = true) {
            std::ostringstream os;
            addSymbols(os, value, hexPrefix);
            os << hex << std::abs(value);
            return os.str();
        }

        string int32ToHex(int32_t value, bool hexPrefix = true) {
            std::ostringstream os;
            addSymbols(os, value, hexPrefix);
            os << hex << std::abs(value);
            return os.str();
        }

        string int16ToHex(int16_t value, bool hexPrefix = true) {
            std::ostringstream os;
            addSymbols(os, value, hexPrefix);
            os << hex << std::abs(value);
            return os.str();
        }

        string int8ToHex(int8_t value, bool hexPrefix = true) {
            std::ostringstream os;
            addSymbols(os, value, hexPrefix);
            os << hex << std::abs(value);
            return os.str();
        }

        std::ostringstream& addSymbols(std::ostringstream& os, int64_t value, bool hexPrefix) {
            if (value < 0) {
                os << "-";
            }
            if (hexPrefix) {
                os << "0x";
            }
            return os;
        }

        int8_t parseByteImmediate(const uint8_t *immediateStart) {
            return static_cast<int8_t>(parseImmediate(immediateStart, 1));
        }

        int16_t parse2ByteImmediate(const uint8_t *immediateStart) {
            return static_cast<int16_t>(parseImmediate(immediateStart, 2));
        }

        int32_t parse4ByteImmediate(const uint8_t *immediateStart) {
            return static_cast<int32_t>(parseImmediate(immediateStart, 4));
        }

        int64_t parse8ByteImmediate(const uint8_t *immediateStart) {
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

        Operand getRegister(uint8_t value) {
            return { getRegisterName(value), 0, true };
        }

        RegistersPair parseTwo64bitRegistersFromModRM(uint8_t byte) {
            ModRM modRM = parseModRM(byte);
            return { { getRegisterName(modRM.reg), 0, true },
                     { getRegisterName(modRM.rm), 0, true } };
        }

        vector<uint8_t> toBytes(const vector<const char*>& bytes) {
            std::string concatenatedBytes;
            for (int i = 0; i < bytes.size(); i++) {
                if (i != 0) {
                    concatenatedBytes += " ";
                }
                concatenatedBytes += std::string(bytes[i]);
            }
            return toBytes(concatenatedBytes);
        }

        vector<uint8_t> toBytes(const string& concatenatedBytes) {
            vector<uint8_t> bytes;
            for (const auto& byteStr : split(concatenatedBytes, ' ')) {
                uint8_t byte = static_cast<uint8_t>(std::stoul(byteStr, 0, 16));
                bytes.push_back(byte);
            }
            return bytes;
        }

        Instruction decodeBytes(const uint8_t* bytes) {
            if (bytes == nullptr) {
                return {};
            }

            Instruction UnknownInstruction { "unknown instruction", {}, {}, 1 };
            const uint8_t* opcode = bytes;
            int instructionLength = 1;

            REX rex{};
            if (bytes[0] >= 0x40 && bytes[0] <= 0x4f) {
                rex = parseRex(bytes[0]);
                opcode++;
                instructionLength++;
            }

            Instruction decoded;
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
                    decoded.mnemonic = opcodesInstructions[*opcode];
                    int32_t value = parse4ByteImmediate(opcode + 1);
                    if (rex.W) {
                        decoded.operandA = { "$" + int32ToHex(value), value, false };
                        decoded.operandB = { "%rax", 0, true };

                    } else {
                        decoded.operandA = { "$" + int32ToHex(value), value, false };
                        decoded.operandB = { "%eax", 0, true };
                    }
                    instructionLength += 4;
                    break;
                }

                    // jmp rel8
                case 0xeb: {
                    decoded.mnemonic = "jmp";
                    int8_t value = parseByteImmediate(opcode + 1);
                    decoded.operandA = { int8ToHex(value) + "(%rip)", value, false };
                    instructionLength += 1;
                    break;
                }

                    // jmp rel32
                case 0xe9: {
                    decoded.mnemonic = "jmp";
                    int32_t value = parse4ByteImmediate(opcode + 1);
                    decoded.operandA = { int32ToHex(value) + "(%rip)", value, false };
                    instructionLength += 4;
                    break;
                }

                // mul reg/mem64
                case 0xf7: {
                    ModRM modRm = parseModRM(*(opcode + 1));
                    decoded.mnemonic = "mul";
                    decoded.operandA = getRegister(modRm.rm);
                    instructionLength += 1;
                    break;
                }

                    // imul
                    // rel32 je, jne, jb
                case 0x0f: {
                    uint8_t secondPart = *(opcode + 1);
                    map<uint8_t, string> secondParts {
                            { 0xaf, "imul" },
                            { 0x84, "je" },
                            { 0x85, "jne" },
                            { 0x82, "jb" }
                    };
                    instructionLength += 1;

                    if (secondParts.find(secondPart) == secondParts.end()) {
                        return UnknownInstruction;
                    }

                    decoded.mnemonic = secondParts[secondPart];
                    if (secondPart == 0xaf) {
                        auto registers = parseTwo64bitRegistersFromModRM(*(opcode + 2));
                        decoded.operandA = registers.first;
                        decoded.operandB = registers.second;
                        instructionLength += 1;
                    }
                    else {
                        int32_t value = parse4ByteImmediate(opcode + 2);
                        decoded.operandA = { int32ToHex(value) + "(%rip)", value, false};
                        instructionLength += 4;
                    }
                    break;
                }



                // CMP reg/mem64, reg64
                case 0x39:
                // CMP reg64, reg/mem64
                case 0x3B: {
                    if (!rex.W) {
                        return UnknownInstruction;
                    }
                    decoded.mnemonic = "cmp";
                    auto registers = parseTwo64bitRegistersFromModRM(*(opcode + 1));
                    if (*opcode == 0x3B) {
                        std::swap(registers.first, registers.second);
                    }
                    decoded.operandA = registers.first;
                    decoded.operandB = registers.second;
                    instructionLength += 1;
                    break;
                }

                    // nop
                case 0x90: {
                    decoded.mnemonic = "nop";
                    break;
                }

                    // int3
                case 0xcc: {
                    decoded.mnemonic = "int3";
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
                    decoded.mnemonic = opcodesInstructions[*opcode];
                    int8_t value = parseByteImmediate(opcode + 1);
                    decoded.operandA = { int8ToHex(value) + "(%rip)", value, false };
                    instructionLength += 1;
                    break;
                }

                    // ret
                case 0xc3: {
                    decoded.mnemonic = "ret";
                    break;
                }

                    // ret imm16
                case 0xc2: {
                    decoded.mnemonic = "ret";
                    int16_t value = parse2ByteImmediate(opcode + 1);
                    decoded.operandA = { "$" + int16ToHex(value), value, false };
                    instructionLength += 2;
                    break;
                }

                    // call rel32off
                case 0xe8: {
                    decoded.mnemonic = "call";
                    int32_t value = parse4ByteImmediate(opcode + 1);
                    decoded.operandA = { int32ToHex(value), value, false };
                    instructionLength += 4;
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
                    decoded.mnemonic = "pop";
                    decoded.operandA = getRegister(*opcode & 0x07);
                    break;
                }

                    // push reg/mem64
                case 0xff:
                    // pop reg/mem64
                case 0x8f: {
                    uint8_t modRMByte = *(opcode + 1);
                    ModRM modRM = parseModRM(modRMByte);
                    instructionLength += 1;

                    // opcode, <mnemonic, reg field>
                    map<uint8_t, std::pair<string, uint8_t>> opcodes {
                            { 0xff, { "push", 6} },
                            { 0x8f, { "pop", 0} }
                    };

                    if (opcodes.find(*opcode)->second.second != modRM.reg) {
                        return UnknownInstruction;
                    }

                    decoded.mnemonic = opcodes[*opcode].first;

                    // register not memory address
                    if (modRM.mod == 0b11) {
                        decoded.operandA = getRegister(modRM.rm);
                    }
                    else if (modRM.mod == 0b00 && modRM.rm == 0b101) {
                        int32_t value = parse4ByteImmediate(opcode + 2);
                        decoded.operandA = { int32ToHex(value) + "(%rip)", value, false };
                        instructionLength += 4;
                    }
                    else if (modRMByte == 0x04 || modRMByte == 0x34) { // pop, push
                        // SIB !!!
                        // modRM.mod == 00 thus SIB.index = 0, SIB.base = 0
                        SIB SIB = parseSIB(*(opcode + 2));
                        uint8_t SIBByte = *(opcode + 2);
                        instructionLength += 1;
                        if (modRM.mod == 0 && SIB.index == 0b100 && SIB.base != 0b101) {
                            decoded.operandA = { '(' + getRegisterName(SIBByte & 0x07) + ')', 0, false };
                        }
                        else {
                            int32_t value = parse4ByteImmediate(opcode + 3);
                            decoded.operandA = { int32ToHex(value), value, false };
                            instructionLength += 4;
                        }
                    }
                    else {
                        // with displacement/offset
                        // pop, push
                        if ((modRMByte & 0xf0) == 0x80 || (modRMByte & 0xf0) == 0xb0) { // 4 byte offset
                            int32_t value = parse4ByteImmediate(opcode + 2);
                            decoded.operandA = { int32ToHex(value), 0, false };
                            instructionLength += 4;
                        } else if ((modRMByte & 0xf0) == 0x40 || (modRMByte & 0xf0) == 0x70) { // 1 byte offset
                            int8_t value = parseByteImmediate(opcode + 2);
                            decoded.operandA = { int8ToHex(value), value, false };
                            instructionLength += 1;
                        }
                        string baseRegister = '(' + getRegisterName(modRMByte & 0x07) + ')';
                        decoded.operandA = { decoded.operandA.representation + baseRegister, 0, false };
                    }
                    break;
                }

                    // push imm64
                case 0x68: {
                    decoded.mnemonic = "push";
                    int64_t value = parse4ByteImmediate(opcode + 1);
                    decoded.operandA = { "$" + int64ToHex(value), value, false };
                    instructionLength += 4;
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
                    decoded.mnemonic = "push";
                    decoded.operandA = getRegister(*opcode & 0x07);
                    break;
                }

                    // MOV reg/mem64, imm32
                case 0xc7: {
                    decoded.mnemonic = "mov";
                    ModRM modRM = parseModRM(*(opcode + 1));
                    instructionLength += 1;

                    int32_t immediate;
                    if (modRM.mod == 0b11) { // imm32 and register
                        immediate = parse4ByteImmediate(opcode + 2);
                        decoded.operandB = getRegister(modRM.rm);
                        instructionLength += 4;
                    } else if(modRM.mod == 0b00 && modRM.rm == 0b101) { // rip
                        immediate = parse4ByteImmediate(opcode + 6);
                        auto offset = parse4ByteImmediate(opcode + 2);
                        decoded.operandB = { int32ToHex(offset) + "(%rip)", 0, false };
                        instructionLength += 8;
                    } else if (modRM.rm == 0b101) { // rbp
                        if (modRM.mod == 0b01) { // 1 byte offset
                            immediate = parse4ByteImmediate(opcode + 3);
                            auto offset = parseByteImmediate(opcode + 2);
                            decoded.operandB = { int8ToHex(offset) + "(" + getRegisterName(modRM.rm) + ")", 0, false };
                            instructionLength += 5;
                        } else { // 0b10 4 byte offset
                            immediate = parse4ByteImmediate(opcode + 6);
                            auto offset = parse4ByteImmediate(opcode + 2);
                            decoded.operandB = { int32ToHex(offset) + "(" + getRegisterName(modRM.rm) + ")", 0, false };
                            instructionLength += 8;
                        }
                    } else if (modRM.rm == 0b100) { // rsp has also SIB
                        instructionLength += 1;
                        SIB sib = parseSIB(*(opcode + 2));
                        if (modRM.mod == 0b00) {
                            immediate = parse4ByteImmediate(opcode + 3);
                            decoded.operandB = { "(" + getRegisterName(modRM.rm) + ")", 0, false };
                            instructionLength += 4;
                        } else { // 0b10
                            immediate = parse4ByteImmediate(opcode + 7);
                            auto offset = parse4ByteImmediate(opcode + 3);
                            decoded.operandB = { int32ToHex(offset) + "(" + getRegisterName(modRM.rm) + ")", 0, false };
                            instructionLength += 8;
                        }
                    } else {
                        if (modRM.mod == 0b00) { // register as memory without offset
                            immediate = parse4ByteImmediate(opcode + 2);
                            decoded.operandB = { "(" + getRegisterName(modRM.rm) + ")", 0, false };
                            instructionLength += 4;
                        } else { // with offset
                            immediate = parse4ByteImmediate(opcode + 6);
                            auto offset = parse4ByteImmediate(opcode + 2);
                            decoded.operandB = { int32ToHex(offset) + "(" + getRegisterName(modRM.rm) + ")", 0, false };
                            instructionLength += 8;
                        }
                    }
                    decoded.operandA = { "$" + int32ToHex(immediate), immediate, false };

                    break;
                }

                    // mov reg/mem64, reg64
                case 0x89:
                    // mov reg64, reg/mem64
                case 0x8b: {
                    decoded.mnemonic = "mov";
                    ModRM modRM = parseModRM(*(opcode + 1));
                    instructionLength += 1;

                    // only registers
                    if (modRM.mod == 0b11) {
                        auto registers = parseTwo64bitRegistersFromModRM(*(opcode + 1));
                        decoded.operandA = registers.first;
                        decoded.operandB = registers.second;
                    }
                    else {
                        Operand regOperand = getRegister(modRM.reg);
                        Operand memOperand;
                        if (modRM.mod == 0b00 && modRM.rm == 0b101) { // rip + offset
                            int32_t offset = parse4ByteImmediate(opcode + 2); // rip has always 4 byte offset?
                            memOperand = {int32ToHex(offset) + "(%rip)", offset, false};
                            instructionLength += 4;
                        } else if (modRM.rm == 0b100) { // rsp uses also SIB byte
                            if (modRM.mod == 0b11) { // no SIB, no offset
                                memOperand = { '(' + getRegisterName(modRM.rm) + ')', 0, false };
                            } else if (modRM.mod == 0b00) { // SIB, no offset
                                memOperand = { '(' + getRegisterName(modRM.rm) + ')', 0, false };
                                instructionLength += 1;
                            } else if (modRM.mod == 0b01) { // SIB, 1 byte offset
                                auto sib = parseSIB(*(opcode + 2)); // maybe only modRM.rm is enough
                                auto offset = parseByteImmediate(opcode + 3);
                                memOperand = { int8ToHex(offset) + '(' + getRegisterName(sib.base) + ')', offset, false };
                                instructionLength += 2;
                            } else { // 0b10, SIB, 4 byte offset
                                auto sib = parseSIB(*(opcode + 2)); // maybe only modRM.rm is enough
                                auto offset = parse4ByteImmediate(opcode + 3);
                                memOperand = { int32ToHex(offset) + '(' + getRegisterName(sib.base) + ')', offset, false };
                                instructionLength += 5;
                            }
                        } else if (modRM.mod == 0b01) { // other registers with 1 byte offset
                            auto offset = parseByteImmediate(opcode + 2);
                            memOperand = { int8ToHex(offset) + '(' + getRegisterName(modRM.rm) + ')', offset, false };
                            instructionLength += 1;
                        } else if (modRM.mod == 0b10) { // other registers with 4 byte offset
                            auto offset = parse4ByteImmediate(opcode + 2);
                            memOperand = { int32ToHex(offset) + '(' + getRegisterName(modRM.rm) + ')', offset, false };
                            instructionLength += 4;
                        } else {
                            memOperand = { '(' + getRegisterName(modRM.rm) + ')', 0, false };
                        }

                        if (*opcode == 0x89) {
                            decoded.operandA = regOperand;
                            decoded.operandB = memOperand;
                        } else { // 0x8b
                            decoded.operandA = memOperand;
                            decoded.operandB = regOperand;
                        }
                    }
                    break;
                }

                default:
                    return UnknownInstruction;
            }

            decoded.length = instructionLength;
            return decoded;
        }

    public:
        // Instruction variants
        Instruction decodeInstruction(const vector<uint8_t>& bytes) {
            return decodeBytes(&bytes[0]);
        }

        Instruction decodeInstruction(const vector<const char*>& bytes) {
            return decodeInstruction(toBytes(bytes));
        }

        Instruction decodeInstruction(const string& concatenatedBytes) {
            return decodeInstruction(toBytes(concatenatedBytes));
        }

        // string variants
        string decodeInstructionToStr(const vector<uint8_t>& bytes) {
            return decodeInstruction(bytes).toStr();
        }

        string decodeInstructionToStr(const vector<const char*>& bytes) {
            return decodeInstruction(bytes).toStr();
        }

        string decodeInstructionToStr(const string &concatenatedBytes) {
            return decodeInstruction(concatenatedBytes).toStr();
        }

        // stream variants
        vector<Instruction> decodeInstructions(const vector<uint8_t>& bytes) {
            vector<Instruction> instructions;
            const uint8_t *instructionPtr = &bytes[0];
            while (instructionPtr != &bytes[0] + bytes.size()) {
                Instruction instruction = decodeBytes(instructionPtr);
                instructions.push_back(instruction);
                instructionPtr += instruction.length;
            }
            return instructions;
        }

        vector<Instruction> decodeInstructions(const vector<const char*>& bytes) {
            return decodeInstructions(toBytes(bytes));
        }

        vector<Instruction> decodeInstructions(const string& concatenatedBytes) {
            return decodeInstructions(toBytes(concatenatedBytes));
        }

    };

}

#endif //DECODE_DECODE_H
