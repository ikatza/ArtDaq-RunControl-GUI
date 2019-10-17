#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    configurateWindow();
    DAQState = 0;
    banBOOT = false,banCONFIG = false,banBOOTCONFIG = false, banBOOTED = false, banCONFIGURED = false;
    banRUNNING = false, banPAUSED = false;
    env = QProcessEnvironment::systemEnvironment();
    QProcess* daqinterface_pointer = new QProcess(this);
    daqinterface_pointer = &daq_interface;
    ui->lbStatus->setText("");
    QProcess* kill_proc = new QProcess(this);
    user_str = env.value("USER","DEFAULT");
    kill_proc->start("pkill", QStringList() << "-f" << "pmt.rb" << "-u" << user_str);
    kill_proc->execute("pkill", QStringList() << "-f" << "daqinterface.py" << "-u" << user_str);
    kill_proc->waitForFinished();
    kill_proc->~QProcess();
    connect(ui->bDAQInterface,SIGNAL(clicked(bool)),this,SLOT(bDAQInterfacePressed()));
    connect(daqinterface_pointer,SIGNAL(readyReadStandardOutput()),this,SLOT(DAQInterfaceOutput()));
    connect(&DAQInterface_logwatcher,SIGNAL(fileChanged(QString)),this,SLOT(bDebugPressed()));
    connect(daqinterface_pointer,SIGNAL(started()),this,SLOT(setButtonsDAQInterfaceInitialized()));
    connect(ui->bBelen,SIGNAL(clicked(bool)),this,SLOT(MensajeParaBelen()));
    connect(ui->bDAQcomp,SIGNAL(clicked(bool)),this,SLOT(bListDAQComps()));
    connect(ui->bDAQconf,SIGNAL(clicked(bool)),this,SLOT(bListDAQConfigs()));
    connect(ui->lvComponents,SIGNAL(clicked(QModelIndex)),this,SLOT(lvComponentsSelected()));
    connect(ui->lvConfigurations,SIGNAL(clicked(QModelIndex)),this,SLOT(lvConfigurationsSelected()));
    connect(ui->lvConfigBOOT,SIGNAL(clicked(QModelIndex)),this,SLOT(lvBOOTConfigSelected()));
    connect(ui->bBOOT,SIGNAL(clicked(bool)),this,SLOT(bBOOTPressed()));
    connect(ui->bCONFIG,SIGNAL(clicked(bool)),this,SLOT(bCONFIGPressed()));
    connect(ui->bStart,SIGNAL(clicked(bool)),this,SLOT(bSTARTPressed()));
    connect(&timer,SIGNAL(timeout()),this,SLOT(checkStatus()));
    connect(ui->bStop,SIGNAL(clicked(bool)),this,SLOT(bSTOPPressed()));
    connect(ui->bTerminate,SIGNAL(clicked(bool)),this,SLOT(bTERMINATEPressed()));
    connect(ui->bEndSession,SIGNAL(clicked(bool)),this,SLOT(bEndSessionPressed()));
    connect(ui->bDebug,SIGNAL(clicked(bool)),this,SLOT(bDebugPressed()));
    connect(ui->bNewExperiment,SIGNAL(clicked(bool)),this,SLOT(bNewExperimentPressed()));
    connect(ui->bEditExperiment,SIGNAL(clicked(bool)),this,SLOT(bEditExperimentPressed()));
    connect(ui->bDeleteExperiment,SIGNAL(clicked(bool)),this,SLOT(bDeleteExperimentPressed()));
    connect(ui->bNewProfile,SIGNAL(clicked(bool)),this,SLOT(bNewProfilePressed()));
    connect(ui->bEditProfile,SIGNAL(clicked(bool)),this,SLOT(bEditProfilePressed()));
    connect(ui->bDeleteProfile,SIGNAL(clicked(bool)),this,SLOT(bDeleteProfilePressed()));
    env = QProcessEnvironment::systemEnvironment();
    initializeButtons();
    state_diagram.setWindowTitle("DAQInterface State Diagram");
    state_diagram.setFixedSize(state_diagram.geometry().width(),state_diagram.geometry().height());
    state_diagram.show();

    this->populateComboExperiments();
    this->populateComboProfiles();
    this->populateListViews();
    connect(ui->comboExperiment,SIGNAL(currentIndexChanged(int)),this,SLOT(comboExperimentIndexChanged()));
    connect(ui->comboProfiles,SIGNAL(currentTextChanged(QString)),this,SLOT(populateListViews()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::configurateWindow(){

    this->setWindowTitle("ARTDAQ RUN CONTROL");
    this->setFixedSize(this->geometry().width(),this->geometry().height());
    ui->taDAQInterface->setReadOnly(true);

}


void MainWindow::bEndSessionPressed(){

    QMessageBox msgBox;
    msgBox.setText("End session");
    msgBox.setInformativeText("Do you really wish to end the session?\n All the artDAQ processes will be destroyed ");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    QProcess* kill_p = new QProcess(this);
    QString user_str = env.value("USER", "DEFAULT");
    switch (ret) {
      case QMessageBox::Yes:
            kill_p->start("pkill", QStringList() << "-f" << "pmt.rb" << "-u" << user_str);
            kill_p->execute("pkill", QStringList() << "-f" << "daqinterface.py" << "-u" << user_str);
            initializeButtons();
            timer.stop();
            break;
      case QMessageBox::No:
            break;
      default:
          break;
    }
    status("offline");

}

void MainWindow::initializeButtons(){

    ui->bDAQcomp->setEnabled(false);
    ui->bDAQconf->setEnabled(false);
    ui->bEndSession->setEnabled(false);
    ui->bBelen->setVisible(false);
    ui->bBelen->setEnabled(false);
    ui->bCONFIG->setEnabled(false);
    ui->bBOOT->setEnabled(false);
    ui->bBOOTandCONFIG->setEnabled(false);
    ui->bStart->setEnabled(false);
    ui->bStop->setEnabled(false);
    ui->bTerminate->setEnabled(false);
    ui->bPause->setEnabled(false);
    ui->bDAQInterface->setEnabled(true);
    qDebug()<<status_map.value("stopped");
    ui->lbStatus->setText(status_map.value("stopped").toUpper());
}

void MainWindow::checkStatus(){

    daq_commands.start("status.sh",QStringList()<<"");
    daq_commands.waitForFinished();
    QByteArray byte_status = daq_commands.readAll();
    QTextCodec* codec;
    QStringList daq_string = codec->codecForMib(106)->toUnicode(byte_status).split("'",QString::KeepEmptyParts);
    qDebug() << "xmlrpc_c: " << commDAQInterface.getDAQInterfaceStatus();
    if(daq_string.count()>1){
        state_diagram.setOnline();
        QString str_status = daq_string.at(1);
        qDebug()<<str_status;
        ui->lbStatus->setText(status_map.value(str_status).toUpper());
        status(str_status);
    }
    else{
        ui->taDAQInterface->document()->setPlainText(daq_string.at(0));
        status("offline");
    }

}

void MainWindow::status(QString status){

    int est = status_map_int.value(status);

    switch (est){
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
        break;
    case 4: // running
        banRUNNING = true;
        banPAUSED = false;
        statusTransition();
        state_diagram.setStateDiagramRunning();
        state_diagram.setOnlineButtonGreen();
        break;
    case 5: // pause
        banRUNNING = false;
        banPAUSED = true;
        break;
    case 6: // booting
        state_diagram.setStateDiagramBooting();
        state_diagram.setOnlineButtonGreen();
        break;
    case 7: // configuring
        state_diagram.setStateDiagramConfiguring();
        state_diagram.setOnlineButtonGreen();
        break;
    case 8: // starting
        state_diagram.setStateDiagramStartingRun();
        state_diagram.setOnlineButtonGreen();
        break;
    case 9: // stopping
        state_diagram.setStateDiagramStoppingRun();
        state_diagram.setOnlineButtonGreen();
        break;
    case 10: // terminating
        state_diagram.setStateDiagramTerminating();
        state_diagram.setOnlineButtonGreen();
        break;
    case 99:
        initializeButtons();
        timer.stop();
        state_diagram.setOffline();
        state_diagram.setStateDiagramOff();
        state_diagram.setOnlineButtonRed();
        break;
    default:
        break;
    }

}

void MainWindow::bSTOPPressed(){

    commDAQInterface.sendTransitionSTOP();

}

void MainWindow::bTERMINATEPressed(){

    commDAQInterface.sendTransitionTERMINATE();

}

void MainWindow::bSTARTPressed(){

    commDAQInterface.sendTransitionSTART();

}

void MainWindow::bBOOTPressed(){

    commDAQInterface.setDAQInterfaceComponents(list_comps_selected);
    qDebug()<<list_BOOTConfig_selected;
    commDAQInterface.sendTransitionBOOT(list_BOOTConfig_selected);

}

void MainWindow::bCONFIGPressed(){

    commDAQInterface.sendTransitionCONFIG(list_config_selected);
    qDebug()<<list_config_selected; 

}

void MainWindow::lvBOOTConfigSelected(){

    QString env_str = env.value("DAQINTERFACE_USER_DIR","DEFAULT");
    list_BOOTConfig_selected.clear();
    QStringList list_str;
    QModelIndexList list = ui->lvConfigBOOT->selectionModel()->selectedRows();
    if(list.length() != 0)
    {
        qDebug()<<list.length();
        for(QModelIndex idx : list){
            list_str = idx.model()->data(idx,Qt::DisplayRole).toString().split(' ',QString::KeepEmptyParts);
            QString str = env_str + "/" + list_str.first();
            list_BOOTConfig_selected.append(str);
            list_str.clear();
        }
        qDebug()<<list_BOOTConfig_selected;
        banBOOTCONFIG = true;

    }else{
        banBOOTCONFIG = false;
    }
    isLVSelected();
}

void MainWindow::lvComponentsSelected(){

    list_comps_selected.clear();
    QStringList list_str;
    QModelIndexList list = ui->lvComponents->selectionModel()->selectedRows();
    if(list.length() != 0)
    {
        qDebug()<<list.length();
        for(QModelIndex idx : list){
            list_str = idx.model()->data(idx,Qt::DisplayRole).toString().split(' ',QString::KeepEmptyParts);
            list_comps_selected.append(list_str.first());
            list_str.clear();
        }
        qDebug()<<list_comps_selected;
        banBOOT = true;

    }else{
        banBOOT = false;
    }
    isLVSelected();
}

void MainWindow::lvConfigurationsSelected(){

    list_config_selected.clear();
    QStringList list_str;
    QModelIndexList list = ui->lvConfigurations->selectionModel()->selectedRows();
    if(list.length() != 0)
    {
        qDebug()<<list.length();
        for(QModelIndex idx : list){
            list_str = idx.model()->data(idx,Qt::DisplayRole).toString().split(' ',QString::KeepEmptyParts);
            list_config_selected.append(list_str.first());
            list_str.clear();
        }
        qDebug()<<list_config_selected;
        banCONFIG = true;

    }else{
        banCONFIG = false;
    }
    isLVSelected();
}

void MainWindow::statusTransition(){

    if(banRUNNING){
        ui->bStart->setEnabled(false);
        ui->bStop->setEnabled(true);
    }else{
        ui->bStop->setEnabled(false);
    }
}

void MainWindow::isLVSelected(){

    if(banBOOT && banCONFIG && banBOOTCONFIG && !banBOOTED && !banCONFIGURED){
        ui->bBOOT->setEnabled(true);
        ui->bCONFIG->setEnabled(false);
        ui->bBOOTandCONFIG->setEnabled(true);
        ui->bTerminate->setEnabled(false);
        ui->bStart->setEnabled(false);
        qDebug()<<"selected: 1";
    }else if(banBOOT && banBOOTCONFIG && !banBOOTED){
        ui->bBOOT->setEnabled(true);
        qDebug()<<"selected: 2";
    }else if(!banBOOT || !banBOOTCONFIG){
        ui->bBOOT->setEnabled(false);
        ui->bCONFIG->setEnabled(false);
        ui->bBOOTandCONFIG->setEnabled(false);
        qDebug()<<"selected: 3";
    }else if(!banCONFIG && banBOOTED){
        ui->bBOOT->setEnabled(false);
        ui->bCONFIG->setEnabled(false);
        ui->bBOOTandCONFIG->setEnabled(false);
        qDebug()<<"selected: 4";
    }else if(banBOOTED && banCONFIG && !banCONFIGURED){
        ui->bBOOT->setEnabled(false);
        ui->bCONFIG->setEnabled(true);
        ui->bBOOTandCONFIG->setEnabled(false);
        ui->bTerminate->setEnabled(true);
        qDebug()<<"selected: 5";
    }else if(banBOOTED && banCONFIGURED){
        ui->bBOOT->setEnabled(false);
        ui->bCONFIG->setEnabled(false);
        ui->bBOOTandCONFIG->setEnabled(false);
        ui->bStart->setEnabled(true);
        qDebug()<<"selected: 6";
    }else if(banBOOTED && !banCONFIGURED){
        ui->bBOOT->setEnabled(false);
        ui->bCONFIG->setEnabled(true);
        ui->bBOOTandCONFIG->setEnabled(false);
        qDebug()<<"selected: 7";
    }
}

void MainWindow::setButtonsDAQInterfaceInitialized(bool started){
    if(started){
        ui->bDAQInterface->setEnabled(false);
        ui->bDAQcomp->setEnabled(true);
        ui->bDAQconf->setEnabled(true);
        ui->bEndSession->setEnabled(true);
        timer.start(1000);
    }
}

void MainWindow::bDAQInterfacePressed(){

    wd = env.value("ARTDAQ_DAQINTERFACE_DIR","DEFAULT");
    daq_interface.setWorkingDirectory(wd);
    daq_commands.setWorkingDirectory(wd);
    QStringList daqinterface_start_commands;
    QString base_port_str = env.value("ARTDAQ_BASE_PORT","DEFAULT");
    QString ports_per_partition_str = env.value("ARTDAQ_PORTS_PER_PARTITION","DEFAULT");
    QString partition_number_str = env.value("DAQINTERFACE_PARTITION_NUMBER","DEFAULT");
    DAQInterface_logdir = env.value("DAQINTERFACE_LOGDIR","DEFAULT");
    DAQInterface_logdir = DAQInterface_logdir + "/DAQInterface_partition" + partition_number_str + ".log";
    QString rpc_port_str = QString::number(base_port_str.toInt() + partition_number_str.toInt()*ports_per_partition_str.toInt());

    // //////// old way
    // daqinterface_start_commands << "stdbuf -oL ./rc/control/daqinterface.py --partition-number"
    //                             << partition_number_str
    //                             << "--rpc-port" << rpc_port_str;
    // daq_interface.start(daqinterface_start_commands.join(" "));
    // //////// old way

    
    // //////// estebans way; sadly not working
    // daqinterface_start_commands << "stdbuf -oL" << wd + "/rc/control/daqinterface.py --partition-number"
    //                             << partition_number_str
    //                             << "--rpc-port" << rpc_port_str;
    // DAQInterfaceProcess_started = daq_interface.startDetached(daqinterface_start_commands.join(" "));
    // DAQInterface_PID = daq_interface.processId();
    // setButtonsDAQInterfaceInitialized(DAQInterfaceProcess_started);
    // qDebug() << daqinterface_start_commands;
    // //DAQInterface_logdir = "/home/ecristal/Debug.log";
    // //DAQInterface_logwatcher.addPath(DAQInterface_logdir);

    // //////// estebans way

    /////// new way; this works... maybe breaking something?
    daq_interface.start("./bin/DAQInterface.sh");
    DAQInterfaceProcess_started = true;
    DAQInterface_PID = daq_interface.processId();
    setButtonsDAQInterfaceInitialized(DAQInterfaceProcess_started);
    /////// new way; this works... maybe breaking something?
    
    state_diagram.setLCDPartitionNumber(partition_number_str.toInt());
    state_diagram.setLCDPortNumber(rpc_port_str.toInt());

    return;
}

void MainWindow::DAQInterfaceOutput(){


    QByteArray daq_byte_array = daq_interface.readAllStandardOutput();
    //if(daq_byte_array == ""){return;}
    QTextCodec* codec;
    daq_string = codec->codecForMib(106)->toUnicode(daq_byte_array);
    ui->taDAQInterface->document()->setPlainText(daq_string);
    qDebug()<<daq_string;
    switch (DAQState) {
    case 1:
        lvComps();
        break;
    case 2:
        lvConfigs();
        break;
    default:
        break;
    }
}

void MainWindow::lvComps(){

    QStringListModel* model = new QStringListModel(this);
    QStringList list = daq_string.split('\n', QString::SkipEmptyParts);
    list.removeFirst();
    model->setStringList(list);
    ui->lvComponents->setModel(model);
    ui->lvComponents->setSelectionMode(QAbstractItemView::MultiSelection);
    DAQState = 0;
}

void MainWindow::bListDAQComps(){

    //daq_commands.start("listdaqcomps.sh");
    commDAQInterface.listDAQInterfaceComponents();
    DAQState = 1;
    QThread::msleep(100);
}

void MainWindow::lvConfigs(){

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
}

void MainWindow::bListDAQConfigs(){

    commDAQInterface.listDAQInterfaceConfigs();
    DAQState = 2;
    QRegExp reg("*.txt");
    reg.setPatternSyntax(QRegExp::Wildcard);
    QString env_str = env.value("DAQINTERFACE_USER_DIR","DEFAULT");
    QDirIterator dirIt(env_str);
    QString str;
    QStringList list_str, list_config;
    while(dirIt.hasNext()){
        str = dirIt.next();
        if(reg.exactMatch(str)){

            qDebug()<<"config file "<<str;
            list_str = str.split('/', QString::SkipEmptyParts);
            qDebug()<<list_str.last();
            list_config.append(list_str.last());
        }else{
            qDebug()<<"not config file";
        }

    }

    QStringListModel* model = new QStringListModel(this);
    model->setStringList(list_config);
    ui->lvConfigBOOT->setModel(model);

    QThread::msleep(100);
}

void MainWindow::bDebugPressed(){
    qDebug() << "Debug";
//     QString experimentName = ui->comboExperiment->currentText();
//     guiDatabase.saveExperimentProfileView(experimentName);
//     guiDatabase.updateExperimentProfiles(experimentName);
}

void MainWindow::bNewExperimentPressed(){

    newExperimentDialog *dialogNewExperiment = new newExperimentDialog(this);
    dialogNewExperiment->setWindowTitle("New Experiment");
    int result = dialogNewExperiment->exec();
    if(result == QDialog::Accepted){

    }else if(result == QDialog::Rejected){

    }

}

void MainWindow::bEditExperimentPressed(){

    QVector<QStringList> componentList;
    QStringList configurationList;
    int index = ui->comboExperiment->currentIndex();
    QString experimentName = ui->comboExperiment->itemText(index);
    newExperimentDialog *dialogNewExperiment = new newExperimentDialog(this);
    dialogNewExperiment->setWindowTitle("Edit Experiment");
    dialogNewExperiment->setLabelTitle("Edit Experiment");
    dialogNewExperiment->setExperimentDialog(experimentName, componentList, configurationList);
    int result = dialogNewExperiment->exec();
    if(result == QDialog::Accepted){

    }else if(result == QDialog::Rejected){

    }

}

void MainWindow::bDeleteExperimentPressed(){
    QMessageBox msgBox;
    int index = ui->comboExperiment->currentIndex();
    QString experimentName = ui->comboExperiment->itemText(index);
    msgBox.setText("Delete experiment " + experimentName + " from Database");
    msgBox.setInformativeText("Do you really wish to delete the current experiment information from the database?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    switch (ret) {
      case QMessageBox::Yes:
            break;
      case QMessageBox::No:
            break;
      default:
          break;
    }
}

void MainWindow::populateComboExperiments(){

}

void MainWindow::bNewProfilePressed(){

    newProfileDialog *profileDialog = new newProfileDialog();
    profileDialog->setWindowTitle("New Profile");
    int result = profileDialog->exec();
    if(result == QDialog::Accepted){
    }else if(result == QDialog::Rejected){

    }

}

void MainWindow::populateComboProfiles(){

}

void MainWindow::comboExperimentIndexChanged(){
    this->populateComboProfiles();
}

void MainWindow::populateListViews(){

    if(ui->comboProfiles->currentText() == "default"){
        ui->bEditProfile->setEnabled(false);
        ui->bDeleteProfile->setEnabled(false);
    }else{
        ui->bEditProfile->setEnabled(true);
        ui->bDeleteProfile->setEnabled(true);
    }

    QStringList components;
    QStringListModel* model = new QStringListModel(this);
    model->setStringList(components);
    ui->lvComponents->setModel(model);
    QStringList configurations;
    QStringListModel* model2 = new QStringListModel(this);
    model2->setStringList(configurations);
    ui->lvConfigurations->setModel(model2);
    QStringList bootfiles;
    QStringListModel* model3 = new QStringListModel(this);
    model3->setStringList(bootfiles);
    ui->lvConfigBOOT->setModel(model3);
    //qDebug()<< configurations << bootfiles;
}

void MainWindow::bEditProfilePressed(){

    newProfileDialog *dialogNewProfile = new newProfileDialog(this);
    dialogNewProfile->setWindowTitle("Edit Profile");
    dialogNewProfile->setLabelTitle("Edit Profile");
    int result = dialogNewProfile->exec();
    if(result == QDialog::Accepted){
    }else if(result == QDialog::Rejected){

    }

}

void MainWindow::bDeleteProfilePressed(){
    QMessageBox msgBox;
    int index = ui->comboProfiles->currentIndex();
    QString profileName = ui->comboProfiles->itemText(index);
    msgBox.setText("Delete experiment " + profileName + " from Database");
    msgBox.setInformativeText("Do you really wish to delete the current profile information from the database?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    switch (ret) {
      case QMessageBox::Yes:
      case QMessageBox::No:
            break;
      default:
          break;
    }
}


QProcessEnvironment MainWindow::getQProcessEnvironment(){
    return this->env;
}

void MainWindow::MensajeParaBelen(){
    // podemos dejar esto ;)
    ui->taDAQInterface->document()->setPlainText("Sos la mujer mas hermosa, no puedo dejar de amarte. Sos una bendicion en mi vida. Te quiero muchisimo");

}
