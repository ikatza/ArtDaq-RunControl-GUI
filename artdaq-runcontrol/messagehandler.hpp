#ifndef MESSAGEHANDLER_HPP
#define MESSAGEHANDLER_HPP



#include <QtGlobal>
#include <QTime>
#include <iostream>
#include <QDebug>

#define COLOR_DEBUG "\033[1;37m"
#define COLOR_INFO "\033[1;32m"
#define COLOR_WARN "\033[1;33m"
#define COLOR_CRITICAL "\033[1;31m"
#define COLOR_FATAL "\033[41m"
#define COLOR_RESET "\033[0m"

#define LOG_WRITE(OUTPUT, COLOR, LEVEL, MSG, FILE, LINE, FUNCTION) OUTPUT \
<< COLOR << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss:zzz").toStdString() \
<< " " LEVEL " " << COLOR_RESET \
<< "\nFILE::" << FILE \
<< " LINE::" << LINE \
<< " FUNCTION::" << FUNCTION \
<< "\n\t" \
<< COLOR << MSG << COLOR_RESET << "\n";

void customLogWriter(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // MESSAGEHANDLER_HPP
