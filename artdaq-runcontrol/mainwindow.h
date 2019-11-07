#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QProcessEnvironment>
#include <QStringList>
#include <QStringListModel>
#include <QDebug>
#include <QTextCodec>
#include <QThread>
#include <QAbstractItemView>
#include <QDirIterator>
#include <QRegExp>
#include <QModelIndexList>
#include <QModelIndex>
#include <QMap>
#include <QTimer>
#include <QSizePolicy>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileSystemWatcher>
#include <QVector>

#include "daqinterfacestate.h"
#include "xmlrpc_gui_comm.h"
#include "newexperimentdialog.h"
#include "newprofiledialog.h"
#include "conftool_import.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QProcessEnvironment getQProcessEnvironment();

private slots:

    void bEndSessionPressed();
    void bDAQInterfacePressed();
    void DAQInterfaceOutput();
    void MensajeParaBelen();
    void bListDAQComps();
    void bListDAQConfigs();
    void lvComps();
    void lvConfigs();
    void initializeButtons();
    void setButtonsDAQInterfaceInitialized(bool started);
    void isLVSelected();
    void lvComponentsSelected();
    void lvConfigurationsSelected();
    void bBOOTPressed();
    void lvBOOTConfigSelected();
    void bCONFIGPressed();
    void bSTARTPressed();
    void status(QString status);
    void checkStatus();
    void statusTransition();
    void bSTOPPressed();
    void bTERMINATEPressed();
    void configurateWindow();
    //void menuSourceConfigFilePressed();
    //void bNewExperimentPressed();
    //void bEditExperimentPressed();
    //void bDeleteExperimentPressed();
    //void bNewProfilePressed();
    //void populateComboProfiles();
    //void comboExperimentIndexChanged();
    //void populateListViews();
    //void bEditProfilePressed();
    //void bDeleteProfilePressed();
    void bDebugPressed();

    void bImportFromDatabase();
protected:
    QProcessEnvironment env;

private:
    Ui::MainWindow *ui;
    QProcess daq_interface;
    QProcess daq_commands;
    QString daq_string, user_str, wd;
    QString DAQInterface_logdir;
    QStringList list_comps_selected, list_config_selected, list_BOOTConfig_selected;
    int DAQState;
    int DAQInterface_PID;
    bool DAQInterfaceProcess_started;
    bool banBOOT,banCONFIG,banBOOTCONFIG, banBOOTED,banCONFIGURED,banRUNNING,banPAUSED;
    QTimer timer;
    QFileSystemWatcher DAQInterface_logwatcher;
    QMap<QString,QString> status_map = {{"stopped","stopped"},
                                            {"booting","booting"},
                                            {"booted","booted"},
                                            {"configuring","configuring"},
                                            {"configured","configured"},
                                            {"ready","ready"},
                                            {"starting","starting"},
                                            {"running","running"},
                                            {"paused","paused"},
                                            {"stopping","stopping"},
                                            {"terminating","terminating"}
                                           };

    QMap<QString,int> status_map_int = {{"stopped",1},
                                            {"booted",2},
                                            {"ready",3},
                                            {"running",4},
                                            {"paused",5},
                                            {"booting",6},
                                            {"configuring",7},
                                            {"starting",8},
                                            {"stopping",9},
                                            {"terminating",10},
                                            {"offline",99}
                                           };
    daqInterfaceState state_diagram;
    xmlrpc_gui_comm commDAQInterface;
    void populateComboExperiments();
};

#endif // MAINWINDOW_H
