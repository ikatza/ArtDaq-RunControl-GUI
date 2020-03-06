#include "mainwindow.hpp"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  configurateWindow();
  DAQState = 0;
  banBOOT = false, banCONFIG = false, banBOOTCONFIG = false, banBOOTED = false, banCONFIGURED = false;
  banRUNNING = false, banPAUSED = false;
  QProcess* daqinterface_pointer = new QProcess(this);
  daqinterface_pointer = &daq_interface;
  ui->lbStatus->setText("");
  QProcess* kill_proc = new QProcess(this);
  kill_proc->start("pkill", QStringList() << "-f" << "pmt.rb" << "-u" << env_vars::user);
  kill_proc->execute("pkill", QStringList() << "-f" << "daqinterface.py" << "-u" << env_vars::user);
  kill_proc->waitForFinished();
  kill_proc->~QProcess();
  connect(ui->bDAQInterface, SIGNAL(clicked(bool)), this, SLOT(bDAQInterfacePressed()));
  connect(daqinterface_pointer, SIGNAL(readyReadStandardOutput()), this, SLOT(DAQInterfaceOutput()));
  connect(&DAQInterface_logwatcher, SIGNAL(fileChanged(QString)), this, SLOT(bDebugPressed()));
  connect(ui->bBelen, SIGNAL(clicked(bool)), this, SLOT(MensajeParaBelen()));
  connect(ui->bDAQcomp, SIGNAL(clicked(bool)), this, SLOT(bListDAQComps()));
  connect(ui->bDAQconf, SIGNAL(clicked(bool)), this, SLOT(bListDAQConfigs()));
  connect(ui->lvComponents, SIGNAL(clicked(QModelIndex)), this, SLOT(lvComponentsSelected()));
  connect(ui->lvConfigurations, SIGNAL(clicked(QModelIndex)), this, SLOT(lvConfigurationsSelected()));
  connect(ui->lvConfigBOOT, SIGNAL(clicked(QModelIndex)), this, SLOT(lvBOOTConfigSelected()));
  connect(ui->bBOOT, SIGNAL(clicked(bool)), this, SLOT(bBOOTPressed()));
  connect(ui->bCONFIG, SIGNAL(clicked(bool)), this, SLOT(bCONFIGPressed()));
  connect(ui->bStart, SIGNAL(clicked(bool)), this, SLOT(bSTARTPressed()));
  connect(&timer, SIGNAL(timeout()), this, SLOT(checkStatus()));
  connect(ui->bStop, SIGNAL(clicked(bool)), this, SLOT(bSTOPPressed()));
  connect(ui->bTerminate, SIGNAL(clicked(bool)), this, SLOT(bTERMINATEPressed()));
  connect(ui->bEndSession, SIGNAL(clicked(bool)), this, SLOT(bEndSessionPressed()));
  connect(ui->bDebug, SIGNAL(clicked(bool)), this, SLOT(bDebugPressed()));
  connect(ui->bListDatabaseRunConfigurations, SIGNAL(clicked(bool)), this, SLOT(bListDatabaseRunConfigurations()));
  connect(ui->checkBoxDatabase, SIGNAL(toggled(bool)), this, SLOT(checkBoxDatabaseChanged()));
  connect(ui->bStartRun, SIGNAL(clicked(bool)), this, SLOT(bStartRunPressed()));
  initializeButtons();
  configurateMenuBar();
  state_diagram.setWindowTitle("DAQInterface State Diagram");
  state_diagram.setFixedSize(state_diagram.geometry().width(), state_diagram.geometry().height());
  state_diagram.show();
}

MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::configurateWindow()
{
  qDebug() << "Starting function";
  this->setWindowTitle("ARTDAQ RUN CONTROL");
  //this->setFixedSize(this->geometry().width(), this->geometry().height());
  ui->taDAQInterface->setReadOnly(true);
  this->setDBConfigurationFHICL_dir(env_vars::env.value("HOME") + "/work-db-v4-dir");
  this->originalWindowSize = this->geometry().size();
  this->originalQuadraticMeanConfigurationFontSize = (double)qSqrt((qreal)(this->originalWindowSize.height() * this->originalWindowSize.height() + this->originalWindowSize.width() * this->originalWindowSize.width()));
  this->gbDAQInterfaceCommandsPosition = ui->groupBox_DAQInterfaceCommands->pos();
  this->gbDAQInterfaceCommandsSize = ui->groupBox_DAQInterfaceCommands->geometry().size();
  this->gbTransitionCommandsPosition = ui->groupBox_TransitionCommands->pos();
  this->gbTransitionCommandsSize = ui->groupBox_TransitionCommands->geometry().size();
  this->gbDatabasePosition = ui->groupBox_Database->pos();
  this->gbDatabaseSize = ui->groupBox_Database->geometry().size();
  this->gbDAQInterfacePosition = ui->groupBox_DAQInterface->pos();
  this->gbDAQInterfaceSize = ui->groupBox_DAQInterface->geometry().size();
  this->bStartPosition = ui->bStart->pos();
  this->bStartSize = ui->bStart->geometry().size();
  this->bStopPosition = ui->bStop->pos();
  this->bStopSize = ui->bStop->geometry().size();
  this->bPausePosition = ui->bPause->pos();
  this->bPauseSize = ui->bPause->geometry().size();
  this->bTerminatePosition = ui->bTerminate->pos();
  this->bTerminateSize = ui->bTerminate->geometry().size();
  this->bStartRunPosition = ui->bStartRun->pos();
  this->bStartRunSize = ui->bStartRun->geometry().size();
  this->bListDatabaseRunConfigurationsPosition = ui->bListDatabaseRunConfigurations->pos();
  this->bListDatabaseRunConfigurationsSize = ui->bListDatabaseRunConfigurations->geometry().size();
  this->bDAQcompPosition = ui->bDAQcomp->pos();
  this->bDAQcompSize = ui->bDAQcomp->geometry().size();
  this->bDAQconfPosition = ui->bDAQconf->pos();
  this->bDAQconfSize = ui->bDAQconf->geometry().size();
  this->bBOOTPosition = ui->bBOOT->pos();
  this->bBOOTSize = ui->bBOOT->geometry().size();
  this->bCONFIGPosition = ui->bCONFIG->pos();
  this->bCONFIGSize = ui->bCONFIG->geometry().size();
  this->bDAQInterfacePosition = ui->bDAQInterface->pos();
  this->bDAQInterfaceSize = ui->bDAQInterface->geometry().size();
  this->bEndSessionPosition = ui->bEndSession->pos();
  this->bEndSessionSize = ui->bEndSession->geometry().size();
  this->taDAQInterfacePosition = ui->taDAQInterface->pos();
  this->taDAQInterfaceSize = ui->taDAQInterface->geometry().size();
  this->checkBoxDatabasePosition = ui->checkBoxDatabase->pos();
  this->checkBoxDatabaseSize = ui->checkBoxDatabase->geometry().size();
  this->lvComponentsPosition = ui->lvComponents->pos();
  this->lvComponentsSize = ui->lvComponents->geometry().size();
  this->lvConfigurationsPosition = ui->lvConfigurations->pos();
  this->lvConfigurationsSize = ui->lvConfigurations->geometry().size();
  this->lvConfigBOOTPosition = ui->lvConfigBOOT->pos();
  this->lvConfigBOOTSize = ui->lvConfigBOOT->geometry().size();
  this->lbConfigurationsPosition = ui->lbConfigurations->pos();
  this->lbConfigurationsSize = ui->lbConfigurations->geometry().size();
  this->lbComponentsPosition = ui->lbComponents->pos();
  this->lbComponentsSize = ui->lbComponents->geometry().size();
  this->lbBOOTConfigPosition = ui->lbBOOTConfig->pos();
  this->lbBOOTConfigSize = ui->lbBOOTConfig->geometry().size();
  this->lbStatusTitlePosition = ui->lbStatusTitle->pos();
  this->lbStatusTitleSize = ui->lbStatusTitle->geometry().size();
  this->lbStatusPosition = ui->lbStatus->pos();
  this->lbStatusSize = ui->lbStatus->geometry().size();
  this->lbMessagesPosition = ui->lbMessages->pos();
  this->lbMessagesSize = ui->lbMessages->geometry().size();
  this->lbConfigurationsFont = ui->lbConfigurations->font();
  this->lbComponentsFont = ui->lbComponents->font();
  this->lbBOOTConfigFont = ui->lbBOOTConfig->font();
  this->lbStatusFont = ui->lbStatus->font();
  this->lbStatusTitleFont = ui->lbStatusTitle->font();
  this->bStartRunFont = ui->bStartRun->font();
  this->gbDAQInterfaceCommandsFont = ui->groupBox_DAQInterfaceCommands->font();
  this->gbDAQInterfaceFont = ui->groupBox_DAQInterface->font();
  this->taDAQInterfaceFont = ui->taDAQInterface->font();
  this->lvComponentsFont = ui->lvComponents->font();
  this->lvBOOTConfigFont = ui->lvConfigBOOT->font();
  this->lvConfigurationsFont = ui->lvConfigurations->font();
  qDebug() << "Ending function";
}

