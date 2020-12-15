#ifndef DB_DIALOG_HPP
#define DB_DIALOG_HPP

#include <QDialog>
#include <QProcess>
#include <QString>
#include <QPair>
#include <QDebug>
#include <QByteArray>
#include <QTextCodec>
#include <QStringList>
#include <QStringListModel>
#include <QModelIndexList>
#include <QModelIndex>
#include <QRegularExpression>
#include <QRegularExpressionMatch>


namespace Ui {
class db_dialog;
}

class db_dialog : public QDialog {
  Q_OBJECT

public:
  explicit db_dialog(QWidget* parent = 0);
  ~db_dialog();
  QPair<QString, QString> getSelectedDBConfig() const;
  void setSelectedDBConfig(const QString& value);

private slots:
  void populateLvConfiguration();
  void tfConfigNameModified();
  void bSelectPressed();
  void listViewClicked();
  void bRefreshListPressed();

private:
  Ui::db_dialog *ui;
  const QString export_dir_base = "/tmp/artdaq-runcontrol-gui/db/";
  QString selectedDBConfig;
  QStringList daq_string;
};

#endif // DB_DIALOG_HPP
