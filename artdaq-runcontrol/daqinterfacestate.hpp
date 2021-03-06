#ifndef DAQINTERFACESTATE_HPP
#define DAQINTERFACESTATE_HPP

#include <QWidget>
#include <QGraphicsScene>
#include <QPixmap>
#include <QString>
#include <QCoreApplication>
#include <QDebug>
#include <QTimer>

#include "gui_utility_functions.hpp"

namespace Ui {
class daqInterfaceState;
}

class daqInterfaceState : public QWidget {
  Q_OBJECT

public:
  explicit daqInterfaceState(QWidget *parent = 0);
  ~daqInterfaceState();
  void setStateDiagramStopped();
  void setStateDiagramReady();
  void setStateDiagramRunning();
  void setStateDiagramBooted();
  void setStateDiagramBooting();
  void setStateDiagramConfiguring();
  void setStateDiagramStartingRun();
  void setStateDiagramStoppingRun();
  void setStateDiagramTerminating();
  void setOnlineButtonGreen();
  void setLCDPartitionNumber(int number);
  void setLCDPortNumber(int number);
  void setLCDRunNumber(int number);
  void setOnline();
  void setOffline();
  void setOnlineButtonRed();
  int getRun_number() const;
  void setRun_number(int run_number);
  bool IsRunning() const;
  void setIsRunning(bool value);

public slots:
  void setStateDiagramOff();
private slots:
  void setOnlineButtonLightGreen();
  void setRunNumberLCDRed();
  void setRunNumberLCDGreen();
private:
  Ui::daqInterfaceState *ui;
  QString imagesDirectory;
  QTimer timerTransition;
  QTimer timerOnline;
  int run_number;
  bool isRunning = false;
  QGraphicsScene *scene;
};

#endif // DAQINTERFACESTATE_HPP