void MainWindow::configurateMenuBar()
{
  qDebug() << "Starting function";
  QAction *optionsMenu = new QAction("&Options", this);
  QAction *exitMenu = new QAction("&Exit", this);
  QAction *windowMenu = new QAction("&Show state diagram", this);
  QMenu *Tools;
  Tools = menuBar()->addMenu("&File");
  Tools->addAction(exitMenu);
  Tools = menuBar()->addMenu("&Tools");
  Tools->addAction(optionsMenu);
  Tools = menuBar()->addMenu("&Window");
  Tools->addAction(windowMenu);
  connect(optionsMenu, SIGNAL(triggered(bool)), this, SLOT(openMenuOptionsDialog()));
  connect(exitMenu, SIGNAL(triggered(bool)), this, SLOT(closeProgram()));
  connect(windowMenu, SIGNAL(triggered(bool)), this, SLOT(showDaqInterfaceStateWindow()));
  qDebug() << "Ending function";
}

void MainWindow::showDaqInterfaceStateWindow()
{
  qDebug() << "Starting function";
  state_diagram.show();
  qDebug() << "Ending function";
}

void MainWindow::closeProgram()
{
  qDebug() << "Starting function";
  QMessageBox msgBox;
  msgBox.setText("Closing Program");
  msgBox.setInformativeText("Do you really wish to close the program?\n All the artDAQ processes will be destroyed ");
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(QMessageBox::No);
  int ret = msgBox.exec();
  QProcess* kill_p = new QProcess(this);
  switch (ret) {
  case QMessageBox::Yes:
    kill_p->start("pkill", QStringList() << "-f" << "pmt.rb" << "-u" << env_vars::user);
    kill_p->execute("pkill", QStringList() << "-f" << "daqinterface.py" << "-u" << env_vars::user);
    exit(0);
    break;
  case QMessageBox::No:
    break;
  default:
    break;
  }
  qDebug() << "Ending function";
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  qDebug() << "Starting function";
  QProcess* kill_p = new QProcess(this);
  QMessageBox::StandardButton msgBox = QMessageBox::question( this, "artdaqRunControl",
                                       tr("Do you really wish to close the program?\n All the artDAQ processes will be destroyed"),
                                       QMessageBox::No | QMessageBox::Yes,
                                       QMessageBox::Yes);
  if (msgBox != QMessageBox::Yes) {
    event->ignore();
  }
  else {
    kill_p->start("pkill", QStringList() << "-f" << "pmt.rb" << "-u" << env_vars::user);
    kill_p->execute("pkill", QStringList() << "-f" << "daqinterface.py" << "-u" << env_vars::user);
    event->accept();
    exit(0);
  }
  qDebug() << "Ending function";
}


void MainWindow::bEndSessionPressed()
{
  qDebug() << "Starting function";
  QMessageBox msgBox;
  msgBox.setText("End session");
  msgBox.setInformativeText("Do you really wish to end the session?\n All the artDAQ processes will be destroyed ");
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(QMessageBox::No);
  int ret = msgBox.exec();
  QProcess* kill_p = new QProcess(this);
  switch (ret) {
  case QMessageBox::Yes:
    kill_p->start("pkill", QStringList() << "-f" << "pmt.rb" << "-u" << env_vars::user);
    kill_p->execute("pkill", QStringList() << "-f" << "daqinterface.py" << "-u" << env_vars::user);
    initializeButtons();
    initializeLV();
    timer.stop();
    banBOOT = false;
    banBOOTCONFIG = false;
    banBOOTED = false;
    banCONFIG = false;
    banCONFIGURED = false;
    banPAUSED = false;
    banRUNNING = false;
    banStartRunPressed = false;
    break;
  case QMessageBox::No:
    break;
  default:
    break;
  }
  status("offline");
  qDebug() << "Ending function";
}

void MainWindow::initializeButtons()
{
  qDebug() << "Starting function";
  ui->bDAQcomp->setEnabled(false);
  ui->bDAQconf->setEnabled(false);
  ui->bEndSession->setEnabled(false);
  ui->bBelen->setVisible(false);
  ui->bBelen->setEnabled(false);
  ui->bCONFIG->setEnabled(false);
  ui->bBOOT->setEnabled(false);
  ui->bStartRun->setEnabled(false);
  ui->bStart->setEnabled(false);
  ui->bStop->setEnabled(false);
  ui->bTerminate->setEnabled(false);
  ui->bPause->setEnabled(false);
  ui->bDAQInterface->setEnabled(true);
  ui->lbStatus->setText(status_map.value("stopped").toUpper());
  ui->checkBoxDatabase->setChecked(false);
  ui->checkBoxDatabase->setEnabled(false);
  ui->bListDatabaseRunConfigurations->setEnabled(false);
  ui->bDebug->setVisible(false);
  ui->bStartRun->setText("  RUN");

  QString imagesDirectory = QCoreApplication::applicationDirPath() + "/../resources/images/";
  QPixmap button_image(imagesDirectory + "start_run.png");
  QIcon ButtonIcon(button_image);
  ui->bStartRun->setIcon(ButtonIcon);
  ui->bStartRun->setIconSize(0.9 * button_image.rect().size());
  this->bStartRunIconSize = 0.9 * button_image.rect().size();
  qDebug() << "Ending function";
}

void MainWindow::initializeLV()
{
  qDebug() << "Starting function";
  QStringListModel* model = new QStringListModel(this);
  QStringList empty;
  model->setStringList(empty);
  ui->lvConfigBOOT->setModel(model);
  ui->lvComponents->setModel(model);
  ui->lvConfigurations->setModel(model);
  qDebug() << "Ending function";
}

void MainWindow::checkStatus()
{
  daq_commands.start("status.sh", QStringList() << "");
  daq_commands.waitForFinished();
  QByteArray byte_status = daq_commands.readAll();
  QTextCodec* codec = QTextCodec::codecForName("UTF-8");
  QStringList daq_string = codec->codecForMib(106)->toUnicode(byte_status).split("'", QString::KeepEmptyParts);
  // qDebug() << "xmlrpc_c: " << commDAQInterface.getDAQInterfaceStatus();
  if(daq_string.count() > 1) {
    state_diagram.setOnline();
    QString str_status = daq_string.at(1);
    // qDebug()<<str_status;
    ui->lbStatus->setText(status_map.value(str_status).toUpper());
    status(str_status);
    checkTransitionStartRunPressed(str_status);
  }
  else {
    ui->taDAQInterface->document()->setPlainText(daq_string.at(0));
    status("offline");
  }
}

