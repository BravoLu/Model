#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QtOpenGL/QGLWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include <iterator>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>


extern int Mode;
extern QString filename  ;
class openglwindow : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
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
