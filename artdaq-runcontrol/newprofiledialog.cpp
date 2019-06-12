#include "newprofiledialog.h"
#include "ui_newprofiledialog.h"

newProfileDialog::newProfileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newProfileDialog)
{
    ui->setupUi(this);
    this->initializeButtons();
    this->configurateLVs();
    connect(ui->bAddComponentToList,SIGNAL(clicked(bool)),this,SLOT(addComponentsToLVAddComponents()));
    connect(ui->bRemoveComponentFromList,SIGNAL(clicked(bool)),this,SLOT(removeComponentsFromLVAddComponents()));
    connect(this,SIGNAL(accepted()),this,SLOT(bbOkPressed()));
    this->populateComboBootFiles();
}

newProfileDialog::~newProfileDialog()
{
    delete ui;
}

void newProfileDialog::configurateLVs(){
    ui->lvAddToProfile->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->lvInExperiment->setSelectionMode(QAbstractItemView::MultiSelection);
}

void newProfileDialog::initializeButtons(){

    QString imagesDirectory = QCoreApplication::applicationDirPath() + "/../resources/images/";
    QIcon icon_plus;
    QIcon icon_minus;
    QPixmap pixmap_plus(imagesDirectory + "plus.png");
    icon_plus.addPixmap(pixmap_plus);
    ui->bAddComponentToList->setIcon(icon_plus);
    ui->bAddComponentToList->setText("");
    QPixmap pixmap_minus(imagesDirectory + "minus.png");
    icon_minus.addPixmap(pixmap_minus);
    ui->bRemoveComponentFromList->setIcon(icon_minus);
    ui->bRemoveComponentFromList->setText("");

}

void newProfileDialog::populateComboConfigurations(QStringList configurationList){

    for(QString configuration : configurationList){
        ui->comboConfiguration->addItem(configuration);
    }

}

void newProfileDialog::populateComboBootFiles(){

    QRegExp reg("*.txt");
    reg.setPatternSyntax(QRegExp::Wildcard);
    QString env_str = QProcessEnvironment::systemEnvironment().value("DAQINTERFACE_USER_DIR","DEFAULT");
    QDirIterator dirIt(env_str);
    QString str;
    QStringList list_str;
    while(dirIt.hasNext()){
        str = dirIt.next();
        if(reg.exactMatch(str)){
            list_str = str.split('/', QString::SkipEmptyParts);
            ui->comboBootFile->addItem(list_str.last());
        }
    }

}

void newProfileDialog::populateLVInExperiment(QVector<QStringList> componentList){

    for(QStringList component: componentList){
        this->componentList.append(component.at(0));
    }
    QStringListModel *model = new QStringListModel(this);
    model->setStringList(this->componentList);
    ui->lvInExperiment->setModel(model);
}

void newProfileDialog::addComponentsToLVAddComponents(){

    QStringList componentListToBeAdded_;
    QStringList componentListToBeRemoved = this->componentList;

    QModelIndexList selectedComponents = ui->lvInExperiment->selectionModel()->selectedRows();

    for(QModelIndex idx : selectedComponents){
        QString idx_str = idx.model()->data(idx,Qt::DisplayRole).toString();
        componentListToBeAdded_.append(idx_str);
    }

    this->componentListToBeAdded = this->componentListToBeAdded + componentListToBeAdded_;

    for(QString comp : componentListToBeRemoved){
        for(QString comp_added : componentListToBeAdded){
            if(comp == comp_added){
                componentListToBeRemoved.removeAt(componentListToBeRemoved.indexOf(comp));
            }
        }
    }

    QStringListModel *model = new QStringListModel(this);
    model->setStringList(componentListToBeAdded);
    ui->lvAddToProfile->setModel(model);

    QStringListModel *model2 = new QStringListModel(this);
    model2->setStringList(componentListToBeRemoved);
    ui->lvInExperiment->setModel(model2);
}