void MainWindow::status(QString status)
{
  int est = status_map_int.value(status);

  switch (est) {
  case 1: //stopped
    //banBOOT = true;
    //banBOOTCONFIG = false;
    banBOOTED = false;
    //banCONFIG = false;
    banCONFIGURED = false;
    banRUNNING = false;
    banPAUSED = false;
    isLVSelected();
    statusTransition();
    state_diagram.setStateDiagramStopped();
    state_diagram.setOnlineButtonGreen();
    setButtonsStoppedEnabled();
    ui->checkBoxDatabase->setEnabled(true);
    if(ui->checkBoxDatabase->isChecked()) {
      ui->bListDatabaseRunConfigurations->setEnabled(true);
    }
    break;
  case 2: //booted
    //banBOOT = false;
    //banBOOTCONFIG = false;
    banBOOTED = true;
    //banCONFIG = true;
    banCONFIGURED = false;
    banRUNNING = false;
    banPAUSED = false;
    isLVSelected();
    statusTransition();
    state_diagram.setStateDiagramBooted();
    state_diagram.setOnlineButtonGreen();
    setButtonsStoppedDisabled();
    ui->bTerminate->setEnabled(true);
    ui->checkBoxDatabase->setEnabled(false);
    ui->bListDatabaseRunConfigurations->setEnabled(false);
    break;
  case 3: //ready
    //banBOOT = false;
    //banBOOTCONFIG = false;
    banBOOTED = true;
    banCONFIGURED = true;
    //banCONFIG = false;
    banRUNNING = false;
    banPAUSED = false;
    isLVSelected();
    statusTransition();
    state_diagram.setStateDiagramReady();
    state_diagram.setOnlineButtonGreen();
    setButtonsStoppedDisabled();
    ui->bTerminate->setEnabled(true);
    ui->checkBoxDatabase->setEnabled(false);
    ui->bListDatabaseRunConfigurations->setEnabled(false);
    break;
  case 4: // running
    banRUNNING = true;
    banPAUSED = false;
    statusTransition();
    state_diagram.setStateDiagramRunning();
    state_diagram.setOnlineButtonGreen();
    setButtonsStoppedDisabled();
    ui->checkBoxDatabase->setEnabled(false);
    ui->bListDatabaseRunConfigurations->setEnabled(false);
    break;
  case 5: // pause
    banRUNNING = false;
    banPAUSED = true;
    break;
  case 6: // booting
    state_diagram.setStateDiagramBooting();
    state_diagram.setOnlineButtonGreen();
    setButtonsStoppedDisabled();
    setAllButtonsDisabled();
    ui->checkBoxDatabase->setEnabled(false);
    ui->bListDatabaseRunConfigurations->setEnabled(false);
    break;
  case 7: // configuring
    state_diagram.setStateDiagramConfiguring();
    state_diagram.setOnlineButtonGreen();
    setButtonsStoppedDisabled();
    setAllButtonsDisabled();
    ui->checkBoxDatabase->setEnabled(false);
    ui->bListDatabaseRunConfigurations->setEnabled(false);
    break;
  case 8: // starting
    state_diagram.setStateDiagramStartingRun();
    state_diagram.setOnlineButtonGreen();
    setButtonsStoppedDisabled();
    setAllButtonsDisabled();
    ui->checkBoxDatabase->setEnabled(false);
    ui->bListDatabaseRunConfigurations->setEnabled(false);
    break;
  case 9: // stopping
    state_diagram.setStateDiagramStoppingRun();
    state_diagram.setOnlineButtonGreen();
    setButtonsStoppedDisabled();
    setAllButtonsDisabled();
    ui->checkBoxDatabase->setEnabled(false);
    ui->bListDatabaseRunConfigurations->setEnabled(false);
    break;
  case 10: // terminating
    state_diagram.setStateDiagramTerminating();
    state_diagram.setOnlineButtonGreen();
    setButtonsStoppedDisabled();
    setAllButtonsDisabled();
    ui->checkBoxDatabase->setEnabled(false);
    ui->bListDatabaseRunConfigurations->setEnabled(false);
    break;
  case 99:
    initializeButtons();
    timer.stop();
    state_diagram.setOffline();
    state_diagram.setStateDiagramOff();
    state_diagram.setOnlineButtonRed();
    DAQInterfaceProcess_started = false;
    break;
  default:
    break;
  }
}

void MainWindow::setButtonsStoppedEnabled()
{
  ui->bEndSession->setEnabled(true);
  if(!ui->checkBoxDatabase->isEnabled()) {
    ui->bDAQcomp->setEnabled(true);
    ui->bDAQconf->setEnabled(true);
  }
}

void MainWindow::setButtonsStoppedDisabled()
{
  ui->bEndSession->setEnabled(false);
  ui->bDAQcomp->setEnabled(false);
  ui->bDAQconf->setEnabled(false);
}

void MainWindow::setAllButtonsDisabled()
{
  ui->bEndSession->setEnabled(false);
  ui->bDAQcomp->setEnabled(false);
  ui->bDAQconf->setEnabled(false);
  ui->bBOOT->setEnabled(false);
  ui->bCONFIG->setEnabled(false);
  ui->bStart->setEnabled(false);
  ui->bStop->setEnabled(false);
  ui->bTerminate->setEnabled(false);
  ui->bStartRun->setEnabled(false);
}

void MainWindow::bSTOPPressed()
{
  qDebug() << "Starting function";
  commDAQInterface.sendTransitionSTOP();
  qDebug() << "Ending function";
}

void MainWindow::bTERMINATEPressed()
{
  qDebug() << "Starting function";
  commDAQInterface.sendTransitionTERMINATE();
  qDebug() << "Ending function";
}

void MainWindow::bSTARTPressed()
{
  qDebug() << "Starting function";
  commDAQInterface.sendTransitionSTART();
  qDebug() << "Ending function";
}

void MainWindow::bBOOTPressed()
{
  qDebug() << "Starting function";
  commDAQInterface.setDAQInterfaceComponents(list_comps_selected);
  commDAQInterface.sendTransitionBOOT(list_BOOTConfig_selected);
  qDebug() << "Ending function";
}

void MainWindow::bCONFIGPressed()
{
  qDebug() << "Starting function";
  commDAQInterface.sendTransitionCONFIG(list_config_selected);
  qDebug() << "Ending function";
}

void MainWindow::lvBOOTConfigSelected()
{
  qDebug() << "Starting function";
  list_BOOTConfig_selected.clear();
  QStringList list_str;
  QModelIndexList list = ui->lvConfigBOOT->selectionModel()->selectedRows();
  if(list.length() != 0) {
    for(QModelIndex idx : list) {
      list_str = idx.model()->data(idx, Qt::DisplayRole).toString().split(' ', QString::KeepEmptyParts);
      QString s_ = env_vars::daqInt_user_dir + "/" + list_str.first();
      list_BOOTConfig_selected.append(s_);
      list_str.clear();
    }
    banBOOTCONFIG = true;
  }
  else {
    QStringListModel* unselectedListModel = (QStringListModel*)ui->lvConfigBOOT->model();
    if(unselectedListModel->stringList().length() == 1) {
      list_BOOTConfig_selected.clear();
      QString s_ = env_vars::daqInt_user_dir + "/" + unselectedListModel->stringList().first();
      list_BOOTConfig_selected.append(s_);
      banBOOTCONFIG = true;
      banBOOT = true;
    }
    else {
      banBOOTCONFIG = false;
      banBOOT = true;
    }
  }
  isLVSelected();
  qDebug() << "Ending function";
}

void MainWindow::lvComponentsSelected()
{
  qDebug() << "Starting function";
  if(!ui->checkBoxDatabase->isChecked()) {
    list_comps_selected.clear();
    QStringList list_str;
    QModelIndexList list = ui->lvComponents->selectionModel()->selectedRows();
    if(list.length() != 0) {
      for(QModelIndex idx : list) {
        list_str = idx.model()->data(idx, Qt::DisplayRole).toString().split(' ', QString::KeepEmptyParts);
        list_comps_selected.append(list_str.first());
        list_str.clear();
      }
      banBOOT = true;
    }
    else {
      banBOOT = false;
    }
    isLVSelected();
  }
  else {
    list_comps_selected.clear();
    QStringList list_str;
    QModelIndexList list = ui->lvComponents->selectionModel()->selectedRows();
    if(list.length() != 0) {
      for(QModelIndex idx : list) {
        list_str = idx.model()->data(idx, Qt::DisplayRole).toString().split(' ', QString::KeepEmptyParts);
        list_comps_selected.append(list_str.first());
        list_str.clear();
      }
      banBOOT = true;
    }
    else {
      QStringListModel* unselectedListModel = (QStringListModel*)ui->lvComponents->model();
      list_comps_selected = unselectedListModel->stringList();
      banBOOT = true;
    }
    isLVSelected();
  }
  qDebug() << "Ending function";
}

