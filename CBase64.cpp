

#include <CBase64.h>
#include <sstream>
#include <cstring>
#include <vector>
#include <iostream>

using namespace std;

static unsigned char* PosTable;

//---------------------------------------------------------------------------
//
// Klasse:  static
// Methode: InitPosTablePos
//
//---------------------------------------------------------------------------

static void InitPosTablePos()
{
    if (PosTable == NULL)
    {
        PosTable = new unsigned char[256];
        memset(PosTable, 0, 256);
        const unsigned char* Base64tab = (unsigned char*) "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        for (int i = 0; Base64tab[i] != 0; i++)
        {
            PosTable[Base64tab[i]] = i + 1;
        }
    }
}

//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   FindPos
//
//---------------------------------------------------------------------------

static int FindPos(const char c)
{
    return PosTable[c] - 1;
}

//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   ShiftIn
//
//---------------------------------------------------------------------------


static void ShiftIn(unsigned int& c, int n)
{
    c <<= 6;
    c |= n;
}

//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   SaveC
//
//---------------------------------------------------------------------------

static void SaveC(unsigned int c, std::vector<unsigned char>& erg, int k)
{
    unsigned char c0 = (c >> 16);
    unsigned char c1 = (c >>  8) & 0xFF;
    unsigned char c2 = c & 0xFF;

    if (k > 0)
    {
        erg.push_back(c0);
    }
    if (k > 1)
    {
        erg.push_back(c1);
    }
    if (k > 2)
    {
        erg.push_back(c2);
    }
}

//---------------------------------------------------------------------------
//
// Klasse:    CBase64
// Methode:   DecodeBase64
//
//---------------------------------------------------------------------------

size_t CBase64::DecodeBase64(const std::string& Base64, std::vector<unsigned char>& erg)
{
    int len = Base64.size();
    int k = 0;

    InitPosTablePos();
    unsigned int c = 0;
    for (int i = 0; i < len; i++)
    {
        int n = FindPos(Base64[i]);
        if (n >= 0)
        {
            ShiftIn(c, n);
            if (++k == 4)
            {
                SaveC(c, erg, 4);
                c = 0;
                k = 0;
            }
        }
    }

    if (k > 0)
    {
        for (int km = k; km != 4; km++)
        {
            ShiftIn(c, 0);
        }
        SaveC(c, erg, k);
    }
    return erg.size();
}

//---------------------------------------------------------------------------
//
// Klasse:    CBase64
// Methode:   EncodeBase64
//
//---------------------------------------------------------------------------

std::string CBase64::EncodeBase64(const char* InputSigned, size_t fsize)
{
    static const char* Base64tab = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const unsigned char* Input = (const unsigned char*) InputSigned;
    std::stringstream Base64Str;

    size_t K    = fsize / 3;
    size_t Rest = fsize % 3;
    size_t x = 0;
    size_t AnzahlZeichen = (((fsize + Rest) / 3) * 4);
    size_t AnzahlZeilen  = (AnzahlZeichen / 76) + (AnzahlZeichen % 76);

    for (int i = 0; i < K; i++)
    {
        Base64Str << Base64tab[Input[0] >> 2]
                  << Base64tab[((Input[0] & 0x03) << 4) | (Input[1] >> 4)]
                  << Base64tab[((Input[1] & 0x0F) << 2) | (Input[2] >> 6)]
                  << Base64tab[Input[2] & 0x3F];

        Input += 3;
        if (++x == 19)
        {
            x = 0;
            Base64Str << endl;
        }
    }

    if (Rest > 0)
    {
        Base64Str << Base64tab[Input[0] >> 2];
        if (Rest == 2)
        {
            Base64Str << Base64tab[((Input[0] & 0x03) << 4) | (Input[1] >> 4)]
                      << Base64tab[((Input[1] & 0x0F) << 2)];
        }
        else
        {
            Base64Str << Base64tab[((Input[0] & 0x03) << 4)] << '=';
        }
        Base64Str << '=';
    }
    return Base64Str.str();
}
