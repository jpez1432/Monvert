
#include "Stdafx.h"
#include "CObj.hpp"

CObj::CObj(void)
{

}

CObj::~CObj(void)
{
    Destroy();
}

void CObj::Destroy(void)
{
    Vertices.clear();
    TexCoords.clear();
    Normals.clear();
    Faces.clear();
    Groups.clear();
    Materials.clear();
}

bool CObj::Load(std::string Filename)
{
    std::string Line;
    glm::vec3 Vertex;
    glm::vec2 TexCoord;
    glm::vec3 Normal;
    Face Face;
    Group Group;
    Material Material;

    Destroy();

    Path = Filename.substr(0, Filename.find_last_of("\\") + 1);
    Filename = Filename.substr(Filename.find_last_of("\\") + 1, Filename.size());

    //Filename.insert(0, "Resources\\Models\\");
    std::ifstream StreamIn(std::string(Path + Filename).c_str(), std::ios::in);

    if (StreamIn.fail()) {
        return false;
    }

    while (!StreamIn.eof()  && !StreamIn.fail()) {

        StreamIn >> Line;

        if (Line == "mtllib") {

            StreamIn >> Filename;

            //MaterialFilename.insert(0, "Resources\\Models\\");
            std::ifstream StreamInMat(std::string(Path + Filename).c_str(), std::ios::in);

            if (StreamInMat.fail()) {
                return false;
            }

            while (!StreamInMat.eof()) {

                StreamInMat >> Line;

                if (Line == "newmtl") {

                    StreamInMat >> Material.Name;

                } else if (Line == "illum") {

                    StreamInMat >> Material.Illumination;

                } else if (Line == "Kd") {

                    StreamInMat >> Material.Diffuse.x >> Material.Diffuse.y >> Material.Diffuse.z;

                } else if (Line == "Ka") {

                   StreamInMat >> Material.Ambient.x >> Material.Ambient.y >> Material.Ambient.z;

                } else if (Line == "Ks") {

                    StreamInMat >> Material.Specular.x >> Material.Specular.y >> Material.Specular.z;

                } else if (Line == "Ke") {

                    StreamInMat >> Material.Emissive.x >> Material.Emissive.y >> Material.Emissive.z;

                } else if (Line == "Ns") {

                    StreamInMat >> Material.Shininess;

                } else if (Line == "map_Kd") {

                    StreamInMat >> Material.DiffuseMap;
                    Materials.push_back(Material);

                }

            }

            StreamInMat.close();

        } if (Line == "usemtl") {

            StreamIn >> Line;

            for (unsigned int i = 0; i < Materials.size(); i++) {
                if (Materials[i].Name == Line) {
                    Groups[Groups.size() - 1].Material = i;
                    continue;
                }
            }

        } if (Line == "v") {

            StreamIn >> Vertex.x >> Vertex.y >> Vertex.z;
            Vertices.push_back(Vertex);

        } else if (Line == "vn") {

//            StreamIn >> Normal.x >> Normal.y >> Normal.z;
//            Normals.push_back(Normal);

        } else if (Line == "vt") {

            StreamIn >> TexCoord.x >> TexCoord.y;
            TexCoords.push_back(TexCoord);

        } else if (Line == "g") {

            StreamIn >> Group.Name;
            Groups.push_back(Group);

        } else if (Line == "f") {

            for (unsigned int i = 0; i < 3; i++) {

                StreamIn >> Line;
                Face.Vertex[i] = atoi(Line.substr(0, Line.find_first_of("/")).c_str()) - 1;
                Face.TexCoord[i] = atoi(Line.substr(Line.find_first_of("/") + 1, Line.find_last_of("/") - 1 - Line.find_first_of("/") + 1).c_str()) - 1;
                Face.Normal[i] = atoi(Line.substr(Line.find_last_of("/") + 1, Line.length()).c_str()) - 1;

            }
            Face.GroupIndex = Groups.size() - 1;
            Faces.push_back(Face);

        }

    }

    StreamIn.close();

    CalculateNormals();

    return true;

}