void MainWindow::lvConfigurationsSelected()
{
  qDebug() << "Starting function";
  if(DAQState == 3) {
    banCONFIG = true;
    isLVSelected();
  }
  else if(!ui->checkBoxDatabase->isChecked()) {
    list_config_selected.clear();
    QStringList list_str;
    QModelIndexList list = ui->lvConfigurations->selectionModel()->selectedRows();
    if(list.length() != 0) {
      for(QModelIndex idx : list) {
        list_str = idx.model()->data(idx, Qt::DisplayRole).toString().split(' ', QString::KeepEmptyParts);
        list_config_selected.append(list_str.first());
        list_str.clear();
      }
      banCONFIG = true;
    }
    else {
      QStringListModel* unselectedListModel = (QStringListModel*)ui->lvConfigurations->model();
      if(unselectedListModel->stringList().length() == 1) {
        list_config_selected = unselectedListModel->stringList();
        banCONFIG = true;
      }
      else {
        banCONFIG = false;
      }
    }
    isLVSelected();
  }
  qDebug() << "Ending function";
}

void MainWindow::statusTransition()
{
  if(banRUNNING) {
    ui->bStart->setEnabled(false);
    ui->bStop->setEnabled(true);
  }
  else {
    ui->bStop->setEnabled(false);
  }
}

void MainWindow::isLVSelected()
{
  if(banBOOT && banCONFIG && banBOOTCONFIG && !banBOOTED && !banCONFIGURED) {
    ui->bBOOT->setEnabled(true);
    ui->bCONFIG->setEnabled(false);
    ui->bStartRun->setEnabled(true);
    ui->bTerminate->setEnabled(false);
    ui->bStart->setEnabled(false);
    // qDebug()<<"selected: 1";
  }
  else if(banBOOT && banCONFIG && banBOOTCONFIG && !banBOOTED) {
    ui->bBOOT->setEnabled(true);
    // qDebug()<<"selected: 2";
  }
  else if(!banBOOT || !banBOOTCONFIG || !banCONFIG) {
    ui->bBOOT->setEnabled(false);
    ui->bCONFIG->setEnabled(false);
    ui->bStartRun->setEnabled(false);
    // qDebug()<<"selected: 3";
  }
  else if(!banCONFIG && banBOOTED) {
    ui->bBOOT->setEnabled(false);
    ui->bCONFIG->setEnabled(false);
    ui->bStartRun->setEnabled(false);
    // qDebug()<<"selected: 4";
  }
  else if(banBOOTED && banCONFIG && !banCONFIGURED) {
    ui->bBOOT->setEnabled(false);
    ui->bCONFIG->setEnabled(true);
    ui->bStartRun->setEnabled(false);
    ui->bTerminate->setEnabled(true);
    // qDebug()<<"selected: 5";
  }
  else if(banBOOTED && banCONFIGURED) {
    ui->bBOOT->setEnabled(false);
    ui->bCONFIG->setEnabled(false);
    ui->bStartRun->setEnabled(false);
    ui->bStart->setEnabled(true);
    // qDebug()<<"selected: 6";
  }
  else if(banBOOTED && !banCONFIGURED) {
    ui->bBOOT->setEnabled(false);
    ui->bCONFIG->setEnabled(true);
    ui->bStartRun->setEnabled(false);
    // qDebug()<<"selected: 7";
  }
}

void MainWindow::setButtonsDAQInterfaceInitialized(bool started)
{
  qDebug() << "Starting function";
  if(started) {
    ui->bDAQInterface->setEnabled(false);
    ui->bDAQcomp->setEnabled(true);
    ui->bDAQconf->setEnabled(true);
    ui->bEndSession->setEnabled(true);
    ui->checkBoxDatabase->setEnabled(true);
    timer.start(1000);
  }
  qDebug() << "Ending function";
}

void MainWindow::bDAQInterfacePressed()
{
  qDebug() << "Starting function";
  daq_interface.setWorkingDirectory(env_vars::daqInt_wd);
  daq_commands.setWorkingDirectory(env_vars::daqInt_wd);

  // QStringList daqinterface_start_commands;

  // qDebug() << "All env variables:";
  // QString env_variable;
  // QStringList paths_list = env.toStringList();
  // foreach( env_variable, paths_list ) qDebug() << env_variable;

  // //////// old way
  // daqinterface_start_commands << "stdbuf -oL ./rc/control/daqinterface.py --partition-number"
  //                             << partition_number_str
  //                             << "--rpc-port" << rpc_port_str;
  // daq_interface.start(daqinterface_start_commands.join(" "));
  // //////// old way


  // //////// estebans way; sadly not working
  // daqinterface_start_commands << "stdbuf -oL" << wd + "/rc/control/daqinterface.py --partition-number"
  //                            << partition_number_str
  //                            << "--rpc-port" << rpc_port_str;
  // DAQInterfaceProcess_started = daq_interface.startDetached(daqinterface_start_commands.join(" "));
  // DAQInterface_PID = daq_interface.processId();
  // setButtonsDAQInterfaceInitialized(DAQInterfaceProcess_started);
  // qDebug() << daqinterface_start_commands;
  // //DAQInterface_logfile = "/home/ecristal/Debug.log";
  // //DAQInterface_logwatcher.addPath(DAQInterface_logfile);

  // //////// estebans way


  if (env_vars::daqInt_user_sourcefile != "EMPTY") {
    qDebug() << "env_vars::daqInt_user_sourcefile: " << env_vars::daqInt_user_sourcefile;
    daq_interface.start("./bin/DAQInterface.sh");
    DAQInterfaceProcess_started = true;
    DAQInterface_PID = daq_interface.processId();
    setButtonsDAQInterfaceInitialized(DAQInterfaceProcess_started);

    state_diagram.setLCDPartitionNumber(env_vars::partition_number.toInt());
    state_diagram.setLCDPortNumber(env_vars::rpc_port.toInt());
    checkBoxDatabaseChanged();
  }
  else {
    qCritical("Source script hasn't been sourced.\n"
              "DAQINTERFACE_USER_SOURCEFILE: %s",
              qUtf8Printable(env_vars::daqInt_user_sourcefile));
    QCoreApplication::exit(1);
  }
  qDebug() << "Ending function";
  return;
}

void MainWindow::DAQInterfaceOutput()
{
  //qDebug() << "Starting function";
  QByteArray daq_byte_array = daq_interface.readAllStandardOutput();
  //daq_interface.waitForFinished();
  QTextCodec* codec = QTextCodec::codecForName("UTF-8");
  daq_string = codec->codecForMib(106)->toUnicode(daq_byte_array);
  daqInterfaceTextAreaLog = daqInterfaceTextAreaLog + daq_string;
  ui->taDAQInterface->document()->setPlainText(daqInterfaceTextAreaLog);
  QScrollBar* scroll = ui->taDAQInterface->verticalScrollBar();
  scroll->setValue(scroll->maximum());
  // qDebug() << "Inside " << __func__ << " , daq_string: " << daq_string << "\n";
  switch (DAQState) {
  case 1:
    lvComps();
    break;
  case 2:
    lvConfigs();
    break;
  case 3:
    populateLVConfigurationsFromDatabase();
    populateLVComponentsFromDatabase();
    populateLVBOOTConfigurationsFromDatabase();
    this->lvComponentsSelected();
    this->lvConfigurationsSelected();
    this->lvBOOTConfigSelected();
    DAQState = 0;
  default:
    break;
  }
  //qDebug() << "Ending function";
}

void MainWindow::lvComps()
{
  qDebug() << "Starting function";
  QStringListModel* model = new QStringListModel(this);
  QStringList list = daq_string.split('\n', QString::SkipEmptyParts);
  list.removeFirst();
  model->setStringList(list);
  ui->lvComponents->setModel(model);
  ui->lvComponents->setSelectionMode(QAbstractItemView::MultiSelection);
  DAQState = 0;
  this->lvComponentsSelected();
  qDebug() << "Ending function";
}

void MainWindow::bListDAQComps()
{
  qDebug() << "Starting function";
  commDAQInterface.listDAQInterfaceComponents();
  DAQState = 1;
  QThread::msleep(100);
  qDebug() << "Ending function";
}

void MainWindow::lvConfigs()
{
  qDebug() << "Starting function";
  QStringListModel* model = new QStringListModel(this);
  QStringList list = daq_string.split("\n\n", QString::SkipEmptyParts);
  //qDebug()<<list.at(0);
  //list.removeFirst();
  //list.removeLast();
  QString list_config = list.at(0);
  list = list_config.split('\n');
  //qDebug()<<list;
  list.removeFirst();
  list.removeFirst();
  model->setStringList(list);
  ui->lvConfigurations->setModel(model);
  DAQState = 0;
  this->lvConfigurationsSelected();
  this->lvBOOTConfigSelected();
  this->lvComponentsSelected();
  qDebug() << "Ending function";
}

