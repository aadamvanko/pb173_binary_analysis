#ifndef DECODE_DECODE_HPP
#define DECODE_DECODE_HPP

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <cassert>

namespace InstructionDecoding {

    using std::string;
    using std::cout;
    using std::endl;
    using std::vector;

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

    public:
        string decode(const string& concatenatedBytes) {
            assert(!concatenatedBytes.empty());

            vector<uint8_t> bytes;
            for (const auto& byteStr : split(concatenatedBytes, ' ')) {
                assert(!byteStr.empty());
                uint8_t byte = static_cast<uint8_t>(std::stoul(byteStr, 0, 16));
                bytes.push_back(byte);
            }
            assert(!bytes.empty());

            if (bytes[0] < 0x40 || bytes[0] > 0x4f) {
                return "unknown instruction";
            }

            return concatenatedBytes;
        }
    };

}

#endif //DECODE_DECODE_H
