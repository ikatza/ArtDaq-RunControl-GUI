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

    try{
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
    }catch(bsoncxx::exception e){
        qDebug()<<"Exception thrown in method : \nguiMongoDB::uploadNewExperiment(QString name, QVector<QStringList> componentsList, QStringList configurations)"
               <<"\nException info: " << e.what();
    }

}

void guiMongoDB::retreiveExperiment(QString name, QVector<QStringList> *componentList, QStringList *configurationList){

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
    }
    }catch(bsoncxx::exception e){
    qDebug()<<"Exception thrown in method : \nguiMongoDB::retreiveExperiment(QString name, QVector<QStringList> *componentList, QStringList *configurationList)"
           <<"\nException info: " << e.what();
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
    }catch(bsoncxx::exception e){
    qDebug()<<"Exception thrown in method : \nguiMongoDB::retreiveExperiment(QString name, QVector<QStringList> *componentList, QStringList *configurationList, QStringList *bootfileList)"
           <<"\nException info: " << e.what();
    }

}

void guiMongoDB::removeExperiment(QString experimentName){
    try{
    guiCollection.delete_one(guiDocument << "name" << experimentName.toStdString() << bsoncxx::builder::stream::finalize);
    }catch(bsoncxx::exception e){
    qDebug()<<"Exception thrown in method : \nguiMongoDB::removeExperiment(QString experimentName)"
           <<"\nException info: " << e.what();
    }

}

void guiMongoDB::findExperiments(QStringList *experiments){

    try{
    mongocxx::cursor query_result = guiCollection.find(guiDocument << bsoncxx::builder::stream::finalize);
    for(auto doc:query_result){
        auto name_view = doc["name"];
        QString experimentName = QString::fromStdString(name_view.get_utf8().value.to_string());
        experiments->append(experimentName);
    }
    }catch(bsoncxx::exception e){
    qDebug()<<"Exception thrown in method : \nguiMongoDB::findExperiments(QStringList *experiments)"
            <<"\nException info: " << e.what();
    }
}

void guiMongoDB::addProfileToExperiment(QString experimentName, QString profileName, QString selectedConfiguration, QStringList selectedComponents, QString bootFile){

    try{
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
    }catch(bsoncxx::exception e){
    qDebug()<<"Exception thrown in method : \n"
            <<"guiMongoDB::addProfileToExperiment(QString experimentName, QString profileName, QString selectedConfiguration, QStringList selectedComponents, QString bootFile)"
            <<"\nException info: " << e.what();
    }

}

void guiMongoDB::addProfileToExperiment(QString experimentName, QString profileName, QStringList selectedConfiguration, QVector<QStringList> selectedComponents, QStringList bootFiles){

    try{
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
    }catch(bsoncxx::exception e){
    qDebug()<<"Exception thrown in method : \n"
            <<"guiMongoDB::addProfileToExperiment(QString experimentName, QString profileName, QStringList selectedConfiguration, QVector<QStringList> selectedComponents, QStringList bootFiles)"
            <<"\nException info: " << e.what();
    }
}

QStringList guiMongoDB::retrieveExperimentProfiles(QString experimentName){

    try{
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
    }catch(bsoncxx::exception e){
    qDebug()<<"Exception thrown in method : \n"
            <<"guiMongoDB::retrieveExperimentProfiles(QString experimentName)"
            <<"\nException info: " << e.what();
            QStringList StringList;
            return StringList;
    }
}

QStringList guiMongoDB::retreiveProfileComponents(QString experimentName, QString profileName){

    try{
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
    }catch(bsoncxx::exception e){
    qDebug()<<"Exception thrown in method : \n"
            <<"guiMongoDB::retreiveProfileComponents(QString experimentName, QString profileName)"
            <<"\nException info: " << e.what();
            QStringList StringList;
            return StringList;
    }
}

QStringList guiMongoDB::retreiveProfileConfiguration(QString experimentName, QString profileName){

    try{
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
    }catch(bsoncxx::exception e){
    qDebug()<<"Exception thrown in method : \n"
            <<"guiMongoDB::retreiveProfileConfiguration(QString experimentName, QString profileName)"
            <<"\nException info: " << e.what();
            QStringList StringList;
            return StringList;
    }
}

