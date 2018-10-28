#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <math.h>

using std::vector;
using std::string;

vector<int> encode(string message_ascii, int word_length) {

    vector<string> blocks;
    int num_blocks = message_ascii.size() / word_length;
    if(message_ascii.size() % word_length != 0) num_blocks++;

    for(int i = 0; i < num_blocks; ++i) {
        blocks.push_back(message_ascii.substr(i * word_length, i * word_length + word_length));
    }

    int num_additions = 0;

    for(int i = 0; i < blocks.size(); ++i) {
        for(int j = 1; j <= word_length; ++j) {
            if((j&(j-1))==0) {
                num_additions++;
                blocks[i].insert(blocks[i].begin()+j-1, '0');
            }
        }
    }

    std::cout << "After adding control bits: \n";
    for(char c: blocks[0]) std::cout << c;
    std::cout << "\n\n";

    vector<int> controlBits;

    for(int p = 0; p < num_additions; ++p) {
        int sum = 0;
        for(int i = pow(2, p); i < blocks[0].size() + 1; i += 2*pow(2, p)) {
            for(int j = i; j < std::min(i + pow(2, p), (double)blocks[0].size() + 1); ++j) {

                sum = (sum + blocks[0][j-1]) % 2;

            }
        }
        blocks[0][pow(2,p) - 1] = sum + '0';
        controlBits.push_back(sum);
    }

    std::cout << "After calculating control bits: \n";

    for(char c: blocks[0]) std::cout << c;
    std::cout << "\n";

    return controlBits;

}

void decode(string message, vector<int> controlBits) {

    vector<int> newControlBits;

    for(int p = 0; p < controlBits.size(); ++p) {
        int sum = 0;
        for(int i = pow(2, p); i < message.size() + 1; i += 2*pow(2, p)) {
            for(int j = i; j < std::min(i + pow(2, p),  (double)message.size() + 1); ++j) {

                sum = (sum + message[j-1]) % 2;

            }
        }
        newControlBits.push_back(sum % 2);
    }

    std::cout << "\n\nNew control bits:\n";
    for(int i: newControlBits) std::cout << i << " ";

    int error_position = 0;
    for(int i = 0; i < controlBits.size(); ++i) {
        if(controlBits[i] != newControlBits[i]) {
            error_position += pow(2 , i);
        }
    }

    std::cout << "\n\nPosition error: " << error_position;
}


int main() {

    string message = "habr";
    vector<int> controlBits_sent = encode("0100010000111101", 16);

    std::cout << "Check control bits: \n";
    for(int i: controlBits_sent) std::cout << i << " ";

    //11
    decode("000010000100001010101", controlBits_sent);

    return 0;
}