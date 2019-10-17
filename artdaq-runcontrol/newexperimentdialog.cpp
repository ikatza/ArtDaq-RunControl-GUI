#include "newexperimentdialog.h"
#include "ui_newexperimentdialog.h"

newExperimentDialog::newExperimentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newExperimentDialog)
{
    ui->setupUi(this);
    this->initializeComponentsTable();
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->bAddComponentToList,SIGNAL(clicked(bool)),this,SLOT(bAddComponentToListPressed()));
    connect(ui->bBrowseComponentList,SIGNAL(clicked(bool)),this,SLOT(bBrowseComponentListPressed()));
    connect(ui->bAddComponentListFile,SIGNAL(clicked(bool)),this,SLOT(bAddComponentListFilePressed()));
    connect(ui->bAddConfigurationToList,SIGNAL(clicked(bool)),this,SLOT(bAddConfigurationToListPressed()));
    connect(ui->bBrowseConfigurationFolder,SIGNAL(clicked(bool)),this,SLOT(bBrowseConfigurationListPressed()));
    connect(ui->bAddConfigurationFolder,SIGNAL(clicked(bool)),this,SLOT(bAddConfigurationFolderPressed()));
    connect(ui->bRemoveComponentFromList,SIGNAL(clicked(bool)),this,SLOT(bRemoveComponentFromListPressed()));
    connect(ui->bRemoveConfigurationFromList,SIGNAL(clicked(bool)),this,SLOT(bRemoveConfigurationtFromListPressed()));
    connect(this,SIGNAL(accepted()),this,SLOT(bbOkPressed()));
    this->initializeButtons();
}

newExperimentDialog::~newExperimentDialog()
{
    delete ui;
}

void newExperimentDialog::initializeButtons(){

    QString imagesDirectory = QCoreApplication::applicationDirPath() + "/../resources/images/";
    QIcon icon_plus;
    QIcon icon_minus;
    QPixmap pixmap_plus(imagesDirectory + "plus.png");
    icon_plus.addPixmap(pixmap_plus);
    ui->bAddComponentToList->setIcon(icon_plus);
    ui->bAddComponentListFile->setIcon(icon_plus);
    ui->bAddConfigurationFolder->setIcon(icon_plus);
    ui->bAddConfigurationToList->setIcon(icon_plus);
    ui->bAddComponentToList->setText("");
    ui->bAddComponentListFile->setText("");
    ui->bAddConfigurationFolder->setText("");
    ui->bAddConfigurationToList->setText("");
    QPixmap pixmap_minus(imagesDirectory + "minus.png");
    icon_minus.addPixmap(pixmap_minus);
    ui->bRemoveComponentFromList->setIcon(icon_minus);
    ui->bRemoveConfigurationFromList->setIcon(icon_minus);
    ui->bRemoveComponentFromList->setText("");
    ui->bRemoveConfigurationFromList->setText("");

}

void newExperimentDialog::initializeComponentsTable(){

    ui->tableComponents->setRowCount(1);
    ui->tableComponents->setColumnCount(4);
    QStringList columnHeader = {"Name","Port","Host","Subsystem"};
    ui->tableComponents->setHorizontalHeaderLabels(columnHeader);
    int frameWidth = ui->tableComponents->frameGeometry().size().width();
    int columnWidth = frameWidth/4;
    ui->tableComponents->setColumnWidth(0,columnWidth-3);
    ui->tableComponents->setColumnWidth(1,columnWidth-3);
    ui->tableComponents->setColumnWidth(2,columnWidth-3);
    ui->tableComponents->setColumnWidth(3,columnWidth-3);

    ui->tableConfigurations->setRowCount(1);
    ui->tableConfigurations->setColumnCount(4);
    columnHeader.clear();
    columnHeader.append("Configuration");
    ui->tableConfigurations->setHorizontalHeaderLabels(columnHeader);
    frameWidth = ui->tableConfigurations->frameGeometry().size().width();
    columnWidth = frameWidth;
    ui->tableConfigurations->setColumnWidth(0,columnWidth-3);

}

void newExperimentDialog::clearAddComponentsTextfield(){

    ui->tfComponentName->setText("");
    ui->tfComponentHost->setText("");
    ui->tfComponentPort->setText("");
    ui->tfComponentSubsystem->setText("");
}

