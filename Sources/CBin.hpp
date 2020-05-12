
#ifndef CBIN_HPP
#define CBIN_HPP

#include "Common.hpp"
#include "CTexture.hpp"

class CBin
{

public:

    struct {
        int ID;
        int Scale;
        int Unknown1;
        int Unknown2;
        int NumVertices;
    } Header;

    struct AnimatedTexture {
        int NumTextures;
        int Delay;
        std::vector<std::string>Filenames;
    };

    struct VertexInfo {
        int Index;
        glm::i32vec2 TexCoord;
    };

    struct Face {
        int Type;
        int TextureIndex;
        std::vector<VertexInfo>Vertices;
    };


    std::string Path, Filename;

    std::vector<glm::i32vec3> Vertices;
    std::vector<glm::vec3> Normals;
    std::vector<glm::vec3> FaceNormals;
    std::vector<std::string> Textures;
    std::vector<AnimatedTexture> AnimatedTextures;
    std::vector<glm::i8vec4> Colors;
    std::vector<Face> Faces;

    CTexture Texture;

    void CalculateNormals(void);

public:

    CBin(void);
    ~CBin(void);

    bool Load(std::string Filename);
    bool Save(std::string Filename);
    bool Export(std::string Filename, bool Invert, bool Scale);
    void Destroy(void);

};

#endif
