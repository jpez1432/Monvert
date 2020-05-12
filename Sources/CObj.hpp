
#ifndef COBJ_HPP
#define COBJ_HPP

#include "Common.hpp"
#include "CTexture.hpp"

class CObj
{

public:

    struct Material {
        std::string Name;
        unsigned int Illumination;
        glm::vec3 Diffuse;
        glm::vec3 Ambient;
        glm::vec3 Specular;
        glm::vec3 Emissive;
        float Shininess;
        std::string DiffuseMap;
    };

    struct Face {
        unsigned int GroupIndex;
        glm::uvec3 Vertex;
        glm::uvec3 TexCoord;
        glm::uvec3 Normal;
    };

    struct Group {
        std::string Name;
        unsigned int Material;
    };

    std::string Path, Filename;

    std::vector<glm::vec3> Vertices;
    std::vector<glm::vec2> TexCoords;
    std::vector<glm::vec3> Normals;
    std::vector<glm::vec3> FaceNormals;
    std::vector<Face> Faces;
    std::vector<Group> Groups;
    std::vector<Material> Materials;

    CTexture Texture;

    void CalculateNormals(void);

public:

    CObj(void);
    ~CObj(void);

    bool Load(std::string Filename);
    bool Save(std::string Filename);
    bool Export(std::string Filename, bool Invert, bool Scale, int FaceType);
    void Destroy(void);

};

#endif