bool CObj::Save(std::string Filename)
{
    Path = Filename.substr(0, Filename.find_last_of("\\") + 1);
    Filename = Filename.substr(Filename.find_last_of("\\") + 1, Filename.size());

    std::ofstream StreamOut(std::string(Path + Filename).c_str(), std::ios::out);

    if (StreamOut.fail()) {
        return false;
    }

    StreamOut << "mtllib " << Filename.substr(0, Filename.find_last_of("\\")) + ".mtl" << std::endl;

    for (unsigned int i = 0; i < Vertices.size(); i++) {
        StreamOut << "v " << Vertices[i].x << " " << Vertices[i].y << " " << Vertices[i].z << std::endl;
    }

    for (unsigned int i = 0; i < Normals.size(); i++) {
        StreamOut << "vn " << Normals[i].x << " " << Normals[i].y << " " << Normals[i].z << std::endl;
    }

    for (unsigned int i = 0; i < TexCoords.size(); i++) {
        StreamOut << "vt " << TexCoords[i].x << " " << TexCoords[i].y << std::endl;
    }

    for (unsigned int i = 0; i < Groups.size(); i++) {


        StreamOut << "g " << Groups[i].Name << std::endl;
        StreamOut << "usemtl " << Groups[i].Material << std::endl;

        for (unsigned int j = 0; j < Faces.size(); j++) {

            if (Faces[j].GroupIndex == i) {

                  StreamOut << "f " << Faces[j].Vertex[0] + 1 << "/" << Faces[j].TexCoord[0] + 1 << "/" << Faces[j].Normal[0] + 1 << " "
                                    << Faces[j].Vertex[1] + 1 << "/" << Faces[j].TexCoord[1] + 1 << "/" << Faces[j].Normal[1] + 1 << " "
                                    << Faces[j].Vertex[2] + 1 << "/" << Faces[j].TexCoord[2] + 1 << "/" << Faces[j].Normal[2] + 1 << std::endl;

            }
        }
    }

    StreamOut.close();

    StreamOut.open(std::string(Filename.substr(0, Filename.find_last_of("\\")) + ".mtl").c_str(), std::ios::out | std::ios::trunc);

    if (StreamOut.fail()) {
        return false;
    }

    for (unsigned int i = 0; i < Materials.size(); i++) {
        StreamOut << "newmtl " << Materials[i].Name << std::endl;
        StreamOut << "illum " << Materials[i].Illumination << std::endl;
        StreamOut << "Kd " << Materials[i].Diffuse.x << " " << Materials[i].Diffuse.y << " " << Materials[i].Diffuse.z << std::endl;
        StreamOut << "Ka " << Materials[i].Ambient.x << " " << Materials[i].Ambient.y << " " << Materials[i].Ambient.z << std::endl;
        StreamOut << "Ks " << Materials[i].Specular.x << " " << Materials[i].Specular.y << " " << Materials[i].Specular.z << std::endl;
        StreamOut << "Ke " << Materials[i].Emissive.x << " " << Materials[i].Emissive.y << " " << Materials[i].Emissive.z << std::endl;
        StreamOut << "Ks " << Materials[i].Shininess << std::endl;
        StreamOut << "map_Kd " << Materials[i].DiffuseMap << std::endl;
    }

    StreamOut.close();

    return true;
}


