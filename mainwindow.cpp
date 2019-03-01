#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //QTimer *ptimer = new QTimer(timer);
    ui->setupUi(this);
    configurarVentana();
    DAQState = 0;
    banBOOT = false,banCONFIG = false,banBOOTCONFIG = false, banBOOTED = false, banCONFIGURED = false;
    banRUNNING = false, banPAUSED = false;
    QProcess* daqinterface_pointer = new QProcess(this);
    daqinterface_pointer = &daq_interface;
    ui->lbStatus->setText("");
    QProcess* kill_proc = new QProcess(this);
    kill_proc->start("pkill",QStringList()<<"-f"<<"pmt.rb");
    kill_proc->execute("pkill",QStringList()<<"-f"<<"daqinterface.py");
    kill_proc->waitForFinished();
    kill_proc->~QProcess();
    connect(ui->bDAQInterface,SIGNAL(clicked(bool)),this,SLOT(bDAQInterfacePressed()));
    connect(daqinterface_pointer,SIGNAL(readyReadStandardOutput()),this,SLOT(DAQInterfaceOutput()));
    connect(daqinterface_pointer,SIGNAL(started()),this,SLOT(setBotonesDAQInterfaceInicializado()));
    connect(ui->bBelen,SIGNAL(clicked(bool)),this,SLOT(MensajeParaBelen()));
    connect(ui->bDAQcomp,SIGNAL(clicked(bool)),this,SLOT(bListDAQComps()));
    connect(ui->bDAQconf,SIGNAL(clicked(bool)),this,SLOT(bListDAQConfigs()));
    connect(ui->lvComponentes,SIGNAL(clicked(QModelIndex)),this,SLOT(lvComponentesSelected()));
    connect(ui->lvConfiguraciones,SIGNAL(clicked(QModelIndex)),this,SLOT(lvConfiguracionesSelected()));
    connect(ui->lvConfigBOOT,SIGNAL(clicked(QModelIndex)),this,SLOT(lvBOOTConfigSelected()));
    connect(ui->bBOOT,SIGNAL(clicked(bool)),this,SLOT(bBOOTPressed()));
    connect(ui->bCONFIG,SIGNAL(clicked(bool)),this,SLOT(bCONFIGPressed()));
    connect(ui->bStart,SIGNAL(clicked(bool)),this,SLOT(bSTARTPressed()));
    connect(&timer,SIGNAL(timeout()),this,SLOT(checkStatus()));
    connect(ui->bStop,SIGNAL(clicked(bool)),this,SLOT(bSTOPPressed()));
    connect(ui->bTerminate,SIGNAL(clicked(bool)),this,SLOT(bTERMINATEPressed()));
    connect(ui->bFinalizarSesion,SIGNAL(clicked(bool)),this,SLOT(bFinalizarSesionPressed()));
    env = QProcessEnvironment::systemEnvironment();
    //connect(ui->actionSource_config_file,SIGNAL(triggered()),this,SLOT(menuSourceConfigFilePressed()));
    inicializarBotones();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::configurarVentana(){

    this->setWindowTitle("ARTDAQ RUN CONTROL");
    this->setFixedSize(this->geometry().width(),this->geometry().height());
    ui->taDAQInterface->setReadOnly(true);

}

/*void MainWindow::menuSourceConfigFilePressed(){
    qDebug()<<"menuSourceConfigFilePressed";
    QString home_str = env.value("HOME","DEFAULT");
    QString fileName = QFileDialog::getOpenFileName(this,tr("Source configuration file"), home_str, tr("Source Files (*.sh)"));
    QProcess source;
    char x = '"';
    QString source_str = tr("\"source");
    QString quote = QString(x);
    fileName = quote + "source " + fileName + quote;
    qDebug()<<fileName;
    source.start("bash", QStringList()<<"-c"<<fileName);
    source.waitForFinished();
    QByteArray daq_byte_array = source.readAllStandardOutput();
    //if(daq_byte_array == ""){return;}
    QTextCodec* codec;
    daq_string = codec->codecForMib(106)->toUnicode(daq_byte_array);
    ui->taDAQInterface->document()->setPlainText(daq_string);

}*/

