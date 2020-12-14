#include "xmlrpc_gui_comm.hpp"
#include <QDebug>

xmlrpc_gui_comm::xmlrpc_gui_comm()
{
  serverUrl = "http://localhost:" + env_vars::rpc_port + "/RPC2";
}

QString xmlrpc_gui_comm::getDAQInterfaceStatus()
{
  if(this->shellScriptsEnabled){
    QProcess di_status;
    di_status.setWorkingDirectory(env_vars::daqInt_wd);
    di_status.start("status.sh", QStringList() << "");
    if(!di_status.waitForFinished()){
      qCritical() << "DAQInterface status timed out.\n"
                  << "Restart DAQInterface.";
      // TODO: get out or do some other actions?
    }
    QByteArray byte_status = di_status.readAll();
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QStringList daqinterface_string = codec->codecForMib(106)->toUnicode(byte_status).split("'", QString::KeepEmptyParts);
    QString daqinterface_status;
    if(daqinterface_string.size() > 1){
      daqinterface_status = daqinterface_string.at(1);
      return daqinterface_status;
    }
    else{
      daqinterface_status = daqinterface_string.at(0);
      return daqinterface_status;
    }
  }
  else{
    try {
      xmlrpc_c::value result;
      xmlrpc_c::paramList params;
      QString a = "state";
      QString b = "daqint";
      params.add(xmlrpc_c::value_string(b.toStdString()));
      // qDebug()<< "serverUrl: " << serverUrl ;
      guiClient.call(serverUrl.toStdString(), a.toStdString(), params, &result);
      QString result_ = QString::fromStdString(xmlrpc_c::value_string(result));
      return result_;
      //qDebug()<< "from xmlrpc_c: "<<result_;
    }
    catch(std::exception const & e) {
      qCritical() << "Call to DAQInterface failed because: "
                  << e.what();
      return QString::fromUtf8(e.what());
    }
  }
}

void xmlrpc_gui_comm::listDAQInterfaceComponents()
{
  if(this->shellScriptsEnabled){
    QProcess di_comps;
    di_comps.setWorkingDirectory(env_vars::daqInt_wd);
    di_comps.start("listdaqcomps.sh", QStringList() << "");
    if(!di_comps.waitForFinished()){
      qCritical() << "DAQInterface listdaqcomps timed out.\n"
                  << "Restart DAQInterface.";
      // TODO: get out or do some other actions?
    }
  }
  else{
    try {
      xmlrpc_c::value result;
      QString a = "listdaqcomps";
      guiClient.call(serverUrl.toStdString(), a.toStdString(), &result);
    }
    catch(std::exception const & e) {
      qCritical() << "Call to DAQInterface failed because: "
                  << e.what();
    }
  }
}

void xmlrpc_gui_comm::listDAQInterfaceConfigs()
{
  if(this->shellScriptsEnabled){
    QProcess di_configs;
    di_configs.setWorkingDirectory(env_vars::daqInt_wd);
    di_configs.start("listconfigs.sh", QStringList() << "");
    if(!di_configs.waitForFinished()){
      qCritical() << "DAQInterface listconfigs timed out.\n"
                  << "Restart DAQInterface.";
      // TODO: get out or do some other actions?
    }
  }
  else{
    try {
      xmlrpc_c::value result;
      QString a = "listconfigs";
      guiClient.call(serverUrl.toStdString(), a.toStdString(), &result);
    }
    catch(std::exception const & e) {
      qCritical() << "Call to DAQInterface failed because: "
                  << e.what();
    }
  }
}

