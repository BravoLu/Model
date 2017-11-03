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


private slots:
    void openFile();
    void saveFile();
    void Texture_1();
    void Texture_2();
    void Texture_3();
    void help();
    void lineModel();
    void faceModel();
    void pointModel();

};

#endif // MAINWINDOW_H
