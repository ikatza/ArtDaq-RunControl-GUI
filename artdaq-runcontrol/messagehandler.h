#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H



#include <QtGlobal>
#include <QTime>
#include <iostream>

#define COLOR_DEBUG "\033[32;1m"
#define COLOR_INFO "\033[32;1m"
#define COLOR_WARN "\033[33;1m"
#define COLOR_CRITICAL "\033[31;1m"
#define COLOR_FATAL "\033[33;1m"
#define COLOR_RESET "\033[0m"

#define LOG_WRITE(OUTPUT, COLOR, LEVEL, MSG, FILE, LINE, FUNCTION) OUTPUT << COLOR << QTime::currentTime().toString("hhmmsszzz").toStdString() << " " LEVEL " " << COLOR_RESET << MSG << FILE << LINE << FUNCTION << "\n"

void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

void customLogWriter(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // MESSAGEHANDLER_H