void MainWindow::bFinalizarSesionPressed(){

    QMessageBox msgBox;
    msgBox.setText("Finalizar sesion");
    msgBox.setInformativeText("Realmente desea finalizar la sesion?\n se destruiran todos los procesos de ARTDAQ");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    QProcess* kill_p = new QProcess(this);
    switch (ret) {
      case QMessageBox::Yes:

            kill_p->start("pkill",QStringList()<<"-f"<<"pmt.rb");
            kill_p->execute("pkill",QStringList()<<"-f"<<"daqinterface.py");
            inicializarBotones();
            timer.stop();
            break;
      case QMessageBox::No:
            break;
      default:
          break;
    }

}

void MainWindow::inicializarBotones(){

    ui->bDAQcomp->setEnabled(false);
    ui->bDAQconf->setEnabled(false);
    ui->bFinalizarSesion->setEnabled(false);
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
    QString str_status = daq_string.at(1);
    qDebug()<<str_status;
    ui->lbStatus->setText(status_map.value(str_status).toUpper());
    estado(str_status);
}

void MainWindow::estado(QString estado){

    int est = status_map_int.value(estado);

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
        estadoTransicion();
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
        estadoTransicion();
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
        estadoTransicion();
        break;
    case 4: // running
        banRUNNING = true;
        banPAUSED = false;
        estadoTransicion();
        break;
    case 5: // pause
        banRUNNING = false;
        banPAUSED = true;
        break;
    default:
        break;
    }

}

void MainWindow::bSTOPPressed(){

    daq_commands.start("send_transition.sh",QStringList()<<"stop");
    daq_commands.waitForFinished();

}

void MainWindow::bTERMINATEPressed(){

    daq_commands.start("send_transition.sh",QStringList()<<"terminate");
    daq_commands.waitForFinished();

}

void MainWindow::bSTARTPressed(){

    daq_commands.start("send_transition.sh",QStringList()<<"start");
    daq_commands.waitForFinished();
    /*QThread::msleep(500);
    QByteArray byte_status;
    QTextCodec* codec;
    QStringList daq_string;
    QString str_status;
    QRegExp reg;
    reg.setPattern("running");
    bool nullbyte = true;
    byte_status = daq_commands.readAll();
    byte_status.clear();
    while(nullbyte)
    {
       daq_commands.start("status.sh",QStringList()<<"");
       daq_commands.waitForFinished();
       byte_status = daq_commands.readAll();
       daq_string = codec->codecForMib(106)->toUnicode(byte_status).split("'",QString::KeepEmptyParts);

       if(daq_string.length() != 0){
        str_status = daq_string.at(1);
        qDebug()<<str_status;
        ui->lbStatus->setText(status_map.value(str_status).toUpper());
        this->repaint();
        reg.setPatternSyntax(QRegExp::Wildcard);
        if(reg.exactMatch(str_status)){

                nullbyte = false;
                ui->bStart->setEnabled(false);
                ui->bStop->setEnabled(true);
                //ui->bPause->setEnabled(true);
                banRUNNING = true;
        }else{
            estado(str_status);
        }
       }

    }*/
    /*daq_commands.execute("status.sh",QStringList()<<"");
    daq_commands.waitForBytesWritten();
    byte_status = daq_commands.readAll();
    QTextCodec* codec;
    QStringList daq_string = codec->codecForMib(106)->toUnicode(byte_status).split("'",QString::KeepEmptyParts);
    QString str_status = daq_string.at(1);
    ui->lbStatus->setText(str_status.toUpper());
    QRegExp reg("running");
    reg.setPatternSyntax(QRegExp::Wildcard);
    if(reg.exactMatch(str_status)){


    }*/
}

void MainWindow::bBOOTPressed(){

    daq_commands.start("setdaqcomps.sh",lista_comps_selected);
    daq_commands.waitForFinished(30000);
    QThread::msleep(100);
    lista_BOOTConfig_selected.push_front("boot");
    qDebug()<<lista_BOOTConfig_selected;
    daq_commands.execute("send_transition.sh",lista_BOOTConfig_selected);
    daq_commands.waitForFinished(30000);
    daq_commands.start("status.sh",QStringList()<<"");
    daq_commands.waitForFinished();
    QByteArray byte_status = daq_commands.readAll();
    QTextCodec* codec;
    QStringList daq_string = codec->codecForMib(106)->toUnicode(byte_status).split("'",QString::KeepEmptyParts);
    QString str_status = daq_string.at(1);
    ui->lbStatus->setText(status_map.value(str_status).toUpper());
    QRegExp reg("booting");
    reg.setPatternSyntax(QRegExp::Wildcard);
    while(reg.exactMatch(str_status)){

        daq_string.clear();
        daq_commands.start("status.sh",QStringList()<<"");
        daq_commands.waitForFinished();
        byte_status = daq_commands.readAll();
        daq_string = codec->codecForMib(106)->toUnicode(byte_status).split("'",QString::KeepEmptyParts);
        str_status = daq_string.at(1);
        ui->lbStatus->setText(status_map.value(str_status).toUpper());
        this->repaint();
        //DAQInterfaceOutput();

    }
    reg.setPattern("booted");
    ui->lbStatus->setText(status_map.value(str_status).toUpper());
    if(reg.exactMatch(str_status)){

        banBOOTED = true;
        isLVSelected();

    }else{

        banBOOTED = false;
        isLVSelected();

    }
    qDebug()<<str_status;
    lista_BOOTConfig_selected.removeFirst();

}

