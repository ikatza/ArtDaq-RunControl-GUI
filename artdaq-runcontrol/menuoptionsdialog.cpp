#include "menuoptionsdialog.hpp"
#include "ui_menuoptionsdialog.h"

MenuOptionsDialog::MenuOptionsDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::MenuOptionsDialog)
{
  ui->setupUi(this);
  ui->tabFonts->setTabText(0,"Fonts");
  connect(ui->fcbFontType,SIGNAL(currentIndexChanged(int)),this,SLOT(fontChanged()));
  connect(ui->sbFontSize,SIGNAL(valueChanged(int)),this,SLOT(fontChanged()));
  connect(ui->cbEnableAutoResizing,SIGNAL(toggled(bool)),this,SLOT(cbEnableAutoResizingChanged()));
}

MenuOptionsDialog::~MenuOptionsDialog()
{
  delete ui;
}

void MenuOptionsDialog::setupFontComboBox(){
  ui->fcbFontType->setCurrentFont(this->getFromMainWindowFont());
  ui->sbFontSize->setValue(this->getFromMainWindowFont().pointSize());
  QString fontType_ = ui->fcbFontType->currentText();
  int fontSize_ = ui->sbFontSize->value();
  this->setFontSize(fontSize_);
  this->setFontType(fontType_);
}

void MenuOptionsDialog::setupCheckBoxEnableAutoResizing(){
  ui->cbEnableAutoResizing->setChecked(this->enableAutoResizing);
}

void MenuOptionsDialog::setFromMainWindowFont(const QFont font){
  this->fromMainWindowsFont = font;
}

QFont MenuOptionsDialog::getFromMainWindowFont() const{
  return this->fromMainWindowsFont;
}

void MenuOptionsDialog::fontChanged(){
  QString fontType_ = ui->fcbFontType->currentText();
  int fontSize_ = ui->sbFontSize->value();
  this->setFontSize(fontSize_);
  this->setFontType(fontType_);
  ui->taFontVisualization->setFontPointSize((qreal)fontSize_);
  ui->taFontVisualization->setFontFamily(fontType_);
  ui->taFontVisualization->setText(this->lorem_ipsum);
}

void MenuOptionsDialog::cbEnableAutoResizingChanged(){
  this->enableAutoResizing = ui->cbEnableAutoResizing->isChecked();
  if(ui->cbEnableAutoResizing->isChecked()){
    ui->sbFontSize->setEnabled(false);
  }else{
    ui->sbFontSize->setEnabled(true);
  }
}

bool MenuOptionsDialog::getEnableAutoResizing() const
{
  return enableAutoResizing;
}

void MenuOptionsDialog::setEnableAutoResizing(bool value)
{
  enableAutoResizing = value;
}

int MenuOptionsDialog::getFontSize() const
{
  return fontSize;
}

void MenuOptionsDialog::setFontSize(int value)
{
  fontSize = value;
}

QString MenuOptionsDialog::getFontType() const
{
  return fontType;
}

void MenuOptionsDialog::setFontType(const QString &value)
{
  fontType = value;
}
