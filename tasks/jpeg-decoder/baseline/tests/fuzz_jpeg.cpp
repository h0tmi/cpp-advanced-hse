#include <decoder.h>

#include <sstream>
#include <string>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    std::string s(reinterpret_cast<const char *>(data), size);
    std::stringstream ss(s);
    try {
        auto image = Decode(ss);
        (void)image;
    } catch (...) {
    }
    return 0;
}