void MainWindow::bCONFIGPressed(){

    lista_config_selected.push_front("config");
    qDebug()<<lista_config_selected;
    daq_commands.start("send_transition.sh",lista_config_selected);
    daq_commands.waitForFinished(30000);
    daq_commands.start("status.sh",QStringList()<<"");
    daq_commands.waitForFinished();
    QByteArray byte_status = daq_commands.readAll();
    QTextCodec* codec;
    QStringList daq_string = codec->codecForMib(106)->toUnicode(byte_status).split("'",QString::KeepEmptyParts);
    QString str_status = daq_string.at(1);
    qDebug()<<str_status;
    ui->lbStatus->setText(status_map.value(str_status).toUpper());
    QRegExp reg("configuring");
    reg.setPatternSyntax(QRegExp::Wildcard);
    while(reg.exactMatch(str_status)){

        daq_string.clear();
        daq_commands.start("status.sh",QStringList()<<"");
        daq_commands.waitForFinished();
        byte_status = daq_commands.readAll();
        daq_string = codec->codecForMib(106)->toUnicode(byte_status).split("'",QString::KeepEmptyParts);
        str_status = daq_string.at(1);
        ui->lbStatus->setText(status_map.value(str_status).toUpper());
        this->repaint();
        //DAQInterfaceOutput();

    }
    reg.setPattern("ready");
    ui->lbStatus->setText(status_map.value(str_status).toUpper());
    if(reg.exactMatch(str_status)){

        banCONFIGURED = true;
        isLVSelected();

    }
    reg.setPattern("booted");
    if(reg.exactMatch(str_status)){
        banBOOTED = true;
        banCONFIGURED = false;
        isLVSelected();
    }
    reg.setPattern("stopped");
    if(reg.exactMatch(str_status)){
        banBOOTED = false;
        banCONFIGURED = false;
        isLVSelected();
    }
    qDebug()<<str_status;
    lista_config_selected.removeFirst();

}

void MainWindow::lvBOOTConfigSelected(){

    lista_BOOTConfig_selected.clear();
    QStringList lista_str;
    QModelIndexList lista = ui->lvConfigBOOT->selectionModel()->selectedRows();
    if(lista.length() != 0)
    {
        qDebug()<<lista.length();
        for(QModelIndex indice : lista){
            lista_str = indice.model()->data(indice,Qt::DisplayRole).toString().split(' ',QString::KeepEmptyParts);
            QString str = "docs/"+lista_str.first();
            lista_BOOTConfig_selected.append(str);
            lista_str.clear();
        }
        qDebug()<<lista_BOOTConfig_selected;
        banBOOTCONFIG = true;

    }else{
        banBOOTCONFIG = false;
    }
    isLVSelected();
}

void MainWindow::lvComponentesSelected(){

    lista_comps_selected.clear();
    QStringList lista_str;
    QModelIndexList lista = ui->lvComponentes->selectionModel()->selectedRows();
    if(lista.length() != 0)
    {
        qDebug()<<lista.length();
        for(QModelIndex indice : lista){
            lista_str = indice.model()->data(indice,Qt::DisplayRole).toString().split(' ',QString::KeepEmptyParts);
            lista_comps_selected.append(lista_str.first());
            lista_str.clear();
        }
        qDebug()<<lista_comps_selected;
        banBOOT = true;

    }else{
        banBOOT = false;
    }
    isLVSelected();
}

