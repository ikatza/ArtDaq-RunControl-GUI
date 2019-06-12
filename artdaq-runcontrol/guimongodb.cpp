#include "guimongodb.h"

guiMongoDB::guiMongoDB()
{

}

void guiMongoDB::start(){

    QProcess *mongoDB = new QProcess();
    mongoDB->start("mongod");
    guiCollection= conn["guiDatabase"]["experiment"];

}

void guiMongoDB::uploadNewExperiment(QString name, QVector<QStringList> componentsList, QStringList configurations){

    using namespace bsoncxx;
    QString componentName, componentPort, componentHost, componentSubsystem;
    builder::stream::document builder{};

    auto in_array = builder << "name"
                            << name.toStdString()
                            << "components"
                            << builder::stream::open_array;
    for (QStringList component: componentsList) {
        componentName = component.at(0);
        componentHost = component.at(1);
        componentPort = component.at(2);
        componentSubsystem = component.at(3);
        in_array = in_array << builder::stream::open_document << "name" << componentName.toStdString()
                            << "host" << componentHost.toStdString()
                            << "port" << componentPort.toStdString()
                            << "subsystem" << componentSubsystem.toStdString()
                            << builder::stream::close_document;
    }
    in_array = in_array << builder::stream::close_array
                        << "configurations"
                        << builder::stream::open_array;

    for(QString str : configurations){
        in_array = in_array << str.toStdString();
    }

    auto out = in_array << builder::stream::close_array
                        << "profiles"
                        << builder::stream::open_array
                        << builder::stream::close_array;

    document::value doc_value = out << builder::stream::finalize;

    bsoncxx::document::view view_ = doc_value.view();
    guiCollection.insert_one(view_);

}

void guiMongoDB::retreiveExperiment(QString name, QVector<QStringList> *componentList, QStringList *configurationList){

    QString str;
    QStringList componentList_;
    mongocxx::cursor query_result = guiCollection.find(guiDocument << "name" << name.toStdString() << bsoncxx::builder::stream::finalize);
    for(auto doc:query_result){
        auto components_view = doc["components"];
        auto configurations_view = doc["configurations"];
        auto components_ = components_view.get_array().value;
        for(auto comp:components_){
            auto comp_ = comp.get_document().value;
            auto component_name_view = comp_["name"];
            QString component_name = str.fromStdString(component_name_view.get_utf8().value.to_string());
            auto component_host_view = comp_["host"];
            QString component_host = str.fromStdString(component_host_view.get_utf8().value.to_string());
            auto component_port_view = comp_["port"];
            QString component_port = str.fromStdString(component_port_view.get_utf8().value.to_string());
            auto component_subsystem_view = comp_["subsystem"];
            QString component_subsystem = str.fromStdString(component_subsystem_view.get_utf8().value.to_string());
            componentList_.append(component_name);
            componentList_.append(component_host);
            componentList_.append(component_port);
            componentList_.append(component_subsystem);
            componentList->append(componentList_);
            componentList_.clear();
        }
        auto configurations_ = configurations_view.get_array().value;
        for(auto ele:configurations_){
            configurationList->append(str.fromStdString(ele.get_utf8().value.to_string()));
        }
    }

}

