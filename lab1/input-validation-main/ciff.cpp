#include <fstream>
#include <iostream>
#include <string>
#include <cstdint>
#include <tuple>
#include <vector>
using namespace std;

class CIFF {
public:
    string magic = "CIFF";
    uint64_t header_size_long = 0;
    uint64_t content_size_long = 0;
    uint64_t width = 0;
    uint64_t height = 0;
    string caption = "";
    vector<string> tags;
    vector<tuple<uint8_t,uint8_t,uint8_t>> pixels;

    bool is_valid = true;
    
    static CIFF parse_ciff_file(string &file_path) {
        CIFF new_ciff;

        try {
            ifstream file(file_path, ios::binary);

            if (!file) {
                throw runtime_error("Cannot open file");
            }
            
            //char magic_buf[5] = {0};
            //file.read(magic_buf, 4);

            //if (dd)

            cout << "OK" << endl;
            

        } catch (const exception& e) {
            new_ciff.is_valid = false;
        }
        return new_ciff;
    }


};


int main() {
    string test_file = "test-vectors/test1.ciff"; // Cseréld ki a valós névre!

    CIFF my_image = CIFF::parse_ciff_file(test_file);

    if (my_image.is_valid) {
        cout << "VALID" << endl;
    } else {
        cout << "Nem ervenyes" << endl;
    }

    return 0;
}