void MainWindow::lvConfiguracionesSelected(){

    lista_config_selected.clear();
    QStringList lista_str;
    QModelIndexList lista = ui->lvConfiguraciones->selectionModel()->selectedRows();
    if(lista.length() != 0)
    {
        qDebug()<<lista.length();
        for(QModelIndex indice : lista){
            lista_str = indice.model()->data(indice,Qt::DisplayRole).toString().split(' ',QString::KeepEmptyParts);
            lista_config_selected.append(lista_str.first());
            lista_str.clear();
        }
        qDebug()<<lista_config_selected;
        banCONFIG = true;

    }else{
        banCONFIG = false;
    }
    isLVSelected();
}

void MainWindow::estadoTransicion(){

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

void MainWindow::setBotonesDAQInterfaceInicializado(){

    ui->bDAQInterface->setEnabled(false);
    ui->bDAQcomp->setEnabled(true);
    ui->bDAQconf->setEnabled(true);
    ui->bFinalizarSesion->setEnabled(true);
    timer.start(500);
}

void MainWindow::bDAQInterfacePressed(){

    QString env_str = env.value("ARTDAQ_DAQINTERFACE_DIR","DEFAULT");
    //qDebug()<<env_str;
    //QString wd = "/home/nfs/ecristal/sbnd-daq/localProducts_sbndaq_v0_01_03_debug_e15/artdaq_daqinterface/v3_03_02";
    QString wd = env_str;
    daq_interface.setWorkingDirectory(wd);
    daq_commands.setWorkingDirectory(wd);
    QStringList daqinterface_start_commands;
    QString partition_number_str = env.value("DAQINTERFACE_PARTITION_NUMBER","DEFAULT");
    QString rpc_port_str = env.value("DAQINTERFACE_PORT","DEFAULT");
    daqinterface_start_commands <<"./rc/control/daqinterface.py --partition-number"<<partition_number_str<<"--rpc-port"<<rpc_port_str;
    daq_interface.start(daqinterface_start_commands.join(" "));
    /*daq_interface.start("python",QStringList()<<"/root/artdaq-demo-base/artdaq-utilities-daqinterface/rc/control/daqinterface.py");
    bool started = daq_interface.waitForStarted();
    
    if (started){qDebug("Started");}else{qDebug("Fallo"); return;}*/

   
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
    QStringList lista = daq_string.split('\n', QString::SkipEmptyParts);
    lista.removeFirst();
    model->setStringList(lista);
    ui->lvComponentes->setModel(model);
    ui->lvComponentes->setSelectionMode(QAbstractItemView::MultiSelection);
    DAQState = 0;
}

void MainWindow::bListDAQComps(){

    daq_commands.start("listdaqcomps.sh");
    DAQState = 1;
    QThread::msleep(100);
}

void MainWindow::lvConfigs(){

    QStringListModel* model = new QStringListModel(this);
    QStringList lista = daq_string.split("\n\n", QString::SkipEmptyParts);
    //qDebug()<<lista.at(0);
    //lista.removeFirst();
    //lista.removeLast();
    QString lista_config = lista.at(0);
    lista = lista_config.split('\n');
    //qDebug()<<lista;
    lista.removeFirst();
    lista.removeFirst();
    model->setStringList(lista);
    ui->lvConfiguraciones->setModel(model);
    DAQState = 0;
}

void MainWindow::bListDAQConfigs(){

    daq_commands.start("listconfigs.sh");
    DAQState = 2;
    QRegExp reg("*.txt");
    reg.setPatternSyntax(QRegExp::Wildcard);
    QString env_str = env.value("ARTDAQ_DAQINTERFACE_DIR","DEFAULT");
    QDirIterator dirIt(env_str + "/docs");
    QString str;
    QStringList lista_str, lista_config;
    while(dirIt.hasNext()){
        str = dirIt.next();
        if(reg.exactMatch(str)){

            qDebug()<<"config file "<<str;
            lista_str = str.split('/', QString::SkipEmptyParts);
            qDebug()<<lista_str.last();
            lista_config.append(lista_str.last());
        }else{
            qDebug()<<"not config file";
        }

    }

    QStringListModel* model = new QStringListModel(this);
    model->setStringList(lista_config);
    ui->lvConfigBOOT->setModel(model);

    QThread::msleep(100);
}

void MainWindow::MensajeParaBelen(){

    ui->taDAQInterface->document()->setPlainText("Sos la mujer mas hermosa, no puedo dejar de amarte. Sos una bendicion en mi vida. Te quiero muchisimo");

}
