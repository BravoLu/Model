#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H


#include <QOpenGLWidget>
#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <QOpenGLFunctions_3_0>
#include <objmode.h>

extern QString modelname;
extern int autoRound;
extern int color;
extern int LightMode;
extern int Mode;
extern float Shininess;
extern QString filename  ;
class openglwindow : public QOpenGLWidget, protected QOpenGLFunctions_3_0
{
    Q_OBJECT
public:
    openglwindow(QWidget *parent = 0);
    ~openglwindow();
    void initializeGL();
    void render();
    void resizeGL(int width,int height);
    void paintGL();
    void loadGLTextures();
    void init();
    void setLightRes();
//    void draw(const Shader& shader);
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);

//public slots:
//    void Texture_1();
//    void Texture_2();
//    void Texture_3();
};

#endif // OPENGLWINDOW_H
