#include <iostream>
#include <sstream>

#include "decode.hpp"

std::string toHex(int byte) {
    std::ostringstream os;
    os << std::hex << byte;
    std::string s = os.str();
    return os.str();
}

int main(int argc, char* argv[]) {
    using InstructionDecoding::Instruction;
    using std::string;
    using std::cout;
    using std::endl;
    using std::cin;

    InstructionDecoding::Decoder decoder;/*
    std::vector<Instruction> instructions;
    if (argc > 1) {
        instructions = decoder.decodeInstructions(std::vector<const char*>(argv + 1, argv + argc));
    }
    else {
        string concatenatedBytes;
        string byte;
        while (cin >> byte) {
            concatenatedBytes += (concatenatedBytes.empty() ? "" : " ") + byte;
        }
        instructions = decoder.decodeInstructions(concatenatedBytes);
    }

    for (const auto& instruction : instructions) {
        cout << instruction << endl;
    }

    for (InstructionDecoding::Instruction instruction : InstructionDecoding::Decoder().decodeInstructions(std::vector<const char*>(argv + 1, argv + argc))) {
        std::cout << instruction.toStr() << std::endl;
    }

    std::cout << "----------------------------------------------------";
*/
    // xor
    std::cout << decoder.decodeInstructionToStr("35 ab cf 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 35 ab cf 00 00") << std::endl;

    // add
    std::cout << decoder.decodeInstructionToStr("05 ab cf 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 05 ab cf 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 05 fb ff ff ff") << std::endl;

    // imul
    std::cout << decoder.decodeInstructionToStr("48 0f af " + toHex(0b11000000)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 0f af " + toHex(0b11000011)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 0f af " + toHex(0b11000001)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 0f af " + toHex(0b11000010)) << std::endl;

    std::cout << decoder.decodeInstructionToStr("48 0f af " + toHex(0b11011000)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 0f af " + toHex(0b11001000)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 0f af " + toHex(0b11010000)) << std::endl;

    // mul
    std::cout << decoder.decodeInstructionToStr("48 f7 e0") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 f7 e3") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 f7 e1") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 f7 e2") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 f7 e6") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 f7 e7") << std::endl;

    // cmp
    std::cout << decoder.decodeInstructionToStr("48 3d ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("3d ab 00 00 00") << std::endl;

    // cmp
    std::cout << decoder.decodeInstructionToStr("48 39 " + toHex(0b11000000)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 39 " + toHex(0b11000011)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 39 " + toHex(0b11000001)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 39 " + toHex(0b11000010)) << std::endl;

    std::cout << decoder.decodeInstructionToStr("48 39 " + toHex(0b11011000)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 39 " + toHex(0b11001000)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 39 " + toHex(0b11010000)) << std::endl;

    // cmp
    std::cout << decoder.decodeInstructionToStr("48 3b " + toHex(0b11000000)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 3b " + toHex(0b11000011)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 3b " + toHex(0b11000001)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 3b " + toHex(0b11000010)) << std::endl;

    std::cout << decoder.decodeInstructionToStr("48 3b " + toHex(0b11011000)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 3b " + toHex(0b11001000)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 3b " + toHex(0b11010000)) << std::endl;

    // nop
    std::cout << decoder.decodeInstructionToStr("90") << std::endl;

    // int3
    std::cout << decoder.decodeInstructionToStr("cc") << std::endl;

    // jmp
    std::cout << decoder.decodeInstructionToStr("eb ab") << std::endl;
    std::cout << decoder.decodeInstructionToStr("e9 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("e9 fb ff ff ff") << std::endl;

    // je, jne, jb
    std::cout << decoder.decodeInstructionToStr("74 ab") << std::endl;
    std::cout << decoder.decodeInstructionToStr("75 ab") << std::endl;
    std::cout << decoder.decodeInstructionToStr("72 ab") << std::endl;

    // je, jne, jb
    std::cout << decoder.decodeInstructionToStr("74 7b") << std::endl;
    std::cout << decoder.decodeInstructionToStr("75 7b") << std::endl;
    std::cout << decoder.decodeInstructionToStr("72 7b") << std::endl;

    // je, jne, jb
    std::cout << decoder.decodeInstructionToStr("0f 84 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("0f 85 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("0f 82 ab 00 00 00") << std::endl;

    // je, jne, jb
    std::cout << decoder.decodeInstructionToStr("0f 84 fb ff ff ff") << std::endl;
    std::cout << decoder.decodeInstructionToStr("0f 85 fb ff ff ff") << std::endl;
    std::cout << decoder.decodeInstructionToStr("0f 82 fb ff ff ff") << std::endl;

    // ret
    std::cout << decoder.decodeInstructionToStr("c3") << std::endl;
    std::cout << decoder.decodeInstructionToStr("c2 ab 00") << std::endl;

    // call
    std::cout << decoder.decodeInstructionToStr("e8 ab 00 00 00") << std::endl;

    // pop
    std::cout << decoder.decodeInstructionToStr("58") << std::endl;
    std::cout << decoder.decodeInstructionToStr("5b") << std::endl;
    std::cout << decoder.decodeInstructionToStr("59") << std::endl;
    std::cout << decoder.decodeInstructionToStr("5a") << std::endl;
    std::cout << decoder.decodeInstructionToStr("5d") << std::endl;
    std::cout << decoder.decodeInstructionToStr("5c") << std::endl;
    std::cout << decoder.decodeInstructionToStr("5f") << std::endl;
    std::cout << decoder.decodeInstructionToStr("5e") << std::endl;

    // pop
    std::cout << decoder.decodeInstructionToStr("8f 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 03") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 01") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 02") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 45 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 45 01") << std::endl;

    // pop
    std::cout << decoder.decodeInstructionToStr("8f 80 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 83 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 81 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 82 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 85 ab 00 00 00") << std::endl;

    // pop
    std::cout << decoder.decodeInstructionToStr("8f c0") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f c3") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f c1") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f c2") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f c5") << std::endl;

    // pop
    std::cout << decoder.decodeInstructionToStr("8f 05 00 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 05 00 00 00 01") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 05 ab 00 00 00") << std::endl;

    // pop
    std::cout << decoder.decodeInstructionToStr("8f 04 25 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 04 25 00 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 04 24") << std::endl;

    // pop
    std::cout << decoder.decodeInstructionToStr("8f 04 25 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 04 25 00 00 00 00") << std::endl;

    // push imm64
    std::cout << decoder.decodeInstructionToStr("68 ab 00 00 00") << std::endl;

    // push reg64
    std::cout << decoder.decodeInstructionToStr("50") << std::endl;
    std::cout << decoder.decodeInstructionToStr("51") << std::endl;
    std::cout << decoder.decodeInstructionToStr("52") << std::endl;
    std::cout << decoder.decodeInstructionToStr("53") << std::endl;
    std::cout << decoder.decodeInstructionToStr("54") << std::endl;
    std::cout << decoder.decodeInstructionToStr("55") << std::endl;
    std::cout << decoder.decodeInstructionToStr("56") << std::endl;
    std::cout << decoder.decodeInstructionToStr("57") << std::endl;

    // push reg/mem64
    std::cout << decoder.decodeInstructionToStr("ff b0 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff b3 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff b1 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff b2 ab 00 00 00") << std::endl;

    std::cout << decoder.decodeInstructionToStr("ff 75 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff 75 01") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff b5 ab 00 00 00") << std::endl;

    std::cout << decoder.decodeInstructionToStr("ff 35 00 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff 35 01 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff 34 25 00 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff 34 25 ab cd ef 00") << std::endl;

    std::cout << decoder.decodeInstructionToStr("ff 30") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff 33") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff 31") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff 32") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff 35 00 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff 34 24") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff 75 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff 36") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff 37") << std::endl;

    std::cout << decoder.decodeInstructionToStr("ff 34 25 ab cd ef 00") << std::endl;

    // mov
    std::cout << decoder.decodeInstructionToStr("48 89 c0") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 89 c3") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 89 c1") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 89 c2") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 89 d8") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 89 c8") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 89 d0") << std::endl;

    std::cout << decoder.decodeInstructionToStr("48 8b 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 8b 18") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 8b 08") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 8b 10") << std::endl;

    std::cout << decoder.decodeInstructionToStr("48 89 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 89 03") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 89 01") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 89 02") << std::endl;

    std::cout << decoder.decodeInstructionToStr("48 89 05 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 89 85 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 89 1d ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 89 9d ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 8b 05 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 8b 85 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 8b 1d ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 8b 9d ab 00 00 00") << std::endl;

    std::cout << decoder.decodeInstructionToStr("48 89 00") << std::endl; //                mov    %rax,(%rax)
    std::cout << decoder.decodeInstructionToStr("48 89 03") << std::endl; //                mov    %rax,(%rbx)
    std::cout << decoder.decodeInstructionToStr("48 89 01") << std::endl; //                mov    %rax,(%rcx)
    std::cout << decoder.decodeInstructionToStr("48 89 02") << std::endl; //                mov    %rax,(%rdx)
    std::cout << decoder.decodeInstructionToStr("48 89 05 00 00 00 00") << std::endl; //    mov    %rax,0x0(%rip)        # 400596 <main+0x106>
    std::cout << decoder.decodeInstructionToStr("48 89 45 00") << std::endl; //             mov    %rax,0x0(%rbp)
    std::cout << decoder.decodeInstructionToStr("48 89 07") << std::endl; //                mov    %rax,(%rdi)
    std::cout << decoder.decodeInstructionToStr("48 89 06") << std::endl; //                mov    %rax,(%rsi)

    std::cout << decoder.decodeInstructionToStr("48 8b 00") << std::endl; //                mov    (%rax),%rax
    std::cout << decoder.decodeInstructionToStr("48 8b 03") << std::endl; //                mov    (%rbx),%rax
    std::cout << decoder.decodeInstructionToStr("48 8b 01") << std::endl; //                mov    (%rcx),%rax
    std::cout << decoder.decodeInstructionToStr("48 8b 02") << std::endl; //                mov    (%rdx),%rax
    std::cout << decoder.decodeInstructionToStr("48 8b 05 00 00 00 00") << std::endl; //    mov    0x0(%rip),%rax        # 4005b3 <main+0x123>
    std::cout << decoder.decodeInstructionToStr("48 8b 45 00") << std::endl; //             mov    0x0(%rbp),%rax
    std::cout << decoder.decodeInstructionToStr("48 8b 07") << std::endl; //                mov    (%rdi),%rax
    std::cout << decoder.decodeInstructionToStr("48 8b 06") << std::endl; //                mov    (%rsi),%rax

    std::cout << decoder.decodeInstructionToStr("48 89 80 ab 00 00 00") << std::endl; //    mov    %rax,0xab(%rax)
    std::cout << decoder.decodeInstructionToStr("48 89 83 ab 00 00 00") << std::endl; //    mov    %rax,0xab(%rbx)
    std::cout << decoder.decodeInstructionToStr("48 89 81 ab 00 00 00") << std::endl; //    mov    %rax,0xab(%rcx)
    std::cout << decoder.decodeInstructionToStr("48 89 82 ab 00 00 00") << std::endl; //    mov    %rax,0xab(%rdx)
    std::cout << decoder.decodeInstructionToStr("48 89 05 ab 00 00 00") << std::endl; //    mov    %rax,0xab(%rip)        # 40068b <main+0x1fb>
    std::cout << decoder.decodeInstructionToStr("48 89 85 ab 00 00 00") << std::endl; //    mov    %rax,0xab(%rbp)
    std::cout << decoder.decodeInstructionToStr("48 89 87 ab 00 00 00") << std::endl; //    mov    %rax,0xab(%rdi)
    std::cout << decoder.decodeInstructionToStr("48 89 86 ab 00 00 00") << std::endl; //    mov    %rax,0xab(%rsi)

    std::cout << decoder.decodeInstructionToStr("48 8b 80 ab 00 00 00") << std::endl; //    mov    0xab(%rax),%rax
    std::cout << decoder.decodeInstructionToStr("48 8b 83 ab 00 00 00") << std::endl; //    mov    0xab(%rbx),%rax
    std::cout << decoder.decodeInstructionToStr("48 8b 81 ab 00 00 00") << std::endl; //    mov    0xab(%rcx),%rax
    std::cout << decoder.decodeInstructionToStr("48 8b 82 ab 00 00 00") << std::endl; //    mov    0xab(%rdx),%rax
    std::cout << decoder.decodeInstructionToStr("48 8b 05 ab 00 00 00") << std::endl; //    mov    0xab(%rip),%rax        # 4006c3 <main+0x233>
    std::cout << decoder.decodeInstructionToStr("48 8b 85 ab 00 00 00") << std::endl; //    mov    0xab(%rbp),%rax
    std::cout << decoder.decodeInstructionToStr("48 8b 87 ab 00 00 00") << std::endl; //    mov    0xab(%rdi),%rax
    std::cout << decoder.decodeInstructionToStr("48 8b 86 ab 00 00 00") << std::endl; //    mov    0xab(%rsi),%rax

    std::cout << decoder.decodeInstructionToStr("48 89 40 fb") << std::endl; //             mov    %rax,-0x5(%rax)
    std::cout << decoder.decodeInstructionToStr("48 89 43 fb") << std::endl; //             mov    %rax,-0x5(%rbx)
    std::cout << decoder.decodeInstructionToStr("48 89 41 fb") << std::endl; //             mov    %rax,-0x5(%rcx)
    std::cout << decoder.decodeInstructionToStr("48 89 42 fb") << std::endl; //             mov    %rax,-0x5(%rdx)
    std::cout << decoder.decodeInstructionToStr("48 89 05 fb ff ff ff") << std::endl; //    mov    %rax,-0x5(%rip)        # 40063f <main+0x1af>
    std::cout << decoder.decodeInstructionToStr("48 89 45 fb") << std::endl; //             mov    %rax,-0x5(%rbp)
    std::cout << decoder.decodeInstructionToStr("48 89 47 fb") << std::endl; //             mov    %rax,-0x5(%rdi)
    std::cout << decoder.decodeInstructionToStr("48 89 46 fb") << std::endl; //             mov    %rax,-0x5(%rsi)

    std::cout << decoder.decodeInstructionToStr("48 8b 40 fb") << std::endl; //             mov    -0x5(%rax),%rax
    std::cout << decoder.decodeInstructionToStr("48 8b 43 fb") << std::endl; //             mov    -0x5(%rbx),%rax
    std::cout << decoder.decodeInstructionToStr("48 8b 41 fb") << std::endl; //             mov    -0x5(%rcx),%rax
    std::cout << decoder.decodeInstructionToStr("48 8b 42 fb") << std::endl; //             mov    -0x5(%rdx),%rax
    std::cout << decoder.decodeInstructionToStr("48 8b 05 fb ff ff ff") << std::endl; //    mov    -0x5(%rip),%rax        # 400662 <main+0x1d2>
    std::cout << decoder.decodeInstructionToStr("48 8b 47 fb") << std::endl; //             mov    -0x5(%rdi),%rax
    std::cout << decoder.decodeInstructionToStr("48 8b 46 fb") << std::endl; //             mov    -0x5(%rsi),%rax

    std::cout << decoder.decodeInstructionToStr("48 89 c4") << std::endl; //                mov    %rax,%rsp
    std::cout << decoder.decodeInstructionToStr("48 89 04 24") << std::endl; //             mov    %rax,(%rsp)
    std::cout << decoder.decodeInstructionToStr("48 89 04 24") << std::endl; //             mov    %rax,(%rsp)
    std::cout << decoder.decodeInstructionToStr("48 89 84 24 ab 00 00 00") << std::endl; //    mov    %rax,0xab(%rsp)
    std::cout << decoder.decodeInstructionToStr("48 89 44 24 fb") << std::endl; //          mov    %rax,-0x5(%rsp)
    std::cout << decoder.decodeInstructionToStr("48 89 e0") << std::endl; //                mov    %rsp,%rax
    std::cout << decoder.decodeInstructionToStr("48 8b 04 24") << std::endl; //             mov    (%rsp),%rax
    std::cout << decoder.decodeInstructionToStr("48 8b 04 24") << std::endl; //             mov    (%rsp),%rax
    std::cout << decoder.decodeInstructionToStr("48 8b 84 24 ab 00 00 00") << std::endl; //    mov    0xab(%rsp),%rax
    std::cout << decoder.decodeInstructionToStr("48 8b 44 24 fb") << std::endl; //          mov    -0x5(%rsp),%rax

    std::cout << "--------------- mov with immediates -------------------" << std::endl;
    // mov with immediates
    std::cout << decoder.decodeInstructionToStr("48 c7 c0 00 00 00 00") << std::endl; //    mov    $0x0,%rax
    std::cout << decoder.decodeInstructionToStr("48 c7 c3 00 00 00 00") << std::endl; //    mov    $0x0,%rbx
    std::cout << decoder.decodeInstructionToStr("48 c7 c1 00 00 00 00") << std::endl; //    mov    $0x0,%rcx
    std::cout << decoder.decodeInstructionToStr("48 c7 c2 00 00 00 00") << std::endl; //    mov    $0x0,%rdx
    std::cout << decoder.decodeInstructionToStr("48 c7 c5 00 00 00 00") << std::endl; //    mov    $0x0,%rbp
    std::cout << decoder.decodeInstructionToStr("48 c7 c7 00 00 00 00") << std::endl; //    mov    $0x0,%rdi
    std::cout << decoder.decodeInstructionToStr("48 c7 c6 00 00 00 00") << std::endl; //    mov    $0x0,%rsi
    std::cout << decoder.decodeInstructionToStr("48 c7 c0 ab 00 00 00") << std::endl; //    mov    $0xab,%rax
    std::cout << decoder.decodeInstructionToStr("48 c7 c3 ab 00 00 00") << std::endl; //    mov    $0xab,%rbx
    std::cout << decoder.decodeInstructionToStr("48 c7 c1 ab 00 00 00") << std::endl; //    mov    $0xab,%rcx
    std::cout << decoder.decodeInstructionToStr("48 c7 c2 ab 00 00 00") << std::endl; //    mov    $0xab,%rdx
    std::cout << decoder.decodeInstructionToStr("48 c7 c5 ab 00 00 00") << std::endl; //    mov    $0xab,%rbp
    std::cout << decoder.decodeInstructionToStr("48 c7 c7 ab 00 00 00") << std::endl; //    mov    $0xab,%rdi
    std::cout << decoder.decodeInstructionToStr("48 c7 c6 ab 00 00 00") << std::endl; //    mov    $0xab,%rsi
    std::cout << decoder.decodeInstructionToStr("48 c7 c0 fb ff ff ff") << std::endl; //    mov    $0xfffffffffffffffb,%rax
    std::cout << decoder.decodeInstructionToStr("48 c7 c3 fb ff ff ff") << std::endl; //    mov    $0xfffffffffffffffb,%rbx
    std::cout << decoder.decodeInstructionToStr("48 c7 c1 fb ff ff ff") << std::endl; //    mov    $0xfffffffffffffffb,%rcx
    std::cout << decoder.decodeInstructionToStr("48 c7 c2 fb ff ff ff") << std::endl; //    mov    $0xfffffffffffffffb,%rdx
    std::cout << decoder.decodeInstructionToStr("48 c7 c5 fb ff ff ff") << std::endl; //    mov    $0xfffffffffffffffb,%rbp
    std::cout << decoder.decodeInstructionToStr("48 c7 c7 fb ff ff ff") << std::endl; //    mov    $0xfffffffffffffffb,%rdi
    std::cout << decoder.decodeInstructionToStr("48 c7 c6 fb ff ff ff") << std::endl; //    mov    $0xfffffffffffffffb,%rsi
    std::cout << decoder.decodeInstructionToStr("48 c7 c4 00 00 00 00") << std::endl; //    mov    $0x0,%rsp
    std::cout << decoder.decodeInstructionToStr("48 c7 c4 ab 00 00 00") << std::endl; //    mov    $0xab,%rsp
    std::cout << decoder.decodeInstructionToStr("48 c7 c4 fb ff ff ff") << std::endl; //    mov    $0xfffffffffffffffb,%rsp

    std::cout << decoder.decodeInstructionToStr("c7 00 00 00 00 00") << std::endl; //       movl   $0x0,(%rax)
    std::cout << decoder.decodeInstructionToStr("c7 03 00 00 00 00") << std::endl; //       movl   $0x0,(%rbx)
    std::cout << decoder.decodeInstructionToStr("c7 01 00 00 00 00") << std::endl; //       movl   $0x0,(%rcx)
    std::cout << decoder.decodeInstructionToStr("c7 02 00 00 00 00") << std::endl; //       movl   $0x0,(%rdx)
    std::cout << decoder.decodeInstructionToStr("c7 45 00 00 00 00 00") << std::endl; //    movl   $0x0,0x0(%rbp)
    std::cout << decoder.decodeInstructionToStr("c7 07 00 00 00 00") << std::endl; //       movl   $0x0,(%rdi)
    std::cout << decoder.decodeInstructionToStr("c7 06 00 00 00 00") << std::endl; //       movl   $0x0,(%rsi)
    std::cout << decoder.decodeInstructionToStr("c7 00 ab 00 00 00") << std::endl; //       movl   $0xab,(%rax)
    std::cout << decoder.decodeInstructionToStr("c7 03 ab 00 00 00") << std::endl; //       movl   $0xab,(%rbx)
    std::cout << decoder.decodeInstructionToStr("c7 01 ab 00 00 00") << std::endl; //       movl   $0xab,(%rcx)
    std::cout << decoder.decodeInstructionToStr("c7 02 ab 00 00 00") << std::endl; //       movl   $0xab,(%rdx)
    std::cout << decoder.decodeInstructionToStr("c7 45 00 ab 00 00 00") << std::endl; //    movl   $0xab,0x0(%rbp)
    std::cout << decoder.decodeInstructionToStr("c7 07 ab 00 00 00") << std::endl; //       movl   $0xab,(%rdi)
    std::cout << decoder.decodeInstructionToStr("c7 06 ab 00 00 00") << std::endl; //       movl   $0xab,(%rsi)
    std::cout << decoder.decodeInstructionToStr("c7 00 fb ff ff ff") << std::endl; //       movl   $0xfffffffb,(%rax)
    std::cout << decoder.decodeInstructionToStr("c7 03 fb ff ff ff") << std::endl; //       movl   $0xfffffffb,(%rbx)
    std::cout << decoder.decodeInstructionToStr("c7 01 fb ff ff ff") << std::endl; //       movl   $0xfffffffb,(%rcx)
    std::cout << decoder.decodeInstructionToStr("c7 02 fb ff ff ff") << std::endl; //       movl   $0xfffffffb,(%rdx)
    std::cout << decoder.decodeInstructionToStr("c7 45 00 fb ff ff ff") << std::endl; //    movl   $0xfffffffb,0x0(%rbp)
    std::cout << decoder.decodeInstructionToStr("c7 07 fb ff ff ff") << std::endl; //       movl   $0xfffffffb,(%rdi)
    std::cout << decoder.decodeInstructionToStr("c7 06 fb ff ff ff") << std::endl; //       movl   $0xfffffffb,(%rsi)
    std::cout << decoder.decodeInstructionToStr("c7 04 24 00 00 00 00") << std::endl; //    movl   $0x0,(%rsp)
    std::cout << decoder.decodeInstructionToStr("c7 04 24 ab 00 00 00") << std::endl; //    movl   $0xab,(%rsp)
    std::cout << decoder.decodeInstructionToStr("c7 04 24 fb ff ff ff") << std::endl; //    movl   $0xfffffffb,(%rsp)
    std::cout << decoder.decodeInstructionToStr("c7 00 00 00 00 00") << std::endl; //       movl   $0x0,(%rax)
    std::cout << decoder.decodeInstructionToStr("c7 03 00 00 00 00") << std::endl; //       movl   $0x0,(%rbx)
    std::cout << decoder.decodeInstructionToStr("c7 01 00 00 00 00") << std::endl; //       movl   $0x0,(%rcx)
    std::cout << decoder.decodeInstructionToStr("c7 02 00 00 00 00") << std::endl; //       movl   $0x0,(%rdx)
    std::cout << decoder.decodeInstructionToStr("c7 45 00 00 00 00 00") << std::endl; //    movl   $0x0,0x0(%rbp)
    std::cout << decoder.decodeInstructionToStr("c7 07 00 00 00 00") << std::endl; //       movl   $0x0,(%rdi)
    std::cout << decoder.decodeInstructionToStr("c7 06 00 00 00 00") << std::endl; //       movl   $0x0,(%rsi)
    std::cout << decoder.decodeInstructionToStr("c7 00 ab 00 00 00") << std::endl; //       movl   $0xab,(%rax)
    std::cout << decoder.decodeInstructionToStr("c7 03 ab 00 00 00") << std::endl; //       movl   $0xab,(%rbx)
    std::cout << decoder.decodeInstructionToStr("c7 01 ab 00 00 00") << std::endl; //       movl   $0xab,(%rcx)
    std::cout << decoder.decodeInstructionToStr("c7 02 ab 00 00 00") << std::endl; //       movl   $0xab,(%rdx)
    std::cout << decoder.decodeInstructionToStr("c7 45 00 ab 00 00 00") << std::endl; //    movl   $0xab,0x0(%rbp)
    std::cout << decoder.decodeInstructionToStr("c7 07 ab 00 00 00") << std::endl; //       movl   $0xab,(%rdi)
    std::cout << decoder.decodeInstructionToStr("c7 06 ab 00 00 00") << std::endl; //       movl   $0xab,(%rsi)
    std::cout << decoder.decodeInstructionToStr("c7 00 fb ff ff ff") << std::endl; //       movl   $0xfffffffb,(%rax)
    std::cout << decoder.decodeInstructionToStr("c7 03 fb ff ff ff") << std::endl; //       movl   $0xfffffffb,(%rbx)
    std::cout << decoder.decodeInstructionToStr("c7 01 fb ff ff ff") << std::endl; //       movl   $0xfffffffb,(%rcx)
    std::cout << decoder.decodeInstructionToStr("c7 02 fb ff ff ff") << std::endl; //       movl   $0xfffffffb,(%rdx)
    std::cout << decoder.decodeInstructionToStr("c7 45 00 fb ff ff ff") << std::endl; //    movl   $0xfffffffb,0x0(%rbp)
    std::cout << decoder.decodeInstructionToStr("c7 07 fb ff ff ff") << std::endl; //       movl   $0xfffffffb,(%rdi)
    std::cout << decoder.decodeInstructionToStr("c7 06 fb ff ff ff") << std::endl; //       movl   $0xfffffffb,(%rsi)
    std::cout << decoder.decodeInstructionToStr("c7 04 24 00 00 00 00") << std::endl; //    movl   $0x0,(%rsp)
    std::cout << decoder.decodeInstructionToStr("c7 04 24 ab 00 00 00") << std::endl; //    movl   $0xab,(%rsp)
    std::cout << decoder.decodeInstructionToStr("c7 04 24 fb ff ff ff") << std::endl; //    movl   $0xfffffffb,(%rsp)
    std::cout << decoder.decodeInstructionToStr("c7 80 cd 00 00 00 00 00 00 00") << std::endl; //     movl   $0x0,0xcd(%rax)
    std::cout << decoder.decodeInstructionToStr("c7 83 cd 00 00 00 00 00 00 00") << std::endl; //    movl   $0x0,0xcd(%rbx)
    std::cout << decoder.decodeInstructionToStr("c7 81 cd 00 00 00 00 00 00 00") << std::endl; //    movl   $0x0,0xcd(%rcx)
    std::cout << decoder.decodeInstructionToStr("c7 82 cd 00 00 00 00 00 00 00") << std::endl; //    movl   $0x0,0xcd(%rdx)
    std::cout << decoder.decodeInstructionToStr("c7 85 cd 00 00 00 00 00 00 00") << std::endl; //    movl   $0x0,0xcd(%rbp)
    std::cout << decoder.decodeInstructionToStr("c7 87 cd 00 00 00 00 00 00 00") << std::endl; //    movl   $0x0,0xcd(%rdi)
    std::cout << decoder.decodeInstructionToStr("c7 86 cd 00 00 00 00 00 00 00") << std::endl; //     movl   $0x0,0xcd(%rsi)
    std::cout << decoder.decodeInstructionToStr("c7 80 cd 00 00 00 ab 00 00 00") << std::endl; //    movl   $0xab,0xcd(%rax)
    std::cout << decoder.decodeInstructionToStr("c7 83 cd 00 00 00 ab 00 00 00") << std::endl; //    movl   $0xab,0xcd(%rbx)
    std::cout << decoder.decodeInstructionToStr("c7 81 cd 00 00 00 ab 00 00 00") << std::endl; //    movl   $0xab,0xcd(%rcx)
    std::cout << decoder.decodeInstructionToStr("c7 82 cd 00 00 00 ab 00 00 00") << std::endl; //    movl   $0xab,0xcd(%rdx)
    std::cout << decoder.decodeInstructionToStr("c7 85 cd 00 00 00 ab 00 00 00") << std::endl; //    movl   $0xab,0xcd(%rbp)
    std::cout << decoder.decodeInstructionToStr("c7 87 cd 00 00 00 ab 00 00 00") << std::endl; //    movl   $0xab,0xcd(%rdi)
    std::cout << decoder.decodeInstructionToStr("c7 86 cd 00 00 00 ab 00 00 00") << std::endl; //    movl   $0xab,0xcd(%rsi)
    std::cout << decoder.decodeInstructionToStr("c7 80 cd 00 00 00 fb ff ff ff") << std::endl; //   movl   $0xfffffffb,0xcd(%rax)
    std::cout << decoder.decodeInstructionToStr("c7 83 cd 00 00 00 fb ff ff ff") << std::endl; //    movl   $0xfffffffb,0xcd(%rbx)
    std::cout << decoder.decodeInstructionToStr("c7 81 cd 00 00 00 fb ff ff ff") << std::endl; //    movl   $0xfffffffb,0xcd(%rcx)
    std::cout << decoder.decodeInstructionToStr("c7 82 cd 00 00 00 fb ff ff ff") << std::endl; //   movl   $0xfffffffb,0xcd(%rdx)
    std::cout << decoder.decodeInstructionToStr("c7 85 cd 00 00 00 fb ff ff ff") << std::endl; //    movl   $0xfffffffb,0xcd(%rbp)
    std::cout << decoder.decodeInstructionToStr("c7 87 cd 00 00 00 fb ff ff ff") << std::endl; //    movl   $0xfffffffb,0xcd(%rdi)
    std::cout << decoder.decodeInstructionToStr("c7 86 cd 00 00 00 fb ff ff ff") << std::endl; //    movl   $0xfffffffb,0xcd(%rsi)
    std::cout << decoder.decodeInstructionToStr("c7 84 24 cd 00 00 00 00 00 00 00") << std::endl; //    movl   $0x0,0xcd(%rsp)
    std::cout << decoder.decodeInstructionToStr("c7 84 24 cd 00 00 00 ab 00 00 00") << std::endl; //    movl   $0xab,0xcd(%rsp)
    std::cout << decoder.decodeInstructionToStr("c7 84 24 cd 00 00 00 fb ff ff ff") << std::endl; //    movl   $0xfffffffb,0xcd(%rsp)

    std::cout << decoder.decodeInstructionToStr("c7 05 00 00 00 00 00 00 00 00") << std::endl; //    movl   $0x0,0x0(%rip)        # 400960 <main+0x4e0>
    std::cout << decoder.decodeInstructionToStr("c7 05 00 00 00 00 ab 00 00 00") << std::endl; //    movl   $0xab,0x0(%rip)        # 40096a <main+0x4ea>
    std::cout << decoder.decodeInstructionToStr("c7 05 00 00 00 00 fb ff ff ff") << std::endl; //   movl   $0xfffffffb,0x0(%rip)        # 400974 <main+0x4f4>
    std::cout << decoder.decodeInstructionToStr("c7 05 00 00 00 00 00 00 00 00") << std::endl; //   movl   $0x0,0x0(%rip)        # 40097e <main+0x4fe>
    std::cout << decoder.decodeInstructionToStr("c7 05 00 00 00 00 ab 00 00 00") << std::endl; //   movl   $0xab,0x0(%rip)        # 400988 <main+0x508>
    std::cout << decoder.decodeInstructionToStr("c7 05 00 00 00 00 fb ff ff ff") << std::endl; //   movl   $0xfffffffb,0x0(%rip)        # 400992 <main+0x512>
    std::cout << decoder.decodeInstructionToStr("c7 05 cd 00 00 00 00 00 00 00") << std::endl; //   movl   $0x0,0xcd(%rip)        # 400a69 <main+0x5e9>
    std::cout << decoder.decodeInstructionToStr("c7 05 cd 00 00 00 ab 00 00 00") << std::endl; //   movl   $0xab,0xcd(%rip)        # 400a73 <main+0x5f3>
    std::cout << decoder.decodeInstructionToStr("c7 05 cd 00 00 00 fb ff ff ff") << std::endl; //   movl   $0xfffffffb,0xcd(%rip)        # 400a7d <main+0x5fd>


    return 0;
}