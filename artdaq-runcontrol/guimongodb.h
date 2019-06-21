#ifndef GUIMONGODB_H
#define GUIMONGODB_H

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/collection.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/exception/exception.hpp>
#include <QProcess>
#include <QVector>
#include <QDebug>


class guiMongoDB
{
public:
    guiMongoDB();
    void start();
    void uploadNewExperiment(QString name, QVector<QStringList> componentsList, QStringList configurations);
    void retreiveExperiment(QString name, QVector<QStringList> *componentsList, QStringList *configurations);
    void removeExperiment(QString experimentName);
    void findExperiments(QStringList *experiments);
    void addProfileToExperiment(QString experimentName, QString profileName, QString selectedConfiguration, QStringList selectedComponents, QString bootFile);
    void addProfileToExperiment(QString experimentName, QString profileName, QStringList selectedConfiguration, QVector<QStringList> selectedComponents, QStringList bootFiles);
    QStringList retrieveExperimentProfiles(QString experimentName);
    QStringList retreiveProfileComponents(QString experimentName, QString profileName);
    QStringList retreiveProfileConfiguration(QString experimentName, QString profileName);
    QStringList retreiveProfileBootFiles(QString experimentName, QString profileName);
    void retreiveExperiment(QString name, QVector<QStringList> *componentList, QStringList *configurationList, QStringList *bootfileList);
    void removeProfileFromExperiment(QString experimentName, QString profileToBeRemoved);
    void saveExperimentProfiles(QString experimentName);
    void updateExperiment(QString experimentName, QString newExperimentName, QVector<QStringList> componentsList, QStringList configurations, QStringList bootfilesList);
    bsoncxx::document::element getExperimentProfileView(QString experimentName);
    void editProfileInExperiment(QString experimentName, QString profileName, QString newProfileName, QStringList componentsList, QStringList configurationList, QStringList bootfilesList);
private:
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{}};
    mongocxx::collection guiCollection;
    bsoncxx::builder::stream::document guiDocument{};
    QString profiles_element_temp;
};

#endif // GUIMONGODB_H
