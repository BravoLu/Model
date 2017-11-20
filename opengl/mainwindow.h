#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include <QtOpenGL/QGLWidget>
#include "openglwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static QString texoption;
private:
    Ui::MainWindow *ui;
    int mWidth;
    int mHeight;

    void resizeEvent(QResizeEvent *event);
private:
    QAction *openAction;
    QAction *saveAction;
    QAction *texture_1;
    QAction *texture_2;
    QAction *texture_3;
    QAction *AutoTurnRound;

private slots:
    void openFile();
    void saveFile();
    void Texture_1();
    void Texture_2();
    void Texture_3();
    void Texture_4();
    void Texture_5();
    void Texture_6();
    void Texture_7();
    void Texture_8();
    void Texture_9();
    void Texture_10();
    void greenColor();
    void redColor();
    void blueColor();
    void whiteColor();
    void Texture_none();
    void help();
    void lineModel();
    void faceModel();
    void pointModel();
    void AutoTurnRoundFun();
    void dragSlider();
    void PointLight();
    void DirectionalLight();
    void FlashLight();
    void develop();
    void loadTexture();
};


#endif // MAINWINDOW_H
