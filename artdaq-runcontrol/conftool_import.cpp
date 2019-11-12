#include "conftool_import.h"
#include "ui_conftool_import.h"

conftool_import::conftool_import(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::conftool_import)
{
    ui->setupUi(this);
    QString wd = QCoreApplication::applicationDirPath() + "/../dbConfigurations";
    conftoolpy.setWorkingDirectory(wd);
    ui->bOK->button(QDialogButtonBox::Ok)->setText("Import");
    connect(ui->tfConfigName,SIGNAL(textEdited(QString)),this,SLOT(tfConfigNameModified()));
    connect(ui->bOK->button(QDialogButtonBox::Ok),SIGNAL(clicked(bool)),this,SLOT(bImportPressed()));
    connect(ui->lvConfigurationList,SIGNAL(clicked(QModelIndex)),this,SLOT(listViewClicked()));
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
    QStringList daq_string = codec->codecForMib(106)->toUnicode(byte_status).split("\n",QString::KeepEmptyParts);
    qDebug()<<daq_string;
    QStringListModel* model = new QStringListModel(this);
    model->setStringList(daq_string);
    ui->lvConfigurationList->setModel(model);
    this->listViewClicked();
}

void conftool_import::tfConfigNameModified(){

    QString comboText = ui->tfConfigName->text();
    conftoolpy.start("conftool.py",QStringList()<<"getListOfAvailableRunConfigurations"<<comboText);
    conftoolpy.waitForFinished();
    QByteArray byte_status = conftoolpy.readAll();
    QTextCodec* codec;
    QStringList daq_string = codec->codecForMib(106)->toUnicode(byte_status).split("\n",QString::KeepEmptyParts);
    QStringListModel* model = new QStringListModel(this);
    model->setStringList(daq_string);
    ui->lvConfigurationList->setModel(model);

}

void conftool_import::bImportPressed(){
    QStringList db_profile_stringlist;
    QModelIndexList list = ui->lvConfigurationList->selectionModel()->selectedRows();
    for(QModelIndex idx : list){
        db_profile_stringlist = idx.model()->data(idx,Qt::DisplayRole).toString().split(' ',QString::KeepEmptyParts);
    }
    conftoolpy.start("conftool.py",QStringList()<<"exportConfiguration"<<db_profile_stringlist.at(0));
    conftoolpy.waitForFinished();
}

void conftool_import::listViewClicked(){
    QModelIndexList selectedList = ui->lvConfigurationList->selectionModel()->selectedRows();
    int selectedSize = selectedList.size();
    if(selectedSize == 0){
        ui->bOK->button(QDialogButtonBox::Ok)->setEnabled(false);
    }else{
        ui->bOK->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}