void xmlrpc_gui_comm::setDAQInterfaceComponents(const QStringList& components)
{
  if(this->shellScriptsEnabled){
    QProcess di_setcomps;
    di_setcomps.setWorkingDirectory(env_vars::daqInt_wd);
    di_setcomps.start("setdaqcomps.sh", components);
    if(!di_setcomps.waitForFinished()){
      qCritical() << "DAQInterface setdaqcomps timed out.\n"
                  << "components: " << components << "\n"
                  << "Restart DAQInterface.";
      // TODO: get out or do some other actions?
    }
  }
  else{
    try{
      xmlrpc_c::value result;
      xmlrpc_c::paramList params;
      QString a = "setdaqcomps";

      //qDebug()<<boardreader_list_file;
      QFile file(env_vars::boardreader_list_file);
      file.open(QIODevice::ReadOnly);

      QTextStream in(&file);
      QString cmd = "";
      QString comp = "";
      QString arbitrary_command = "";
      QStringList comp_line, arbitrary_command_stringlist;

      QString line;
      bool has_arbitrary_command = false;

      qDebug() << "Components: " << components << "size: " << components.size();
      std::map<std::string, xmlrpc_c::value> structData;
      std::vector<xmlrpc_c::value> array_list;

      while(!in.atEnd()) {
        line = in.readLine();
        comp_line = line.split(" ");
        for(int i = 0; i < components.size(); i++) {
          comp = components.at(i);
          if(comp_line.at(0) == comp) {
            qDebug() << comp;
            int comp_line_size = comp_line.size();
            if(comp_line_size > 5){
              comp_line_size = 5; // valid for now taking note of daqinterface wiki: Note that to set the Nth field for a BoardReader, you'll also need to set fields 1 through N-1.
              arbitrary_command_stringlist = line.split("\"");
              arbitrary_command =  "\"" + arbitrary_command_stringlist.at(1) + "\"";
              has_arbitrary_command = true;
            }
            for(int j=1; j < comp_line_size; j++){ // iterate through columns
              cmd = comp_line.at(j);
              qDebug() << comp_line.at(j);
              array_list.push_back(xmlrpc_c::value_string(cmd.toStdString()));
            }
            if(comp_line_size == 2){ // default values for port and subsystem
              cmd = "-1";
              array_list.push_back(xmlrpc_c::value_string(cmd.toStdString()));
              cmd = "1";
              array_list.push_back(xmlrpc_c::value_string(cmd.toStdString()));
            }else if(comp_line_size == 3){
              cmd = "1";
              array_list.push_back(xmlrpc_c::value_string(cmd.toStdString()));
            }
            else if(has_arbitrary_command){
              cmd = arbitrary_command;
              qDebug() << cmd;
              array_list.push_back(xmlrpc_c::value_string(cmd.toStdString()));
              has_arbitrary_command = false;
            }
            std::pair<std::string, xmlrpc_c::value> member(comp.toStdString(), xmlrpc_c::value_array(array_list));
            structData.insert(member);
            array_list.clear();
          }
        }
      }
      params.add(xmlrpc_c::value_struct(structData));
      guiClient.call(serverUrl.toStdString(), a.toStdString(), params, &result);
    }
    catch(std::exception const & e) {
      qCritical() << "Call to DAQInterface failed because: "
                  << e.what();
    }
  }
}

void xmlrpc_gui_comm::sendTransitionBOOT(const QStringList& selected_boot_file)
{
  if(this->shellScriptsEnabled){
    QProcess di_boot;
    di_boot.setWorkingDirectory(env_vars::daqInt_wd);
    di_boot.start("send_transition.sh", QStringList() << "boot" << selected_boot_file.at(0));
    if(!di_boot.waitForFinished()){
      qCritical() << "DAQInterface send_transition BOOT timed out.\n"
                  << "selected boot: " << selected_boot_file.at(0) << "\n"
                  << "Restart DAQInterface.";
      // TODO: get out or do some other actions?
    }
  }
  else{
    try {
      xmlrpc_c::value result;
      xmlrpc_c::paramList params;
      QString a = "state_change";
      QString b = "daqint";
      QString c = "booting";
      QString boot_file = selected_boot_file.at(0);

      std::map<std::string, xmlrpc_c::value> structData;
      std::pair<std::string, xmlrpc_c::value> member("boot_filename", xmlrpc_c::value_string(boot_file.toStdString()));
      structData.insert(member);

      params.add(xmlrpc_c::value_string(b.toStdString()));
      params.add(xmlrpc_c::value_string(c.toStdString()));
      params.add(xmlrpc_c::value_struct(structData));
      guiClient.call(serverUrl.toStdString(), a.toStdString(), params, &result);

    }
    catch(std::exception const & e) {
      qCritical() << "Call to DAQInterface failed because: "
                  << e.what();
    }
  }
}

void xmlrpc_gui_comm::sendTransitionCONFIG(const QStringList& selected_config)
{
  if(this->shellScriptsEnabled){
    QProcess di_config;
    di_config.setWorkingDirectory(env_vars::daqInt_wd);
    di_config.start("send_transition.sh", QStringList() << "config" << selected_config.at(0));
    if(!di_config.waitForFinished()){
      qCritical() << "DAQInterface send_transition CONFIG timed out.\n"
                  << "selected config: " << selected_config.at(0) << "\n"
                  << "Restart DAQInterface.";
      // TODO: get out or do some other actions?
    }
  }
  else{
    try {
      xmlrpc_c::value result;
      xmlrpc_c::paramList params;
      QString a = "state_change";
      QString b = "daqint";
      QString c = "configuring";
      QString selected_config_ = selected_config.at(0);

      std::map<std::string, xmlrpc_c::value> structData;
      std::vector<xmlrpc_c::value> array_list;
      array_list.push_back(xmlrpc_c::value_string(selected_config_.toStdString()));
      std::pair<std::string, xmlrpc_c::value> member("config", xmlrpc_c::value_array(array_list));
      structData.insert(member);

      params.add(xmlrpc_c::value_string(b.toStdString()));
      params.add(xmlrpc_c::value_string(c.toStdString()));
      params.add(xmlrpc_c::value_struct(structData));
      guiClient.call(serverUrl.toStdString(), a.toStdString(), params, &result);

    }
    catch(std::exception const & e) {
      qCritical() << "Call to DAQInterface failed because: "
                  << e.what();
    }
  }
}

