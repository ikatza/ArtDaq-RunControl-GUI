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

private slots:
    void populateLvConfiguration();
    void tfConfigNameModified();
    void bImportPressed();
    void listViewClicked();
    void bRefreshListPressed();
private:
    Ui::conftool_import *ui;
    QProcess conftoolpy;
    QStringList daq_string;
};

#endif // CONFTOOL_IMPORT_H
