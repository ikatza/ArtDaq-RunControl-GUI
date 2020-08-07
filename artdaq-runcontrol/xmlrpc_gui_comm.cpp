#include "xmlrpc_gui_comm.hpp"
#include <QDebug>

xmlrpc_gui_comm::xmlrpc_gui_comm()
{
  serverUrl = "http://localhost:" + env_vars::rpc_port + "/RPC2";
}

QString xmlrpc_gui_comm::getDAQInterfaceStatus()
{
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

void xmlrpc_gui_comm::listDAQInterfaceComponents()
{
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

void xmlrpc_gui_comm::listDAQInterfaceConfigs()
{
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

void xmlrpc_gui_comm::setDAQInterfaceComponents(QStringList components)
{

  try {
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

void xmlrpc_gui_comm::sendTransitionBOOT(QStringList selected_boot_file)
{

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

void xmlrpc_gui_comm::sendTransitionCONFIG(QStringList selected_config)
{

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

void xmlrpc_gui_comm::sendTransitionSTART()
{
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

void xmlrpc_gui_comm::sendTransitionSTOP()
{
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

void xmlrpc_gui_comm::sendTransitionTERMINATE()
{
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