QStringList guiMongoDB::retreiveProfileBootFiles(QString experimentName, QString profileName){

    try{
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
    }catch(bsoncxx::exception e){
    qDebug()<<"Exception thrown in method : \n"
            <<"guiMongoDB::retreiveProfileBootFiles(QString experimentName, QString profileName)"
            <<"\nException info: " << e.what();
            QStringList StringList;
            return StringList;
    }
}

void guiMongoDB::removeProfileFromExperiment(QString experimentName, QString profileToBeRemoved){

    try{
    this->guiCollection.update_one(guiDocument << "name" << experimentName.toStdString() << bsoncxx::builder::stream::finalize,
                          guiDocument << "$pop" << bsoncxx::builder::stream::open_document
                          << "profiles"
                          << profileToBeRemoved.toStdString()
                          << bsoncxx::builder::stream::close_document
                          << bsoncxx::builder::stream::finalize);
    }catch(bsoncxx::exception e){
    qDebug()<<"Exception thrown in method : \n"
            <<"guiMongoDB::removeProfileFromExperiment(QString experimentName, QString profileToBeRemoved)"
            <<"\nException info: " << e.what();
    }

}

void guiMongoDB::saveExperimentProfiles(QString experimentName){

    try{
    mongocxx::cursor query_result = guiCollection.find(guiDocument << "name" << experimentName.toStdString() << bsoncxx::builder::stream::finalize);
    for(auto doc:query_result){

        this->profiles_element_temp = QString::fromStdString(bsoncxx::to_json(doc));
    }
    }catch(bsoncxx::exception e){
    qDebug()<<"Exception thrown in method : \n"
            <<"guiMongoDB::saveExperimentProfileView(QString experimentName)"
            <<"\nException info: " << e.what();
    }

}

bsoncxx::document::element guiMongoDB::getExperimentProfileView(QString experimentName){

    try{
    mongocxx::cursor query_result = guiCollection.find(guiDocument << "name" << experimentName.toStdString() << bsoncxx::builder::stream::finalize);
    bsoncxx::document::element profiles_element;
    for(auto doc:query_result){
        profiles_element = doc["profiles"];
    }
    qDebug()<<"returned element";
    return profiles_element;
    }catch(bsoncxx::exception e){
    qDebug()<<"Exception thrown in method : \n"
            <<"guiMongoDB::getExperimentProfileView(QString experimentName)"
            <<"\nException info: " << e.what();
            bsoncxx::document::element profiles_element;
            return profiles_element;
    }
}

