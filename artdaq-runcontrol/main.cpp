#include "mainwindow.h"
#include "messagehandler.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  qInstallMessageHandler(customLogWriter);
  //qDebug() << "Sample Debug message.";
  //qInfo() << "Sample Info message.";
  //qWarning() << "Sample Warning message.";
  //qCritical() << "Sample Critical message.";
  // qFatal doesn't support stream format.
  // It will throw a core dump and abnormally end execution. 
  // qFatal("Sample Fatal message.");
  QApplication app(argc, argv);
  MainWindow w;
  w.show();

  return app.exec();
}
