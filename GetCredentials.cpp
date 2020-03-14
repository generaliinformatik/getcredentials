#include "GetCredentials.hpp"
#include "ui_GetCredentials.h"
#include <QCommandLineParser>
#include <QMessageBox>
#include <QTextStream>

GetCredentials::GetCredentials(QWidget *parent)
  : QDialog(parent)
  , ui(new Ui::GetCredentials)
{
  ui->setupUi(this);

  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

  QCommandLineParser parser;
  parser.addOptions({
      { "description",    "set description text",    "description" }
    , { "disableUser",    "disable user field"                     }
    , { "emptyPassword",  "allow empty password"                   }
    , { "help",           "show help"                              }
    , { "user",           "set default user",        "user"        }
    , { "verifyPassword", "enter password two times"               }
    , { "windowTitle",    "set window title",        "title"       }
  });

  parser.process(*qApp);

  if (parser.isSet("help"))
    parser.showHelp();

  if (not parser.value("description").isEmpty())
    ui->lblDescription->setText(parser.value("description"));
  ui->leUser->setDisabled(parser.isSet("disableUser"));
  m_allowEmptyPassword = parser.isSet("emptyPassword");
  ui->leUser->setText(parser.value("user"));
  m_verifyPassword = parser.isSet("verifyPassword");
  if (not m_verifyPassword)
    ui->lePassword2->close();
  if (not parser.value("windowTitle").isEmpty())
    setWindowTitle(parser.value("windowTitle"));
}

GetCredentials::~GetCredentials()
{
  delete ui;
}

void GetCredentials::on_pbOk_clicked()
{
  if (ui->lePassword->text().isEmpty() and
      not m_allowEmptyPassword)
  {
    QMessageBox messageBox;
    messageBox.setWindowTitle("empty password");
    messageBox.setText("The password cannot be empty!");
    messageBox.setStandardButtons(QMessageBox::Ok);
    messageBox.setIcon(QMessageBox::Critical);
    messageBox.exec();
  }
  else if (m_verifyPassword and
           ui->lePassword->text() != ui->lePassword2->text())
  {
    QMessageBox messageBox;
    messageBox.setWindowTitle("password mismatch");
    messageBox.setText("The passwords do not match!");
    messageBox.setStandardButtons(QMessageBox::Ok);
    messageBox.setIcon(QMessageBox::Critical);
    messageBox.exec();
  }
  else
  {
    QTextStream(stdout) << ui->leUser->text();
    QTextStream(stderr) << ui->lePassword->text();

    close();
  }
}

void GetCredentials::on_pbCancel_clicked()
{
  qApp->exit(1);
}
