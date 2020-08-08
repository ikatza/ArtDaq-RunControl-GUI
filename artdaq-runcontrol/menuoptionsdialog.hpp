#ifndef MENUOPTIONSDIALOG_HPP
#define MENUOPTIONSDIALOG_HPP

#include <QDialog>
#include <QFont>
#include <QDebug>

namespace Ui {
class MenuOptionsDialog;
}

class MenuOptionsDialog : public QDialog {
  Q_OBJECT

public:
  explicit MenuOptionsDialog(QWidget *parent = 0);
  ~MenuOptionsDialog();

  void setFromMainWindowFont(const QFont font);
  QFont getFromMainWindowFont() const;
  QString getFontType() const;
  void setFontType(const QString &value);
  int getFontSize() const;
  void setFontSize(int value);
  void setupFontComboBox();
  bool getEnableAutoResizing() const;
  void setEnableAutoResizing(bool value);
  void setupCheckBoxEnableAutoResizing();
  bool getEnableShellScripts();
  void setEnableShellScripts(bool value);
private slots:
  void fontChanged();
  void cbEnableAutoResizingChanged();
private:
  Ui::MenuOptionsDialog *ui;
  QFont fromMainWindowsFont;
  QString fontType;
  int fontSize;
  bool enableAutoResizing;
  QString lorem_ipsum = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
                        "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
                        "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris "
                        "nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in "
                        "reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla "
                        "pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa "
                        "qui officia deserunt mollit anim id est laborum.";
};

#endif // MENUOPTIONSDIALOG_HPP
