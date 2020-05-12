
#ifndef CTEXTURE_HPP
#define CTEXTURE_HPP

#include "Common.hpp"

class CTexture
{

public:

    GLuint TextureID;
    GLint Format, Channels;
    GLint Width, Height;
    GLubyte *Data;

    std::string Path, Filename;

public:

    CTexture(void);
    ~CTexture(void);

    bool Save(std::string Filename);
    bool Load(std::string Filename);

    bool ExportRaw(std::string Input, std::string Output, bool Invert);
    bool ExportBmp(std::string Input, std::string Output, bool Invert);

};

#endif
