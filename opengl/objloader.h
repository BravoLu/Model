#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <vector>
#include <string>
#include <QOpenGLFunctions_3_3_Core.h>
using namespace std;

class ObjLoader{

public:

    ObjLoader(string filename);
    //void Init();
    void Draw();

public:

    //GLint lastFrameTime;
    //GLint animationLastingTime;

    vector<GLfloat>vSets;
    GLint vNum;
    vector<GLuint> fSets;  //绘制三角形,32个三角形
    vector<GLuint> FSets;  //绘制矩形，480个矩形
    GLint fNum;
    GLint FNum;
    vector<GLfloat>vnSets;
    vector<GLfloat>vtSets;

};
#endif // OBJLOADER_H
