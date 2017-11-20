#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QtWidgets>
#include <QFont>
#include <QPalette>
#include <string>
#include <QFileInfo>

int autoRound = 0;
int color = 0;
int LightMode = 1;
int Mode = 0;
float Shininess = 0.5;
QString modelname = "c.obj";
QString filename ="/images/resources/china.png";
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{



    ui->setupUi(this);
    openAction = new QAction(QIcon(":/images/resources/file-open.png"), tr("&载入模型"), this);
    ui->action_10->setShortcut(QKeySequence::Open);
    openAction->setShortcuts(QKeySequence::Open);
    ui->action_10->setStatusTip(tr("载入模型"));
    openAction->setStatusTip(tr("载入模型"));
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->action_10, &QAction::triggered, this, &MainWindow::openFile);


    saveAction = new QAction(QIcon(":/images/resources/file-save.png"), tr("&保存模型"), this);
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

    texture_1 = new QAction(QIcon(":/images/resources/wall.png"),tr("&Texture1..."),this);
    connect(texture_1,&QAction::triggered, this, &MainWindow::Texture_1);
    connect(ui->action_2,&QAction::triggered, this, &MainWindow::Texture_1);

    texture_2 = new QAction(QIcon(":/images/resources/lishi.png"),tr("&Texture2..."),this);
    connect(texture_2,&QAction::triggered, this, &MainWindow::Texture_2);
    connect(ui->action_3,&QAction::triggered, this, &MainWindow::Texture_2);


    texture_3 = new QAction(QIcon(":/images/resources/china.png"),tr("&Texture3..."),this);
    connect(texture_3,&QAction::triggered, this, &MainWindow::Texture_3);
    connect(ui->action_4,&QAction::triggered, this, &MainWindow::Texture_3);

    connect(ui->action_20,&QAction::triggered, this, &MainWindow::Texture_4);
    connect(ui->action_21,&QAction::triggered, this, &MainWindow::Texture_5);
    connect(ui->action_22,&QAction::triggered, this, &MainWindow::Texture_6);
    connect(ui->action_23,&QAction::triggered, this, &MainWindow::Texture_7);
    connect(ui->action_24,&QAction::triggered, this, &MainWindow::Texture_8);
    connect(ui->action_25,&QAction::triggered, this, &MainWindow::Texture_9);
    connect(ui->action_26,&QAction::triggered, this, &MainWindow::Texture_10);
    connect(ui->action_27,&QAction::triggered, this, &MainWindow::Texture_none);
    connect(ui->action_28,&QAction::triggered, this, &MainWindow::loadTexture);

    AutoTurnRound = new QAction(QIcon(":/images/resources/start.png"),tr("&自动旋转"),this);
    connect(AutoTurnRound, &QAction::triggered,this,&MainWindow::AutoTurnRoundFun);
    connect(ui->action_R_Ctrl_R,&QAction::triggered,this,&MainWindow::AutoTurnRoundFun);


    //光源选择
    connect(ui->action_12, &QAction::triggered, this ,&MainWindow::PointLight);
    connect(ui->action_14, &QAction::triggered, this ,&MainWindow::DirectionalLight);
    connect(ui->action_19,&QAction::triggered, this ,&MainWindow::FlashLight);

    //颜色选择
    connect(ui->action, &QAction::triggered, this ,&MainWindow::greenColor);
    connect(ui->action_13, &QAction::triggered, this ,&MainWindow::redColor);
    connect(ui->action_16, &QAction::triggered, this ,&MainWindow::blueColor);
    connect(ui->action_17, &QAction::triggered, this ,&MainWindow::whiteColor);

    //帮助
    connect(ui->action_15,&QAction::triggered, this, &MainWindow::help);
    connect(ui->action_35,&QAction::triggered, this, &MainWindow::develop);

     QToolBar *toolBar = addToolBar(tr("&File"));
     toolBar->addAction(openAction);
     toolBar->addAction(saveAction);
     toolBar->addAction(texture_1);
     toolBar->addAction(texture_2);
     toolBar->addAction(texture_3);
     toolBar->addAction(AutoTurnRound);

     ui->verticalSlider->setMinimum(0);
     ui->verticalSlider->setMaximum(100);
     ui->verticalSlider->setValue(50);

     //设置字体颜色
     QPalette pa;
     pa.setColor(QPalette::WindowText,Qt::white);
     ui->label->setPalette(pa);
     connect(ui->verticalSlider,&QSlider::sliderMoved,this,&MainWindow::dragSlider);
     connect(ui->verticalSlider,&QSlider::valueChanged,this,&MainWindow::dragSlider);
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
    ui->verticalSlider->setGeometry(mWidth-100,50,41,596);
    ui->label->setGeometry(mWidth-110,650,72,21);
}
void MainWindow::saveFile()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save File"), ".", tr("png File(*.png)"));
    
}

void MainWindow::openFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("obj Files(*.obj)"));
    modelname = path;

}
void MainWindow::Texture_1(){
    filename = ":/images/resources/wall3.png";

}
void MainWindow::Texture_2(){
    filename = ":/images/resources/纹理1.png";
}
void MainWindow::Texture_3(){
    filename = ":/images/resources/纹理.png";
}
void MainWindow::Texture_4(){
    filename = ":/images/resources/bricks2.png";
}
void MainWindow::Texture_5(){
    filename = ":/images/resources/wood.png";
}
void MainWindow::Texture_6(){
    filename = ":/images/resources/marble.png";
}
void MainWindow::Texture_7(){
    filename = ":/images/resources/metal.png";
}
void MainWindow::Texture_8(){
    filename = ":/images/resources/bricks2_disp.png";
}
void MainWindow::Texture_9(){
    filename = ":/images/resources/bricks2_normal.png";
}
void MainWindow::Texture_10(){
    filename = ":/images/resources/toy_box_diffuse.png";
}
void MainWindow::Texture_none(){
    filename = "";
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
    Mode = 0; //面模型
}
void MainWindow::pointModel()
{
    Mode = 2; //点模型
}
void MainWindow::PointLight()
{
    LightMode = 1;
}
void MainWindow::DirectionalLight()
{
    LightMode = 0;
}
void MainWindow::FlashLight(){
    LightMode = 2;
}

void MainWindow::AutoTurnRoundFun()
{
    autoRound = 1;
}
void MainWindow::greenColor()
{
    color = 1;
}
void MainWindow::redColor()
{
    color = 2;
}
void MainWindow::blueColor()
{
    color = 3;
}
void MainWindow::whiteColor()
{
    color = 0;
}
void MainWindow::dragSlider(){
    Shininess = ui->verticalSlider->value() / (float) 100 ;
}
void MainWindow::develop()
{
    QDialog *dialog;
    dialog = new QDialog();
    dialog->setWindowTitle(tr("开发人员"));
    QTextBrowser *text = new QTextBrowser(dialog);
    text->setPlainText("开发人员：卢少豪\n开发时间：2017-10 ~ 2017-12\n");
    text->resize(300,300);
    dialog->resize(300,300);
    dialog->exec();
}

void MainWindow::loadTexture()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open picture"), ".", tr("obj Files(*.png)"));
    QString file_name,file_path,file_suffix;
    QFileInfo fileinfo;
    fileinfo = QFileInfo(path);
    //文件名
    file_name = fileinfo.fileName();
    //文件后缀
    file_suffix = fileinfo.suffix();
    //绝对路径
    file_path = fileinfo.absolutePath();

    filename = path;

}
