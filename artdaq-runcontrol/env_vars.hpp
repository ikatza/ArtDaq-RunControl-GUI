#ifndef ENV_VARS_HPP
#define ENV_VARS_HPP

#include <QProcessEnvironment>

namespace env_vars {
const QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
const QString user = env.value("USER", "DEFAULT");
const QString daqInt_user_sourcefile = env.value("DAQINTERFACE_USER_SOURCEFILE", "EMPTY");
const QString daqInt_user_dir = env.value("DAQINTERFACE_USER_DIR", "DEFAULT");
const QString daqInt_wd = env.value("ARTDAQ_DAQINTERFACE_DIR", "DEFAULT");
// QString ConfigurationFHICL_default = env.value("DAQINTERFACE_FHICL_DIRECTORY", "DEFAULT");
const QString base_port = env.value("ARTDAQ_BASE_PORT", "DEFAULT");
const QString ports_per_partition = env.value("ARTDAQ_PORTS_PER_PARTITION", "DEFAULT");
const QString partition_number = env.value("DAQINTERFACE_PARTITION_NUMBER", "DEFAULT");
const QString rpc_port = QString::number(base_port.toInt() + partition_number.toInt() * ports_per_partition.toInt());
const QString DAQInterface_logdir = env.value("DAQINTERFACE_LOGDIR", "DEFAULT");
const QString DAQInterface_logfile = DAQInterface_logdir + "/DAQInterface_partition" + partition_number + ".log";
const QString boardreader_list_file = env.value("DAQINTERFACE_KNOWN_BOARDREADERS_LIST", "NOT FOUND");
const QString daqInt_settings = env.value("DAQINTERFACE_SETTINGS", "NOT FOUND");

}

#endif // ENV_VARS_HPP