void newProfileDialog::removeComponentsFromLVAddComponents(){

    QStringList componentListToBeRemoved_;
    QStringList componentListInExperiment_ = this->componentList;
    QModelIndexList selectedComponents = ui->lvAddToProfile->selectionModel()->selectedRows();

    for(QModelIndex idx : selectedComponents){
        QString idx_str = idx.model()->data(idx,Qt::DisplayRole).toString();
        componentListToBeRemoved_.append(idx_str);
    }

    for(QString comp : this->componentListToBeAdded){
        for(QString comp_removed : componentListToBeRemoved_){
            if(comp == comp_removed){
                this->componentListToBeAdded.removeAt(this->componentListToBeAdded.indexOf(comp));
            }
        }
    }

    for(QString comp : componentListInExperiment_){
        for(QString comp_added : componentListToBeAdded){
            if(comp == comp_added){
                componentListInExperiment_.removeAt(componentListInExperiment_.indexOf(comp));
            }
        }
    }

    QStringListModel *model = new QStringListModel(this);
    model->setStringList(this->componentListToBeAdded);
    ui->lvAddToProfile->setModel(model);

    QStringListModel *model2 = new QStringListModel(this);
    model2->setStringList(componentListInExperiment_);
    ui->lvInExperiment->setModel(model2);

}

void newProfileDialog::bbOkPressed(){

    this->profileName = ui->tfProfileName->text();
    this->selectedConfiguration = ui->comboConfiguration->currentText();
    this->bootFile = ui->comboBootFile->currentText();

}

void newProfileDialog::accept(){

    bool configurationFlag = true;
    bool profileNameFlag = true;
    bool componentsFlag = true;
    QString profileName_ = ui->tfProfileName->text();
    if(profileName_ == ""){
        QMessageBox msg;
        msg.setText("Textfield Profile Name is empty.");
        msg.exec();
        ui->tfProfileName->setFocus();
        profileNameFlag = false;
    }
    if(this->componentListToBeAdded.isEmpty()){
        QMessageBox msg;
        msg.setText("Component list to be added to profile is empty.");
        msg.exec();
        ui->lvAddToProfile->setFocus();
        componentsFlag = false;
    }
    if(ui->comboConfiguration->currentText() == ""){
        QMessageBox msg;
        msg.setText("Configuration to be added to profile is empty.");
        msg.exec();
        ui->comboConfiguration->setFocus();
        configurationFlag = false;
    }

    if(profileNameFlag && configurationFlag && componentsFlag){
        this->done(this->Accepted);
    }

}

QString newProfileDialog::getProfileName(){
    return this->profileName;
}

QStringList newProfileDialog::getComponentsToBeAdded(){
    return this->componentListToBeAdded;
}

QString newProfileDialog::getSelectedConfiguration(){
    return this->selectedConfiguration;
}

QString newProfileDialog::getSelectedBootFile(){
    return this->bootFile;
}

void newProfileDialog::setListViews(QStringList experimentComponents, QStringList profileComponents){

    this->componentList = experimentComponents;
    this->componentListToBeAdded = profileComponents;
    for(QString componentInExperiment : experimentComponents){
        for(QString profileComponent : profileComponents){
            if(componentInExperiment == profileComponent){
                experimentComponents.removeAt(experimentComponents.indexOf(componentInExperiment));
            }
        }
    }

    QStringListModel *model = new QStringListModel(this);
    model->setStringList(profileComponents);
    ui->lvAddToProfile->setModel(model);

    QStringListModel *model2 = new QStringListModel(this);
    model2->setStringList(experimentComponents);
    ui->lvInExperiment->setModel(model2);
}

void newProfileDialog::setTfProfileName(QString profileName){
    ui->tfProfileName->setText(profileName);
}

void newProfileDialog::setComboConfigurations(QStringList experimentConfigurationList){
    QStringListModel *model = new QStringListModel(this);
    model->setStringList(experimentConfigurationList);
    ui->comboConfiguration->setModel(model);
}

void newProfileDialog::setComboBootConfiguration(QStringList experimentBootList){
    QStringListModel *model = new QStringListModel(this);
    model->setStringList(experimentBootList);
    ui->comboBootFile->setModel(model);
}

void newProfileDialog::setProfileDialog(QString experimentName, QStringList experimentComponentList, QStringList profileComponentList, QStringList configurationList, QStringList bootfileList){

    this->setTfProfileName(experimentName);
    this->setComboConfigurations(configurationList);
    this->setComboBootConfiguration(bootfileList);
    this->setListViews(experimentComponentList,profileComponentList);

}

void newProfileDialog::setLabelTitle(QString title){
    ui->labelTitle->setText(title);
}




