void guiMongoDB::updateExperiment(QString experimentName, QString newExperimentName, QVector<QStringList> componentsList, QStringList configurations, QStringList bootfilesList){

    try{
        this->guiCollection.update_one(guiDocument << "name" << experimentName.toStdString() << bsoncxx::builder::stream::finalize,
                              guiDocument << "$set" << bsoncxx::builder::stream::open_document
                              << "name"
                              << newExperimentName.toStdString()
                              << bsoncxx::builder::stream::close_document
                              << bsoncxx::builder::stream::finalize);
        QString componentName, componentPort, componentHost, componentSubsystem;
        bsoncxx::builder::stream::document builder{};

        auto in_array = builder << "components"
                                << bsoncxx::builder::stream::open_array;
        QStringList newComponentsList;
        QStringList newConfigurationsList;
        for (QStringList component: componentsList) {
            componentName = component.at(0);
            newComponentsList.append(componentName);
            componentHost = component.at(1);
            componentPort = component.at(2);
            componentSubsystem = component.at(3);
            in_array = in_array << bsoncxx::builder::stream::open_document << "name" << componentName.toStdString()
                                << "host" << componentHost.toStdString()
                                << "port" << componentPort.toStdString()
                                << "subsystem" << componentSubsystem.toStdString()
                                << bsoncxx::builder::stream::close_document;
        }
        auto out = in_array << bsoncxx::builder::stream::close_array
                            << bsoncxx::builder::stream::finalize;
        this->guiCollection.update_one(guiDocument << "name" << newExperimentName.toStdString() << bsoncxx::builder::stream::finalize,
                              guiDocument << "$set"
                              << out
                              << bsoncxx::builder::stream::finalize);
        in_array = builder << "configurations"
                           << bsoncxx::builder::stream::open_array;
        for(QString configuration : configurations){
            in_array = in_array << configuration.toStdString();
            newConfigurationsList.append(configuration);
        }
        out = in_array << bsoncxx::builder::stream::close_array
                       << bsoncxx::builder::stream::finalize;
        this->guiCollection.update_one(guiDocument << "name" << newExperimentName.toStdString() << bsoncxx::builder::stream::finalize,
                              guiDocument << "$set"
                              << out
                              << bsoncxx::builder::stream::finalize);
        QStringList oldDefaultComponentsList;
        QStringList oldDefaultConfigurationsList;
        QStringList oldDefaultBootfilesList;
        mongocxx::cursor query_result = guiCollection.find(guiDocument << "name" << newExperimentName.toStdString() << bsoncxx::builder::stream::finalize);

        for(auto doc:query_result){
            auto profiles_element = doc["profiles"].get_array().value;
            for(auto profile : profiles_element){
                QString profileName = QString::fromStdString(profile["name"].get_utf8().value.to_string());
                if(profileName == "default"){
                    auto components_view = profile["components"].get_array().value;
                    for(auto component : components_view){
                        oldDefaultComponentsList.append(QString::fromStdString(component.get_utf8().value.to_string()));
                    }
                    auto configurations_view = profile["configuration"].get_array().value;
                    for(auto configuration : configurations_view){
                        oldDefaultConfigurationsList.append(QString::fromStdString(configuration.get_utf8().value.to_string()));
                    }
                    auto bootfiles_view = profile["boot file"].get_array().value;
                    for(auto bootfile : bootfiles_view){
                        oldDefaultBootfilesList.append(QString::fromStdString(bootfile.get_utf8().value.to_string()));
                    }
                }
            }
        }
        qDebug()<<oldDefaultComponentsList<<oldDefaultConfigurationsList<<oldDefaultBootfilesList;

        bool compareFlag = false;
        for(QString oldComponent : oldDefaultComponentsList){
            for(QString newComponent : newComponentsList ){
                if(oldComponent == newComponent){
                    compareFlag = true;
                }
            }
            if(compareFlag == false){
                oldDefaultComponentsList.removeAt(oldDefaultComponentsList.indexOf(oldComponent));
            }
        }

        compareFlag = false;
        for(QString oldConfiguration : oldDefaultConfigurationsList){
            for(QString newConfiguration : newConfigurationsList){
                if(oldConfiguration == newConfiguration){
                    compareFlag = true;
                }
            }
            if(compareFlag == false){
                oldDefaultConfigurationsList.removeAt(oldDefaultConfigurationsList.indexOf(oldConfiguration));
            }
        }

        compareFlag = false;
        for(QString oldBootfile : oldDefaultBootfilesList){
            for(QString newBootfile : bootfilesList){
                if(oldBootfile == newBootfile){
                    compareFlag = true;
                }
            }
            if(compareFlag == false){
                oldDefaultBootfilesList.removeAt(oldDefaultBootfilesList.indexOf(oldBootfile));
            }
        }
        qDebug()<<"----------------------------------\n"<<oldDefaultComponentsList<<oldDefaultConfigurationsList<<oldDefaultBootfilesList;
        mongocxx::cursor query_result2 = guiCollection.find(guiDocument << "name" << newExperimentName.toStdString() << bsoncxx::builder::stream::finalize);

        QStringList profileComponents;
        QStringList profileConfigurations;
        QStringList profileBootfiles;
        for(auto doc:query_result2){
            auto profiles_element = doc["profiles"].get_array().value;
            for(auto profile : profiles_element){
                QString profileName = QString::fromStdString(profile["name"].get_utf8().value.to_string());
                QString configurations_view;
                QString bootfile_view;
                qDebug()<<profileName;
                if(profileName != "default"){
                    auto components_view = profile["components"].get_array().value;
                    for(auto component : components_view){
                        QString component_ = QString::fromStdString(component.get_utf8().value.to_string());
                        profileComponents.append(component_);
                    }
                    configurations_view = QString::fromStdString(profile["configuration"].get_utf8().value.to_string());
                    bootfile_view = QString::fromStdString(profile["boot file"].get_utf8().value.to_string());
                }
                compareFlag = false;
                for(QString profileComponent : profileComponents){
                    for(QString oldComponent : oldDefaultComponentsList){
                        if(oldComponent == profileComponent){
                            compareFlag = true;
                        }
                    }
                    if(compareFlag == false){
                        profileComponents.removeAt(profileComponents.indexOf(profileComponent));
                    }
                }
                compareFlag = false;
                for(QString oldConfiguration : oldDefaultConfigurationsList){
                    if(oldConfiguration == configurations_view){
                        compareFlag = true;
                    }
                }
                if(compareFlag == false){
                    configurations_view = "";
                }
                compareFlag = false;
                for(QString oldBootfile : oldDefaultBootfilesList){
                    if(oldBootfile == bootfile_view){
                        compareFlag = true;
                    }
                }
                if(compareFlag == false){
                    bootfile_view = "";
                }
                profileBootfiles.append(bootfile_view);
                profileConfigurations.append(configurations_view);
                qDebug()<<profileName<<profileComponents<<profileConfigurations<<profileBootfiles;
                this->editProfileInExperiment(newExperimentName,profileName,profileName,profileComponents,profileConfigurations,profileBootfiles);
                profileComponents.clear();
                profileConfigurations.clear();
                profileBootfiles.clear();
            }
        }
        this->editProfileInExperiment(newExperimentName,"default","default",oldDefaultComponentsList,oldDefaultConfigurationsList,oldDefaultBootfilesList);


    }catch(bsoncxx::exception e){
    qDebug()<<"Exception thrown in method : \n"
            <<"guiMongoDB::updateExperimentProfiles(QString experimentName)"
            <<"\nException info: " << e.what();
    }

}

