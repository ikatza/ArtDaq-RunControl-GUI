#ifndef DAQINTERFACESTATE_H
#define DAQINTERFACESTATE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QPixmap>
#include <QString>
#include <QCoreApplication>
#include <QDebug>
#include <QTimer>

namespace Ui {
class daqInterfaceState;
}

class daqInterfaceState : public QWidget
{
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
    void isOnline();
    void isOffline();
    void setOnlineButtonRed();
public slots:
    void setStateDiagramOff();
private slots:
    void setOnlineButtonLightGreen();
private:
    Ui::daqInterfaceState *ui;
    QString imagesDirectory;
    QTimer timerTransition;
    QTimer timerOnline;
};

#endif // DAQINTERFACESTATE_H