void guiMongoDB::retreiveExperiment(QString name, QVector<QStringList> *componentList, QStringList *configurationList, QStringList *bootfileList){

    try{
    QString str;
    QStringList componentList_;
    mongocxx::cursor query_result = guiCollection.find(guiDocument << "name" << name.toStdString() << bsoncxx::builder::stream::finalize);
    for(auto doc:query_result){
        auto components_view = doc["components"];
        auto configurations_view = doc["configurations"];
        auto components_ = components_view.get_array().value;
        for(auto comp:components_){
            auto comp_ = comp.get_document().value;
            auto component_name_view = comp_["name"];
            QString component_name = str.fromStdString(component_name_view.get_utf8().value.to_string());
            auto component_host_view = comp_["host"];
            QString component_host = str.fromStdString(component_host_view.get_utf8().value.to_string());
            auto component_port_view = comp_["port"];
            QString component_port = str.fromStdString(component_port_view.get_utf8().value.to_string());
            auto component_subsystem_view = comp_["subsystem"];
            QString component_subsystem = str.fromStdString(component_subsystem_view.get_utf8().value.to_string());
            componentList_.append(component_name);
            componentList_.append(component_host);
            componentList_.append(component_port);
            componentList_.append(component_subsystem);
            componentList->append(componentList_);
            componentList_.clear();
        }
        auto configurations_ = configurations_view.get_array().value;
        for(auto ele:configurations_){
            configurationList->append(str.fromStdString(ele.get_utf8().value.to_string()));
        }
        QStringList bootfileList_ = this->retreiveProfileBootFiles(name,"default");
        for(QString bootfile : bootfileList_){
            bootfileList->append(bootfile);
        }
    }
    }
    catch(bsoncxx::exception e){
        qDebug()<<e.what();
    }

}

void guiMongoDB::removeExperiment(QString experimentName){

    guiCollection.delete_one(guiDocument << "name" << experimentName.toStdString() << bsoncxx::builder::stream::finalize);

}

void guiMongoDB::findExperiments(QStringList *experiments){

    mongocxx::cursor query_result = guiCollection.find(guiDocument << bsoncxx::builder::stream::finalize);
    for(auto doc:query_result){
        auto name_view = doc["name"];
        QString experimentName = QString::fromStdString(name_view.get_utf8().value.to_string());
        experiments->append(experimentName);
    }
}

void guiMongoDB::addProfileToExperiment(QString experimentName, QString profileName, QString selectedConfiguration, QStringList selectedComponents, QString bootFile){

    using namespace bsoncxx;
    builder::stream::document builder{};
    auto in_array = builder << "name"
                            << profileName.toStdString()
                            << "components"
                            << builder::stream::open_array;
    for(QString component : selectedComponents){
        in_array = in_array << component.toStdString();
    }
    auto out = in_array << builder::stream::close_array
                        << "configuration"
                        << selectedConfiguration.toStdString()
                        << "boot file"
                        << bootFile.toStdString()
                        << builder::stream::finalize;

    this->guiCollection.update_one(guiDocument << "name" << experimentName.toStdString() << bsoncxx::builder::stream::finalize,
                          guiDocument << "$push" << bsoncxx::builder::stream::open_document
                          << "profiles"
                          << out
                          << bsoncxx::builder::stream::close_document
                          << bsoncxx::builder::stream::finalize);
}

void guiMongoDB::addProfileToExperiment(QString experimentName, QString profileName, QStringList selectedConfiguration, QVector<QStringList> selectedComponents, QStringList bootFiles){

    using namespace bsoncxx;
    builder::stream::document builder{};
    auto in_array = builder << "name"
                            << profileName.toStdString()
                            << "components"
                            << builder::stream::open_array;
    for(QStringList component : selectedComponents){
        in_array = in_array << component.at(0).toStdString();
    }
    in_array = in_array << builder::stream::close_array
                        << "configuration"
                        << builder::stream::open_array;
    for(QString configuration : selectedConfiguration){
        in_array = in_array << configuration.toStdString();
    }
    in_array = in_array << builder::stream::close_array
                        << "boot file"
                        << builder::stream::open_array;
    for(QString bootFile : bootFiles){
        in_array = in_array << bootFile.toStdString();
    }

    auto out = in_array << builder::stream::close_array
                        << builder::stream::finalize;

    this->guiCollection.update_one(guiDocument << "name" << experimentName.toStdString() << bsoncxx::builder::stream::finalize,
                          guiDocument << "$push" << bsoncxx::builder::stream::open_document
                          << "profiles"
                          << out
                          << bsoncxx::builder::stream::close_document
                          << bsoncxx::builder::stream::finalize);
}

