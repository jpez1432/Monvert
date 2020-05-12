
#include "Stdafx.h"
#include "CBin.hpp"

CBin::CBin(void)
{

}

CBin::~CBin(void)
{
    Destroy();
}

void CBin::Destroy(void)
{
    Vertices.clear();
    Normals.clear();
    FaceNormals.clear();
    Textures.clear();
    AnimatedTextures.clear();
    Colors.clear();
    Faces.clear();
}

bool CBin::Load(std::string Filename)
{
    int BlockID;
    int Count;
    char Buffer[32];

    glm::i8vec4 Color;
    Face Split, Face;
    Face.Vertices.resize(3);

    Destroy();

    Path = Filename.substr(0, Filename.find_last_of("\\") + 1);
    Filename = Filename.substr(Filename.find_last_of("\\") + 1, Filename.size());

    //Filename.insert(0, "Resources\\Models\\");
    std::ifstream StreamIn(std::string(Path + Filename).c_str(), std::ios::in | std::ios::binary);

    if (StreamIn.fail()) {
        return false;
    }

    StreamIn.read((char*)&Header, sizeof(Header));

    if (Header.ID == 0x20) {

        std::cout << "Animated Bin Model Found, Loading First Frame" << std::endl;

        StreamIn.read((char*)&Buffer, sizeof(Buffer));
        Load(Buffer);

        return true;

    } else if (Header.ID != 0x14) {

       std::cout << "Error Loading Model, Invalid Model ID Number" << std::endl;

    }

    Vertices.resize(Header.NumVertices);
    StreamIn.read((char*)&Vertices[0], sizeof(glm::i32vec3) * Header.NumVertices);

    StreamIn.read((char*)&BlockID, sizeof(BlockID));

    while (BlockID != 0 && !StreamIn.eof()) {

        switch (BlockID) {

        case 0x03:

            StreamIn.seekg(sizeof(glm::i32vec3) * Header.NumVertices + 8, std::ios::cur);

            break;

        case 0x17:

            StreamIn.seekg(8, std::ios::cur);
            break;

        case 0x0D:

            StreamIn.seekg(4, std::ios::cur);
            StreamIn.read((char*)&Buffer[0], sizeof(char) * 16);

            Textures.push_back(std::string(Buffer));
            transform(Textures[Textures.size() - 1].begin(), Textures[Textures.size() - 1].end(), Textures[Textures.size() - 1].begin(), ::toupper);

            break;

        case 0x1D:

            StreamIn.seekg(4, std::ios::cur);
            StreamIn.read((char*)&Count, sizeof(int));
            StreamIn.seekg(4 * sizeof(int), std::ios::cur);
            StreamIn.read((char*)&Buffer[0], sizeof(char) * 32);
            StreamIn.seekg(32 * (Count - 1), std::ios::cur);

            Textures.push_back(std::string(Buffer));
            transform(Textures[Textures.size() - 1].begin(), Textures[Textures.size() - 1].end(), Textures[Textures.size() - 1].begin(), ::toupper);

            break;

        case 0x0A:

            StreamIn.read((char*)&Color, sizeof(glm::i8vec4));

            break;

        case 0x19:

            Split.Type = BlockID;
            Split.TextureIndex = Textures.size() - 1;

            StreamIn.read((char*)&Count, sizeof(Count));
            StreamIn.seekg(4 * sizeof(int), std::ios::cur);

            Split.Vertices.resize(Count);

            for (int i = 0; i < Count; i++) {
                StreamIn.read((char*)&Split.Vertices[i].Index, sizeof(Split.Vertices[i].Index));
            }

            if (Count == 3) {

                Faces.push_back(Split);

            } else {

                Face.Type = BlockID;
                Face.TextureIndex = Textures.size() - 1;

                Face.Vertices[0].Index = Split.Vertices[0].Index;
                Face.Vertices[1].Index = Split.Vertices[1].Index;
                Face.Vertices[2].Index = Split.Vertices[2].Index;
                Faces.push_back(Face);

                Face.Vertices[0].Index = Split.Vertices[2].Index;
                Face.Vertices[1].Index = Split.Vertices[3].Index;
                Face.Vertices[2].Index = Split.Vertices[0].Index;
                Faces.push_back(Face);

            }

            break;

        case 0x0E:
        case 0x11:
        case 0x18:
        case 0x29:
        case 0x33:
        case 0x34:

            Split.Type = BlockID;
            Split.TextureIndex = Textures.size() - 1;

            StreamIn.read((char*)&Count, sizeof(Count));
            StreamIn.seekg(4 * sizeof(int), std::ios::cur);

            Split.Vertices.resize(Count);

            for (int i = 0; i < Count; i++) {
                StreamIn.read((char*)&Split.Vertices[i].Index, sizeof(Split.Vertices[i].Index));
                StreamIn.read((char*)&Split.Vertices[i].TexCoord, sizeof(Split.Vertices[i].TexCoord));
            }

            if (Count == 3) {

                Faces.push_back(Split);

            } else {

                Face.Type = Split.Type;
                Face.TextureIndex = Textures.size() - 1;

                Face.Vertices[0] = Split.Vertices[0];
                Face.Vertices[1] = Split.Vertices[1];
                Face.Vertices[2] = Split.Vertices[2];
                Faces.push_back(Face);

                Face.Vertices[0] = Split.Vertices[2];
                Face.Vertices[1] = Split.Vertices[3];
                Face.Vertices[2] = Split.Vertices[0];
                Faces.push_back(Face);

            }

            break;

        default:            transform(Textures[Textures.size() - 1].begin(), Textures[Textures.size() - 1].end(), Textures[Textures.size() - 1].begin(), ::toupper);

            char Type[8];
            std::cout << "Unknown Model Face Type - " + std::string(itoa(BlockID, Type, 8)) + ", Skipping..." << std::endl;

            break;

        }

        StreamIn.read((char*)&BlockID, sizeof(BlockID));
    }

    StreamIn.close();

//    Normals.resize(Vertices.size());
    CalculateNormals();

    return true;
}

