#ifndef MESH_H
#define MESH_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shader.h"
#include <QtOpenGL/QGLWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>


struct Vertex
{
    glm::vec3 position;  //v
    glm::vec2 texCoords;  //vt
    glm::vec3 normal;   //vn
};

struct Texture
{
    GLuint id;
    aiTextureType type;
    std::string path;
};


class Mesh:protected QOpenGLFunctions_3_3_Core
{
public:
    void draw(Shader& shader)//绘制Mesh
    {
        if(VAOId == 0 || VBOId ==0 || EBOId == 0)
        {
            return;
        }
        shader.use(); // ?  //glUseShader(ID);
        glBindVertexArray(this->VAOId);
        int diffuseCnt = 0 , specularCnt = 0 , texUnitCnt = 0;
        for(std::vector<Texture>::const_iterator it = this->textures.begin();this->textures.end()!=it;it++)
        {
            switch(it->type)
            {
                case aiTextureType_DIFFUSE:  //漫反射
                {
                    glActiveTexture(GL_TEXTURE0 + texUnitCnt);
                    glBindTexture(GL_TEXTURE_2D, it->id);  //texture的ID
                    std::stringstream samplerNameStr;
                    samplerNameStr << "texture_diffuse" << diffuseCnt++ ;
                    glUniform1i(glGetUniformLocation(shader.programId,samplerNameStr.str().c_str()), texUnitCnt++);
                }
                break;
                case aiTextureType_SPECULAR:
                {
                     glActiveTexture(GL_TEXTURE0 + texUnitCnt);
                     glBindTexture(GL_TEXTURE_2D, it->id);
                     std::stringstream samplerNameStr;
                     samplerNameStr << "texture_specular" << specularCnt++;
                     glUniform1i(glGetUniformLocation(shader.programId,samplerNameStr.str().c_str()), texUnitCnt++);
                }
                break;
                default:
                    std::cerr << "Warning::Mesh::draw, texture type" << it->type << " current not supported." <<std::endl;
                break;
            }
        }

        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT , 0);
        glBindVertexArray(0);
        glUseProgram(0);
    }
    Mesh():VAOId(0) ,VBOId(0) ,EBOId(0){}

    Mesh(std::vector<Vertex>& vertData,
           std::vector<Texture> & textures,
               std::vector<GLuint>& indices):VAOId(0), VBOId(0), EBOId(0)   //const
    {
        setData(vertData, textures, indices);
    }


    void setData(std::vector<Vertex>& vertData,
        std::vector<Texture> & textures,
        std::vector<GLuint>& indices)   //const
    {
        this->vertData = vertData;
        this->indices = indices;
        this->textures = textures;
        if (!vertData.empty() && !indices.empty())
        {
            this->setupMesh();
        }
    }

    void final()
    {
        glDeleteVertexArrays(1, &this->VAOId);
        glDeleteBuffers(1, &this->VBOId);
        glDeleteBuffers(1, &this->EBOId);
    }

    ~Mesh()
    {

    }
private:
    std::vector<Vertex> vertData;//顶点坐标
    std::vector<GLuint> indices; //索引坐标
    std::vector<Texture> textures; //纹理
    GLuint VAOId, VBOId, EBOId;

    void setupMesh()
    {
         glGenVertexArrays(1, &this->VAOId);
         glGenBuffers(1, &this->VBOId);
         glGenBuffers(1, &this->EBOId);

         glBindVertexArray(this->VAOId);
         glBindBuffer(GL_ARRAY_BUFFER, this->VBOId);
         glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * this->vertData.size(),
             &this->vertData[0], GL_STATIC_DRAW);
         // 顶点位置属性
         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
             sizeof(Vertex), (GLvoid*)0);
         glEnableVertexAttribArray(0);
         // 顶点纹理坐标
         glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
             sizeof(Vertex), (GLvoid*)(3 * sizeof(GL_FLOAT)));
         glEnableVertexAttribArray(1);
         // 顶点法向量属性
         glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
             sizeof(Vertex), (GLvoid*)(5 * sizeof(GL_FLOAT)));
         glEnableVertexAttribArray(2);
         // 索引数据
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBOId);
         glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)* this->indices.size(),
             &this->indices[0], GL_STATIC_DRAW);
         glBindBuffer(GL_ARRAY_BUFFER, 0);
         glBindVertexArray(0);
    }
};

#endif // MESH_H
