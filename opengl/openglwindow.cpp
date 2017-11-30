


#include <QtGui>
#include <QtWidgets>
#include <iostream>
#include <QMouseEvent>
#include "openglwindow.h"
#include "mainwindow.h"
#include "ObjLoader.h"
#include <QOpenGLFunctions_1_0>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <time.h>
#include <objmode.h>
#include <GLFW/glfw3.h>


using namespace std;
double times = 0;
int xx = 0;
double yy = 0;
glm::vec3 cameraPos    = glm::vec3(0.0f,0.0f,150.0f);
glm::vec3 cameraPos_1    = glm::vec3(0.0f,0.0f,5.0f);

glm::vec3 worldCentrol = glm::vec3(0.0f,0.0f,0.0f);
glm::vec3 cameraUp     = glm::vec3(0.0f,1.0f,0.0f);
const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
//构建ModelMatrix,进行”局部空间“到”世界空间“的转换
glm::vec3 transVec_1 = glm::vec3(0.0f,-0.4f,0.0f);
glm::vec3 transVec = glm::vec3(0.0f,-10.0f,0.0f);
glm::vec3 lightPos = glm::vec3(0.0f,0.0f,1000.0f);
glm::vec3 lightDirection = glm::vec3(0.0f,0.0f,150.f);

GLfloat yaw   = 0.0f;  //偏航角
GLfloat pitch = 0.0f;  //俯仰角
GLfloat lastX = 0;
GLfloat lastY = 0;

ObjMode mo;
QVector<VertexData> nVertextData;
QVector<unsigned int> index;
QVector<VertexData> nVertextData1;
QVector<unsigned int> index1;
const GLuint NumVertexShader = 1; //着色器数目
const GLuint NumFragmentShader = 1;
const GLuint NumShaderProgram = 1;
const GLuint NumVAO = 1;
const GLuint NumVBO = 1;
const GLuint NumEBO = 1;

GLuint texture;
GLuint IDVertexShader[NumVertexShader];
GLuint VertexShader[NumVertexShader];
GLuint IDFragmentShader[NumFragmentShader];
GLuint FragmentShader[NumFragmentShader];
GLuint IDShaderProgram[NumShaderProgram];
GLuint ShaderProgram[NumShaderProgram];
GLuint IDVAO[NumVAO];
GLuint IDvao[NumVAO];
GLuint IDVBO[NumVBO];
GLuint IDvbo[NumVBO];
GLuint IDEBO[NumEBO];
GLuint IDebo[NumEBO];
GLuint VAO[NumVAO];
GLuint VBO[NumVBO];
GLuint EBO[NumEBO];

const GLchar *vertexShaderSource =
          "#version 330 core\n"
          "layout(location = 0) in vec3 position;\n"
          "layout (location = 1) in vec2 texCoord;\n"
          "layout (location = 2) in vec3 normal;\n"

          "uniform mat4 model;\n"
          "uniform mat4 view;\n"
          "uniform mat4 projection;\n"

          "out vec3 FragPos;\n"
          "out vec2 TexCoord;\n"
          "out vec3 Normal;\n"
          "void main()\n"
          "{\n"
          "  FragPos = vec3(model * vec4(position,1.0));\n"
          "  Normal = mat3(transpose(inverse(model))) * normal;\n"
          "  gl_Position =projection * view *model* vec4(position, 1.0f);\n"
          "  TexCoord = texCoord;\n"
          "}\n";


const GLchar *fragmentShaderSource =
          "#version 330 core\n"
          "in vec2 TexCoord;\n"
          "in vec3 Normal;\n"
          "in vec3 FragPos;\n"

          "out vec4 color;\n"
          "struct Material{\n"
          "   sampler2D diffuse;\n"
          "   vec3 specular;\n"
          "   float shininess;\n"
          "};\n"
          "struct Light{\n"
          "   vec3 position;\n"
          "   vec3 ambient;\n"
          "   vec3 direction;\n"
          "   float cutOff;"
          "   float outerCutOff;"
          "   vec3 diffuse;\n"
          "   vec3 specular;\n"
          "   float constant;\n"
          "   float linear;\n"
          "   float quadratic;\n"
          "};\n"
          "uniform int LightMode;\n"
          "uniform vec3 viewPos;\n"
          "uniform Material material;\n"
          "uniform Light light;\n"
          "uniform int ColorMode;\n"
          "void main()\n"
          "{\n"
          "  vec3 ambient = light.ambient * texture(material.diffuse,TexCoord).rgb;\n"

          "  vec3 norm = normalize(Normal);\n"
          "  vec3 lightDir;"

          "  if(LightMode == 1||LightMode == 2)\n"
          "  lightDir = normalize(light.position - FragPos);\n"
          "  if(LightMode == 0)\n"
          "  lightDir = normalize(-light.direction);\n"
          "  float diff = max(dot(norm,lightDir),0.0);\n"
          "  vec3 diffuse = light.diffuse * diff*texture(material.diffuse,TexCoord).rgb;\n"  //将环境光得材质颜色设置为漫反射材质颜色同样的值

          "  vec3 viewDir = normalize(viewPos - FragPos);\n"
          "  vec3 reflectDir = reflect(-lightDir , norm);\n"
          "  float spec = pow(max(dot(viewDir, reflectDir),0.0),material.shininess);\n"
          "  vec3 specular = light.specular * (spec * material.specular);\n"

          "  float theta = dot(lightDir,normalize(-light.direction));\n"
          "  float epsilon = (light.cutOff - light.outerCutOff);\n"
          "  float intensity = clamp((theta - light.outerCutOff) /epsilon, 0.0, 1.0);\n"
          "  if(LightMode == 2){\n"
          "  diffuse *= intensity;\n"
          "  specular *= intensity;\n"
          "  }\n"

          "  float distance = length(light.position - FragPos);\n"
          "  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));\n"
          "  if(LightMode == 0 || LightMode == 2){\n"
          "  ambient *= attenuation;\n"
          "  diffuse *= attenuation;\n"
          "  specular *= attenuation;\n"
          "  }\n"

          "  vec3 result = ambient + diffuse + specular;\n"
          "  if(ColorMode == 0)"
          "  color = vec4(result, 1.0);\n"
          "  if(ColorMode == 1)"
          "  color = vec4(result,1.0)*vec4(0,1,0,1);\n"
          "  if(ColorMode == 2)"
          "  color = vec4(result,1.0)*vec4(1,0,0,1);\n"
          "  if(ColorMode == 3)"
          "  color = vec4(result,1.0)*vec4(0,0,1,1);\n"
          "}\n";


