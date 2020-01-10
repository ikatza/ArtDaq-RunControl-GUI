#ifndef XMLRPC_GUI_COMM_H
#define XMLRPC_GUI_COMM_H

#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/client_simple.hpp>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <vector>
#include <QDirIterator>

#include "env_vars.h"

class xmlrpc_gui_comm {
public:
  xmlrpc_gui_comm();
  QString getDAQInterfaceStatus();
  void listDAQInterfaceComponents();
  void setDAQInterfaceComponents(QStringList components);
  void sendTransitionBOOT(QStringList selected_boot_file);
  void sendTransitionCONFIG(QStringList selected_config);
  void sendTransitionSTOP();
  void sendTransitionTERMINATE();
  void sendTransitionSTART();
  void listDAQInterfaceConfigs();
private:
  QString serverUrl = "http://localhost:5570/RPC2";
  // QString methodName = "listdaqcomps";
  xmlrpc_c::clientSimple guiClient;

};

#endif // XMLRPC_GUI_COMM_H
