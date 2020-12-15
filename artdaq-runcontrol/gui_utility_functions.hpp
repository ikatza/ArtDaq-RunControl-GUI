#ifndef GUI_UTILITY_FUNCTIONS_HPP
#define GUI_UTILITY_FUNCTIONS_HPP

#include <QProcess>
#include <QTextCodec>

namespace gui_utility_functions {
  inline int getRunNumber(){
    QProcess srr;
    srr.start("show_recent_runs.sh", QStringList() << "1");
    srr.waitForFinished();
    QByteArray byte_status = srr.readAll();
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QStringList srr_sl = codec ->
      codecForMib(106)->toUnicode(byte_status).split(" ", QString::KeepEmptyParts);
    // int runNumber;
    if(srr_sl.count() > 1) {
      return srr_sl.at(1).toInt();
    }
    return -1; // non-possible run number
  }
}

#endif // GUI_UTILITY_FUNCTIONS_HPP
