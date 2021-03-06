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
#include <QDir>
#include <QDirIterator>
#include <QRegExp>
#include <QModelIndexList>
#include <QModelIndex>
#include <QMap>
#include <QTimer>
#include <QSizePolicy>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QFileSystemWatcher>
#include <QPair>
#include <QVector>
#include <QScrollBar>
#include <QTextStream>
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
  explicit MainWindow(QWidget* parent = 0);
  ~MainWindow();
  // QProcessEnvironment getQProcessEnvironment();

  // QString getDBConfigurationFHICL_dir() const;
  void setDBConfigurationFHICL_dir(const QString& dir);

private slots:
  void bEndSessionPressed();
  void bDAQInterfacePressed();
  void DAQInterfaceOutput();
  void MensajeParaBelen();
  void bLastRunConfigPressed();
  void saveRunConfig(const QString& runFileName);
  void retrieveConfigFromFile(const QString& runFileName);
  void bListDAQCompsEtConfigs();
  void listDAQComps();
  void listDAQConfigs();
  void populateLVComps(const QString& di_comps_output);
  void populateLVConfigs(const QString& di_configs_output);
  void setButtonsDAQInterfaceInitialized(bool started);
  void isLVSelected();
  void lvComponentsSelected();
  void lvConfigurationsSelected();
  void bBOOTPressed();
  void lvBOOTConfigSelected();
  void bCONFIGPressed();
  void bSTARTPressed();
  void status(const QString& status);
  void checkStatus();
  void statusTransition();
  void bSTOPPressed();
  void bTERMINATEPressed();
  void bDebugPressed();
  void bListDatabaseRunConfigurations();
  void checkBoxDatabaseChanged();
  void populateLVComponentsFromDatabase();
  void populateLVConfigurationsFromDatabase();
  void populateLVBOOTConfigurationsFromDatabase();
  void bRunPressed();
  void checkTransitionRunPressed(const QString& status);
  void resizeEvent(QResizeEvent* event);
  void configurateWindow();
  void configurateMenuBar();
  void initializeFlags();
  void initializeButtons();
  void initializeLV();
  void openMenuOptionsDialog();
  void resizeWindow();
  void setButtonsStoppedEnabled();
  void setButtonsStoppedDisabled();
  void setAllButtonsDisabled();
  void closeProgram();
  void closeEvent(QCloseEvent *event);
  void showDaqInterfaceStateWindow();
  void saveConfigDialog();
  void openConfigDialog();

private:
  Ui::MainWindow *ui;
  QProcess daq_interface;
  QString daq_string;
  const QDir homePath, localConfigPath;
  const QString lastRunFileName;
  QString DBConfigurationFHICL_dir;
  QString daqInterfaceTextAreaLog;
  QStringList list_comps_selected, list_config_selected,
    list_BOOTConfig_selected;
  int DAQState;
  long long int DAQInterface_PID;
  bool DAQInterfaceProcess_started;
  bool flgBOOT, flgCONFIG, flgBOOTCONFIG, flgBOOTED,
    flgCONFIGURED, flgRUNNING, flgPAUSED, flgRunPressed;
  bool runConfigSignalIssued, runStartSignalIssued;
  QTimer timer;
  QFileSystemWatcher DAQInterface_logwatcher;
  // QMap<QString, QString> status_map =
  // {
  //   {"stopped", "stopped"},
  //   {"booting", "booting"},
  //   {"booted", "booted"},
  //   {"configuring", "configuring"},
  //   {"configured", "configured"},
  //   {"ready", "ready"},
  //   {"starting", "starting"},
  //   {"running", "running"},
  //   {"paused", "paused"},
  //   {"stopping", "stopping"},
  //   {"terminating", "terminating"}
  // };
  QMap<QString, int> status_map_int =
  {
    {"stopped",      1},
    {"booted",       2},
    {"ready",        3},
    {"running",      4},
    {"paused",       5},
    {"booting",      6},
    {"configuring",  7},
    {"starting",     8},
    {"stopping",     9},
    {"terminating", 10},
    {"offline",     99}
  };
  daqInterfaceState state_diagram;
  xmlrpc_gui_comm commDAQInterface;
  QPair<QString, QString> dbSelectedConfig;
  QSize originalWindowSize;
  QSize gbDAQInterfaceCommandsSize, gbTransitionCommandsSize, gbDatabaseSize, gbDAQInterfaceSize;
  QPoint gbDAQInterfaceCommandsPosition, gbTransitionCommandsPosition, gbDatabasePosition, gbDAQInterfacePosition;
  QSize bStartSize, bStopSize, bPauseSize, bTerminateSize, bRunSize, bListDatabaseRunConfigurationsSize;
  QPoint bStartPosition, bStopPosition, bPausePosition, bTerminatePosition, bRunPosition, bListDatabaseRunConfigurationsPosition;
  QSize bDAQCompEtConfSize, bLastRunConfigSize, bBOOTSize, bCONFIGSize, bDAQInterfaceSize, bEndSessionSize;
  QPoint bDAQCompEtConfPosition, bLastRunConfigPosition, bBOOTPosition, bCONFIGPosition, bDAQInterfacePosition, bEndSessionPosition;
  QSize lvComponentsSize, lvConfigurationsSize, lvConfigBOOTSize, taDAQInterfaceSize, checkBoxDatabaseSize;
  QPoint lvComponentsPosition, lvConfigurationsPosition, lvConfigBOOTPosition, taDAQInterfacePosition, checkBoxDatabasePosition;
  QSize lbConfigurationsSize, lbComponentsSize, lbBOOTConfigSize, lbMessagesSize, lbStatusTitleSize, lbStatusSize;
  QPoint lbConfigurationsPosition, lbComponentsPosition, lbBOOTConfigPosition, lbMessagesPosition, lbStatusTitlePosition, lbStatusPosition;
  QFont lbConfigurationsFont, lbComponentsFont, lbBOOTConfigFont, lbMessagesFont, lbStatusTitleFont, lbStatusFont;
  QFont gbDAQInterfaceFont, gbDAQInterfaceCommandsFont, bRunFont, taDAQInterfaceFont, lvComponentsFont, lvConfigurationsFont, lvBOOTConfigFont;
  QSize bRunIconSize;
  double originalQuadraticMeanConfigurationFontSize;
  bool EnableFontAutoResizing = true;
  bool enableShellScripts = false;
  QAction *optionsMenu;
  QAction *exitMenu;
  QAction *openConfigMenu;
  QAction *saveConfigMenu;
  QAction *windowMenu;
  QMenu *Menus;
  QStringListModel* lvComponentsModel;
  QStringListModel* lvConfigurationsModel;
  QStringListModel* lvConfigBOOTModel;
};

#endif // MAINWINDOW_HPP