void xmlrpc_gui_comm::sendTransitionSTART()
{
  if(this->shellScriptsEnabled){
    QProcess di_start;
    di_start.setWorkingDirectory(env_vars::daqInt_wd);
    di_start.start("send_transition.sh", QStringList() << "start");
    if(!di_start.waitForFinished()){
      qCritical() << "DAQInterface send_transition START timed out.\n"
                  << "Restart DAQInterface.";
      // TODO: get out or do some other actions?
    }
  }
  else{
    try {
      xmlrpc_c::value result;
      xmlrpc_c::paramList params;
      QString a = "state_change";
      QString b = "daqint";
      QString c = "starting";
      QStringList run_number_str_list;
      QString run_number_str;
      int run_number = 0;

      run_number = gui_utility_functions::getRunNumber();

      std::map<std::string, xmlrpc_c::value> structData;
      std::pair<std::string, xmlrpc_c::value> member("run_number", xmlrpc_c::value_int(run_number + 1));
      structData.insert(member);

      params.add(xmlrpc_c::value_string(b.toStdString()));
      params.add(xmlrpc_c::value_string(c.toStdString()));
      params.add(xmlrpc_c::value_struct(structData));
      guiClient.call(serverUrl.toStdString(), a.toStdString(), params, &result);

    }
    catch(std::exception const & e) {
      qCritical() << "Call to DAQInterface failed because: "
                  << e.what();
    }
  }
}

void xmlrpc_gui_comm::sendTransitionSTOP()
{
  if(this->shellScriptsEnabled){
    QProcess di_stop;
    di_stop.setWorkingDirectory(env_vars::daqInt_wd);
    di_stop.start("send_transition.sh", QStringList() << "stop");
    if(!di_stop.waitForFinished()){
      qCritical() << "DAQInterface send_transition STOP timed out.\n"
                  << "Restart DAQInterface.";
      // TODO: get out or do some other actions?
    }
  }
  else{
    try {
      xmlrpc_c::value result;
      xmlrpc_c::paramList params;
      QString a = "state_change";
      QString b = "daqint";
      QString c = "stopping";

      std::map<std::string, xmlrpc_c::value> structData;
      std::pair<std::string, xmlrpc_c::value> member("ignored_variable", xmlrpc_c::value_int(999));
      structData.insert(member);

      params.add(xmlrpc_c::value_string(b.toStdString()));
      params.add(xmlrpc_c::value_string(c.toStdString()));
      params.add(xmlrpc_c::value_struct(structData));
      guiClient.call(serverUrl.toStdString(), a.toStdString(), params, &result);
    }
    catch(std::exception const & e) {
      qCritical() << "Call to DAQInterface failed because: "
                  << e.what();
    }
  }
}

void xmlrpc_gui_comm::sendTransitionTERMINATE()
{
  if(this->shellScriptsEnabled){
    QProcess di_terminate;
    di_terminate.setWorkingDirectory(env_vars::daqInt_wd);
    di_terminate.start("send_transition.sh", QStringList() << "terminate");
    if(!di_terminate.waitForFinished()){
      qCritical() << "DAQInterface send_transition TERMINATE timed out.\n"
                  << "Restart DAQInterface.";
      // TODO: get out or do some other actions?
    }
  }
  else{
    try {
      xmlrpc_c::value result;
      xmlrpc_c::paramList params;
      QString a = "state_change";
      QString b = "daqint";
      QString c = "terminating";

      std::map<std::string, xmlrpc_c::value> structData;
      std::pair<std::string, xmlrpc_c::value> member("ignored_variable", xmlrpc_c::value_int(999));
      structData.insert(member);

      params.add(xmlrpc_c::value_string(b.toStdString()));
      params.add(xmlrpc_c::value_string(c.toStdString()));
      params.add(xmlrpc_c::value_struct(structData));
      guiClient.call(serverUrl.toStdString(), a.toStdString(), params, &result);
    }
    catch(std::exception const & e) {
      qCritical() << "Call to DAQInterface failed because: "
                  << e.what();
    }
  }
}

void xmlrpc_gui_comm::setShellScripts(bool value)
{
  this->shellScriptsEnabled = value;
}
