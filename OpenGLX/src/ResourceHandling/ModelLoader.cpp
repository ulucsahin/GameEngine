#define _CRT_SECURE_NO_WARNINGS

#include "ModelLoader.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>


bool ModelLoader::LoadObj(const char* filepath, 
    std::vector<glm::vec3>& out_vertices, 
    std::vector<glm::vec2>& out_uvs,
    std::vector<glm::vec3>& out_normals,
    std::vector<unsigned int>& out_indices)
{
    //
    std::vector<unsigned int> uvIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    FILE* file;

    errno_t err = fopen_s(&file, filepath, "r");
    if (file == NULL) 
    {
        printf("Impossible to open the file !\n");
        return false;
    }

    while (1)
    {
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf_s(file, "%s", lineHeader, _countof(lineHeader));     

        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        if (strcmp(lineHeader, "v") == 0) 
        {
            glm::vec3 vertex;
            fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z, _countof(lineHeader));
            temp_vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vt") == 0) 
        {
            glm::vec2 uv;
            fscanf_s(file, "%f %f\n", &uv.x, &uv.y, _countof(lineHeader));
            temp_uvs.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0) 
        {
            glm::vec3 normal;
            fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z, _countof(lineHeader));
            temp_normals.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0) 
        {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
                &vertexIndex[0], &uvIndex[0], &normalIndex[0], 
                &vertexIndex[1], &uvIndex[1], &normalIndex[1], 
                &vertexIndex[2], &uvIndex[2], &normalIndex[2], 
                _countof(lineHeader));
            if (matches != 9) {
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                return false;
            }
            out_indices.push_back(vertexIndex[0]);
            out_indices.push_back(vertexIndex[1]);
            out_indices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    }

    for (unsigned int i = 0; i < out_indices.size(); i++)
    {
        unsigned int vertexIndex = out_indices[i];
        glm::vec3 vertex = temp_vertices[vertexIndex - 1];
        out_vertices.push_back(vertex);
    }

    //for (unsigned int i = 0; i < uvIndices.size(); i++)
    //{
    //    unsigned int uvIndex = uvIndices[i];
    //    glm::vec2 vertex = temp_vertices[uvIndex - 1];
    //    out_uvs.push_back(vertex);
    //}

    return true;
}

bool ModelLoader::LoadObj2(const char* filepath,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::vec2>& out_texture_coords,
    std::vector<unsigned int>& out_faces)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texture;
    std::vector<int> faceIndex;
    std::vector<int> textureIndex;

    std::ifstream in(filepath, std::ios::in);
    if (!in)
    {
        std::cerr << "Cannot open " << filepath << std::endl;
        exit(1);
    }
    
    std::string line;
    while (std::getline(in, line))
    {
        if (line.substr(0, 2) == "v ") 
        {
            std::istringstream v(line.substr(2));
            glm::vec3 vert;
            double x, y, z;
            v >> x;
            v >> y;
            v >> z;
            vert = glm::vec3(x, y, z);
            vertices.push_back(vert);
        }
        //check for texture co-ordinate
        else if (line.substr(0, 2) == "vt") 
        {

            std::istringstream v(line.substr(3));
            glm::vec2 tex;
            int U, V;
            v >> U; v >> V;
            tex = glm::vec2(U, V);
            texture.push_back(tex);
        }
        //check for faces
        else if (line.substr(0, 2) == "f ")
        {
            int a, b, c; //to store mesh index
            int A, B, C; //to store texture index

            const char* chh = line.c_str();
            sscanf(chh, "f %i/%i %i/%i %i/%i", &a, &A, &b, &B, &c, &C); //here it read the line start with f and store the corresponding values in the variables
            a--; b--; c--;
            A--; B--; C--;
            out_faces.push_back(a); textureIndex.push_back(A);
            out_faces.push_back(b); textureIndex.push_back(B);
            out_faces.push_back(c); textureIndex.push_back(C);
        }
    }

    //the mesh data is finally calculated here
    for (unsigned int i = 0; i < faceIndex.size(); i++)
    {
        glm::vec3 meshData;
        glm::vec2 texData;
        meshData = glm::vec3(vertices[faceIndex[i]].x, vertices[faceIndex[i]].y, vertices[faceIndex[i]].z);
        texData = glm::vec2(texture[textureIndex[i]].x, texture[textureIndex[i]].y);
        out_vertices.push_back(meshData);
        out_texture_coords.push_back(texData);
    }

    //FILE* file;

    //errno_t err = fopen_s(&file, filepath, "r");
    //if (file == NULL)
    //{
    //    printf("Impossible to open the file !\n");
    //    return false;
    //}

    //// loop over lines
    //int debugCount = 0;
    //while (true)
    //{
    //    char lineHeader[512];
    //    // read the first word of the line
    //    int res = fscanf_s(file, "%s", lineHeader, _countof(lineHeader));

    //    if (res == EOF)
    //        break; // EOF = End Of File. Quit the loop.

    //    //std::cout << lineHeader << "========" << std::endl;

    //    // vertice
    //    if (strcmp(lineHeader, "v"))
    //    {   
    //        std::cout << "lineHeader:" << lineHeader << std::endl;
    //        std::cout << "vertice" << std::endl;
    //    }



    //    debugCount++;
    //    if (debugCount == 100)
    //        exit(111);
    //}

    return true;
}
