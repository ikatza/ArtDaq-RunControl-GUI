#ifndef CONFTOOL_IMPORT_H
#define CONFTOOL_IMPORT_H

#include <QDialog>
#include <QProcess>
#include <QString>
#include <QDebug>
#include <QByteArray>
#include <QTextCodec>
#include <QStringList>
#include <QStringListModel>
#include <QPushButton>
#include <QModelIndexList>
#include <QModelIndex>
#include <QCoreApplication>
#include <QRegularExpression>
#include <QRegularExpressionMatch>


namespace Ui {
class conftool_import;
}

class conftool_import : public QDialog
{
    Q_OBJECT

public:
    explicit conftool_import(QWidget *parent = 0);
    ~conftool_import();
    QStringList getListOfDBConfigurations();
  void setSelectedDBConfig(const QString &value);

private slots:
    void populateLvConfiguration();
    void tfConfigNameModified();
  void bSelectPressed();
    void listViewClicked();
    void bRefreshListPressed();

private:
    Ui::conftool_import *ui;
    QProcess conftoolpy;
  QProcess conftoolpy_export;
  QString export_dir_base = "/tmp/artdaq-runcontrol-gui/db/";
  QString conftoolpy_export_output;
  QString selectedDBConfig;

    QStringList daq_string;
    QProcessEnvironment env;
};

#endif // CONFTOOL_IMPORT_H
