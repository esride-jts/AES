static unsigned char* to_bytes(const std::string& text) {
    unsigned char* bytes = new unsigned char[text.length() + 1];
    strcpy(reinterpret_cast<char*>(bytes), text.c_str());
    return bytes;
}

static std::string from_bytes(unsigned char* bytes) {
    return std::string(reinterpret_cast<char*>(bytes));
}

static std::string from_latin1(std::string& text) {
    std::string utf_out;
    for (std::string::iterator it = text.begin(); it != text.end(); ++it)
    {
        uint8_t ch = *it;
        if (ch < 0x80) {
            utf_out.push_back(ch);
        }
        else {
            utf_out.push_back(0xc0 | ch >> 6);
            utf_out.push_back(0x80 | (ch & 0x3f));
        }
    }
    return utf_out;
}

static std::string base64_encode(const std::string& in) {

    std::string out;

    int val = 0, valb = -6;
    for (unsigned char c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4) out.push_back('=');
    return out;
}

static std::string base64_decode(const std::string& in) {

    std::string out;

    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

    int val = 0, valb = -8;
    for (unsigned char c : in) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}