const GLchar *vertexShaderSource_1 =
        "#version 330 core\n"
        "layout(location = 0) in vec3 vPosition;\n"
        "out vec3 Color;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"

        "void main()\n"
        "{\n"
            "gl_Position = projection * view * model * vec4(vPosition, 1.0);\n"
        "}\n";

const GLchar *fragmentShaderSource_1 =
        "#version 330 core\n"
        "out vec4 fColor;\n"
        "void main()\n"
        "{\n"
            "fColor = vec4(1.0f,1.0f,1.0f,1.0f);\n"
        "}\n";

//loop细分参数
const double pi = 3.14159265;
int initVertice = 4;
int numVertice = 4;
int numface = 4;
const int MAX = 1000;
int edgeVertice[MAX][MAX][3] ;
int newIndex[MAX];
float newVertice[MAX];


float vertices[] = {
  -1,-1,-1,
   1,1,-1,
   -1,1,1,
   1,-1,1
};
int indices[] = {
    1,2,3,
    0,3,2,
    0,2,1,
    0,1,3
};
float vertices1[] = {
   -0.25,  -0.25,   -0.25,
    0.25,   0.25,   -0.25,
   -0.25,   0.25,    0.25,
    0.25,  -0.25,    0.25,
    0,      0.5,     0,
    0,      0,       0.5,
    0.5,    0,       0,
    0,     -0.5,     0,
   -0.5,    0,       0,
    0,      0,      -0.5
};
int indices1[] = {
    1,4,6,
    4,2,5,
    6,5,3,
    6,4,5,
    0,7,8,
    7,3,5,
    8,5,2,
    8,7,5,
    0,8,9,
    8,2,4,
    9,4,1,
    9,8,4,
    0,9,7,
    9,1,6,
    7,6,3,
    7,9,6
};

