#include "conftool_import.h"
#include "ui_conftool_import.h"

conftool_import::conftool_import(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::conftool_import)
{
    ui->setupUi(this);
    env = QProcessEnvironment::systemEnvironment();
    wd = QCoreApplication::applicationDirPath();
    conftoolpy.setWorkingDirectory(wd);
    ui->bOK->button(QDialogButtonBox::Ok)->setText("Select");
    connect(ui->tfConfigName,SIGNAL(textEdited(QString)),this,SLOT(tfConfigNameModified()));
    connect(ui->bOK->button(QDialogButtonBox::Ok),SIGNAL(clicked(bool)),this,SLOT(bSelectPressed()));
    connect(ui->lvConfigurationList,SIGNAL(clicked(QModelIndex)),this,SLOT(listViewClicked()));
    connect(ui->bRefreshList,SIGNAL(clicked(bool)),this,SLOT(bRefreshListPressed()));
    this->populateLvConfiguration();
    ui->lvConfigurationList->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

conftool_import::~conftool_import()
{
    delete ui;
}

void conftool_import::populateLvConfiguration(){

    conftoolpy.start("conftool.py",QStringList()<<"getListOfAvailableRunConfigurations");
    conftoolpy.waitForFinished();
    QByteArray byte_status = conftoolpy.readAll();
    QTextCodec* codec;
    daq_string = codec->codecForMib(106)->toUnicode(byte_status).split("\n",QString::KeepEmptyParts);
    daq_string.removeLast();
    daq_string.sort();
    //std::sort(daq_string.begin(),daq_string.last(),std::greater);
    QStringListModel* model = new QStringListModel(this);
    model->setStringList(daq_string);
    ui->lvConfigurationList->setModel(model);
    this->listViewClicked();
}

// QStringList conftool_import::getListOfDBConfigurations(){
//     conftoolpy.start("conftool.py",QStringList()<<"getListOfAvailableRunConfigurations");
//     conftoolpy.waitForFinished();
//     QByteArray byte_status = conftoolpy.readAll();
//     QTextCodec* codec;
//     daq_string = codec->codecForMib(106)->toUnicode(byte_status).split("\n",QString::KeepEmptyParts);
//     daq_string.removeLast();
//     daq_string.sort();
//     //std::sort(daq_string.begin(),daq_string.last(),std::greater);
//     qDebug()<<daq_string;
//     // QStringListModel* model = new QStringListModel(this);
//     // model->setStringList(daq_string);
//     // ui->lvConfigurationList->setModel(model);
//     // this->listViewClicked();
//     return daq_string;
// }

void conftool_import::tfConfigNameModified(){

    QString comboText = ui->tfConfigName->text();
    if(comboText.length() != 0){
        QRegularExpression reg(comboText);
        QRegularExpressionMatch match;
        QStringList dBConfigList_filtered;
        for(QString dBConfig_str : daq_string){
            match = reg.match(dBConfig_str,0,QRegularExpression::PartialPreferCompleteMatch);
            if(match.hasMatch()){
                dBConfigList_filtered.append(dBConfig_str);
            }
        }
        QStringListModel* model = new QStringListModel(this);
        model->setStringList(dBConfigList_filtered);
        ui->lvConfigurationList->setModel(model);
    }else{
        QStringListModel* model = new QStringListModel(this);
        model->setStringList(daq_string);
        ui->lvConfigurationList->setModel(model);
    }

}

void conftool_import::bSelectPressed(){
    QStringList db_profile_stringlist;
    QModelIndexList list = ui->lvConfigurationList->selectionModel()->selectedRows();
    for(QModelIndex idx : list){
        db_profile_stringlist = idx.model()->data(idx,Qt::DisplayRole).toString().split(' ',QString::KeepEmptyParts);
    }
    QString selected_db_config = db_profile_stringlist.at(0);
    qDebug() << "selected_db_config: " << selected_db_config;

    QString export_dir = export_dir_base + selected_db_config;
    conftoolpy_export.start("mkdir", QStringList() << "-vp" << export_dir);
    conftoolpy_export.waitForFinished();
    conftoolpy_export_output = conftoolpy_export.readAllStandardOutput();
    qDebug() << "\n mkdir -vp: " << conftoolpy_export_output;

    conftoolpy_export.setWorkingDirectory(export_dir);

    conftoolpy_export.start("conftool.py",QStringList()<<"exportConfiguration"<< selected_db_config);
    conftoolpy_export.waitForFinished();
    conftoolpy_export_output = conftoolpy_export.readAllStandardOutput();
    qDebug()  << "\n conftool.py exportConfiguration" << conftoolpy_export_output;

    // conftoolpy_export.start("ls");
    // conftoolpy_export.waitForFinished();
    // conftoolpy_export_output = conftoolpy_export.readAllStandardOutput();
    // qDebug()  << "\n ls" << conftoolpy_export_output;

    this->setSelectedDBConfig(selected_db_config);
}

void conftool_import::listViewClicked(){
    QModelIndexList selectedList = ui->lvConfigurationList->selectionModel()->selectedRows();
    int selectedSize = selectedList.size();
    if(selectedSize == 0){
        ui->bOK->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else if(selectedSize == 1){
        ui->bOK->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
    else{}
}

void conftool_import::bRefreshListPressed(){

    populateLvConfiguration();
    tfConfigNameModified();

}

void conftool_import::setSelectedDBConfig(const QString &value)
{
  selectedDBConfig = value;
}

QPair<QString, QString> conftool_import::getSelectedDBConfig() const{
  return qMakePair(selectedDBConfig, export_dir_base + selectedDBConfig);
}
