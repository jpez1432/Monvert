
#include "Stdafx.h"
#include "CTexture.hpp"

CTexture::CTexture(void)
{

}

CTexture::~CTexture(void)
{
//    if (Data) {
//        delete [] Data;
//        Data = NULL;
//    }
}

bool CTexture::Save(std::string Filename)
{
//    SOIL_save_image(Filename.c_str(), SOIL_SAVE_TYPE_PNG, Width, Height, Channels, &Data[0]);

    return true;
}

bool CTexture::Load(std::string Filename)
{
//    Data = SOIL_load_image(Filename.c_str(), &Width, &Height, &Channels, 4);

    return true;
}

bool CTexture::ExportRaw(std::string Input, std::string Output, bool Invert)
{

    BITMAPFILEHEADER BMPHeader;
    BITMAPINFOHEADER BMPInfo;
    RGBQUAD BMPPalette[256];
    std::vector<unsigned char>BMPData;

    Path = Input.substr(0, Input.find_last_of("\\") + 1);
    Filename = Input.substr(Input.find_last_of("\\") + 1, Input.size());

    std::ifstream StreamIn(std::string(Path + Filename).c_str(), std::ios::in | std::ios::binary);

    if (StreamIn.fail()) {
        return false;
    }

    StreamIn.read((char*)&BMPHeader, sizeof(BMPHeader));
    StreamIn.read((char*)&BMPInfo, sizeof(BMPInfo));
    StreamIn.read((char*)&BMPPalette, sizeof(BMPPalette));

    BMPData.resize(BMPInfo.biWidth * BMPInfo.biHeight);
    StreamIn.read((char*)&BMPData[0], sizeof(unsigned char) * BMPData.size());

    StreamIn.close();

    std::ofstream StreamOut(std::string(Path + Output).c_str(), std::ios::out | std::ios::binary);

    if (StreamOut.fail()) {
        return false;
    }

    if (Invert) {

        StreamOut.write((char*)&BMPData[0], sizeof(unsigned char) * BMPData.size());

    } else {
        for (int y = BMPInfo.biHeight; y > -1; y--) {
            for (int x = 0; x < BMPInfo.biWidth; x++) {
                StreamOut.write((char*) &BMPData[x + BMPInfo.biWidth * y], sizeof(char));
            }
        }
    }

    StreamOut.close();

    Output = Output.substr(0, Output.find_last_of(".")) + ".act";
    StreamOut.open(std::string(Path + Output).c_str(), std::ios::out | std::ios::binary);

    if (StreamOut.fail()) {
        return false;
    }

    for (unsigned int i = 0; i < 256; i++) {
        StreamOut.write((char*)&BMPPalette[i].rgbRed, sizeof(BMPPalette[i].rgbRed));
        StreamOut.write((char*)&BMPPalette[i].rgbGreen, sizeof(BMPPalette[i].rgbGreen));
        StreamOut.write((char*)&BMPPalette[i].rgbBlue, sizeof(BMPPalette[i].rgbBlue));
    }

    StreamOut.close();

    return true;
}

bool CTexture::ExportBmp(std::string Input, std::string Output, bool Invert)
{

    BITMAPFILEHEADER BMPHeader;
    BITMAPINFOHEADER BMPInfo;
    RGBQUAD BMPPalette[256];
    std::vector<unsigned char>BMPData;

    //Input.insert(0, "Resources\\Art\\");
    std::ifstream StreamIn(Input.c_str(), std::ios::in | std::ios::binary);

    if (StreamIn.fail()) {
        return false;
    }

    StreamIn.seekg(0, std::ios::end);
    int Size = StreamIn.tellg();
    StreamIn.seekg(0, std::ios::beg);

    BMPData.resize(Size);
    StreamIn.read((char*)&BMPData[0], sizeof(unsigned char) * BMPData.size());

    StreamIn.close();

    Input = Input.substr(0, Input.find_last_of(".")) + ".act";
    StreamIn.open(Input.c_str(), std::ios::in | std::ios::binary);

    if (StreamIn.fail()) {
        return false;
    }

    for (unsigned int i = 0; i < 256; i++) {
        BMPPalette[i].rgbReserved = 0;
        StreamIn.read((char*)&BMPPalette[i].rgbRed, sizeof(BMPPalette[i].rgbRed));
        StreamIn.read((char*)&BMPPalette[i].rgbGreen, sizeof(BMPPalette[i].rgbGreen));
        StreamIn.read((char*)&BMPPalette[i].rgbBlue, sizeof(BMPPalette[i].rgbBlue));
    }

    StreamIn.close();

    //Output.insert(0, "Resources\\Art\\");
    std::ofstream StreamOut(Output.c_str(), std::ios::out | std::ios::binary);

    if (StreamOut.fail()) {
        return false;
    }

    BMPHeader.bfType = 19778;
    BMPHeader.bfSize = 0;
    BMPHeader.bfReserved1 = 0;
    BMPHeader.bfReserved2 = 0;
    BMPHeader.bfOffBits = 1078;

    BMPInfo.biSize = 40;
    BMPInfo.biWidth = sqrt(Size);
    BMPInfo.biHeight = sqrt(Size);
    BMPInfo.biPlanes = 1;
    BMPInfo.biBitCount = 8;
    BMPInfo.biCompression = 0;
    BMPInfo.biSizeImage = 0;
    BMPInfo.biXPelsPerMeter = 0;
    BMPInfo.biYPelsPerMeter = 0;
    BMPInfo.biClrUsed = 0;
    BMPInfo.biClrImportant = 0;

    StreamOut.write((char*)&BMPHeader, sizeof(BMPHeader));
    StreamOut.write((char*)&BMPInfo, sizeof(BMPInfo));
    StreamOut.write((char*)&BMPPalette, sizeof(BMPPalette));

    if (Invert) {

        StreamOut.write((char*)&BMPData[0], sizeof(unsigned char) * BMPData.size());

    } else {
        for (int y = BMPInfo.biHeight; y > -1; y--) {
            for (int x = 0; x < BMPInfo.biWidth; x++) {
                StreamOut.write((char*) &BMPData[x + BMPInfo.biWidth * y], sizeof(char));
            }
        }
    }

    StreamOut.close();


    return true;
}