bool CObj::Export(std::string Filename, bool Invert, bool Scale, int FaceType)
{
    int Type = 0;
    int BlockID;
    glm::vec3 Scaling(1);

    Path = Filename.substr(0, Filename.find_last_of("\\") + 1);
    Filename = Filename.substr(Filename.find_last_of("\\") + 1, Filename.size());

    //Filename.insert(0, "Resources\\Models\\");
    std::ofstream StreamOut(std::string(Path + Filename).c_str(), std::ios::out | std::ios::binary | std::ios::trunc);

    if (StreamOut.fail()) {
        return false;
    }

    switch (FaceType) {
    case 0:
        Type = 25;
        break;
    case 1:
        Type = 24;
        break;
    case 2:
        Type = 41;
        break;
    case 3:
        Type = 17;
        break;
    case 4:
        Type = 51;
        break;
    }

    BlockID = 20;
    StreamOut.write((char*)&BlockID, sizeof(BlockID));

    BlockID = 65536;
    StreamOut.write((char*)&BlockID, sizeof(BlockID));
    StreamOut.seekp(8, std::ios::cur);
    BlockID = Vertices.size();
    StreamOut.write((char*)&BlockID, sizeof(BlockID));

    if (Scale) {
        Scaling = glm::vec3(256, 256, -256);
    }

    for (unsigned int i = 0; i < Vertices.size(); i++) {
        BlockID = Vertices[i][0] * Scaling.x;
        StreamOut.write((char *)&BlockID, sizeof(BlockID));
        BlockID = Vertices[i][1] * Scaling.y;
        StreamOut.write((char *)&BlockID, sizeof(BlockID));
        BlockID = Vertices[i][2] * Scaling.z;
        StreamOut.write((char *)&BlockID, sizeof(BlockID));
    }

//    BlockID = 3;
//    StreamOut.write((char*)&BlockID, sizeof(BlockID));
//    for (unsigned int i = 0; i < Normals.size(); i++) {
//        for (unsigned int j = 0; j < 3; j++) {
//            BlockID = Normals[i][j] * 256;
//            StreamOut.write((char *)&BlockID, sizeof(BlockID));
//        }
//    }
//    StreamOut.seekp(8, std::ios::cur);

    for (unsigned int i = 0; i < Groups.size(); i++) {

        BlockID = 13;
        StreamOut.write((char*)&BlockID, sizeof(BlockID));
        StreamOut.seekp(4, std::ios::cur);

        Filename = Materials[Groups[i].Material].DiffuseMap.substr(Materials[Groups[i].Material].DiffuseMap.find_last_of("\\") + 1, Materials[Groups[i].Material].DiffuseMap.find_last_of("."));
        Texture.ExportRaw(std::string(Path + Materials[Groups[i].Material].DiffuseMap).c_str(), Filename.substr(0, Filename.find_last_of(".")) + ".raw", Invert);
        Materials[Groups[i].Material].DiffuseMap = Filename.substr(0, Filename.find_last_of(".")) + ".raw";

        StreamOut.write((char*)&Materials[Groups[i].Material].DiffuseMap.c_str()[0], sizeof(char) * 16);

        for (unsigned int j = 0; j < Faces.size(); j++) {
            if (Faces[j].GroupIndex == i) {


                BlockID = Type;
                StreamOut.write((char*)&BlockID, sizeof(BlockID));

                BlockID = 3;
                StreamOut.write((char*)&BlockID, sizeof(BlockID));
                StreamOut.seekp(16, std::ios::cur);

                for (int k = 2; k > -1; k--) {
                    StreamOut.write((char*)&Faces[j].Vertex[k], sizeof(Faces[j].Vertex[k]));

                    if (Type != 25) {
                        BlockID = TexCoords[Faces[j].TexCoord[k]].x * (65536 * 256);
                        StreamOut.write((char*)&BlockID, sizeof(BlockID));
                        BlockID = (TexCoords[Faces[j].TexCoord[k]].y) * (65536 * 256);
                        StreamOut.write((char*)&BlockID, sizeof(BlockID));
                    }
//                    StreamOut.seekp(8, std::ios::cur);
                }
            }
        }
    }
    BlockID = 0;
    StreamOut.write((char*)&BlockID, sizeof(BlockID));

    StreamOut.close();

    return true;
}

void CObj::CalculateNormals(void)
{

    glm::vec3 N, A, B;
    std::vector<int> SharedFaces;

    FaceNormals.resize(Faces.size());
    Normals.resize(Vertices.size());
    SharedFaces.resize(Vertices.size());

    for (unsigned int f = 0; f < Faces.size(); f++) {

        A = Vertices[Faces[f].Vertex[2]] - Vertices[Faces[f].Vertex[0]];
        B = Vertices[Faces[f].Vertex[1]] - Vertices[Faces[f].Vertex[0]];

        N = glm::cross(B, A);
        N = glm::normalize(N);

        FaceNormals[f] = N;

        Normals[Faces[f].Vertex[0]] += N;
        Normals[Faces[f].Vertex[1]] += N;
        Normals[Faces[f].Vertex[2]] += N;

        SharedFaces[Faces[f].Vertex[0]]++;
        SharedFaces[Faces[f].Vertex[1]]++;
        SharedFaces[Faces[f].Vertex[2]]++;
    }

    for (unsigned int i = 0; i < Vertices.size(); i++) {
        Normals[i] /= SharedFaces[i];
        Normals[i] = glm::normalize(Normals[i]);
    }

}
