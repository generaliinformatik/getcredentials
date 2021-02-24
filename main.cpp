#include "GetCredentials.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QApplication a(argc, argv);
  GetCredentials w;

  w.show();
  return a.exec();
}
