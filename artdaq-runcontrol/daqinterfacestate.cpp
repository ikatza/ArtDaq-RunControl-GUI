#include "daqinterfacestate.h"
#include "ui_daqinterfacestate.h"

daqInterfaceState::daqInterfaceState(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::daqInterfaceState)
{
    ui->setupUi(this);
    imagesDirectory = QCoreApplication::applicationDirPath() + "/Images/";
    QGraphicsScene *scene = new QGraphicsScene();
    QPixmap m(imagesDirectory + "StateDiagram_all_off.png");
    scene->addPixmap(m);
    ui->graphicsView->setScene(scene);

    ui->labelButtonOnline->setText("");
    QPixmap button_image(imagesDirectory + "button_red.png");
    ui->labelButtonOnline->setPixmap(button_image);

    QPalette palette = ui->lcdPartitionNumber->palette();
    palette.setColor(QPalette::WindowText,Qt::red);
    ui->lcdPartitionNumber->setPalette(palette);

    palette = ui->lcdPortNumber->palette();
    palette.setColor(QPalette::WindowText,Qt::red);
    ui->lcdPortNumber->setPalette(palette);

    ui->labelOnline->setText("OFFLINE");
    palette = ui->labelOnline->palette();
    palette.setColor(QPalette::WindowText,Qt::red);
    ui->labelOnline->setPalette(palette);

    connect(&timerTransition, SIGNAL(timeout()), this, SLOT(setStateDiagramOff()));
    connect(&timerOnline, SIGNAL(timeout()), this, SLOT(setOnlineButtonLightGreen()));
}

daqInterfaceState::~daqInterfaceState()
{
    delete ui;
}

void daqInterfaceState::setStateDiagramOff(){
    QGraphicsScene *scene = new QGraphicsScene();
    QPixmap m(imagesDirectory + "StateDiagram_all_off.png");
    scene->addPixmap(m);
    ui->graphicsView->setScene(scene);
}


void daqInterfaceState::setStateDiagramStopped(){
    QGraphicsScene *scene = new QGraphicsScene();
    QPixmap m(imagesDirectory + "StateDiagram_stopped.png");
    scene->addPixmap(m);
    ui->graphicsView->setScene(scene);
}

void daqInterfaceState::setStateDiagramBooted(){
    QGraphicsScene *scene = new QGraphicsScene();
    QPixmap m(imagesDirectory + "StateDiagram_booted.png");
    scene->addPixmap(m);
    ui->graphicsView->setScene(scene);
}

void daqInterfaceState::setStateDiagramRunning(){
    QGraphicsScene *scene = new QGraphicsScene();
    QPixmap m(imagesDirectory + "StateDiagram_running.png");
    scene->addPixmap(m);
    ui->graphicsView->setScene(scene);
}

void daqInterfaceState::setStateDiagramReady(){
    QGraphicsScene *scene = new QGraphicsScene();
    QPixmap m(imagesDirectory + "StateDiagram_ready.png");
    scene->addPixmap(m);
    ui->graphicsView->setScene(scene);
}

void daqInterfaceState::setStateDiagramBooting(){
    QGraphicsScene *scene = new QGraphicsScene();
    QPixmap m(imagesDirectory + "StateDiagram_booting.png");
    scene->addPixmap(m);
    ui->graphicsView->setScene(scene);
    timerTransition.setSingleShot(true);
    timerTransition.start(500);
}

void daqInterfaceState::setStateDiagramConfiguring(){
    QGraphicsScene *scene = new QGraphicsScene();
    QPixmap m(imagesDirectory + "StateDiagram_configuring.png");
    scene->addPixmap(m);
    ui->graphicsView->setScene(scene);
    timerTransition.setSingleShot(true);
    timerTransition.start(500);
}

void daqInterfaceState::setStateDiagramStartingRun(){
    QGraphicsScene *scene = new QGraphicsScene();
    QPixmap m(imagesDirectory + "StateDiagram_starting_run.png");
    scene->addPixmap(m);
    ui->graphicsView->setScene(scene);
    timerTransition.setSingleShot(true);
    timerTransition.start(500);
}

void daqInterfaceState::setStateDiagramStoppingRun(){
    QGraphicsScene *scene = new QGraphicsScene();
    QPixmap m(imagesDirectory + "StateDiagram_stopping_run.png");
    scene->addPixmap(m);
    ui->graphicsView->setScene(scene);
    timerTransition.setSingleShot(true);
    timerTransition.start(500);
}

void daqInterfaceState::setStateDiagramTerminating(){
    QGraphicsScene *scene = new QGraphicsScene();
    QPixmap m(imagesDirectory + "StateDiagram_terminating.png");
    scene->addPixmap(m);
    ui->graphicsView->setScene(scene);
    timerTransition.setSingleShot(true);
    timerTransition.start(500);
}

void daqInterfaceState::setOnlineButtonGreen(){
    ui->labelButtonOnline->setText("");
    QPixmap button_image(imagesDirectory + "button_green.png");
    ui->labelButtonOnline->setPixmap(button_image);
    timerOnline.setSingleShot(true);
    timerOnline.start(500);
}

void daqInterfaceState::setOnlineButtonRed(){
    ui->labelButtonOnline->setText("");
    QPixmap button_image(imagesDirectory + "button_red.png");
    ui->labelButtonOnline->setPixmap(button_image);
}

void daqInterfaceState::setOnlineButtonLightGreen(){
    ui->labelButtonOnline->setText("");
    QPixmap button_image(imagesDirectory + "button_light_green.png");
    ui->labelButtonOnline->setPixmap(button_image);
}

void daqInterfaceState::setLCDPartitionNumber(int number){
    ui->lcdPartitionNumber->display(number);
}

void daqInterfaceState::setLCDPortNumber(int number){
    ui->lcdPortNumber->display(number);
}

void daqInterfaceState::isOnline(){

    QPalette palette = ui->lcdPartitionNumber->palette();
    palette.setColor(QPalette::WindowText,Qt::darkGreen);
    ui->lcdPartitionNumber->setPalette(palette);

    palette = ui->lcdPortNumber->palette();
    palette.setColor(QPalette::WindowText,Qt::darkGreen);
    ui->lcdPortNumber->setPalette(palette);

    ui->labelOnline->setText("ONLINE");
    palette = ui->labelOnline->palette();
    palette.setColor(QPalette::WindowText,Qt::darkGreen);
    palette.setColor(QPalette::Background,Qt::green);
    ui->labelOnline->setPalette(palette);
}

void daqInterfaceState::isOffline(){

    QPalette palette = ui->lcdPartitionNumber->palette();
    palette.setColor(QPalette::WindowText,Qt::red);
    ui->lcdPartitionNumber->setPalette(palette);

    palette = ui->lcdPortNumber->palette();
    palette.setColor(QPalette::WindowText,Qt::red);
    ui->lcdPortNumber->setPalette(palette);

    ui->labelOnline->setText("OFFLINE");
    palette = ui->labelOnline->palette();
    palette.setColor(QPalette::WindowText,Qt::red);
    ui->labelOnline->setPalette(palette);

    ui->lcdPartitionNumber->display(0);
    ui->lcdPortNumber->display(0);
}


