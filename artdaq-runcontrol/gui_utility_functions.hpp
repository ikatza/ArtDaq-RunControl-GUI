#ifndef GUI_UTILITY_FUNCTIONS_HPP
#define GUI_UTILITY_FUNCTIONS_HPP

#include <QProcess>
#include <QTextCodec>

namespace gui_utility_functions {
  inline int getRunNumber(){
    QProcess gpp;
    gpp.start("show_recent_runs.sh", QStringList() << "1");
    gpp.waitForFinished();
    QByteArray byte_status = gpp.readAll();
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QStringList gpp_stringlist = codec->codecForMib(106)->toUnicode(byte_status).split(" ", QString::KeepEmptyParts);
    int runNumber;
    if(gpp_stringlist.count() > 1) {
      QString number_str = gpp_stringlist.at(1);
      runNumber = number_str.toInt();
    }
    else {
      runNumber = 0;
    }
    return runNumber;
  }
}

#endif // GUI_UTILITY_FUNCTIONS_HPP
