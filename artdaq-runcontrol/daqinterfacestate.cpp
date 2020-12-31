#include "daqinterfacestate.hpp"
#include "ui_daqinterfacestate.h"

daqInterfaceState::daqInterfaceState(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::daqInterfaceState)
{
  ui->setupUi(this);
  imagesDirectory = QCoreApplication::applicationDirPath() + "/../resources/images/";
  qDebug() << Q_FUNC_INFO << "imagesDirectory: " << imagesDirectory;
  this->scene = new QGraphicsScene();
  QImage image;
  image.load(imagesDirectory + "StateDiagram_all_off.png", "PNG");
  QPixmap pixmapImage = QPixmap::fromImage(image);
  scene->addPixmap(pixmapImage);
  ui->graphicsView->setScene(this->scene);

  ui->labelButtonOnline->setText("");
  image.load(imagesDirectory + "button_red.png", "PNG");
  pixmapImage = QPixmap::fromImage(image);
  ui->labelButtonOnline->setPixmap(pixmapImage);

  QPalette palette = ui->lcdPartitionNumber->palette();
  palette.setColor(QPalette::WindowText, Qt::red);
  ui->lcdPartitionNumber->setPalette(palette);

  palette = ui->lcdPortNumber->palette();
  palette.setColor(QPalette::WindowText, Qt::red);
  ui->lcdPortNumber->setPalette(palette);

  palette = ui->lcdRunNumber->palette();
  palette.setColor(QPalette::WindowText, Qt::red);
  ui->lcdRunNumber->setPalette(palette);

  ui->labelOnline->setText("OFFLINE");
  palette = ui->labelOnline->palette();
  palette.setColor(QPalette::WindowText, Qt::red);
  ui->labelOnline->setPalette(palette);

  connect(&timerTransition, SIGNAL(timeout()), this, SLOT(setStateDiagramOff()));
  connect(&timerOnline, SIGNAL(timeout()), this, SLOT(setOnlineButtonLightGreen()));
}

daqInterfaceState::~daqInterfaceState()
{
  delete ui;
  delete scene;
}

void daqInterfaceState::setStateDiagramOff()
{
  QGraphicsScene *scene = ui->graphicsView->scene();
  scene->clear();
  QImage image;
  image.load(imagesDirectory + "StateDiagram_all_off.png", "PNG");
  QPixmap pixmapImage = QPixmap::fromImage(image);
  scene->addPixmap(pixmapImage);
}


void daqInterfaceState::setStateDiagramStopped()
{
  QGraphicsScene *scene = ui->graphicsView->scene();
  scene->clear();
  QImage image;
  image.load(imagesDirectory + "StateDiagram_stopped.png", "PNG");
  QPixmap pixmapImage = QPixmap::fromImage(image);
  scene->addPixmap(pixmapImage);
  this->setRunNumberLCDRed();
}

void daqInterfaceState::setStateDiagramBooted()
{
  QGraphicsScene *scene = ui->graphicsView->scene();
  scene->clear();
  QImage image;
  image.load(imagesDirectory + "StateDiagram_booted.png", "PNG");
  QPixmap pixmapImage = QPixmap::fromImage(image);
  scene->addPixmap(pixmapImage);
}

void daqInterfaceState::setStateDiagramRunning()
{
  QGraphicsScene *scene = ui->graphicsView->scene();
  scene->clear();
  QImage image;
  image.load(imagesDirectory + "StateDiagram_running.png", "PNG");
  QPixmap pixmapImage = QPixmap::fromImage(image);
  scene->addPixmap(pixmapImage);
  this->setRunNumberLCDGreen();
  this->setIsRunning(true);
}

void daqInterfaceState::setStateDiagramReady()
{
  QGraphicsScene *scene = ui->graphicsView->scene();
  scene->clear();
  QImage image;
  image.load(imagesDirectory + "StateDiagram_ready.png", "PNG");
  QPixmap pixmapImage = QPixmap::fromImage(image);
  scene->addPixmap(pixmapImage);
  this->setRunNumberLCDRed();
  this->setRun_number(gui_utility_functions::getRunNumber());
}

void daqInterfaceState::setStateDiagramBooting()
{
  QGraphicsScene *scene = ui->graphicsView->scene();
  scene->clear();
  QImage image;
  image.load(imagesDirectory + "StateDiagram_booting.png", "PNG");
  QPixmap pixmapImage = QPixmap::fromImage(image);
  scene->addPixmap(pixmapImage);
  timerTransition.setSingleShot(true);
  timerTransition.start(500);
}

void daqInterfaceState::setStateDiagramConfiguring()
{
  QGraphicsScene *scene = ui->graphicsView->scene();
  scene->clear();
  QImage image;
  image.load(imagesDirectory + "StateDiagram_configuring.png", "PNG");
  QPixmap pixmapImage = QPixmap::fromImage(image);
  scene->addPixmap(pixmapImage);
  timerTransition.setSingleShot(true);
  timerTransition.start(500);
}

void daqInterfaceState::setStateDiagramStartingRun()
{
  QGraphicsScene *scene = ui->graphicsView->scene();
  scene->clear();
  QImage image;
  image.load(imagesDirectory + "StateDiagram_starting_run.png", "PNG");
  QPixmap pixmapImage = QPixmap::fromImage(image);
  scene->addPixmap(pixmapImage);
  timerTransition.setSingleShot(true);
  timerTransition.start(500);
  this->setIsRunning(false);
}

