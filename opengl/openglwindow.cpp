


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

#include <objmode.h>
using namespace std;

int xx = 0;
double yy = 0;
glm::vec3 cameraPos    = glm::vec3(0.0f,0.0f,150.0f);
glm::vec3 worldCentrol = glm::vec3(0.0f,0.0f,0.0f);
glm::vec3 cameraUp     = glm::vec3(0.0f,1.0f,0.0f);
const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
//构建ModelMatrix,进行”局部空间“到”世界空间“的转换
glm::vec3 transVec = glm::vec3(0.0f,-10.0f,0.0f);
glm::vec3 lightPos = glm::vec3(1.2f,1.0f,2.0f);
glm::vec3 lightDirection = glm::vec3(-0.2f,-1.0f,-0.3f);

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
const GLuint NumVAO = 2;
const GLuint NumVBO = 2;
const GLuint NumEBO = 2;

GLuint texture;
GLuint IDVertexShader[NumVertexShader];
GLuint IDFragmentShader[NumFragmentShader];
GLuint IDShaderProgram[NumShaderProgram];
GLuint IDVAO[NumVAO];
GLuint IDvao[NumVAO];
GLuint IDVBO[NumVBO];
GLuint IDvbo[NumVBO];
GLuint IDEBO[NumEBO];
GLuint IDebo[NumEBO];

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
          "  FragPos = vec3(model * vec4(position,1.0) );\n"
          "  Normal = mat3(transpose(inverse(model))) * normal;\n"
          "  gl_Position =projection * view * model * vec4(position, 1.0f);\n"
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

          "  if(LightMode == 0||LightMode == 2)\n"
          "  lightDir = normalize(light.position - FragPos);\n"
          "  if(LightMode == 1)\n"
          "  lightDir = normalize(-light.position);\n"
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



//          "uniform sampler2D texture_diffuse0;\n"
//"  color = texture(texture_diffuse0,TexCoord);\n"
//"  color = vec4(result, 1.0);\n"
//          "vec4 M_AmbientLightColor = vec4(0.2,0.2,0.2,1.0);\n"
//          "vec4 M_AmbientMaterial = vec4(0.2,0.2,0.2,1.0);\n"
//          "vec4 ambientColor = M_AmbientLightColor*M_AmbientMaterial;\n"
//          "vec4 M_normal = vec4(1.0,1.0,1.0,1.0);\n"
//          "vec4 M_LightPos = vec4(10.0,10.0,0.0);\n"
//          "vec4 LightNormal = normalize(M_LightPos);\n"
//          "vec4 NormalNormal = normalize(M_normal);\n"
//          "vec4 M_DiffuseLightColor = vec4(1.0,1.0,1.0,1.0);\n"
//          "vec4 M_DiffuseMaterial = vec4(0.9,0.9,0.9,1.0);\n"
//          "vec4 diffuseColor = M_DiffuseLightColor * M_DiffuseMaterial* max(0.0,dot(NormalNormal,LightNormal);\n"
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
}

void openglwindow::paintGL()
{
    //反走样

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

    if(LightMode == 0){
        GLint lp = glGetUniformLocation(IDShaderProgram[0],"light.position");
        glUniform3fv(lp,3,&lightPos[0]);
    }
    if(LightMode == 1){
        GLint lp = glGetUniformLocation(IDShaderProgram[0],"light.position");
        glUniform3fv(lp,3,&lightDirection[0]);
    }
    if(LightMode == 2)
    {
        GLint lp = glGetUniformLocation(IDShaderProgram[0],"light.position");
        glUniform3fv(lp,3,&lightPos[0]);
        glm::vec3 front = glm::vec3(0.0f, 0.0f, -100.0f);
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



    view = glm::lookAt(cameraPos,worldCentrol,cameraUp);
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


    glFlush();

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




