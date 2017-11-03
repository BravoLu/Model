#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QtWidgets>

int Mode = 0;
QString filename ="/images/china.png";
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    openAction = new QAction(QIcon(":/images/file-open.png"), tr("&载入模型"), this);
    ui->action_10->setShortcut(QKeySequence::Open);
    openAction->setShortcuts(QKeySequence::Open);
    ui->action_10->setStatusTip(tr("载入模型"));
    openAction->setStatusTip(tr("载入模型"));
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->action_10, &QAction::triggered, this, &MainWindow::openFile);


    saveAction = new QAction(QIcon(":/images/file-save.png"), tr("&保存模型"), this);
    saveAction->setShortcuts(QKeySequence::Save);
    ui->action_11->setShortcuts(QKeySequence::Save);
    saveAction->setStatusTip(tr("保存模型"));
    ui->action_11->setStatusTip(tr("保存模型"));
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    connect(ui->action_11, &QAction::triggered, this, &MainWindow::saveFile);

    ui->action_5->setStatusTip(tr("点模型"));
    connect(ui->action_5,&QAction::triggered,this,&MainWindow::pointModel);

    ui->action_6->setStatusTip(tr("线框模型"));
    connect(ui->action_6,&QAction::triggered,this,&MainWindow::lineModel);


    ui->action_7->setStatusTip(tr("面模型"));
    connect(ui->action_7,&QAction::triggered,this,&MainWindow::faceModel);

    texture_1 = new QAction(QIcon(":/images/wall.png"),tr("&Texture1..."),this);
    connect(texture_1,&QAction::triggered, this, &MainWindow::Texture_1);
    connect(ui->action_2,&QAction::triggered, this, &MainWindow::Texture_1);

    texture_2 = new QAction(QIcon(":/images/lishi.png"),tr("&Texture2..."),this);
    connect(texture_2,&QAction::triggered, this, &MainWindow::Texture_2);
    connect(ui->action_3,&QAction::triggered, this, &MainWindow::Texture_2);


    texture_3 = new QAction(QIcon(":/images/china.png"),tr("&Texture3..."),this);
    connect(texture_3,&QAction::triggered, this, &MainWindow::Texture_3);
    connect(ui->action_4,&QAction::triggered, this, &MainWindow::Texture_3);


    connect(ui->action_15,&QAction::triggered, this, &MainWindow::help);

//    QMenu *file = menuBar()->addMenu(tr("&File"));
//    file->addAction(openAction);
//    file->addAction(saveAction);

     QToolBar *toolBar = addToolBar(tr("&File"));
     toolBar->addAction(openAction);
     toolBar->addAction(saveAction);
     toolBar->addAction(texture_1);
     toolBar->addAction(texture_2);
     toolBar->addAction(texture_3);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    mWidth = ui->centralWidget->width();
    mHeight = ui->centralWidget->height();
    ui->widget->setGeometry(0,0,mWidth,mHeight);
}
void MainWindow::saveFile()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save File"), ".", tr("png File(*.png)"));
    
}

void MainWindow::openFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("obj Files(*.obj)"));
}
void MainWindow::Texture_1(){
    filename = ":/images/wall3.png";

}
void MainWindow::Texture_2(){
    filename = ":/images/纹理1.png";
}
void MainWindow::Texture_3(){
    filename = ":/images/纹理.png";
}

void MainWindow::help(){
    QDialog *dialog;
    dialog = new QDialog();
    dialog->setWindowTitle(tr("帮助(?)"));
    QTextBrowser *text = new QTextBrowser(dialog);
    text->setPlainText("This is a help documentory\n\
by lsh");
    text->resize(600,600);
    dialog->resize(600,600);
    dialog->exec();
}

void MainWindow::lineModel()
{
    Mode = 1; // 线框模型
}
void MainWindow::faceModel()
{
    Mode = 0;
}
void MainWindow::pointModel()
{
    Mode = 2;
}