int indices3[] = {
    1,34,36,
    34,10,35,
    36,35,12,
    36,34,35,
    10,37,39,
    37,4,38,
    39,38,11,
    39,37,38,
    12,40,42,
    40,11,41,
    42,41,6,
    42,40,41,
    12,35,40,
    35,10,39,
    40,39,11,
    40,35,39,
    4,43,45,
    43,13,44,
    45,44,15,
    45,43,44,
    13,46,48,
    46,2,47,
    48,47,14,
    48,46,47,
    15,49,51,
    49,14,50,
    51,50,5,
    51,49,50,
    15,44,49,
    44,13,48,
    49,48,14,
    49,44,48,
    6,52,54,
    52,16,53,
    54,53,18,
    54,52,53,
    16,55,57,
    55,5,56,
    57,56,17,
    57,55,56,
    18,58,60,
    58,17,59,
    60,59,3,
    60,58,59,
    18,53,58,
    53,16,57,
    58,57,17,
    58,53,57,
    6,41,52,
    41,11,61,
    52,61,16,
    52,41,61,
    11,38,62,
    38,4,45,
    62,45,15,
    62,38,45,
    16,63,55,
    63,15,51,
    55,51,5,
    55,63,51,
    16,61,63,
    61,11,62,
    63,62,15,
    63,61,62,
    0,64,66,
    64,19,65,
    66,65,21,
    66,64,65,
    19,67,69,
    67,7,68,
    69,68,20,
    69,67,68,
    21,70,72,
    70,20,71,
    72,71,8,
    72,70,71,
    21,65,70,
    65,19,69,
    70,69,20,
    70,65,69,
    7,73,75,
    73,22,74,
    75,74,23,
    75,73,74,
    22,76,77,
    76,3,59,
    77,59,17,
    77,76,59,
    23,78,79,
    78,17,56,
    79,56,5,
    79,78,56,
    23,74,78,
    74,22,77,
    78,77,17,
    78,74,77,
    8,80,82,
    80,24,81,
    82,81,25,
    82,80,81,
    24,83,84,
    83,5,50,
    84,50,14,
    84,83,50,
    25,85,86,
    85,14,47,
    86,47,2,
    86,85,47,
    25,81,85,
    81,24,84,
    85,84,14,
    85,81,84,
    8,71,80,
    71,20,87,
    80,87,24,
    80,71,87,
    20,68,88,
    68,7,75,
    88,75,23,
    88,68,75,
    24,89,83,
    89,23,79,
    83,79,5,
    83,89,79,
    24,87,89,
    87,20,88,
    89,88,23,
    89,87,88,
    0,66,91,
    66,21,90,
    91,90,27,
    91,66,90,
    21,72,93,
    72,8,92,
    93,92,26,
    93,72,92,
    27,94,96,
    94,26,95,
    96,95,9,
    96,94,95,
    27,90,94,
    90,21,93,
    94,93,26,
    94,90,93,
    8,82,98,
    82,25,97,
    98,97,28,
    98,82,97,
    25,86,99,
    86,2,46,
    99,46,13,
    99,86,46,
    28,100,101,
    100,13,43,
    101,43,4,
    101,100,43,
    28,97,100,
    97,25,99,
    100,99,13,
    100,97,99,
    9,102,104,
    102,29,103,
    104,103,30,
    104,102,103,
    29,105,106,
    105,4,37,
    106,37,10,
    106,105,37,
    30,107,108,
    107,10,34,
    108,34,1,
    108,107,34,
    30,103,107,
    103,29,106,
    107,106,10,
    107,103,106,
    9,95,102,
    95,26,109,
    102,109,29,
    102,95,109,
    26,92,110,
    92,8,98,
    110,98,28,
    110,92,98,
    29,111,105,
    111,28,101,
    105,101,4,
    105,111,101,
    29,109,111,
    109,26,110,
    111,110,28,
    111,109,110,
    0,91,64,
    91,27,112,
    64,112,19,
    64,91,112,
    27,96,114,
    96,9,113,
    114,113,31,
    114,96,113,
    19,115,67,
    115,31,116,
    67,116,7,
    67,115,116,
    19,112,115,
    112,27,114,
    115,114,31,
    115,112,114,
    9,104,118,
    104,30,117,
    118,117,32,
    118,104,117,
    30,108,119,
    108,1,36,
    119,36,12,
    119,108,36,
    32,120,121,
    120,12,42,
    121,42,6,
    121,120,42,
    32,117,120,
    117,30,119,
    120,119,12,
    120,117,119,
    7,122,73,
    122,33,123,
    73,123,22,
    73,122,123,
    33,124,125,
    124,6,54,
    125,54,18,
    125,124,54,
    22,126,76,
    126,18,60,
    76,60,3,
    76,126,60,
    22,123,126,
    123,33,125,
    126,125,18,
    126,123,125,
    7,116,122,
    116,31,127,
    122,127,33,
    122,116,127,
    31,113,128,
    113,9,118,
    128,118,32,
    128,113,118,
    33,129,124,
    129,32,121,
    124,121,6,
    124,129,121,
    33,127,129,
    127,31,128,
    129,128,32,
    129,127,128
};
float vertices3[] = {
    -0.200195,-0.200195,-0.200195,
    0.200195,0.200195,-0.200195,
    -0.200195,0.200195,0.200195,
    0.200195,-0.200195,0.200195,
    0,0.304688,0,
    0,0,0.304688,
    0.304688,0,0,
    0,-0.304688,0,
    -0.304688,0,0,
    0,0,-0.304688,
    0.149414,0.256836,-0.149414,
    0.201172,0.201172,0.0273438,
    0.256836,0.149414,-0.149414,
    -0.149414,0.256836,0.149414,
    -0.149414,0.149414,0.256836,
    0.0273438,0.201172,0.201172,
    0.201172,0.0273438,0.201172,
    0.149414,-0.149414,0.256836,
    0.256836,-0.149414,0.149414,
    -0.149414,-0.256836,-0.149414,
    -0.201172,-0.201172,0.0273438,
    -0.256836,-0.149414,-0.149414,
    0.149414,-0.256836,0.149414,
    -0.0273438,-0.201172,0.201172,
    -0.201172,-0.0273438,0.201172,
    -0.256836,0.149414,0.149414,
    -0.201172,0.0273438,-0.201172,
    -0.149414,-0.149414,-0.256836,
    -0.201172,0.201172,-0.0273438,
    -0.0273438,0.201172,-0.201172,
    0.149414,0.149414,-0.256836,
    0.0273438,-0.201172,-0.201172,
    0.201172,-0.0273438,-0.201172,
    0.201172,-0.201172,-0.0273438,
    0.189453,0.220703,-0.189453,
    0.216797,0.216797,-0.138672,
    0.220703,0.189453,-0.189453,
    0.0820313,0.289063,-0.0820313,
    0.105469,0.273438,0.0195313,
    0.175781,0.25,-0.0664063,
    0.25,0.175781,-0.0664063,
    0.273438,0.105469,0.0195313,
    0.289063,0.0820313,-0.0820313,
    -0.0820313,0.289063,0.0820313,
    -0.0664063,0.25,0.175781,
    0.0195313,0.273438,0.105469,
    -0.189453,0.220703,0.189453,
    -0.189453,0.189453,0.220703,
    -0.138672,0.216797,0.216797,
    -0.0664063,0.175781,0.25,
    -0.0820313,0.0820313,0.289063,
    0.0195313,0.105469,0.273438,
    0.273438,0.0195313,0.105469,
    0.25,-0.0664063,0.175781,
    0.289063,-0.0820313,0.0820313,
    0.105469,0.0195313,0.273438,
    0.0820313,-0.0820313,0.289063,
    0.175781,-0.0664063,0.25,
    0.216797,-0.138672,0.216797,
    0.189453,-0.189453,0.220703,
    0.220703,-0.189453,0.189453,
    0.210938,0.121094,0.121094,
    0.121094,0.210938,0.121094,
    0.121094,0.121094,0.210938,
    -0.189453,-0.220703,-0.189453,
    -0.216797,-0.216797,-0.138672,
    -0.220703,-0.189453,-0.189453,
    -0.0820313,-0.289063,-0.0820313,
    -0.105469,-0.273438,0.0195313,
    -0.175781,-0.25,-0.0664063,
    -0.25,-0.175781,-0.0664063,
    -0.273438,-0.105469,0.0195313,
    -0.289063,-0.0820313,-0.0820313,
    0.0820313,-0.289063,0.0820313,
    0.0664063,-0.25,0.175781,
    -0.0195313,-0.273438,0.105469,
    0.189453,-0.220703,0.189453,
    0.138672,-0.216797,0.216797,
    0.0664063,-0.175781,0.25,
    -0.0195313,-0.105469,0.273438,
    -0.273438,-0.0195313,0.105469,
    -0.25,0.0664063,0.175781,
    -0.289063,0.0820313,0.0820313,
    -0.105469,-0.0195313,0.273438,
    -0.175781,0.0664063,0.25,
    -0.216797,0.138672,0.216797,
    -0.220703,0.189453,0.189453,
    -0.210938,-0.121094,0.121094,
    -0.121094,-0.210938,0.121094,
    -0.121094,-0.121094,0.210938,
    -0.216797,-0.138672,-0.216797,
    -0.189453,-0.189453,-0.220703,
    -0.273438,0.0195313,-0.105469,
    -0.25,-0.0664063,-0.175781,
    -0.175781,-0.0664063,-0.25,
    -0.105469,0.0195313,-0.273438,
    -0.0820313,-0.0820313,-0.289063,
    -0.25,0.175781,0.0664063,
    -0.273438,0.105469,-0.0195313,
    -0.216797,0.216797,0.138672,
    -0.175781,0.25,0.0664063,
    -0.105469,0.273438,-0.0195313,
    -0.0195313,0.105469,-0.273438,
    0.0664063,0.175781,-0.25,
    0.0820313,0.0820313,-0.289063,
    -0.0195313,0.273438,-0.105469,
    0.0664063,0.25,-0.175781,
    0.138672,0.216797,-0.216797,
    0.189453,0.189453,-0.220703,
    -0.121094,0.121094,-0.210938,
    -0.210938,0.121094,-0.121094,
    -0.121094,0.210938,-0.121094,
    -0.138672,-0.216797,-0.216797,
    0.0195313,-0.105469,-0.273438,
    -0.0664063,-0.175781,-0.25,
    -0.0664063,-0.25,-0.175781,
    0.0195313,-0.273438,-0.105469,
    0.175781,0.0664063,-0.25,
    0.105469,-0.0195313,-0.273438,
    0.216797,0.138672,-0.216797,
    0.25,0.0664063,-0.175781,
    0.273438,-0.0195313,-0.105469,
    0.105469,-0.273438,-0.0195313,
    0.175781,-0.25,0.0664063,
    0.273438,-0.105469,-0.0195313,
    0.25,-0.175781,0.0664063,
    0.216797,-0.216797,0.138672,
    0.121094,-0.210938,-0.121094,
    0.121094,-0.121094,-0.210938,
    0.210938,-0.121094,-0.121094
};
float vertices2[] = {
    -0.203125,-0.203125,-0.203125,
    0.203125,0.203125,-0.203125,
    -0.203125,0.203125,0.203125,
    0.203125,-0.203125,0.203125,
    0,0.34375,0,
    0,0,0.34375,
    0.34375,0,0,
    0,-0.34375,0,
    -0.34375,0,0,
    0,0,-0.34375,
    0.15625,0.28125,-0.15625,
    0.21875,0.21875,0.03125,
    0.28125,0.15625,-0.15625,
    -0.15625,0.28125,0.15625,
    -0.15625,0.15625,0.28125,
    0.03125,0.21875,0.21875,
    0.21875,0.03125,0.21875,
    0.15625,-0.15625,0.28125,
    0.28125,-0.15625,0.15625,
    -0.15625,-0.28125,-0.15625,
    -0.21875,-0.21875,0.03125,
    -0.28125,-0.15625,-0.15625,
    0.15625,-0.28125,0.15625,
    -0.03125,-0.21875,0.21875,
    -0.21875,-0.03125,0.21875,
    -0.28125,0.15625,0.15625,
    -0.21875,0.03125,-0.21875,
    -0.15625,-0.15625,-0.28125,
    -0.21875,0.21875,-0.03125,
    -0.03125,0.21875,-0.21875,
    0.15625,0.15625,-0.28125,
    0.03125,-0.21875,-0.21875,
    0.21875,-0.03125,-0.21875,
    0.21875,-0.21875,-0.03125,
};
int indices2[] = {
    1,10,12,
    10,4,11,
    12,11,6,
    12,10,11,
    4,13,15,
    13,2,14,
    15,14,5,
    15,13,14,
    6,16,18,
    16,5,17,
    18,17,3,
    18,16,17,
    6,11,16,
    11,4,15,
    16,15,5,
    16,11,15,
    0,19,21,
    19,7,20,
    21,20,8,
    21,19,20,
    7,22,23,
    22,3,17,
    23,17,5,
    23,22,17,
    8,24,25,
    24,5,14,
    25,14,2,
    25,24,14,
    8,20,24,
    20,7,23,
    24,23,5,
    24,20,23,
    0,21,27,
    21,8,26,
    27,26,9,
    27,21,26,
    8,25,28,
    25,2,13,
    28,13,4,
    28,25,13,
    9,29,30,
    29,4,10,
    30,10,1,
    30,29,10,
    9,26,29,
    26,8,28,
    29,28,4,
    29,26,28,
    0,27,19,
    27,9,31,
    19,31,7,
    19,27,31,
    9,30,32,
    30,1,12,
    32,12,6,
    32,30,12,
    7,33,22,
    33,6,18,
    22,18,3,
    22,33,18,
    7,31,33,
    31,9,32,
    33,32,6,
    33,31,32
};

