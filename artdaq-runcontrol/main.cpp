#include "mainwindow.h"
#include "messagehandler.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  qInstallMessageHandler(customLogWriter);
  // qDebug() << "Sample Debug message.";
  // qInfo() << "Sample Info message.";
  // qWarning() << "Sample Warning message.";
  // qCritical() << "Sample Critical message.";
  // qFatal doesn't support stream format.
  // It will throw a core dump and abnormally end execution.
  // qFatal("Sample Fatal message.");

  qInfo() << "artDAQ Run Control GUI"
          << "\n\tProject Name: " << NAME
          << "\n\tVersion: "      << VERSION
          << "\n\tSystem: "       << SYSTEM
          << "\n\tDeveloped by:"
          << "\n\t\tEsteban Cristaldo (Universidad Nacional de Asunción)"
          << "\n\t\tIker Loïc de Icaza Astiz (University of Sussex)";

  qDebug() << "Build information:"
           << "\n\t\tgit hash: " << GIT_HASH
           << "\n\t\tgit branch: " << GIT_BRANCH
           << "\n\t\tcompile time: " << __DATE__ << " @ " << __TIME__;

  QApplication app(argc, argv);
  MainWindow w;
  w.show();

  return app.exec();
}