QStringList guiMongoDB::retrieveExperimentProfiles(QString experimentName){

    QStringList profileList;
    mongocxx::cursor query_result = guiCollection.find(guiDocument << "name" << experimentName.toStdString() << bsoncxx::builder::stream::finalize);
    for(auto doc:query_result){
        auto profiles_view = doc["profiles"];
        auto profiles = profiles_view.get_array().value;
        for(auto profile:profiles){
            auto profile_name_ = profile["name"];
            QString profile_name = QString::fromStdString(profile_name_.get_utf8().value.to_string());
            profileList.append(profile_name);
        }
    }
    return profileList;
}

QStringList guiMongoDB::retreiveProfileComponents(QString experimentName, QString profileName){

    QStringList profileComponentsList;
    mongocxx::cursor query_result = guiCollection.find(guiDocument << "name" << experimentName.toStdString() << bsoncxx::builder::stream::finalize);
    for(auto doc:query_result){
        auto profiles_view = doc["profiles"];
        auto profiles = profiles_view.get_array().value;
        for(auto profile:profiles){
            auto profile_name_ = profile["name"];
            QString doc_profile_name = QString::fromStdString(profile_name_.get_utf8().value.to_string());
            if(doc_profile_name == profileName){
                auto profile_components = profile["components"].get_array().value;
                for(auto components : profile_components){
                    profileComponentsList.append(QString::fromStdString(components.get_utf8().value.to_string()));
                }
            }
        }
    }
    return profileComponentsList;
}

QStringList guiMongoDB::retreiveProfileConfiguration(QString experimentName, QString profileName){

    QStringList profileConfigurationList;
    mongocxx::cursor query_result = guiCollection.find(guiDocument << "name" << experimentName.toStdString() << bsoncxx::builder::stream::finalize);
    for(auto doc:query_result){
        auto profiles_view = doc["profiles"];
        auto profiles = profiles_view.get_array().value;
        for(auto profile:profiles){
            auto profile_name_ = profile["name"];
            QString doc_profile_name = QString::fromStdString(profile_name_.get_utf8().value.to_string());
            if(doc_profile_name == "default" && profileName == "default"){
                auto profile_configuration = profile["configuration"].get_array().value;
                for(auto configuration : profile_configuration){
                    profileConfigurationList.append(QString::fromStdString(configuration.get_utf8().value.to_string()));
                }
            }else{
                if(doc_profile_name == profileName){
                    QString profile_configuration = QString::fromStdString(profile["configuration"].get_utf8().value.to_string());
                    profileConfigurationList.append(profile_configuration);
                }
            }
        }
    }
    return profileConfigurationList;
}

QStringList guiMongoDB::retreiveProfileBootFiles(QString experimentName, QString profileName){

    QStringList profileBootFilesList;
    mongocxx::cursor query_result = guiCollection.find(guiDocument << "name" << experimentName.toStdString() << bsoncxx::builder::stream::finalize);
    for(auto doc:query_result){
        auto profiles_view = doc["profiles"];
        auto profiles = profiles_view.get_array().value;
        for(auto profile:profiles){
            auto profile_name_ = profile["name"];
            QString doc_profile_name = QString::fromStdString(profile_name_.get_utf8().value.to_string());
            if(doc_profile_name == "default" && profileName == "default"){
                auto profile_bootfile = profile["boot file"].get_array().value;
                for(auto bootfile : profile_bootfile){
                    profileBootFilesList.append(QString::fromStdString(bootfile.get_utf8().value.to_string()));
                }
            }else{
                if(doc_profile_name == profileName){
                    QString profile_configuration = QString::fromStdString(profile["boot file"].get_utf8().value.to_string());
                    profileBootFilesList.append(profile_configuration);
                }
            }
        }
    }
    return profileBootFilesList;
}

