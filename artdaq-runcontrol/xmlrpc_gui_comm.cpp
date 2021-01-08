#include "xmlrpc_gui_comm.hpp"
#include <QDebug>

xmlrpc_gui_comm::xmlrpc_gui_comm() :
  serverUrl("http://localhost:" + env_vars::rpc_port.toStdString() + "/RPC2")
{
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
    if(daqinterface_string.size() > 1){
      return daqinterface_string.at(1);
    }
    else{
      return daqinterface_string.at(0);
    }
  }
  else{
    try {
      xmlrpc_c::value result;
      xmlrpc_c::paramList params;
      std::string a = "state";
      std::string b = "daqint";
      params.add(xmlrpc_c::value_string(b));
      guiClient.call(serverUrl, a, params, &result);
      return QString::fromStdString(xmlrpc_c::value_string(result));
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
      std::string a = "listdaqcomps";
      guiClient.call(serverUrl, a, &result);
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
      std::string a = "listconfigs";
      guiClient.call(serverUrl, a, &result);
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
      std::string a = "setdaqcomps";

      //qDebug()<<boardreader_list_file;
      QFile file(env_vars::boardreader_list_file);
      file.open(QIODevice::ReadOnly);

      QTextStream in(&file);
      std::string cmd = "";
      std::string arbitrary_command = "";
      QStringList comp_line, arbitrary_command_stringlist;

      QString line;
      bool has_arbitrary_command = false;

      qDebug() << "Components: " << components << "size: " << components.size();
      std::map<std::string, xmlrpc_c::value> structData;
      std::vector<xmlrpc_c::value> array_list;

      // TODO: this while block is too convoluted!
      while(!in.atEnd()) {
        line = in.readLine();
        comp_line = line.split(" ");
        for(const auto& comp : components) {
          if(comp_line.at(0) != comp) continue;
          qDebug() << comp;
          int comp_line_size = comp_line.size();
          if(comp_line_size > 5){
            // TODO: valid for now
            // daqinterface wiki: Note that to set the Nth field for
            // a BoardReader, you'll also need to set fields 1
            // through N-1.
            comp_line_size = 5;
            arbitrary_command_stringlist = line.split("\"");
            arbitrary_command =  "\"" + arbitrary_command_stringlist.at(1).toStdString() + "\"";
            has_arbitrary_command = true;
          }
          for(int j=1; j < comp_line_size; j++){ // iterate through columns
            cmd = comp_line.at(j).toStdString();
            qDebug() << comp_line.at(j);
            array_list.push_back(xmlrpc_c::value_string(cmd));
          }
          if(comp_line_size == 2){ // default values for port and subsystem
            cmd = "-1";
            array_list.push_back(xmlrpc_c::value_string(cmd));
            cmd = "1";
            array_list.push_back(xmlrpc_c::value_string(cmd));
          }else if(comp_line_size == 3){
            cmd = "1";
            array_list.push_back(xmlrpc_c::value_string(cmd));
          }
          else if(has_arbitrary_command){
            cmd = arbitrary_command;
            qDebug() << QString::fromStdString(cmd);
            array_list.push_back(xmlrpc_c::value_string(cmd));
            has_arbitrary_command = false;
          }
          std::pair<std::string, xmlrpc_c::value> member(comp.toStdString(), xmlrpc_c::value_array(array_list));
          structData.insert(member);
          array_list.clear();
        }
      }
      params.add(xmlrpc_c::value_struct(structData));
      guiClient.call(serverUrl, a, params, &result);
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
      std::string a = "state_change";
      std::string b = "daqint";
      std::string c = "booting";
      std::string boot_file = selected_boot_file.at(0).toStdString();

      std::map<std::string, xmlrpc_c::value> structData;
      std::pair<std::string, xmlrpc_c::value> member("boot_filename", xmlrpc_c::value_string(boot_file));
      structData.insert(member);

      params.add(xmlrpc_c::value_string(b));
      params.add(xmlrpc_c::value_string(c));
      params.add(xmlrpc_c::value_struct(structData));
      guiClient.call(serverUrl, a, params, &result);

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
      std::string a = "state_change";
      std::string b = "daqint";
      std::string c = "configuring";
      std::string selected_config_ = selected_config.at(0).toStdString();

      std::map<std::string, xmlrpc_c::value> structData;
      std::vector<xmlrpc_c::value> array_list;
      array_list.push_back(xmlrpc_c::value_string(selected_config_));
      std::pair<std::string, xmlrpc_c::value> member("config", xmlrpc_c::value_array(array_list));
      structData.insert(member);

      params.add(xmlrpc_c::value_string(b));
      params.add(xmlrpc_c::value_string(c));
      params.add(xmlrpc_c::value_struct(structData));
      guiClient.call(serverUrl, a, params, &result);

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
      std::string a = "state_change";
      std::string b = "daqint";
      std::string c = "starting";
      int run_number = gui_utility_functions::getRunNumber();

      std::map<std::string, xmlrpc_c::value> structData;
      std::pair<std::string, xmlrpc_c::value> member("run_number", xmlrpc_c::value_int(run_number + 1));
      structData.insert(member);

      params.add(xmlrpc_c::value_string(b));
      params.add(xmlrpc_c::value_string(c));
      params.add(xmlrpc_c::value_struct(structData));
      guiClient.call(serverUrl, a, params, &result);

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
      std::string a = "state_change";
      std::string b = "daqint";
      std::string c = "stopping";

      std::map<std::string, xmlrpc_c::value> structData;
      std::pair<std::string, xmlrpc_c::value> member("ignored_variable", xmlrpc_c::value_int(999));
      structData.insert(member);

      params.add(xmlrpc_c::value_string(b));
      params.add(xmlrpc_c::value_string(c));
      params.add(xmlrpc_c::value_struct(structData));
      guiClient.call(serverUrl, a, params, &result);
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
      std::string a = "state_change";
      std::string b = "daqint";
      std::string c = "terminating";

      std::map<std::string, xmlrpc_c::value> structData;
      std::pair<std::string, xmlrpc_c::value> member("ignored_variable", xmlrpc_c::value_int(999));
      structData.insert(member);

      params.add(xmlrpc_c::value_string(b));
      params.add(xmlrpc_c::value_string(c));
      params.add(xmlrpc_c::value_struct(structData));
      guiClient.call(serverUrl, a, params, &result);
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
