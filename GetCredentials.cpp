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

  ui->lblVersion->setText(GETCREDENTIALS_VERSION);

  // remove question mark from title bar
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

  QCommandLineParser parser;
  // only long options supported, e.g. "--description"
  parser.addOptions({
      { "description",    "set description text",         "description" }
    , { "disableUser",    "disable user field"                          }
    , { "emptyPassword",  "allow empty password"                        }
    , { "help",           "show help"                                   }
    , { "minLength",      "password minimum length",      "length"      }
    , { "user",           "set default user",             "user"        }
    , { "verifyPassword", "require password verification"               }
    , { "windowTitle",    "set window title",             "title"       }
  });

  parser.process(*qApp);

  if (parser.isSet("help"))
    parser.showHelp();   // quits the app

  if (parser.isSet("description"))
    ui->lblDescription->setText(parser.value("description"));

  ui->leUser->setDisabled(parser.isSet("disableUser"));
  m_allowEmptyPassword = parser.isSet("emptyPassword");

  if (parser.isSet("minLength"))
    m_passwordLength = parser.value("minLength").toInt();

  if (parser.isSet("user"))
  {
    ui->leUser->setText(parser.value("user"));
    ui->lePassword->setFocus();
  }

  m_verifyPassword = parser.isSet("verifyPassword");
  if (not m_verifyPassword)
    ui->lePassword2->close();  // remove element from UI

  if (not parser.value("windowTitle").isEmpty())
    setWindowTitle(parser.value("windowTitle"));
}

GetCredentials::~GetCredentials()
{
  delete ui;
}

void GetCredentials::on_pbOk_clicked()
{
  if      (ui->lePassword->text().isEmpty() and
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
  else if (m_passwordLength and
           ui->lePassword->text().length() and
           ui->lePassword->text().length() < m_passwordLength)
  {
    QMessageBox messageBox;
    messageBox.setWindowTitle("password too short");
    messageBox.setText(
        "The password must be at least " + QString::number(m_passwordLength) +
        " characters long."
    );
    messageBox.setStandardButtons(QMessageBox::Ok);
    messageBox.setIcon(QMessageBox::Critical);
    messageBox.exec();
  }
  else
  {
    QTextStream(stdout) << ui->leUser->text();
    QTextStream(stderr) << ui->lePassword->text();

    // Although the documentation specifically states that close() does
    // not emit rejected(), it seems to be the case nevertheless.
    // We have to use accept() here in order to not run into
    // on_GetCredentials_rejected() as this would cause RC=1 even when
    // clicking OK.
    accept();
  }
}

void GetCredentials::on_pbCancel_clicked()
{
  qApp->exit(1);
}

void GetCredentials::on_GetCredentials_rejected()
{
  qApp->exit(1);
}