void MainWindow::bListDAQConfigs()
{
  qDebug() << "Starting function";
  commDAQInterface.listDAQInterfaceConfigs();
  DAQState = 2;
  QRegExp reg("*.txt");
  reg.setPatternSyntax(QRegExp::Wildcard);
  QDirIterator dirIt(env_vars::daqInt_user_dir);
  QString s_;
  QStringList list_str, list_config;
  while(dirIt.hasNext()) {
    s_ = dirIt.next();
    if(reg.exactMatch(s_)) {
      // qDebug() << "config file " << s_;
      list_str = s_.split('/', QString::SkipEmptyParts);
      list_config.append(list_str.last());
    }
    else {
    }
  }

  QStringListModel* model = new QStringListModel(this);
  model->setStringList(list_config);
  ui->lvConfigBOOT->setModel(model);
  QThread::msleep(100);
  qDebug() << "Ending function";
}

void MainWindow::bStartRunPressed()
{
  qDebug() << "Starting function";
  banStartRunPressed = true;
  startRunConfigSignalIssued = false;
  startRunStartSignalIssued = false;
  this->bBOOTPressed();
  qDebug() << "Ending function";
}

void MainWindow::checkTransitionStartRunPressed(QString status)
{
  int est = status_map_int.value(status);

  if(banStartRunPressed) {
    switch(est) {
    case 1: //stopped
      banStartRunPressed = false;
      startRunConfigSignalIssued = false;
      startRunStartSignalIssued = false;
      break;
    case 2: //booted
      if(!startRunConfigSignalIssued) {
        this->bCONFIGPressed();
      }
      else {
        //Add error message
      }
      break;
    case 3: //ready
      if(!startRunStartSignalIssued) {
        this->bSTARTPressed();
      }
      else {
        //Add error message
      }
      startRunConfigSignalIssued = false;
      break;
    case 4: // running
      startRunStartSignalIssued = false;
      break;
    case 5: // pause

      break;
    case 6: // booting

      break;
    case 7: // configuring
      startRunConfigSignalIssued = true;
      break;
    case 8: // starting
      startRunStartSignalIssued = true;
      break;
    case 9: // stopping
      banStartRunPressed = false;
      break;
    case 10: // terminating
      banStartRunPressed = false;
      break;
    case 99:

      break;
    default:
      break;
    }
  }
}

void MainWindow::bDebugPressed()
{
  qDebug() << "Debug";
  // db_dialog *dialogConftoolImport = new db_dialog(this);
  // dialogConftoolImport->setWindowTitle("Import configuration from Database");
  // int result = dialogConftoolImport->exec();
  // if(result == QDialog::Accepted){

  // }
  // else if(result == QDialog::Rejected){}
}

void MainWindow::bListDatabaseRunConfigurations()
{
  qDebug() << "Starting function";
  db_dialog *dbDialog = new db_dialog(this);
  dbDialog->setWindowTitle("List Of Available Run Configurations");
  int result = dbDialog->exec();
  if(result == QDialog::Accepted) {

    // TODO: do something along the lines of the selected answer here:
    // https://stackoverflow.com/questions/9194799/how-to-return-data-from-qdialog
    dbSelectedConfig = dbDialog->getSelectedDBConfig();
    qDebug() << "dialog_selected_config: " << dbSelectedConfig;
    this->populateLVConfigurationsFromDatabase();
    this->populateLVComponentsFromDatabase();
  }
  else if(result == QDialog::Rejected) {}
  qDebug() << "Ending function";
}

void MainWindow::checkBoxDatabaseChanged()
{
  qDebug() << "Starting function";
  bool checked = ui->checkBoxDatabase->isChecked();
  if(checked) {
    ui->bListDatabaseRunConfigurations->setEnabled(true);
    ui->bDAQcomp->setEnabled(false);
    ui->bDAQconf->setEnabled(false);
    initializeLV();
    banBOOTCONFIG = false;
  }
  else {
    // qDebug() << env.value("DAQINTERFACE_FHICL_DIRECTORY","FHICL_DEFAULT not found");
    ui->bListDatabaseRunConfigurations->setEnabled(false);
    ui->bDAQcomp->setEnabled(true);
    ui->bDAQconf->setEnabled(true);
    initializeLV();
    ui->lvConfigurations->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->lvConfigurations->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->lvComponents->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->lvComponents->setEditTriggers(QAbstractItemView::NoEditTriggers);
    banBOOTCONFIG = false;
  }
  qDebug() << "Ending function";
}

