#ifndef NEWPROFILEDIALOG_H
#define NEWPROFILEDIALOG_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <QDebug>
#include <QMessageBox>
#include <QStringList>
#include <QDirIterator>
#include <QProcessEnvironment>

namespace Ui {
class newProfileDialog;
}

class newProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit newProfileDialog(QWidget *parent = 0);
    ~newProfileDialog();
    void populateComboConfigurations(QStringList configurationList);
    void populateLVInExperiment(QVector<QStringList> componentList);
    QString getProfileName();
    QStringList getComponentsToBeAdded();
    QString getSelectedConfiguration();
    QString getSelectedBootFile();
    void setListViews(QStringList experimentComponents, QStringList profileComponents);
    void setTfProfileName(QString profileName);
    void setComboConfigurations(QStringList experimentConfigurationList);
    void setComboBootConfiguration(QStringList experimentBootList);
    void setLabelTitle(QString title);
    void setProfileDialog(QString experimentName, QStringList experimentComponentList, QStringList profileComponentList, QStringList configurationList, QStringList bootfileList);
private slots:
    void addComponentsToLVAddComponents();
    void removeComponentsFromLVAddComponents();
    void bbOkPressed();
private:
    QStringList componentListToBeAdded;
    QStringList componentList;
    QString profileName;
    QString bootFile;
    QString selectedConfiguration;

    Ui::newProfileDialog *ui;
    void initializeButtons();
    void configurateLVs();
    void accept();
    void populateComboBootFiles();
};

#endif // NEWPROFILEDIALOG_H
