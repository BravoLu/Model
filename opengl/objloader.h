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
    vector<GLuint>fSets;
    GLint fNum;
    vector<GLfloat>vnSets;
    vector<GLfloat>vtSets;

};
#endif // OBJLOADER_H
