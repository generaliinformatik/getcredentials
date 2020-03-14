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

  QCommandLineParser parser;
  parser.addOptions({
      { { "d", "description"   }, "set description text", "description" }
    , { { "e", "emptyPassword" }, "allow empty password"                }
    , { { "n", "noUser"        }, "disable user field"                  }
    , { { "u", "user"          }, "set default user",     "user"        }
    , { { "w", "windowTitle"   }, "set window title",     "title"       }
    , { { "?", "help"          }, "show help"                           }
  });

  parser.process(*qApp);

  if (parser.isSet("help"))
    parser.showHelp();

  ui->leUser->setDisabled(parser.isSet("noUser"));
  m_allowEmptyPassword = parser.isSet("emptyPassword");
  if (not parser.value("description").isEmpty())
    ui->lblDescription->setText(parser.value("description"));
  ui->leUser->setText(parser.value("user"));
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
