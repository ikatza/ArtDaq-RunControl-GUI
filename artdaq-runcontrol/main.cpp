#include "mainwindow.h"
#include "messagehandler.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  qInstallMessageHandler(customLogWriter);
  QApplication app(argc, argv);
  MainWindow w;
  w.show();

  return app.exec();
}