bool CBin::Save(std::string Filename)
{

    int BlockID;

    std::ofstream StreamOut(Filename.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);

    if (StreamOut.fail()) {
        return false;
    }

    StreamOut.write((char*)&Header, sizeof(Header));

    StreamOut.write((char*)&Vertices[0], sizeof(glm::i32vec3) * Header.NumVertices);

    if (Normals.size() != 0) {
        BlockID = 3;
        StreamOut.write((char*)&BlockID, sizeof(BlockID));
        StreamOut.write((char*)&Normals[0], sizeof(glm::i32vec3) * Header.NumVertices);
        StreamOut.seekp(8, std::ios::cur);
    }

    for (unsigned int i = 0; i < Textures.size(); i++) {
        BlockID = 13;
        StreamOut.write((char*)&BlockID, sizeof(BlockID));
        StreamOut.seekp(4, std::ios::cur);
        StreamOut.write((char*)&Textures[i].c_str()[0], sizeof(char) * 16);

        for (unsigned int j = 0; j < Faces.size(); j++) {

            if (Faces[j].TextureIndex == int(i)) {

                StreamOut.write((char*)&Faces[i].Type, sizeof(int));

                BlockID = 3;
                StreamOut.write((char*)&BlockID, sizeof(BlockID));
                StreamOut.seekp(16, std::ios::cur);

                for (unsigned int k = 0; k < 3; k++) {

                    StreamOut.write((char*)&Faces[j].Vertices[k].Index, sizeof(int));
                    if (Faces[i].Type != 25) {
                        StreamOut.write((char*)&Faces[j].Vertices[k].TexCoord, sizeof(glm::i32vec2));
                    }

                }
            }
        }

    }


    BlockID = 0;
    StreamOut.write((char*)&BlockID, sizeof(BlockID));

    StreamOut.close();

    return true;
}

