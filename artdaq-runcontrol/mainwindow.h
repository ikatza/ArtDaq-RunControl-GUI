#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QStringList>
#include <QProcessEnvironment>
#include <QDebug>
#include <QTextCodec>
#include <QThread>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QDirIterator>
#include <QRegExp>
#include <QModelIndexList>
#include <QModelIndex>
#include <QMap>
#include <QTimer>
#include <QSizePolicy>
#include <QMessageBox>
#include <QProcessEnvironment>
#include <QFileDialog>
#include "daqinterfacestate.h"
#include "xmlrpc_gui_comm.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

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
    void setButtonsDAQInterfaceInitialized();
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
    void bDebugPressed();
private:
    Ui::MainWindow *ui;
    QProcess daq_interface;
    QProcess daq_commands;
    QProcessEnvironment env;
    QString daq_string;
    QStringList list_comps_selected, list_config_selected, list_BOOTConfig_selected;
    int DAQState;
    bool banBOOT,banCONFIG,banBOOTCONFIG, banBOOTED,banCONFIGURED,banRUNNING,banPAUSED;
    QTimer timer;
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
};

#endif // MAINWINDOW_H
