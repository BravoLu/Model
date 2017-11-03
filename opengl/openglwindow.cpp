


#include <QtGui>
#include <QtWidgets>
#include <iostream>
#include <QMouseEvent>
#include "openglwindow.h"
//OpenGL Mathematics 用于数学变换
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <QtOpenGL/QGLWidget>
#include "mainwindow.h"
#include "ObjLoader.h"
using namespace std;

glm::vec3 cameraPos    = glm::vec3(0.0f,0.0f,3.0f);
glm::vec3 worldCentrol = glm::vec3(0.0f,0.0f,0.0f);
glm::vec3 cameraUp     = glm::vec3(0.0f,1.0f,0.0f);
const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
//构建ModelMatrix,进行”局部空间“到”世界空间“的转换
glm::vec3 transVec     = glm::vec3(0.0f,0.0f,0.0f);

GLfloat yaw   = 0.0f;  //偏航角
GLfloat pitch = 0.0f;  //俯仰角
GLfloat lastX = 0;
GLfloat lastY = 0;

ObjLoader model("cow.obj");
GLint vCnt = model.vNum;
GLint fCnt = model.fNum;
int cnt = (model.fSets).size();
const GLuint NumVertexShader = 1; //着色器数目
const GLuint NumFragmentShader = 1;
const GLuint NumShaderProgram = 1;
const GLuint NumVAO = 1;
const GLuint NumVBO = 1;
const GLuint NumEBO = 1;

GLuint texture;
GLuint IDVertexShader[NumVertexShader];
GLuint IDFragmentShader[NumFragmentShader];
GLuint IDShaderProgram[NumShaderProgram];
GLuint IDVAO[NumVAO];
GLuint IDVBO[NumVBO];
GLuint IDEBO[NumEBO];

const GLchar *vertexShaderSource =
          "#version 330 core\n"
          "layout(location = 0) in vec3 position;\n"
          "layout (location = 1) in vec2 texCoord;\n"
          "layout (location = 2) in vec3 normal;\n"

          "uniform mat4 model;\n"
          "uniform mat4 view;\n"
          "uniform mat4 projection;\n"

          "out vec2 TexCoord;\n"

          "void main()\n"
          "{\n"
          "  gl_Position =projection * view * model * vec4(position, 1.0f);\n"
          "  TexCoord = texCoord;\n"
          "}\n";


const GLchar *fragmentShaderSource =
          "#version 330 core\n"
          "in vec2 TexCoord;\n"
          "out vec4 color;\n"
          "uniform sampler2D texture_diffuse0;\n"
          "uniform sampler2D texture_diffuse1;\n"
          "uniform sampler2D texture_diffuse2;\n"
          "uniform sampler2D texture_specular0;\n"
          "uniform sampler2D texture_specular1;\n"
          "uniform sampler2D texture_specular2;\n"
          "void main()\n"
          "{\n"
          "  color = texture(texture_diffuse0,TexCoord);\n"
          "}\n";

openglwindow::openglwindow(QWidget* parent)
    :QOpenGLWidget(parent)
{
    //设置OpenGL的版本信息
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3,3);
    setFormat(format);
}

openglwindow::~openglwindow()
{

}

void openglwindow::initializeGL()
{

    initializeOpenGLFunctions();
    glClearColor(1.0f,1.0f,1.0f,1.0f);

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
    glBufferData(GL_ARRAY_BUFFER,model.vSets.size()*sizeof(GLfloat),&model.vSets[0],GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IDEBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,model.fSets.size()*sizeof(GLuint),&model.fSets[0],GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    glEnable(GL_DEPTH_TEST);

}
void openglwindow::paintGL()
{
    if(Mode == 1)
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    if(Mode == 0)
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    if(Mode == 2)
    {
     glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
     glPointSize(2.0f);
    //loadGLTextures();
    }

    glClear(GL_COLOR_BUFFER_BIT);
    //实现参数的刷新
    update();

    //loadGLTextures();

    glBindTexture(GL_TEXTURE_2D,texture);
    //渲染彩色正方体
    glUseProgram(IDShaderProgram[0]);

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
    glDrawElements(GL_TRIANGLES,cnt,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);

    //强制刷新缓冲区，保证命令的被执行
    glFlush();

}

void openglwindow::resizeGL(int width,int height)
{
//    qDebug() << width << height;
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

        GLfloat sensitivity = 0.01f;
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
         dummy.fill(Qt::green);
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