void MainWindow::populateLVComponentsFromDatabase()
{
  qDebug() << "Starting function";
  QString config_name = dbSelectedConfig.first;
  config_name.chop(5); // to remove the numbers // TODO: find a better way
  QString selectedDBConfig_dir = dbSelectedConfig.second + "/" + config_name;
  // qDebug() << "selectedDBConfig_dir: " << selectedDBConfig_dir;

  commDAQInterface.listDAQInterfaceComponents();
  QThread::msleep(100);

  DAQState = 3;
  banBOOTCONFIG = false;

  // qDebug() << "Inside " << __func__ << " , daq_string: " << daq_string << "\n";
  QDirIterator dirIt(selectedDBConfig_dir, QDir::AllEntries | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
  QStringList componentlist = daq_string.split('\n', QString::SkipEmptyParts);
  QStringList lvComponentsList;
  for(QString component : componentlist) {
    QStringList component_ = component.split(' ', QString::SkipEmptyParts);
    componentlist.replace(componentlist.indexOf(component), component_.at(0));
  }
  componentlist.removeFirst();
  while (dirIt.hasNext()) {
    QString fileName = dirIt.next();
    QStringList fileName_ = fileName.split('/', QString::KeepEmptyParts);
    fileName = fileName_.last();
    for(QString component : componentlist) {
      QRegExp reg(component + "*");
      reg.setPatternSyntax(QRegExp::Wildcard);
      if(reg.exactMatch(fileName)) {
        lvComponentsList.append(component);
      }
    }
  }

  QStringListModel* model = new QStringListModel(this);
  model->setStringList(lvComponentsList);
  ui->lvComponents->setModel(model);
  ui->lvComponents->setSelectionMode(QAbstractItemView::MultiSelection);
  ui->lvComponents->setEditTriggers(QAbstractItemView::NoEditTriggers);
  list_comps_selected = lvComponentsList;
  qDebug() << "Ending function";
}

void MainWindow::populateLVConfigurationsFromDatabase()
{
  qDebug() << "Starting function";
  QStringList lvConfigurationsList;
  lvConfigurationsList.append(dbSelectedConfig.first);
  QStringListModel* model = new QStringListModel(this);
  model->setStringList(lvConfigurationsList);
  ui->lvConfigurations->setModel(model);
  ui->lvConfigurations->setSelectionMode(QAbstractItemView::NoSelection);
  ui->lvConfigurations->setEditTriggers(QAbstractItemView::NoEditTriggers);
  list_config_selected = lvConfigurationsList;
  qDebug() << "Ending function";
}

void MainWindow::populateLVBOOTConfigurationsFromDatabase()
{
  qDebug() << "Starting function";
  QRegExp reg("*.txt");
  reg.setPatternSyntax(QRegExp::Wildcard);
  QDirIterator dirIt(env_vars::daqInt_user_dir);
  QString s_;
  QStringList list_str, list_config;
  bool foundMatch = false;
  while(dirIt.hasNext()) {
    s_ = dirIt.next();
    if(reg.exactMatch(s_)) {
      // qDebug() << "config file "<< str;
      list_str = s_.split('/', QString::SkipEmptyParts);
      // qDebug() << list_str.last();
      list_config.append(list_str.last());
      foundMatch = true;
    }
  }
  if (foundMatch) {
    QStringListModel* model = new QStringListModel(this);
    model->setStringList(list_config);
    ui->lvConfigBOOT->setModel(model);
    ui->lvConfigBOOT->setEditTriggers(QAbstractItemView::NoEditTriggers);
  }
  else qInfo() << "No common config files found.";
  qDebug() << "Ending function";
}

QString MainWindow::getDBConfigurationFHICL_dir() const
{
  qDebug() << "Starting function";
  return DBConfigurationFHICL_dir;
  qDebug() << "Ending function";
}

void MainWindow::setDBConfigurationFHICL_dir(const QString &value)
{
  qDebug() << "Starting function";
  DBConfigurationFHICL_dir = value;
  qDebug() << "Ending function";
}

QProcessEnvironment MainWindow::getQProcessEnvironment()
{
  qDebug() << "Starting function";
  return env_vars::env;
  qDebug() << "Ending function";
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
  qDebug() << "Starting function";
  QMainWindow::resizeEvent(event);
  this->resizeWindow();
  qDebug() << "Ending function";
}

void MainWindow::resizeWindow()
{
  qDebug() << "Starting function";
  QSize windowSize = this->geometry().size();
  int windowHeigth = windowSize.height();
  int windowWidth = windowSize.width();
  ui->groupBox_DAQInterfaceCommands->move((int)(windowWidth * this->gbDAQInterfaceCommandsPosition.x() / this->originalWindowSize.width()), ui->groupBox_DAQInterfaceCommands->pos().y());
  ui->groupBox_DAQInterfaceCommands->move(ui->groupBox_DAQInterfaceCommands->pos().x(), (int)(windowHeigth * this->gbDAQInterfaceCommandsPosition.y() / this->originalWindowSize.height()));
  ui->groupBox_DAQInterfaceCommands->resize(ui->groupBox_DAQInterfaceCommands->geometry().size().width(), windowHeigth * this->gbDAQInterfaceCommandsSize.height() / this->originalWindowSize.height());
  ui->groupBox_DAQInterfaceCommands->resize(windowWidth * this->gbDAQInterfaceCommandsSize.width() / this->originalWindowSize.width(), ui->groupBox_DAQInterfaceCommands->geometry().size().height());

  ui->groupBox_TransitionCommands->move((int)(windowWidth * this->gbTransitionCommandsPosition.x() / this->originalWindowSize.width()), ui->groupBox_TransitionCommands->pos().y());
  ui->groupBox_TransitionCommands->move(ui->groupBox_TransitionCommands->pos().x(), (int)(windowHeigth * this->gbTransitionCommandsPosition.y() / this->originalWindowSize.height()));
  ui->groupBox_TransitionCommands->resize(ui->groupBox_TransitionCommands->geometry().size().width(), windowHeigth * this->gbTransitionCommandsSize.height() / this->originalWindowSize.height());
  ui->groupBox_TransitionCommands->resize(windowWidth * this->gbTransitionCommandsSize.width() / this->originalWindowSize.width(), ui->groupBox_TransitionCommands->geometry().size().height());

  ui->groupBox_Database->move((int)(windowWidth * this->gbDatabasePosition.x() / this->originalWindowSize.width()), ui->groupBox_Database->pos().y());
  ui->groupBox_Database->move(ui->groupBox_Database->pos().x(), (int)(windowHeigth * this->gbDatabasePosition.y() / this->originalWindowSize.height()));
  ui->groupBox_Database->resize(ui->groupBox_Database->geometry().size().width(), windowHeigth * this->gbDatabaseSize.height() / this->originalWindowSize.height());
  ui->groupBox_Database->resize(windowWidth * this->gbDatabaseSize.width() / this->originalWindowSize.width(), ui->groupBox_Database->geometry().size().height());

  ui->groupBox_DAQInterface->move((int)(windowWidth * this->gbDAQInterfacePosition.x() / this->originalWindowSize.width()), ui->groupBox_DAQInterface->pos().y());
  ui->groupBox_DAQInterface->move(ui->groupBox_DAQInterface->pos().x(), (int)(windowHeigth * this->gbDAQInterfacePosition.y() / this->originalWindowSize.height()));
  ui->groupBox_DAQInterface->resize(ui->groupBox_DAQInterface->geometry().size().width(), windowHeigth * this->gbDAQInterfaceSize.height() / this->originalWindowSize.height());
  ui->groupBox_DAQInterface->resize(windowWidth * this->gbDAQInterfaceSize.width() / this->originalWindowSize.width(), ui->groupBox_DAQInterface->geometry().size().height());

  ui->bStart->move((int)(windowWidth * this->bStartPosition.x() / this->originalWindowSize.width()), ui->bStart->pos().y());
  ui->bStart->move(ui->bStart->pos().x(), (int)(windowHeigth * this->bStartPosition.y() / this->originalWindowSize.height()));
  ui->bStart->resize(ui->bStart->geometry().size().width(), windowHeigth * this->bStartSize.height() / this->originalWindowSize.height());
  ui->bStart->resize(windowWidth * this->bStartSize.width() / this->originalWindowSize.width(), ui->bStart->geometry().size().height());

  ui->bStop->move((int)(windowWidth * this->bStopPosition.x() / this->originalWindowSize.width()), ui->bStop->pos().y());
  ui->bStop->move(ui->bStop->pos().x(), (int)(windowHeigth * this->bStopPosition.y() / this->originalWindowSize.height()));
  ui->bStop->resize(ui->bStop->geometry().size().width(), windowHeigth * this->bStopSize.height() / this->originalWindowSize.height());
  ui->bStop->resize(windowWidth * this->bStopSize.width() / this->originalWindowSize.width(), ui->bStop->geometry().size().height());

  ui->bPause->move((int)(windowWidth * this->bPausePosition.x() / this->originalWindowSize.width()), ui->bPause->pos().y());
  ui->bPause->move(ui->bPause->pos().x(), (int)(windowHeigth * this->bPausePosition.y() / this->originalWindowSize.height()));
  ui->bPause->resize(ui->bPause->geometry().size().width(), windowHeigth * this->bPauseSize.height() / this->originalWindowSize.height());
  ui->bPause->resize(windowWidth * this->bPauseSize.width() / this->originalWindowSize.width(), ui->bPause->geometry().size().height());

  ui->bTerminate->move((int)(windowWidth * this->bTerminatePosition.x() / this->originalWindowSize.width()), ui->bTerminate->pos().y());
  ui->bTerminate->move(ui->bTerminate->pos().x(), (int)(windowHeigth * this->bTerminatePosition.y() / this->originalWindowSize.height()));
  ui->bTerminate->resize(ui->bTerminate->geometry().size().width(), windowHeigth * this->bTerminateSize.height() / this->originalWindowSize.height());
  ui->bTerminate->resize(windowWidth * this->bTerminateSize.width() / this->originalWindowSize.width(), ui->bTerminate->geometry().size().height());

  ui->bStartRun->move((int)(windowWidth * this->bStartRunPosition.x() / this->originalWindowSize.width()), ui->bStartRun->pos().y());
  ui->bStartRun->move(ui->bStartRun->pos().x(), (int)(windowHeigth * this->bStartRunPosition.y() / this->originalWindowSize.height()));
  ui->bStartRun->resize(ui->bStartRun->geometry().size().width(), windowHeigth * this->bStartRunSize.height() / this->originalWindowSize.height());
  ui->bStartRun->resize(windowWidth * this->bStartRunSize.width() / this->originalWindowSize.width(), ui->bStartRun->geometry().size().height());

  ui->bListDatabaseRunConfigurations->move((int)(windowWidth * this->bListDatabaseRunConfigurationsPosition.x() / this->originalWindowSize.width()), ui->bListDatabaseRunConfigurations->pos().y());
  ui->bListDatabaseRunConfigurations->move(ui->bListDatabaseRunConfigurations->pos().x(), (int)(windowHeigth * this->bListDatabaseRunConfigurationsPosition.y() / this->originalWindowSize.height()));
  ui->bListDatabaseRunConfigurations->resize(ui->bListDatabaseRunConfigurations->geometry().size().width(), windowHeigth * this->bListDatabaseRunConfigurationsSize.height() / this->originalWindowSize.height());
  ui->bListDatabaseRunConfigurations->resize(windowWidth * this->bListDatabaseRunConfigurationsSize.width() / this->originalWindowSize.width(), ui->bListDatabaseRunConfigurations->geometry().size().height());

  ui->bDAQcomp->move((int)(windowWidth * this->bDAQcompPosition.x() / this->originalWindowSize.width()), ui->bDAQcomp->pos().y());
  ui->bDAQcomp->move(ui->bDAQcomp->pos().x(), (int)(windowHeigth * this->bDAQcompPosition.y() / this->originalWindowSize.height()));
  ui->bDAQcomp->resize(ui->bDAQcomp->geometry().size().width(), windowHeigth * this->bDAQcompSize.height() / this->originalWindowSize.height());
  ui->bDAQcomp->resize(windowWidth * this->bDAQcompSize.width() / this->originalWindowSize.width(), ui->bDAQcomp->geometry().size().height());

  ui->bDAQconf->move((int)(windowWidth * this->bDAQconfPosition.x() / this->originalWindowSize.width()), ui->bDAQconf->pos().y());
  ui->bDAQconf->move(ui->bDAQconf->pos().x(), (int)(windowHeigth * this->bDAQconfPosition.y() / this->originalWindowSize.height()));
  ui->bDAQconf->resize(ui->bDAQconf->geometry().size().width(), windowHeigth * this->bDAQconfSize.height() / this->originalWindowSize.height());
  ui->bDAQconf->resize(windowWidth * this->bDAQconfSize.width() / this->originalWindowSize.width(), ui->bDAQconf->geometry().size().height());

  ui->bBOOT->move((int)(windowWidth * this->bBOOTPosition.x() / this->originalWindowSize.width()), ui->bBOOT->pos().y());
  ui->bBOOT->move(ui->bBOOT->pos().x(), (int)(windowHeigth * this->bBOOTPosition.y() / this->originalWindowSize.height()));
  ui->bBOOT->resize(ui->bBOOT->geometry().size().width(), windowHeigth * this->bBOOTSize.height() / this->originalWindowSize.height());
  ui->bBOOT->resize(windowWidth * this->bBOOTSize.width() / this->originalWindowSize.width(), ui->bBOOT->geometry().size().height());

  ui->bCONFIG->move((int)(windowWidth * this->bCONFIGPosition.x() / this->originalWindowSize.width()), ui->bCONFIG->pos().y());
  ui->bCONFIG->move(ui->bCONFIG->pos().x(), (int)(windowHeigth * this->bCONFIGPosition.y() / this->originalWindowSize.height()));
  ui->bCONFIG->resize(ui->bCONFIG->geometry().size().width(), windowHeigth * this->bCONFIGSize.height() / this->originalWindowSize.height());
  ui->bCONFIG->resize(windowWidth * this->bCONFIGSize.width() / this->originalWindowSize.width(), ui->bCONFIG->geometry().size().height());

  ui->bDAQInterface->move((int)(windowWidth * this->bDAQInterfacePosition.x() / this->originalWindowSize.width()), ui->bDAQInterface->pos().y());
  ui->bDAQInterface->move(ui->bDAQInterface->pos().x(), (int)(windowHeigth * this->bDAQInterfacePosition.y() / this->originalWindowSize.height()));
  ui->bDAQInterface->resize(ui->bDAQInterface->geometry().size().width(), windowHeigth * this->bDAQInterfaceSize.height() / this->originalWindowSize.height());
  ui->bDAQInterface->resize(windowWidth * this->bDAQInterfaceSize.width() / this->originalWindowSize.width(), ui->bDAQInterface->geometry().size().height());

  ui->bEndSession->move((int)(windowWidth * this->bEndSessionPosition.x() / this->originalWindowSize.width()), ui->bEndSession->pos().y());
  ui->bEndSession->move(ui->bEndSession->pos().x(), (int)(windowHeigth * this->bEndSessionPosition.y() / this->originalWindowSize.height()));
  ui->bEndSession->resize(ui->bEndSession->geometry().size().width(), windowHeigth * this->bEndSessionSize.height() / this->originalWindowSize.height());
  ui->bEndSession->resize(windowWidth * this->bEndSessionSize.width() / this->originalWindowSize.width(), ui->bEndSession->geometry().size().height());

  ui->taDAQInterface->move((int)(windowWidth * this->taDAQInterfacePosition.x() / this->originalWindowSize.width()), ui->taDAQInterface->pos().y());
  ui->taDAQInterface->move(ui->taDAQInterface->pos().x(), (int)(windowHeigth * this->taDAQInterfacePosition.y() / this->originalWindowSize.height()));
  ui->taDAQInterface->resize(ui->taDAQInterface->geometry().size().width(), windowHeigth * this->taDAQInterfaceSize.height() / this->originalWindowSize.height());
  ui->taDAQInterface->resize(windowWidth * this->taDAQInterfaceSize.width() / this->originalWindowSize.width(), ui->taDAQInterface->geometry().size().height());

  ui->lbConfigurations->move((int)(windowWidth * this->lbConfigurationsPosition.x() / this->originalWindowSize.width()), ui->lbConfigurations->pos().y());
  ui->lbConfigurations->move(ui->lbConfigurations->pos().x(), (int)(windowHeigth * this->lbConfigurationsPosition.y() / this->originalWindowSize.height()));
  ui->lbConfigurations->resize(ui->lbConfigurations->geometry().size().width(), windowHeigth * this->lbConfigurationsSize.height() / this->originalWindowSize.height());
  ui->lbConfigurations->resize(windowWidth * this->lbConfigurationsSize.width() / this->originalWindowSize.width(), ui->lbConfigurations->geometry().size().height());

  ui->lbComponents->move((int)(windowWidth * this->lbComponentsPosition.x() / this->originalWindowSize.width()), ui->lbComponents->pos().y());
  ui->lbComponents->move(ui->lbComponents->pos().x(), (int)(windowHeigth * this->lbComponentsPosition.y() / this->originalWindowSize.height()));
  ui->lbComponents->resize(ui->lbComponents->geometry().size().width(), windowHeigth * this->lbComponentsSize.height() / this->originalWindowSize.height());
  ui->lbComponents->resize(windowWidth * this->lbComponentsSize.width() / this->originalWindowSize.width(), ui->lbComponents->geometry().size().height());

  ui->lbBOOTConfig->move((int)(windowWidth * this->lbBOOTConfigPosition.x() / this->originalWindowSize.width()), ui->lbBOOTConfig->pos().y());
  ui->lbBOOTConfig->move(ui->lbBOOTConfig->pos().x(), (int)(windowHeigth * this->lbBOOTConfigPosition.y() / this->originalWindowSize.height()));
  ui->lbBOOTConfig->resize(ui->lbBOOTConfig->geometry().size().width(), windowHeigth * this->lbBOOTConfigSize.height() / this->originalWindowSize.height());
  ui->lbBOOTConfig->resize(windowWidth * this->lbBOOTConfigSize.width() / this->originalWindowSize.width(), ui->lbBOOTConfig->geometry().size().height());

  ui->lbStatus->move((int)(windowWidth * this->lbStatusPosition.x() / this->originalWindowSize.width()), ui->lbStatus->pos().y());
  ui->lbStatus->move(ui->lbStatus->pos().x(), (int)(windowHeigth * this->lbStatusPosition.y() / this->originalWindowSize.height()));
  ui->lbStatus->resize(ui->lbStatus->geometry().size().width(), windowHeigth * this->lbStatusSize.height() / this->originalWindowSize.height());
  ui->lbStatus->resize(windowWidth * this->lbStatusSize.width() / this->originalWindowSize.width(), ui->lbStatus->geometry().size().height());

  ui->lbStatusTitle->move((int)(windowWidth * this->lbStatusTitlePosition.x() / this->originalWindowSize.width()), ui->lbStatusTitle->pos().y());
  ui->lbStatusTitle->move(ui->lbStatusTitle->pos().x(), (int)(windowHeigth * this->lbStatusTitlePosition.y() / this->originalWindowSize.height()));
  ui->lbStatusTitle->resize(ui->lbStatusTitle->geometry().size().width(), windowHeigth * this->lbStatusTitleSize.height() / this->originalWindowSize.height());
  ui->lbStatusTitle->resize(windowWidth * this->lbStatusTitleSize.width() / this->originalWindowSize.width(), ui->lbStatusTitle->geometry().size().height());

  ui->lbMessages->move((int)(windowWidth * this->lbMessagesPosition.x() / this->originalWindowSize.width()), ui->lbMessages->pos().y());
  ui->lbMessages->move(ui->lbMessages->pos().x(), (int)(windowHeigth * this->lbMessagesPosition.y() / this->originalWindowSize.height()));
  ui->lbMessages->resize(ui->lbMessages->geometry().size().width(), windowHeigth * this->lbMessagesSize.height() / this->originalWindowSize.height());
  ui->lbMessages->resize(windowWidth * this->lbMessagesSize.width() / this->originalWindowSize.width(), ui->lbMessages->geometry().size().height());

  ui->checkBoxDatabase->move((int)(windowWidth * this->checkBoxDatabasePosition.x() / this->originalWindowSize.width()), ui->checkBoxDatabase->pos().y());
  ui->checkBoxDatabase->move(ui->checkBoxDatabase->pos().x(), (int)(windowHeigth * this->checkBoxDatabasePosition.y() / this->originalWindowSize.height()));
  ui->checkBoxDatabase->resize(ui->checkBoxDatabase->geometry().size().width(), windowHeigth * this->checkBoxDatabaseSize.height() / this->originalWindowSize.height());
  ui->checkBoxDatabase->resize(windowWidth * this->checkBoxDatabaseSize.width() / this->originalWindowSize.width(), ui->checkBoxDatabase->geometry().size().height());

  ui->lvComponents->move((int)(windowWidth * this->lvComponentsPosition.x() / this->originalWindowSize.width()), ui->lvComponents->pos().y());
  ui->lvComponents->move(ui->lvComponents->pos().x(), (int)(windowHeigth * this->lvComponentsPosition.y() / this->originalWindowSize.height()));
  ui->lvComponents->resize(ui->lvComponents->geometry().size().width(), windowHeigth * this->lvComponentsSize.height() / this->originalWindowSize.height());
  ui->lvComponents->resize(windowWidth * this->lvComponentsSize.width() / this->originalWindowSize.width(), ui->lvComponents->geometry().size().height());

  ui->lvConfigurations->move((int)(windowWidth * this->lvConfigurationsPosition.x() / this->originalWindowSize.width()), ui->lvConfigurations->pos().y());
  ui->lvConfigurations->move(ui->lvConfigurations->pos().x(), (int)(windowHeigth * this->lvConfigurationsPosition.y() / this->originalWindowSize.height()));
  ui->lvConfigurations->resize(ui->lvConfigurations->geometry().size().width(), windowHeigth * this->lvConfigurationsSize.height() / this->originalWindowSize.height());
  ui->lvConfigurations->resize(windowWidth * this->lvConfigurationsSize.width() / this->originalWindowSize.width(), ui->lvConfigurations->geometry().size().height());

  ui->lvConfigBOOT->move((int)(windowWidth * this->lvConfigBOOTPosition.x() / this->originalWindowSize.width()), ui->lvConfigBOOT->pos().y());
  ui->lvConfigBOOT->move(ui->lvConfigBOOT->pos().x(), (int)(windowHeigth * this->lvConfigBOOTPosition.y() / this->originalWindowSize.height()));
  ui->lvConfigBOOT->resize(ui->lvConfigBOOT->geometry().size().width(), windowHeigth * this->lvConfigBOOTSize.height() / this->originalWindowSize.height());
  ui->lvConfigBOOT->resize(windowWidth * this->lvConfigBOOTSize.width() / this->originalWindowSize.width(), ui->lvConfigBOOT->geometry().size().height());

  double quadraticMeanConfigurationFontSize = (double)qSqrt((qreal)(windowHeigth * windowHeigth + windowWidth * windowWidth));

  QFont gbDAQInterfaceCommandsFont_("Cantarell", 11);
  int lbFontSizeDAQInterfaceCommands = (int)(this->gbDAQInterfaceCommandsFont.pointSize() * quadraticMeanConfigurationFontSize / this->originalQuadraticMeanConfigurationFontSize);
  gbDAQInterfaceCommandsFont_.setPointSize(lbFontSizeDAQInterfaceCommands);
  ui->groupBox_DAQInterfaceCommands->setFont(gbDAQInterfaceCommandsFont_);

  QFont gbDAQInterfaceFont_("Cantarell", 11);
  int lbFontSizeDAQInterface = (int)(this->gbDAQInterfaceFont.pointSize() * quadraticMeanConfigurationFontSize / this->originalQuadraticMeanConfigurationFontSize);
  gbDAQInterfaceFont_.setPointSize(lbFontSizeDAQInterface);
  ui->groupBox_DAQInterface->setFont(gbDAQInterfaceFont_);

  QFont bStartRunFont_("Cantarell", 11);
  int lbFontSizeStartRun = (int)(this->bStartRunFont.pointSize() * quadraticMeanConfigurationFontSize / this->originalQuadraticMeanConfigurationFontSize);
  bStartRunFont_.setPointSize(lbFontSizeStartRun);
  ui->bStartRun->setFont(bStartRunFont_);

  QSize bStartRunIconResize = this->bStartRunIconSize * quadraticMeanConfigurationFontSize / this->originalQuadraticMeanConfigurationFontSize;
  ui->bStartRun->setIconSize(bStartRunIconResize);

  if(this->EnableFontAutoResizing) {
    QFont taDAQInterfaceFont_("Cantarell", 11);
    taDAQInterfaceFont_.setFamily(ui->taDAQInterface->font().family());
    int taFontSizeDAQInterface = (int)(this->taDAQInterfaceFont.pointSize() * quadraticMeanConfigurationFontSize / this->originalQuadraticMeanConfigurationFontSize);
    taDAQInterfaceFont_.setPointSize(taFontSizeDAQInterface);
    ui->taDAQInterface->setFont(taDAQInterfaceFont_);

    QFont lvComponentsFont_("Cantarell", 11);
    lvComponentsFont_.setFamily(ui->lvComponents->font().family());
    int lvFontSizeComponents = (int)(this->lvComponentsFont.pointSize() * quadraticMeanConfigurationFontSize / this->originalQuadraticMeanConfigurationFontSize);
    lvComponentsFont_.setPointSize(lvFontSizeComponents);
    ui->lvComponents->setFont(lvComponentsFont_);

    QFont lvConfigurationsFont_("Cantarell", 11);
    lvConfigurationsFont_.setFamily(ui->lvConfigurations->font().family());
    int lvFontSizeConfigurations = (int)(this->lvConfigurationsFont.pointSize() * quadraticMeanConfigurationFontSize / this->originalQuadraticMeanConfigurationFontSize);
    lvConfigurationsFont_.setPointSize(lvFontSizeConfigurations);
    ui->lvConfigurations->setFont(lvConfigurationsFont_);

    QFont lvBOOTConfigFont_("Cantarell", 11);
    lvBOOTConfigFont_.setFamily(ui->lvConfigBOOT->font().family());
    int lvFontSizeBOOTConfig = (int)(this->lvBOOTConfigFont.pointSize() * quadraticMeanConfigurationFontSize / this->originalQuadraticMeanConfigurationFontSize);
    lvBOOTConfigFont_.setPointSize(lvFontSizeBOOTConfig);
    ui->lvConfigBOOT->setFont(lvBOOTConfigFont_);
  }
  qDebug() << "Ending function";
}

void MainWindow::openMenuOptionsDialog()
{
  qDebug() << "Starting function";
  MenuOptionsDialog* menuOptionsDialog = new MenuOptionsDialog(this);
  menuOptionsDialog->setWindowTitle("Options");
  menuOptionsDialog->setFromMainWindowFont(ui->taDAQInterface->font());
  menuOptionsDialog->setupFontComboBox();
  menuOptionsDialog->setEnableAutoResizing(this->EnableFontAutoResizing);
  menuOptionsDialog->setupCheckBoxEnableAutoResizing();
  int result = menuOptionsDialog->exec();
  if(result == QDialog::Accepted) {
    QFont font(menuOptionsDialog->getFontType(), menuOptionsDialog->getFontSize());
    ui->taDAQInterface->setFont(font);
    ui->lvComponents->setFont(font);
    ui->lvConfigurations->setFont(font);
    ui->lvConfigBOOT->setFont(font);
    ui->taDAQInterface->setText(this->daqInterfaceTextAreaLog);
    this->EnableFontAutoResizing = menuOptionsDialog->getEnableAutoResizing();
    this->resizeWindow();
  }
  else {

  }
  qDebug() << "Ending function";
}

void MainWindow::MensajeParaBelen()
{
  // podemos dejar esto ;)
  ui->taDAQInterface->document()->setPlainText("Sos la mujer mas hermosa, no puedo dejar de amarte. Sos una bendicion en mi vida. Te quiero muchisimo");
}
