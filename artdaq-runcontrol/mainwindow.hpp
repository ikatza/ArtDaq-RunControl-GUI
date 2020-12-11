#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

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
#include <QPair>
#include <QVector>
#include <QScrollBar>
#include <QtMath>
#include <QMenuBar>
#include <QCloseEvent>

#include "daqinterfacestate.hpp"
#include "xmlrpc_gui_comm.hpp"
#include "db_dialog.hpp"
#include "env_vars.hpp"
#include "menuoptionsdialog.hpp"
#include "messagehandler.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  QProcessEnvironment getQProcessEnvironment();

  QString getDBConfigurationFHICL_dir() const;
  void setDBConfigurationFHICL_dir(const QString &value);

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
  void bDebugPressed();
  void bListDatabaseRunConfigurations();
  void checkBoxDatabaseChanged();
  void populateLVComponentsFromDatabase();
  void populateLVConfigurationsFromDatabase();
  void populateLVBOOTConfigurationsFromDatabase();
  void initializeLV();
  void bStartRunPressed();
  void checkTransitionStartRunPressed(QString status);
  void resizeEvent(QResizeEvent *event);
  void configurateMenuBar();
  void openMenuOptionsDialog();
  void resizeWindow();
  void setButtonsStoppedEnabled();
  void setButtonsStoppedDisabled();
  void setAllButtonsDisabled();
  void closeProgram();
  void closeEvent(QCloseEvent *event);
  void showDaqInterfaceStateWindow();

private:
  Ui::MainWindow *ui;
  QProcess daq_interface;
  QProcess daq_commands;
  QString daq_string;
  QString DBConfigurationFHICL_dir;
  QString daqInterfaceTextAreaLog;
  QStringList list_comps_selected, list_config_selected, list_BOOTConfig_selected;
  int DAQState;
  long long int DAQInterface_PID;
  bool DAQInterfaceProcess_started;
  bool banBOOT, banCONFIG, banBOOTCONFIG, banBOOTED, banCONFIGURED, banRUNNING, banPAUSED, banStartRunPressed;
  bool startRunConfigSignalIssued, startRunStartSignalIssued;
  QTimer timer;
  QFileSystemWatcher DAQInterface_logwatcher;
  QMap<QString, QString> status_map = {{"stopped", "stopped"},
    {"booting", "booting"},
    {"booted", "booted"},
    {"configuring", "configuring"},
    {"configured", "configured"},
    {"ready", "ready"},
    {"starting", "starting"},
    {"running", "running"},
    {"paused", "paused"},
    {"stopping", "stopping"},
    {"terminating", "terminating"}
  };

  QMap<QString, int> status_map_int = {{"stopped", 1},
    {"booted", 2},
    {"ready", 3},
    {"running", 4},
    {"paused", 5},
    {"booting", 6},
    {"configuring", 7},
    {"starting", 8},
    {"stopping", 9},
    {"terminating", 10},
    {"offline", 99}
  };
  daqInterfaceState state_diagram;
  xmlrpc_gui_comm commDAQInterface;
  QPair<QString, QString> dbSelectedConfig;
  QSize originalWindowSize;
  QSize gbDAQInterfaceCommandsSize, gbTransitionCommandsSize, gbDatabaseSize, gbDAQInterfaceSize;
  QPoint gbDAQInterfaceCommandsPosition, gbTransitionCommandsPosition, gbDatabasePosition, gbDAQInterfacePosition;
  QSize bStartSize, bStopSize, bPauseSize, bTerminateSize, bStartRunSize, bListDatabaseRunConfigurationsSize;
  QPoint bStartPosition, bStopPosition, bPausePosition, bTerminatePosition, bStartRunPosition, bListDatabaseRunConfigurationsPosition;
  QSize bDAQcompSize, bDAQconfSize, bBOOTSize, bCONFIGSize, bDAQInterfaceSize, bEndSessionSize;
  QPoint bDAQcompPosition, bDAQconfPosition, bBOOTPosition, bCONFIGPosition, bDAQInterfacePosition, bEndSessionPosition;
  QSize lvComponentsSize, lvConfigurationsSize, lvConfigBOOTSize, taDAQInterfaceSize, checkBoxDatabaseSize;
  QPoint lvComponentsPosition, lvConfigurationsPosition, lvConfigBOOTPosition, taDAQInterfacePosition, checkBoxDatabasePosition;
  QSize lbConfigurationsSize, lbComponentsSize, lbBOOTConfigSize, lbMessagesSize, lbStatusTitleSize, lbStatusSize;
  QPoint lbConfigurationsPosition, lbComponentsPosition, lbBOOTConfigPosition, lbMessagesPosition, lbStatusTitlePosition, lbStatusPosition;
  QFont lbConfigurationsFont, lbComponentsFont, lbBOOTConfigFont, lbMessagesFont, lbStatusTitleFont, lbStatusFont;
  QFont gbDAQInterfaceFont, gbDAQInterfaceCommandsFont, bStartRunFont, taDAQInterfaceFont, lvComponentsFont, lvConfigurationsFont, lvBOOTConfigFont;
  QSize bStartRunIconSize;
  double originalQuadraticMeanConfigurationFontSize;
  bool EnableFontAutoResizing = true;
  bool enableShellScripts = false;
};

#endif // MAINWINDOW_HPP
