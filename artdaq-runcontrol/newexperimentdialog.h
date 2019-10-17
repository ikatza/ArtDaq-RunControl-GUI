#ifndef NEWEXPERIMENTDIALOG_H
#define NEWEXPERIMENTDIALOG_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QDebug>
#include <QProcessEnvironment>
#include <QFileDialog>
#include <QDirIterator>
#include <QVector>
#include <QMessageBox>
#include <QMenu>
#include <QMouseEvent>
#include <QModelIndex>
#include <QPoint>
#include <QIcon>
#include <QPixmap>
#include <QList>
#include <QTableWidgetSelectionRange>
#include <QModelIndexList>
#include <QModelIndex>

namespace Ui {
class newExperimentDialog;
}

class newExperimentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit newExperimentDialog(QWidget *parent = 0);
    ~newExperimentDialog();

    QVector<QStringList> getComponentsList();
    QStringList getConfigurationsList();
    QString getExperimentName();
    void setLabelTitle(QString title);
    void setExperimentDialog(QString experimentName, QVector<QStringList> componentsList, QStringList configurationList);
    QStringList getBootFilesList();
private slots:
    void bAddComponentToListPressed();
    void bBrowseComponentListPressed();
    void bAddComponentListFilePressed();
    void bAddConfigurationToListPressed();
    void bBrowseConfigurationListPressed();
    void bAddConfigurationFolderPressed();
    void bbOkPressed();
    void accept();
    void bRemoveComponentFromListPressed();
    void bRemoveConfigurationtFromListPressed();
private:

    QString experimentName;
    QVector<QStringList> componentsList;
    QStringList configurationList;

    Ui::newExperimentDialog *ui;
    void initializeComponentsTable();
    void clearAddComponentsTextfield();
    void initializeButtons();
};

#endif // NEWEXPERIMENTDIALOG_H