int addEdgeVertice(int v1Index,int v2Index, int v3Index)
{
    if(v1Index > v2Index)
    {
        int vTmp;
        vTmp = v1Index;
        v1Index = v2Index;
        v2Index = vTmp;
    }
    if(edgeVertice[v1Index-1][v2Index-1][0]== 0)
    {
        numVertice++;
        int newIndexOfVertice = numVertice;
        edgeVertice[v1Index-1][v2Index-1][0] = newIndexOfVertice;
        edgeVertice[v1Index-1][v2Index-1][1] = v3Index;
    }
    else
        edgeVertice[v1Index-1][v2Index-1][2] = v3Index;

    return edgeVertice[v1Index-1][v2Index-1][0];
}

float Sum(float *vertice, int len , int* adjVertice, int col)
{
    float ans = 0;
    for(int i = 0 ; i < len ; i++){
        int index = adjVertice[i];
        ans += vertice[index*3+col];
    }
    return ans;
}

void loop(int& numOfface)
{
    int indeX = 0;
    for(int i = 0 ; i < numface ; i++)
    {
    int vaIndex, vbIndex, vcIndex, vpIndex, vqIndex , vrIndex;
    vaIndex = indices[3*i];
    vbIndex = indices[3*i+1];
    vcIndex = indices[3*i+2];

    vpIndex = addEdgeVertice(vaIndex,vbIndex,vcIndex);
    vqIndex = addEdgeVertice(vbIndex,vcIndex,vaIndex);
    vrIndex = addEdgeVertice(vaIndex,vcIndex,vbIndex);

    newIndex[3*indeX] = vaIndex;
    newIndex[3*indeX+1] = vpIndex;
    newIndex[3*indeX+2] = vrIndex;
    indeX ++;

    newIndex[3*indeX] = vpIndex;
    newIndex[3*indeX+1] = vbIndex;
    newIndex[3*indeX+2] = vqIndex;
    indeX ++;

    newIndex[3*indeX] = vrIndex;
    newIndex[3*indeX+1] = vqIndex;
    newIndex[3*indeX+2] = vcIndex;
    indeX ++;

    newIndex[3*indeX] = vrIndex;
    newIndex[3*indeX+1] = vpIndex;
    newIndex[3*indeX+2] = vqIndex;
    indeX ++;

    }
    numOfface = indeX;
    //新的边点位置
    int vNOpposite1Index ,vNOpposite2Index;
    for(int v1 = 1; v1 <= initVertice; v1++)
{
    for(int v2 = v1+1; v2 <= initVertice ;v2++)
    {
        int  vNIndex = edgeVertice[v1-1][v2-1][0];
        if(vNIndex != 0)
        {
            vNOpposite1Index = edgeVertice[v1-1][v2-1][1];
            vNOpposite2Index = edgeVertice[v1-1][v2-1][2];
            if(vNOpposite2Index == 0)
            {
            newVertice[3*vNIndex] = 1/2.0 * (vertices[3*(v1-1)] + vertices[3*(v2-1)]);
            newVertice[3*vNIndex+1] = 1/2.0 * (vertices[3*(v1-1)+1] + vertices[3*(v2-1)+1]);
            newVertice[3*vNIndex+2] = 1/2.0 * (vertices[3*(v1-1)+2] + vertices[3*(v2-1)+2]);
            }
            else
            {
            newVertice[3*vNIndex] = 3/8.0 * (vertices[3*(v1-1)] + vertices[3*(v2-1)]) + 1/8.0*(vertices[3*(vNOpposite1Index-1)] + vertices[3*(vNOpposite2Index-1)]);
            newVertice[3*vNIndex+1] = 3/8.0 * (vertices[3*(v1-1)+1] + vertices[3*(v2-1)+1]) + 1/8.0*(vertices[3*(vNOpposite1Index-1)+1] + vertices[3*(vNOpposite2Index-1)+1]);
            newVertice[3*vNIndex+2] = 3/8.0 * (vertices[3*(v1-1)+2] + vertices[3*(v2-1)+2]) + 1/8.0*(vertices[3*(vNOpposite1Index-1)+2] + vertices[3*(vNOpposite2Index-1)+2]);
            }
        }
    }
}

int adjVertice[MAX][20];
int length[MAX];  //邻接表长度
for(int v =  1; v <= initVertice; v++)
{
    int end = 0 ;
    for(int vTmp = 1 ; vTmp <= initVertice ; vTmp++)
    {
        if((v<vTmp && edgeVertice[v-1][vTmp-1][0] != 0)||(v>vTmp&&edgeVertice[vTmp-1][v-1][0]!= 0))
            adjVertice[v-1][end++] = vTmp;
    }
    length[v-1] = end;
}


for(int  v = 1 ; v <= initVertice ; v++)
{
    int len = length[v-1] ;
    int end = 0;
    int adjBondaryVertices[200];
    for(int i = 0 ; i < len ; i++)
    {
        int vi = adjVertice[v-1][i] ;
        if( ((vi>v)&&(edgeVertice[v-1][vi-1][2]==0)) || ((vi<v)&&(edgeVertice[vi-1][v-1][2]==0)) )
        {
            adjBondaryVertices[end++] = vi;
        }
    }

    if(end == 2)
    {
        newVertice[3*v] = 6.0/8.0*vertices[3*(v-1)] + 1.0/8.0*(vertices[3*(adjBondaryVertices[0]-1)]+vertices[3*(adjBondaryVertices[1]-1)]);
        newVertice[3*v+1] = 6.0/8.0*vertices[3*(v-1)+1] + 1.0/8.0*(vertices[3*(adjBondaryVertices[0]-1)+1]+vertices[3*(adjBondaryVertices[1]-1)+1]);
        newVertice[3*v+2] = 6.0/8.0*vertices[3*(v-1)+2] + 1.0/8.0*(vertices[3*(adjBondaryVertices[0]-1)+2]+vertices[3*(adjBondaryVertices[1]-1)+2]);
    }
    else
    {
        double beta = 1.0/len*(5.0/8.0 - (3.0/8.0 + 1.0/4.0*cos(2*pi/len)) *(3.0/8.0 + 1.0/4.0*cos(2*pi/len)) );
        newVertice[3*v] = (1-len*beta)*vertices[3*v] + beta*Sum(vertices,len,adjVertice[v-1],0);
        newVertice[3*v+1] = (1-len*beta)*vertices[3*v+1] + beta*Sum(vertices,len,adjVertice[v-1],1);
        newVertice[3*v+2] = (1-len*beta)*vertices[3*v+2] + beta*Sum(vertices,len,adjVertice[v-1],2);
    }

}
}