void newExperimentDialog::bAddComponentToListPressed(){

    QString componentName = ui->tfComponentName->text();
    QString componentHost = ui->tfComponentHost->text();
    QString componentPort = ui->tfComponentPort->text();
    QString componentSubsystem = ui->tfComponentSubsystem->text();
    QTableWidgetItem *tableItemName = new QTableWidgetItem();
    tableItemName->setText(componentName);
    int rows = ui->tableComponents->rowCount();
    ui->tableComponents->setItem(rows-1,0,tableItemName);
    QTableWidgetItem *tableItemHost = new QTableWidgetItem();
    tableItemHost->setText(componentHost);
    ui->tableComponents->setItem(rows-1,1,tableItemHost);
    QTableWidgetItem *tableItemPort = new QTableWidgetItem();
    tableItemPort->setText(componentPort);
    ui->tableComponents->setItem(rows-1,2,tableItemPort);
    QTableWidgetItem *tableItemSubsystem = new QTableWidgetItem();
    tableItemSubsystem->setText(componentSubsystem);
    ui->tableComponents->setItem(rows-1,3,tableItemSubsystem);
    ui->tableComponents->setRowCount(rows+1);
    clearAddComponentsTextfield();

}

void newExperimentDialog::bBrowseComponentListPressed(){
    QString daqinterface_user_dir = QProcessEnvironment::systemEnvironment().value("DAQINTERFACE_USER_DIR","DEFAULT");
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open components list file"), daqinterface_user_dir, tr(""));
    ui->tfComponentListFile->setText(fileName);
}

void newExperimentDialog::bAddComponentListFilePressed(){

    QString fileName = ui->tfComponentListFile->text();
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);

    QTextStream in(&file);
    QStringList comp_line;
    QString line;
    line = in.readLine();

    while(!in.atEnd()) {
        line = in.readLine();
        comp_line = line.split(" ");
        int rows = ui->tableComponents->rowCount();
        QTableWidgetItem *tableItemName = new QTableWidgetItem();
        tableItemName->setText(comp_line.at(0));
        ui->tableComponents->setItem(rows-1,0,tableItemName);
        QTableWidgetItem *tableItemHost = new QTableWidgetItem();
        tableItemHost->setText(comp_line.at(1));
        ui->tableComponents->setItem(rows-1,1,tableItemHost);
        QTableWidgetItem *tableItemPort = new QTableWidgetItem();
        tableItemPort->setText(comp_line.at(2));
        ui->tableComponents->setItem(rows-1,2,tableItemPort);
        QTableWidgetItem *tableItemSubsystem = new QTableWidgetItem();
        tableItemSubsystem->setText(comp_line.at(3));
        ui->tableComponents->setItem(rows-1,3,tableItemSubsystem);
        ui->tableComponents->setRowCount(rows+1);
    }

}

void newExperimentDialog::bAddConfigurationToListPressed(){

    QString configurationName = ui->tfConfigurationName->text();
    QTableWidgetItem *tableItem = new QTableWidgetItem();
    tableItem->setText(configurationName);
    int rows = ui->tableConfigurations->rowCount();
    ui->tableConfigurations->setItem(rows-1,0,tableItem);
    ui->tfConfigurationName->setText("");
    ui->tableConfigurations->setRowCount(rows+1);

}

void newExperimentDialog::bBrowseConfigurationListPressed(){
    QString daqinterface_user_dir = QProcessEnvironment::systemEnvironment().value("DAQINTERFACE_USER_DIR","DEFAULT");
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setOption(QFileDialog::ShowDirsOnly);
    QString configurationDirectory = fileDialog.getExistingDirectory(this,
        tr("Open configuration directory"), daqinterface_user_dir);
    ui->tfConfigurationFolder->setText(configurationDirectory);
}

void newExperimentDialog::bAddConfigurationFolderPressed(){

    QString directory = ui->tfConfigurationFolder->text();
    QDirIterator dirIt(directory, QDir::AllDirs | QDir::NoDotAndDotDot);
    QString configurations;
    QStringList list_str;
    while(dirIt.hasNext()){
        configurations = dirIt.next();
        list_str = configurations.split('/', QString::SkipEmptyParts);
        int rows = ui->tableConfigurations->rowCount();
        QTableWidgetItem *tableItem = new QTableWidgetItem();
        tableItem->setText(list_str.last());
        ui->tableConfigurations->setItem(rows-1,0,tableItem);
        ui->tableConfigurations->setRowCount(rows+1);
    }
}

