#ifndef DAQINTERFACESTATE_HPP
#define DAQINTERFACESTATE_HPP

#include <QWidget>
#include <QGraphicsScene>
#include <QPixmap>
#include <QString>
#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <string.h>
#include <QProcess>
#include <QTextCodec>

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
  void setRun_number(int value);
  bool IsRunning() const;
  void setIsRunning(bool value);

public slots:
  void setStateDiagramOff();
private slots:
  void setOnlineButtonLightGreen();
  void parseRun_number();
  void setRunNumberLCDRed();
  void setRunNumberLCDGreen();
private:
  Ui::daqInterfaceState *ui;
  QString imagesDirectory;
  QTimer timerTransition;
  QTimer timerOnline;
  QProcess gpp; //general purpose process
  int run_number;
  bool isRunning = false;
};

#endif // DAQINTERFACESTATE_HPP