openglwindow::openglwindow(QWidget* parent)
    :QOpenGLWidget(parent)
{
    //设置OpenGL的版本信息
//    QSurfaceFormat format;
//    format.setRenderableType(QSurfaceFormat::OpenGL);
//    format.setProfile(QSurfaceFormat::CoreProfile);
//    format.setVersion(3,3);
//    setFormat(format);
}

openglwindow::~openglwindow()
{

}

void openglwindow::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    if(!mo.loadObjModel("c.obj",nVertextData,index,nVertextData1,index1))
    {
        qDebug("error");
    }
    VertexShader[0] = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader[0], 1, &vertexShaderSource_1, nullptr);
    glCompileShader(VertexShader[0]);
    //检查编译是否出错
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(VertexShader[0], GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(VertexShader[0], 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    /******************************* 片段着色器创建 *******************************/
    /* 第一个片元着色器 */
    FragmentShader[0] = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader[0], 1, &fragmentShaderSource_1, nullptr);
    glCompileShader(FragmentShader[0]);
    //检查编译是否出错
    glGetShaderiv(FragmentShader[0], GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(FragmentShader[0], 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    /********************************* 链接着色器 *********************************/
    /* 第一个着色器程序 */
    ShaderProgram[0] = glCreateProgram();
    glAttachShader(ShaderProgram[0], VertexShader[0]);
    glAttachShader(ShaderProgram[0], FragmentShader[0]);
    glLinkProgram(ShaderProgram[0]);
    //检查链接错误
    glGetProgramiv(ShaderProgram[0], GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ShaderProgram[0], 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    //删除着色器对象（生成着色器程序之后不再需要）
    glDeleteShader(VertexShader[0]);
    glDeleteShader(FragmentShader[0]);

    /******************************** 设置顶点数据 ********************************/
    //彩色正方体
//    GLfloat vertices[] = {
//        0.000000, 1.000000, 0.000000,
//        0.000000, 0.000000, 0.000000,
//        0.500000, 0.000000, 0.000000,
//        0.475528, 0.000000, 0.154508,
//        0.404508, 0.000000, 0.293893,
//        0.293893, 0.000000, 0.404508,
//        0.154508, 0.000000, 0.475528,
//        0.000000, 0.000000, 0.500000,
//        -0.154508, 0.000000, 0.475528,
//        -0.293893, 0.000000, 0.404508,
//        -0.404508, 0.000000, 0.293893,
//        -0.475528, 0.000000, 0.154508,
//        -0.500000, 0.000000, 0.000000,
//        -0.475528, 0.000000, -0.154508,
//        -0.404508, 0.000000, -0.293893,
//        -0.293893, 0.000000, -0.404508,
//        -0.154508, 0.000000, -0.475528,
//        0.000000, 0.000000, -0.500000,
//        0.154508, 0.000000, -0.475528,
//        0.293893, 0.000000, -0.404508,
//        0.404508, 0.000000, -0.293893,
//        0.475528, 0.000000, -0.154508
//    };
//    GLint indices[] = {
//         2, 0, 3,
//         3, 1, 2,
//         3, 0, 4,
//         4, 1, 3,
//         4, 0, 5,
//         5, 1, 4,
//         5, 0, 6,
//         6, 1, 5,
//         6, 0, 7,
//         7, 1, 6,
//         7, 0, 8,
//         8, 1, 7,
//         8, 0, 9,
//         9, 1, 8,
//         9, 0, 10,
//         10, 1, 9,
//         10, 0, 11,
//         11, 1, 10,
//         11, 0, 12,
//         12, 1, 11,
//         12, 0, 13,
//         13, 1, 12,
//         13, 0, 14,
//         14, 1, 13,
//         14, 0, 15,
//         15, 1, 14,
//         15, 0, 16,
//         16, 1 ,15,
//         16 ,0, 17,
//         17, 1, 16,
//         17, 0, 18,
//         18 ,1, 17,
//         18, 0, 19,
//         19, 1, 18,
//         19, 0, 20,
//         20, 1, 19,
//         20, 0, 21,
//         21, 1, 20,
//         21, 0, 2,
//         2, 1, 21
//    };
}

void openglwindow::paintGL()
{
    if(type == 0){
    //反走样
    if(modelchange){
        nVertextData.clear();
        index.clear();
        nVertextData1.clear();
        index1.clear();
        mo.loadObjModel(modelname,nVertextData,index,nVertextData1,index1);
        modelchange = false;
    }
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH,GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

    IDVertexShader[0] = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(IDVertexShader[0],1,&vertexShaderSource,nullptr);
    glCompileShader(IDVertexShader[0]);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(IDVertexShader[0],GL_COMPILE_STATUS,&success);
    if(!success)
    {
        glGetShaderInfoLog(IDVertexShader[0],512,NULL,infoLog);
        std::cout<<"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"<<infoLog<<std::endl;
    }

    IDFragmentShader[0] = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(IDFragmentShader[0],1,&fragmentShaderSource,nullptr);
    glCompileShader(IDFragmentShader[0]);
    glGetShaderiv(IDFragmentShader[0],GL_COMPILE_STATUS,&success);
    if(!success)
    {
        glGetShaderInfoLog(IDFragmentShader[0],512,NULL,infoLog);
        std::cout<<"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"<<infoLog<<std::endl;
    }

    IDShaderProgram[0] = glCreateProgram();
    glAttachShader(IDShaderProgram[0],IDVertexShader[0]);
    glAttachShader(IDShaderProgram[0],IDFragmentShader[0]);
    glLinkProgram(IDShaderProgram[0]);

    glGetProgramiv(IDShaderProgram[0],GL_LINK_STATUS,&success);
    if(!success){
        glGetProgramInfoLog(IDShaderProgram[0],512,NULL,infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog <<std::endl;
    }
    glDeleteShader(IDVertexShader[0]);
    glDeleteShader(IDFragmentShader[0]);

    glGenVertexArrays(NumVAO,IDVAO);
    glGenBuffers(NumVBO,IDVBO);
    glGenBuffers(NumEBO,IDEBO);

    glBindVertexArray(IDVAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER,IDVBO[0]);
    glBufferData(GL_ARRAY_BUFFER,nVertextData.size()*sizeof(VertexData),&nVertextData[0],GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IDEBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,index.size()*sizeof(unsigned int),&index[0],GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(VertexData),(GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(VertexData),(GLvoid*)offsetof(VertexData,texcoord));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(VertexData),(GLvoid*)offsetof(VertexData,normal));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    glEnable(GL_DEPTH_TEST);

    if(Mode == 1)
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    if(Mode == 0)
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    if(Mode == 2)
    {
     glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
     glPointSize(2.0f);
    }
    glClear(GL_COLOR_BUFFER_BIT);
    //实现参数的刷新
    update();

    loadGLTextures();

    glBindTexture(GL_TEXTURE_2D,texture);
    //渲染彩色正方体
    glUseProgram(IDShaderProgram[0]);
    //平行光
    if(LightMode == 0){
        GLint lp = glGetUniformLocation(IDShaderProgram[0],"light.direction");
        glUniform3fv(lp,3,&lightDirection[0]);
    }
    //点光
    if(LightMode == 1){
        GLint lp = glGetUniformLocation(IDShaderProgram[0],"light.position");
        glUniform3fv(lp,3,&lightPos[0]);
    }
    //手电筒
    if(LightMode == 2)
    {
        GLint lp = glGetUniformLocation(IDShaderProgram[0],"light.position");
        glUniform3fv(lp,3,&lightPos[0]);
        glm::vec3 front = glm::vec3(-0.2f, -1.0f, 100.0f);
        glUniform3fv(glGetUniformLocation(IDShaderProgram[0],"light.direction"),3,&front[0]);
        glUniform1f(glGetUniformLocation(IDShaderProgram[0],"light.cutOff"),glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(IDShaderProgram[0],"light.outerCutOff"),glm::cos(glm::radians(17.5f)));
    }

    GLint vp = glGetUniformLocation(IDShaderProgram[0],"viewPos");
    glUniform3fv(vp,3, &cameraPos[0]);

    GLint la = glGetUniformLocation(IDShaderProgram[0],"light.ambient");
    glUniform3f(la,Shininess,Shininess,Shininess);

    GLint ld = glGetUniformLocation(IDShaderProgram[0],"light.diffuse");
    glUniform3f(ld,0.8f,0.8f,0.8f);

    GLint ls = glGetUniformLocation(IDShaderProgram[0],"light.specular");
    glUniform3f(ls,1.0f,1.0f,1.0f);

    GLint ms = glGetUniformLocation(IDShaderProgram[0],"material.specular");
    glUniform3f(ms, 0.5f,0.5f,0.5f);

    GLint msh = glGetUniformLocation(IDShaderProgram[0],"material.shininess");
    glUniform1f(msh,32.0f);

    glUniform1f(glGetUniformLocation(IDShaderProgram[0],"light.constant"),1.0f);
    glUniform1f(glGetUniformLocation(IDShaderProgram[0],"light.linear"),0.0014f);
    glUniform1f(glGetUniformLocation(IDShaderProgram[0],"light.quadratic"),0.000007f);

    glUniform1i(glGetUniformLocation(IDShaderProgram[0],"LightMode"),LightMode);
    glUniform1i(glGetUniformLocation(IDShaderProgram[0],"ColorMode"),color);

    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 model;


    GLint modelLoc = glGetUniformLocation(IDShaderProgram[0],"model");
    GLint viewLoc = glGetUniformLocation(IDShaderProgram[0],"view");
    GLint projLoc = glGetUniformLocation(IDShaderProgram[0],"projection");


    float radius = 150.0f;
    float camX;
    float camZ;

    if(autoRound == 0){
    times = 0;
    view = glm::lookAt(cameraPos,worldCentrol,cameraUp);
    }
    else {
        times += 0.05;
        camX = sin(times)*radius;
        camZ = cos(times)*radius;
        view = glm::lookAt(glm::vec3(camX,0,camZ),worldCentrol,cameraUp);
    }
    projection = glm::perspective(glm::radians(45.0f),4.0f/3.0f, 0.1f, 100000.0f);
    model = glm::translate(model,transVec);
    model = glm::rotate(model,glm::radians(pitch),glm::vec3(1.0f,0.0f,0.0f));
    model = glm::rotate(model,glm::radians(yaw),glm::vec3(1.0f,0.0f,0.0f));

    glUniformMatrix4fv(viewLoc, 1 , GL_FALSE , glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1 , GL_FALSE , glm::value_ptr(projection));
    glUniformMatrix4fv(modelLoc, 1 , GL_FALSE , glm::value_ptr(model));


    glBindVertexArray(IDVAO[0]);
    glDrawElements(GL_TRIANGLES,index.size(),GL_UNSIGNED_INT,0);
    glBindVertexArray(0);

    if(index1.size()!=0){
    glGenVertexArrays(NumVAO,IDvao);
    glGenBuffers(NumVBO,IDvbo);
    glGenBuffers(NumEBO,IDebo);

    glBindVertexArray(IDvao[0]);

    glBindBuffer(GL_ARRAY_BUFFER,IDvbo[0]);
    glBufferData(GL_ARRAY_BUFFER,nVertextData1.size()*sizeof(VertexData),&nVertextData1[0],GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IDebo[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,index1.size()*sizeof(unsigned int),&index1[0],GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(VertexData),(GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(VertexData),(GLvoid*)offsetof(VertexData,texcoord));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(VertexData),(GLvoid*)offsetof(VertexData,normal));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(IDvao[0]);
    glDrawElements(GL_QUADS,index1.size(),GL_UNSIGNED_INT,0);

    }
    glFlush();

    }
    else if(type == 1)
    {
        glEnable(GL_POINT_SMOOTH);
        glHint(GL_POINT_SMOOTH,GL_NICEST);
        glEnable(GL_POINT_SMOOTH);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
        glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
       /* 创建相关对象 */
       glGenVertexArrays(NumVAO, VAO);
       glGenBuffers(NumVBO, VBO);
       glGenBuffers(NumEBO, EBO);

       /* 显示立方体 */
       glBindVertexArray(VAO[0]);    //开始记录状态信息

       glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
       //glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*numVertice, &newVertice[3], GL_STATIC_DRAW);
       switch(LOOP){
            case 1:{
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) ,indices, GL_STATIC_DRAW);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW);
             }
            break;
            case 2:{
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1) ,indices1, GL_STATIC_DRAW);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1),vertices1, GL_STATIC_DRAW);
             }
            break;
            case 3:{
             glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2) ,indices2, GL_STATIC_DRAW);
             glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2),vertices2, GL_STATIC_DRAW);
            }
            break;
            case 4:{
             glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices3) ,indices3, GL_STATIC_DRAW);
             glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3),vertices3, GL_STATIC_DRAW);
            }
            break;
       default:{
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) ,indices, GL_STATIC_DRAW);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW);
       }
           break;

       }

       //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW);

       //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*3*numofface, indices, GL_STATIC_DRAW);

       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
       glEnableVertexAttribArray(0);


       glBindBuffer(GL_ARRAY_BUFFER, 0);

       glBindVertexArray(0);           //结束记录状态信息
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);   //在VAO后解绑，是为了不让VAO把解绑EBO的信息包含进入
       /* 显示立方体 */

       /* 固定属性区域 */
       glEnable(GL_DEPTH_TEST);

        glm::mat4 view;
        glm::mat4 projection;
        glm::mat4 model;


        GLint modelLoc = glGetUniformLocation(ShaderProgram[0],"model");
        GLint viewLoc = glGetUniformLocation(ShaderProgram[0],"view");
        GLint projLoc = glGetUniformLocation(ShaderProgram[0],"projection");



        view = glm::lookAt(cameraPos_1,worldCentrol,cameraUp);
        projection = glm::perspective(glm::radians(45.0f),4.0f/3.0f, 0.1f, 100000.0f);
        model = glm::translate(model,transVec_1);
        model = glm::rotate(model,glm::radians(pitch),glm::vec3(1.0f,0.0f,0.0f));
        model = glm::rotate(model,glm::radians(yaw),glm::vec3(1.0f,0.0f,0.0f));

        glUniformMatrix4fv(viewLoc, 1 , GL_FALSE , glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1 , GL_FALSE , glm::value_ptr(projection));
        glUniformMatrix4fv(modelLoc, 1 , GL_FALSE , glm::value_ptr(model));

        glClear(GL_COLOR_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        //渲染彩色正方体
        glUseProgram(ShaderProgram[0]);
        update();
        glBindVertexArray(VAO[0]);
        switch(LOOP){
        case 1:
            glDrawElements(GL_TRIANGLES,sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
            qDebug() << sizeof(indices)/sizeof(int);
            break;
        case 2:
            glDrawElements(GL_TRIANGLES,sizeof(indices1)/sizeof(int), GL_UNSIGNED_INT, 0);
            break;
        case 3:
            glDrawElements(GL_TRIANGLES,sizeof(indices2)/sizeof(int), GL_UNSIGNED_INT, 0);
            break;
        case 4:
            glDrawElements(GL_TRIANGLES,sizeof(indices3)/sizeof(int), GL_UNSIGNED_INT, 0);
            break;
        default:
            glDrawElements(GL_TRIANGLES,sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
            break;
        }

        glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //强制刷新缓冲区，保证命令被执行
        glFlush();
    }


}

void openglwindow::resizeGL(int width,int height)
{
    Q_UNUSED(width);
    Q_UNUSED(height);
}

void openglwindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons()== Qt::LeftButton)
    {
        GLfloat xoffset = event->x() - lastX;
        GLfloat yoffset = event->y() - lastY;
        lastX = event->x();
        lastY = event->y();

        GLfloat sensitivity = 0.4f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if(pitch > 89.0f)
               pitch = 89.0f;
        if(pitch < -89.0f)
                pitch = -89.0f;
    }
    else if(event->buttons() == Qt::RightButton)
    {
        GLfloat xoffset = event->x() - lastX;
        GLfloat yoffset = event->y() - lastY;
        lastX = event->x();
        lastY = event->y();

        GLfloat sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        transVec += glm::vec3(xoffset,-yoffset,0.0f);
        transVec_1 += glm::vec3(xoffset,-yoffset,0.0f);
//        qDebug() << "transVec_1.x =" << transVec_1.x;
//        qDebug() << "transVec_1.y =" << transVec_1.y;
    }
}

void openglwindow::wheelEvent(QWheelEvent *event)
{
    GLfloat sensitivity = 0.0005f;
    cameraPos *= (1.0f - event->delta() *sensitivity);

}

void openglwindow::mousePressEvent(QMouseEvent *event)
{
    lastX = event->x();
    lastY = event->y();

}
void openglwindow::loadGLTextures()
{

     QImage tex,buf;
     if(!buf.load(filename))
     {
         //qWarning("can't open the image...");
         QImage dummy(128,128,QImage::Format_RGB32);
         switch(color){
         case 0:
             dummy.fill(Qt::white);
             break;
         case 1:
             dummy.fill(Qt::green);
             break;
         case 2:
             dummy.fill(Qt::red);
             break;
         case 3:
             dummy.fill(Qt::blue);
             break;
         }
         buf = dummy;
     }
      tex = QGLWidget::convertToGLFormat(buf);
      glGenTextures(1,&texture);
      glBindTexture(GL_TEXTURE_2D,texture);
      glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,tex.width(),tex.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,tex.bits());
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_LINEAR);

      //当所显示的纹理比加载进来的纹理小/大时，采用GL_LINEAR处理
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

      glGenerateMipmap(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE,0);

}