void newExperimentDialog::bbOkPressed(){

    experimentName = ui->tfExperimentName->text();
    QTableWidgetItem *itemComponentName = new QTableWidgetItem();
    QTableWidgetItem *itemComponentHost = new QTableWidgetItem();
    QTableWidgetItem *itemComponentPort = new QTableWidgetItem();
    QTableWidgetItem *itemComponentSubsystem = new QTableWidgetItem();
    QTableWidgetItem *itemConfiguration = new QTableWidgetItem();
    QStringList component;
    QStringList configuration;
    QVector<QStringList> component_vector;

    for(int i = 0; i<(ui->tableComponents->rowCount() - 1) ; i++){

        itemComponentName = ui->tableComponents->item(i,0);
        itemComponentHost = ui->tableComponents->item(i,1);
        itemComponentPort = ui->tableComponents->item(i,2);
        itemComponentSubsystem = ui->tableComponents->item(i,3);
        component.append(itemComponentName->text());
        component.append(itemComponentHost->text());
        component.append(itemComponentPort->text());
        component.append(itemComponentSubsystem->text());
        component_vector.append(component);
        component.clear();

    }
    componentsList = component_vector;
    //qDebug()<<componentsList;

    for(int i = 0; i<(ui->tableConfigurations->rowCount() - 1) ; i++){

        itemConfiguration = ui->tableConfigurations->item(i,0);
        configuration.append(itemConfiguration->text());

    }
    configurationList = configuration;
    //qDebug()<<configurationList;
}

QVector<QStringList> newExperimentDialog::getComponentsList(){
    return this->componentsList;
}

QStringList newExperimentDialog::getConfigurationsList(){
    return this->configurationList;
}

QString newExperimentDialog::getExperimentName(){
    return this->experimentName;
}

void newExperimentDialog::accept(){

    QString experimentName_ = ui->tfExperimentName->text();
    if(experimentName_ == ""){
        QMessageBox msg;
        msg.setText("Textfield Experiment Name is empty.");
        msg.exec();
        ui->tfExperimentName->setFocus();
    }else{
        this->done(this->Accepted);
    }

}

void newExperimentDialog::bRemoveComponentFromListPressed(){

    QItemSelectionModel *selectionModel = ui->tableComponents->selectionModel();
    QModelIndexList indexList = selectionModel->selectedRows();
    for(QModelIndex index:indexList){
        ui->tableComponents->removeRow(index.row());
    }
    if(ui->tableComponents->rowCount() == 0){
        ui->tableComponents->setRowCount(1);
        ui->tableComponents->setColumnCount(4);
    }
}

void newExperimentDialog::bRemoveConfigurationtFromListPressed(){

    QItemSelectionModel *selectionModel = ui->tableConfigurations->selectionModel();
    QModelIndexList indexList = selectionModel->selectedRows();
    for(QModelIndex index:indexList){
        ui->tableConfigurations->removeRow(index.row());
    }
    if(ui->tableConfigurations->rowCount() == 0){
        ui->tableConfigurations->setRowCount(1);
        ui->tableConfigurations->setColumnCount(1);
    }
}

void newExperimentDialog::setLabelTitle(QString title){
    ui->labelTitle->setText(title);
}

void newExperimentDialog::setExperimentDialog(QString experimentName, QVector<QStringList> componentsList, QStringList configurationList){

    ui->tfExperimentName->setText(experimentName);
    for(QStringList component : componentsList){
        QString componentName = component.at(0);
        QString componentHost = component.at(1);
        QString componentPort = component.at(2);
        QString componentSubsystem = component.at(3);
        QTableWidgetItem *tableItemName = new QTableWidgetItem();
        tableItemName->setText(componentName);
        int rows = ui->tableComponents->rowCount();
        ui->tableComponents->setItem(rows-1,0,tableItemName);
        QTableWidgetItem *tableItemHost = new QTableWidgetItem();
        tableItemHost->setText(componentHost);
        ui->tableComponents->setItem(rows-1,1,tableItemHost);
        QTableWidgetItem *tableItemPort = new QTableWidgetItem();
        tableItemPort->setText(componentPort);
        ui->tableComponents->setItem(rows-1,2,tableItemPort);
        QTableWidgetItem *tableItemSubsystem = new QTableWidgetItem();
        tableItemSubsystem->setText(componentSubsystem);
        ui->tableComponents->setItem(rows-1,3,tableItemSubsystem);
        ui->tableComponents->setRowCount(rows+1);
    }
    for(QString configurationName : configurationList){
        QTableWidgetItem *tableItem = new QTableWidgetItem();
        tableItem->setText(configurationName);
        int rows = ui->tableConfigurations->rowCount();
        ui->tableConfigurations->setItem(rows-1,0,tableItem);
        ui->tfConfigurationName->setText("");
        ui->tableConfigurations->setRowCount(rows+1);
    }
}

QStringList newExperimentDialog::getBootFilesList(){
    QRegExp reg("*.txt");
    reg.setPatternSyntax(QRegExp::Wildcard);
    QString env_str = QProcessEnvironment::systemEnvironment().value("DAQINTERFACE_USER_DIR","DEFAULT");
    QDirIterator dirIt(env_str);
    QString str;
    QStringList list_str, bootList;
    while(dirIt.hasNext()){
        str = dirIt.next();
        if(reg.exactMatch(str)){
            list_str = str.split('/', QString::SkipEmptyParts);
            bootList.append(list_str.last());
        }
    }
    return bootList;
}
























