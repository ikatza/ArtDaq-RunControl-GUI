#include "messagehandler.h"

void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
  QByteArray localMsg = msg.toLocal8Bit();
  const char *file = context.file ? context.file : "";
  const char *function = context.function ? context.function : "";
  switch (type) {
  case QtDebugMsg:
    fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
    break;
  case QtInfoMsg:
    fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
    break;
  case QtWarningMsg:
    fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
    break;
  case QtCriticalMsg:
    fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
    break;
  case QtFatalMsg:
    fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
    break;
  }
}



void customLogWriter(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
  QByteArray localMsg = msg.toLocal8Bit();
  const char *file = context.file ? context.file : "";
  const char *function = context.function ? context.function : "";
  switch (type) {
  case QtDebugMsg:
    LOG_WRITE(std::cout, COLOR_DEBUG, "DEBUG", localMsg.constData(), file, context.line, function);
    break;
  case QtInfoMsg:
    LOG_WRITE(std::cout, COLOR_INFO, "INFO", localMsg.constData(), file, context.line, function);
    break;
  case QtWarningMsg:
    LOG_WRITE(std::cout, COLOR_WARN, "WARNING", localMsg.constData(), file, context.line, function);
    break;
  case QtCriticalMsg:
    LOG_WRITE(std::cout, COLOR_CRITICAL, "CRITICAL", localMsg.constData(), file, context.line, function);
    break;
  case QtFatalMsg:
    LOG_WRITE(std::cout, COLOR_FATAL, "FATAL", localMsg.constData(), file, context.line, function);
    break;
  }
}

// void logInitialize(void) {
//   qInstallMessageHandler(myCustomLogWriter);
// }


// void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
// {
//     QByteArray localMsg = msg.toLocal8Bit();
//     const char *file = context.file ? context.file : "";
//     const char *function = context.function ? context.function : "";
//     switch (type) {
//     case QtDebugMsg:
//         fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//         break;
//     case QtInfoMsg:
//         fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//         break;
//     case QtWarningMsg:
//         fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//         break;
//     case QtCriticalMsg:
//         fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//         break;
//     case QtFatalMsg:
//         fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//         break;
//     }
// }

