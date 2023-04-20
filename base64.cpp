#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Base64编码表
const string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

// Base64解码表
vector<unsigned char> base64_chars_decode(256, -1);

// 初始化Base64解码表
void init_base64_chars_decode() {
    for (size_t i = 0; i < base64_chars.size(); ++i) {
        base64_chars_decode[base64_chars[i]] = static_cast<unsigned char>(i);
    }
}

// Base64编码函数
string base64_encode(const string& input) {
    string output;
    output.reserve((input.size() + 2) / 3 * 4);
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    for (auto byte : input) {
        char_array_3[i++] = byte;
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++) {
                output += base64_chars[char_array_4[i]];
            }
            i = 0;
        }
    }

    if (i) {
        for (int j = i; j < 3; j++) {
            char_array_3[j] = '\0';
        }

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (int j = 0; (j < i + 1); j++) {
            output += base64_chars[char_array_4[j]];
        }

        while ((i++ < 3)) {
            output += '=';
        }
    }

    return output;
}

// Base64解码函数
string base64_decode(const string& input) {
    string output;
    output.reserve(input.size() * 3 / 4);
    int i = 0;
    int j = 0;
    unsigned char char_array_4[4], char_array_3[3];

    for (auto c : input) {
        if (isalnum(c) || (c == '+') || (c == '/')) {
            char_array_4[i++] = c;
            if (i == 4) {
                for (i = 0; i < 4; i++) {
                    char_array_4[i] = base64_chars_decode[char_array_4[i]];
                }

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for (i = 0; (i < 3); i++) {
                    output += char_array_3[i];
                }
                i = 0;
            }
        }
    }

    if (i) {
        for (int j = i; j < 4; j++) {
            char_array_4[j] = 0;
        }

        for (int j = 0; j < 4; j++) {
            char_array_4[j] = base64_chars_decode[char_array_4[j]];
        }

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (int j = 0; (j < i - 1); j++) {
            output += char_array_3[j];
        }
    }

    return output;
}

int main() {
    string input_str;
    string output_str;
    char choice;

    init_base64_chars_decode();

    cout << "请选择操作：\n1.加密\n2.解密\n";
    cin >> choice;
    cin.ignore();

    switch (choice) {
        case '1':
            cout << "请输入要加密的字符串：\n";
            getline(cin, input_str);

            output_str = base64_encode(input_str);
            cout << "加密后的字符串为：" << output_str << "\n";
            break;

        case '2':
            cout << "请输入要解密的字符串：\n";
            getline(cin, input_str);

            output_str = base64_decode(input_str);
            cout << "解密后的字符串为：" << output_str << "\n";
            break;

        default:
            cout << "无效的选择！\n";
            break;
    }

    return 0;
}