bool CBin::Export(std::string Filename, bool Invert, bool Scale)
{
    glm::vec3 Scaling(1);

    Path = Filename.substr(0, Filename.find_last_of("\\") + 1);
    Filename = Filename.substr(Filename.find_last_of("\\") + 1, Filename.size());
    std::string Material = Filename.substr(0, Filename.find_last_of(".")) + ".mtl";

    //Filename.insert(0, "Resources\\Models\\");
    std::ofstream StreamOut(std::string(Path + Filename).c_str(), std::ios::out | std::ios::trunc);

    if (StreamOut.fail()) {
        return false;
    }

    if (Scale) {
        Scaling = glm::vec3(256, 256, -256);
    }


    StreamOut << "mtllib " << Material << std::endl;

    for (unsigned int i = 0; i < Vertices.size(); i++) {
        StreamOut << "v " << Vertices[i].x / Scaling.x << " " << Vertices[i].y / Scaling.y  << " " << Vertices[i].z / Scaling.z  << std::endl;
    }

    for (unsigned int i = 0; i < Normals.size(); i++) {
        StreamOut << "vn " << Normals[i].x << " " << Normals[i].y << " " << -Normals[i].z << std::endl;
    }

    for (unsigned int i = 0; i < Faces.size(); i++) {
        StreamOut << "vt " << Faces[i].Vertices[0].TexCoord.x / float(65536.0f * 256.0f) << " " << 1.0f - Faces[i].Vertices[0].TexCoord.y / float(65536.0f * 256.0f) << std::endl;
        StreamOut << "vt " << Faces[i].Vertices[1].TexCoord.x / float(65536.0f * 256.0f) << " " << 1.0f - Faces[i].Vertices[1].TexCoord.y / float(65536.0f * 256.0f) << std::endl;
        StreamOut << "vt " << Faces[i].Vertices[2].TexCoord.x / float(65536.0f * 256.0f) << " " << 1.0f - Faces[i].Vertices[2].TexCoord.y / float(65536.0f * 256.0f) << std::endl;
    }

    int Index = 1;
    glm::uvec3 Offset(0);
    for (unsigned int i = 0; i < Textures.size(); i++) {

        StreamOut << "g " << Textures[i].substr(0, Textures[i].find_last_of(".")) << std::endl;
        StreamOut << "usemtl " << Textures[i].substr(0, Textures[i].find_last_of(".")) << std::endl;

        for (unsigned int j = 0; j < Faces.size(); j++) {

            if (Faces[j].TextureIndex == int(i)) {
                Offset[0] = Index; Index++;
                Offset[1] = Index; Index++;
                Offset[2] = Index; Index++;
                StreamOut << "f " << Faces[j].Vertices[2].Index + 1 << "/" << Offset[2] << "/" << Faces[j].Vertices[2].Index + 1 << " " <<
                                     Faces[j].Vertices[1].Index + 1 << "/" << Offset[1] << "/" << Faces[j].Vertices[1].Index + 1 << " " <<
                                     Faces[j].Vertices[0].Index + 1 << "/" << Offset[0] << "/" << Faces[j].Vertices[0].Index + 1 << std::endl;
            }
        }
    }

    StreamOut.close();

    StreamOut.open(std::string(Path + Material).c_str(), std::ios::out | std::ios::trunc);

    if (StreamOut.fail()) {
        return false;
    }

    for (unsigned int i = 0; i < Textures.size(); i++) {
        transform(Textures[i].begin(), Textures[i].end(), Textures[i].begin(), ::toupper);
    }

    std::sort(Textures.begin(), Textures.end());
    Textures.erase(std::unique(Textures.begin(), Textures.end()), Textures.end());

    for (unsigned int i = 0; i < Textures.size(); i++) {
        StreamOut << std::endl;
        StreamOut << "newmtl " << Textures[i].substr(0, Textures[i].find_last_of(".")) << std::endl;
        StreamOut << "illum 2" << std::endl;
        StreamOut << "Kd 0.800000 0.800000 0.800000" << std::endl;
        StreamOut << "Ka 0.200000 0.200000 0.200000" << std::endl;
        StreamOut << "Ks 0.000000 0.000000 0.000000" << std::endl;
        StreamOut << "Ke 0.000000 0.000000 0.000000" << std::endl;
        StreamOut << "Ns 0.000000" << std::endl;
        StreamOut << "map_Kd " << "..\\Art\\" << Textures[i].substr(0, Textures[i].find_last_of(".")) << ".bmp" << std::endl;

        Texture.ExportBmp(std::string(Path + "..\\Art\\" + Textures[i]).c_str(), std::string(Path + "..\\Art\\" + Textures[i].substr(0, Textures[i].find_last_of(".")) + ".bmp").c_str(), Invert);
    }

    StreamOut.close();

    return true;
}

void CBin::CalculateNormals(void)
{

    glm::vec3 N, A, B;
    std::vector<int> SharedFaces;

    Normals.resize(Vertices.size());
    SharedFaces.resize(Vertices.size());

    for (unsigned int f = 0; f < Faces.size(); f++) {

        A = Vertices[Faces[f].Vertices[2].Index] - Vertices[Faces[f].Vertices[0].Index];
        B = Vertices[Faces[f].Vertices[1].Index] - Vertices[Faces[f].Vertices[0].Index];

        N = glm::cross(B, A);
        N = glm::normalize(N);

        FaceNormals.push_back(N);

        Normals[Faces[f].Vertices[0].Index] += N;
        Normals[Faces[f].Vertices[1].Index] += N;
        Normals[Faces[f].Vertices[2].Index] += N;

        SharedFaces[Faces[f].Vertices[0].Index]++;
        SharedFaces[Faces[f].Vertices[1].Index]++;
        SharedFaces[Faces[f].Vertices[2].Index]++;
    }

    for (unsigned int i = 0; i < Vertices.size(); i++) {
        Normals[i] /= SharedFaces[i];
        Normals[i] = glm::normalize(Normals[i]);
    }

}