void daqInterfaceState::setStateDiagramStoppingRun()
{
  QGraphicsScene *scene = ui->graphicsView->scene();
  scene->clear();
  QImage image;
  image.load(imagesDirectory + "StateDiagram_stopping_run.png", "PNG");
  QPixmap pixmapImage = QPixmap::fromImage(image);
  scene->addPixmap(pixmapImage);
  timerTransition.setSingleShot(true);
  timerTransition.start(500);
  this->setIsRunning(false);
}

void daqInterfaceState::setStateDiagramTerminating()
{
  QGraphicsScene *scene = ui->graphicsView->scene();
  scene->clear();
  QImage image;
  image.load(imagesDirectory + "StateDiagram_terminating.png", "PNG");
  QPixmap pixmapImage = QPixmap::fromImage(image);
  scene->addPixmap(pixmapImage);
  timerTransition.setSingleShot(true);
  timerTransition.start(500);
}

void daqInterfaceState::setOnlineButtonGreen()
{
  ui->labelButtonOnline->setText("");
  QImage image;
  image.load(imagesDirectory + "button_green.png", "PNG");
  QPixmap pixmapImage = QPixmap::fromImage(image);
  ui->labelButtonOnline->setPixmap(pixmapImage);
  this->setRun_number(gui_utility_functions::getRunNumber());
  if(this->IsRunning()) {
    this->setLCDRunNumber(this->getRun_number());
  }
  timerOnline.setSingleShot(true);
  timerOnline.start(500);
}

void daqInterfaceState::setOnlineButtonRed()
{
  ui->labelButtonOnline->setText("");
  QImage image;
  image.load(imagesDirectory + "button_red.png", "PNG");
  QPixmap pixmapImage = QPixmap::fromImage(image);
  ui->labelButtonOnline->setPixmap(pixmapImage);
}

void daqInterfaceState::setOnlineButtonLightGreen()
{
  ui->labelButtonOnline->setText("");
  QImage image;
  image.load(imagesDirectory + "button_light_green.png", "PNG");
  QPixmap pixmapImage = QPixmap::fromImage(image);
  ui->labelButtonOnline->setPixmap(pixmapImage);
}

int daqInterfaceState::getRun_number() const
{
  return run_number;
}

void daqInterfaceState::setRun_number(int run_number)
{
  this->run_number = run_number;
}

void daqInterfaceState::setLCDPartitionNumber(int number)
{
  ui->lcdPartitionNumber->display(number);
}

void daqInterfaceState::setLCDPortNumber(int number)
{
  ui->lcdPortNumber->display(number);
}

void daqInterfaceState::setLCDRunNumber(int number)
{
  ui->lcdRunNumber->display(number);
}

void daqInterfaceState::setOnline()
{

  QPalette palette = ui->lcdPartitionNumber->palette();
  palette.setColor(QPalette::WindowText, Qt::darkGreen);
  ui->lcdPartitionNumber->setPalette(palette);

  palette = ui->lcdPortNumber->palette();
  palette.setColor(QPalette::WindowText, Qt::darkGreen);
  ui->lcdPortNumber->setPalette(palette);

  ui->labelOnline->setText("ONLINE");
  palette = ui->labelOnline->palette();
  palette.setColor(QPalette::WindowText, Qt::darkGreen);
  palette.setColor(QPalette::Background, Qt::green);
  ui->labelOnline->setPalette(palette);
}

void daqInterfaceState::setRunNumberLCDGreen()
{
  QPalette palette = ui->lcdRunNumber->palette();
  palette.setColor(QPalette::WindowText, Qt::darkGreen);
  ui->lcdRunNumber->setPalette(palette);
}

bool daqInterfaceState::IsRunning() const
{
  return isRunning;
}

void daqInterfaceState::setIsRunning(bool value)
{
  isRunning = value;
}

void daqInterfaceState::setRunNumberLCDRed()
{
  QPalette palette = ui->lcdRunNumber->palette();
  palette.setColor(QPalette::WindowText, Qt::red);
  ui->lcdRunNumber->setPalette(palette);
}

void daqInterfaceState::setOffline()
{

  timerOnline.stop();
  timerTransition.stop();

  QPalette palette = ui->lcdPartitionNumber->palette();
  palette.setColor(QPalette::WindowText, Qt::red);
  ui->lcdPartitionNumber->setPalette(palette);

  palette = ui->lcdPortNumber->palette();
  palette.setColor(QPalette::WindowText, Qt::red);
  ui->lcdPortNumber->setPalette(palette);

  palette = ui->lcdRunNumber->palette();
  palette.setColor(QPalette::WindowText, Qt::red);
  ui->lcdRunNumber->setPalette(palette);

  ui->labelOnline->setText("OFFLINE");
  palette = ui->labelOnline->palette();
  palette.setColor(QPalette::WindowText, Qt::red);
  ui->labelOnline->setPalette(palette);

  ui->lcdPartitionNumber->display(0);
  ui->lcdPortNumber->display(0);
  ui->lcdRunNumber->display(0);
}