void guiMongoDB::removeProfileFromExperiment(QString experimentName, QString profileToBeRemoved){

    this->guiCollection.update_one(guiDocument << "name" << experimentName.toStdString() << bsoncxx::builder::stream::finalize,
                          guiDocument << "$pop" << bsoncxx::builder::stream::open_document
                          << "profiles"
                          << profileToBeRemoved.toStdString()
                          << bsoncxx::builder::stream::close_document
                          << bsoncxx::builder::stream::finalize);

}

void guiMongoDB::saveExperimentProfileView(QString experimentName){

    mongocxx::cursor query_result = guiCollection.find(guiDocument << "name" << experimentName.toStdString() << bsoncxx::builder::stream::finalize);
    for(auto doc:query_result){
        this->profiles_element_temp = doc["profiles"];
    }

}

void guiMongoDB::updateExperimentProfiles(QString experimentName){

    qDebug()<<experimentName;
    auto profiles = this->profiles_element_temp.get_array().value;
    QStringList profileComponents;
    QStringList profileConfigurations;
    QStringList profileBootfiles;
    QStringList profileComponents_;
    QStringList profileConfigurations_;
    QStringList profileBootfiles_;
    for(auto profile : profiles){
        QString profileName = QString::fromStdString(profile["name"].get_utf8().value.to_string());
        qDebug()<<profileName;
        if(profileName == "default"){
            auto profile_components = profile["components"].get_array().value;
            for(auto component : profile_components){
                profileComponents.append(QString::fromStdString(component.get_utf8().value.to_string()));
            }
            auto profile_configurations = profile["configuration"].get_array().value;
            for(auto configuration : profile_configurations){
                profileConfigurations.append(QString::fromStdString(configuration.get_utf8().value.to_string()));
            }
            auto profile_bootfiles = profile["boot file"].get_array().value;
            for(auto bootfile : profile_bootfiles){
                profileBootfiles.append(QString::fromStdString(bootfile.get_utf8().value.to_string()));
            }
        }
    }

    int compareFlag = 0;

    for(auto profile : profiles){
        profileComponents_.clear();
        profileConfigurations_.clear();
        profileBootfiles_.clear();
        QString profileName = QString::fromStdString(profile["name"].get_utf8().value.to_string());
        if(profileName != "default"){
            auto profile_components = profile["components"].get_array().value;
            for(auto component : profile_components){
                profileComponents_.append(QString::fromStdString(component.get_utf8().value.to_string()));
            }
            auto profile_configurations = profile["configuration"].get_utf8().value;
            profileConfigurations_.append(QString::fromStdString(profile_configurations.to_string()));
            auto profile_bootfiles = profile["boot file"].get_utf8().value;
            profileBootfiles_.append(QString::fromStdString(profile_bootfiles.to_string()));
        }
        for(QString component1 : profileComponents_){
            compareFlag = 0;
            for(QString component2 : profileComponents){
                if(component1 == component2){
                    compareFlag++;
                }
            }
            qDebug()<<compareFlag;
            if(compareFlag != 0){
                profileComponents_.removeAt(profileComponents_.indexOf(component1));
            }
        }
        for(QString configuration1 : profileConfigurations_){
            compareFlag = 0;
            for(QString configuration2 : profileConfigurations){
                if(configuration1 == configuration2){
                    compareFlag++;
                }
            }
            if(compareFlag != 0){
                profileConfigurations_.removeAt(profileConfigurations_.indexOf(configuration1));
            }
        }
        for(QString bootfile1 : profileBootfiles_){
            compareFlag = 0;
            for(QString bootfile2 : profileBootfiles){
                if(bootfile1 == bootfile2){
                    compareFlag++;
                }
            }
            if(compareFlag != 0){
                profileBootfiles_.removeAt(profileBootfiles_.indexOf(bootfile1));
            }
        }
        QVector<QStringList> profileComponentsVector;
        profileComponentsVector.append(profileComponents_);
        qDebug()<<profileComponentsVector<<profileConfigurations_<<profileBootfiles_;
        //this->addProfileToExperiment(experimentName, profileName, profileConfigurations_, profileComponentsVector, profileBootfiles_);
    }

}



