void guiMongoDB::editProfileInExperiment(QString experimentName, QString profileName, QString newProfileName, QStringList componentsList, QStringList configurationList, QStringList bootfilesList){
    try{
        bsoncxx::builder::stream::document builder{};
        if(profileName == "default"){
            auto in_array = builder << "name"
                                    << newProfileName.toStdString()
                                    << "components"
                                    << bsoncxx::builder::stream::open_array;
            for(QString component : componentsList){
                in_array = in_array << component.toStdString();
            }
            in_array = in_array << bsoncxx::builder::stream::close_array
                                << "configuration"
                                << bsoncxx::builder::stream::open_array;
            for(QString configuration : configurationList){
                in_array = in_array << configuration.toStdString();
            }
            in_array = in_array << bsoncxx::builder::stream::close_array
                                << "boot file"
                                << bsoncxx::builder::stream::open_array;
            for(QString bootfile : bootfilesList){
                in_array = in_array << bootfile.toStdString();
            }
            auto out = in_array << bsoncxx::builder::stream::close_array
                                << bsoncxx::builder::stream::finalize;
            this->guiCollection.update_one(guiDocument << "name" << experimentName.toStdString() << bsoncxx::builder::stream::finalize,
                                  guiDocument << "$set" << bsoncxx::builder::stream::open_document
                                  << QString("profiles.$." + profileName).toStdString()
                                  << out
                                  << bsoncxx::builder::stream::close_document
                                  << bsoncxx::builder::stream::finalize);


            qDebug()<<"updated default";
        }else{
            auto in_array = builder << "name"
                                    << newProfileName.toStdString()
                                    << "components"
                                    << bsoncxx::builder::stream::open_array;
            for(QString component : componentsList){
                in_array = in_array << component.toStdString();
            }
            auto out = in_array << bsoncxx::builder::stream::close_array
                            << "configuration"
                            << configurationList.at(0).toStdString()
                            << "boot file"
                            << bootfilesList.at(0).toStdString()
                            << bsoncxx::builder::stream::finalize;

            this->guiCollection.update_one(guiDocument << "name" << experimentName.toStdString() << bsoncxx::builder::stream::finalize,
                                  guiDocument << "$set" << bsoncxx::builder::stream::open_document
                                  << QString("profiles.$." + profileName).toStdString()
                                  << out
                                  << bsoncxx::builder::stream::close_document
                                  << bsoncxx::builder::stream::finalize);

        }

    }catch(bsoncxx::exception e){
    qDebug()<<"Exception thrown in method : \n"
            <<"editProfileInExperiment(QString experimentName, QString profileName)"
            <<"\nException info: " << e.what();
    }
}



















