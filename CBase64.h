
#ifndef CBase64_H
#define CBase64_H

#include <vector>
#include <string>

class CBase64
{
    public:

        static std::string EncodeBase64(const char* Input, size_t fsize);
        static size_t DecodeBase64(const std::string& Base64, std::vector<unsigned char>& erg);

};

#endif