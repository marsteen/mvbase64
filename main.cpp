
#include <string>
#include <fstream>
#include <iostream>
#include <CBase64.h>

using namespace std;

static size_t GetFileSize(const char* Filename)
{
    size_t FileSize;
    ifstream infile(Filename, ios::in | ios::binary);
    infile.seekg(0, ios::end);
    FileSize = infile.tellg();
    infile.close();
    return FileSize;
}


static char* ReadFile(const char* fname, size_t& fsize)
{
    char* buffer = nullptr;
    fsize = 0;
    if (GetFileSize(fname) > 0)
    {
        ifstream infile(fname, ios::binary);
        if (infile.good())
        {
            fsize = GetFileSize(fname);
            buffer = new char[fsize + 1];
            infile.read(buffer, fsize);
            infile.close();
            buffer[fsize] = 0;
        }
    }

    if (buffer == nullptr)
    {
        cout << "***** file read error:" << fname << endl;
    }

    return buffer;
}


static bool WriteFile(const char* fname, const char* buffer, size_t fsize)
{
    bool r = false;
    ofstream outfile(fname, ios::binary);
    if (outfile.good())
    {
        outfile.write(buffer, fsize);
        outfile.close();
        r = true;
    }
    return r;
}


int main(int argc, char* argv[])
{
    bool ShowUsage = true;
    if (argc >= 4)
    {
        if (string(argv[1]) == "-encode")
        {
            size_t fsize;
            char* bindata = ReadFile(argv[2], fsize);
            if (bindata != nullptr)
            {
                const std::string base64data = CBase64::EncodeBase64(bindata, fsize);
                WriteFile(argv[3], base64data.c_str(), base64data.size());
                delete[] bindata;
            }
            ShowUsage = false;
        }
        else
        if (string(argv[1]) == "-decode")
        {
            size_t fsize;
            size_t decsize;
            const char* base64data = (const char*) ReadFile(argv[2], fsize);
            if (base64data != nullptr)
            {
                std::vector<unsigned char> bindata;
                CBase64::DecodeBase64(base64data, bindata);
                WriteFile(argv[3], (const char*) &(bindata[0]), bindata.size());
                delete[] base64data;
            }
            ShowUsage = false;
        }
    }

    if (ShowUsage)
    {
        cout << "usage: base64 -encode <inputfile> <outputfile>" << endl
             << "   or: base64 -decode <inputfile> <outputfile>" << endl
             << "version 1.0" << endl;
    }

    return 0;
}
