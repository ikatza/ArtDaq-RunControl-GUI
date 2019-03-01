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

    void bFinalizarSesionPressed();
    void bDAQInterfacePressed();
    void DAQInterfaceOutput();
    void MensajeParaBelen();
    void bListDAQComps();
    void bListDAQConfigs();
    void lvComps();
    void lvConfigs();
    void inicializarBotones();
    void setBotonesDAQInterfaceInicializado();
    void isLVSelected();
    void lvComponentesSelected();
    void lvConfiguracionesSelected();
    void bBOOTPressed();
    void lvBOOTConfigSelected();
    void bCONFIGPressed();
    void bSTARTPressed();
    void estado(QString estado);
    void checkStatus();
    void estadoTransicion();
    void bSTOPPressed();
    void bTERMINATEPressed();
    void configurarVentana();
    //void menuSourceConfigFilePressed();
private:
    Ui::MainWindow *ui;
    QProcess daq_interface;
    QProcess daq_commands;
    QProcessEnvironment env;
    QString daq_string;
    QStringList lista_comps_selected, lista_config_selected, lista_BOOTConfig_selected;
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
                                            {"paused",5}
                                           };
};

#endif // MAINWINDOW_H
