#include <fstream>
#include <iostream>
#include <string>
#include <cstdint>
#include <tuple>
#include <vector>
#include <stdexcept>

using namespace std;

class CIFF {
public:
    string magic = "CIFF";
    uint64_t header_size = 0;
    uint64_t content_size = 0;
    uint64_t width = 0;
    uint64_t height = 0;
    string caption = "";
    vector<string> tags;
    vector<tuple<uint8_t, uint8_t, uint8_t>> pixels;

    bool is_valid = true;
    
    static CIFF parse_ciff_file(const string &file_path) {
        CIFF new_ciff;
        uint64_t bytes_read = 0;
        
        try {
            ifstream file(file_path, ios::binary);

            if (!file) {
                throw runtime_error("Cannot open file");
            }
            
            // magic bytes
            char magic_buf[5] = {0};
            file.read(magic_buf, 4);

            if (file.gcount() != 4)
                throw runtime_error("File too short for magic characters");
            
            bytes_read += 4;
            new_ciff.magic = string(magic_buf);
        
            if (new_ciff.magic != "CIFF")
                throw runtime_error("Invalid magic characters, expected 'CIFF'");
            
            // heder size
            file.read(reinterpret_cast<char*>(&new_ciff.header_size), 8);
            if (file.gcount() != 8)
                throw runtime_error("File too short for header size");
            bytes_read += 8;

            if (new_ciff.header_size < 38)
                throw runtime_error("Header size out of range, must be at least 38");
            
            // content size
            file.read(reinterpret_cast<char*>(&new_ciff.content_size), 8);
            if (file.gcount() != 8)
                throw runtime_error("File too short for content size");
            bytes_read += 8;
            
            // width
            file.read(reinterpret_cast<char*>(&new_ciff.width), 8);
            if (file.gcount() != 8)
                throw runtime_error("File too short for width size");
            bytes_read += 8;

            // height
            file.read(reinterpret_cast<char*>(&new_ciff.height), 8);
            if (file.gcount() != 8)
                throw runtime_error("File too short for height size");
            bytes_read += 8;

            // contetn size
            if (new_ciff.content_size != (new_ciff.width * new_ciff.height * 3)) {
                throw runtime_error("Content size does not match width * height * 3");
            }

            // caption
            char c;
            file.read(&c, 1);
            if (file.gcount() != 1)
                throw runtime_error("Unexpected EOF while reading caption");
            bytes_read += 1;

            while (c != '\n') {
                new_ciff.caption += c;
                file.read(&c, 1);
                if (file.gcount() != 1)
                    throw runtime_error("Unexpected EOF while reading caption");
                bytes_read += 1;

                if (bytes_read > new_ciff.header_size)
                    throw runtime_error("Caption parsing exceeded header size");
            }

            // tags
            string tag = "";
            while (bytes_read < new_ciff.header_size) {
                file.read(&c, 1);
                if (file.gcount() != 1)
                    throw runtime_error("Invalid image (EOF while reading tags)");
                bytes_read += 1;

                if (c == '\n')
                    throw runtime_error("Tags cannot contain newline characters");

                tag += c;
                if (c == '\0') {
                    new_ciff.tags.push_back(tag);
                    tag = "";
                }

                if (bytes_read == new_ciff.header_size && c != '\0')
                    throw runtime_error("The last character of the header is not null");
            }

            for (const auto& t : new_ciff.tags) {
                if (t.empty() || t.back() != '\0') {
                    throw runtime_error("A tag does not end with a null character");
                }
            }

            // pixels
            new_ciff.pixels.reserve(new_ciff.width * new_ciff.height);
            uint64_t pixels_end = new_ciff.header_size + new_ciff.content_size;

            while (bytes_read < pixels_end) {
                uint8_t rgb[3];
                file.read(reinterpret_cast<char*>(rgb), 3);
                
                if (file.gcount() != 3)
                    throw runtime_error("Unexpected EOF while reading pixels");
                bytes_read += 3;

                new_ciff.pixels.push_back(make_tuple(rgb[0], rgb[1], rgb[2]));
            }

            // extra data
            file.read(&c, 1);
            if (file.gcount() > 0)
                throw runtime_error("Extra data found after the expected end of file");

        } catch (const exception& e) {
            new_ciff.is_valid = false;
        }

        return new_ciff;
    }
};

int main() {
    string test_file = "test-vectors/test1.ciff";

    CIFF my_image = CIFF::parse_ciff_file(test_file);

    if (my_image.is_valid) {
        cout << "VALID" << endl;
        cout << "Kep merete: " << my_image.width << "x" << my_image.height << endl;
        cout <<  my_image.caption << endl;
    } else {
        cout << "Nem ervenyes" << endl;
    }

    return 0;
}