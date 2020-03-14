#ifndef GETCREDENTIALS_HPP
#define GETCREDENTIALS_HPP

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class GetCredentials; }
QT_END_NAMESPACE

class GetCredentials : public QDialog
{
  Q_OBJECT

public:
  GetCredentials(QWidget *parent = nullptr);
  ~GetCredentials();

private slots:
  void on_pbOk_clicked();

  void on_pbCancel_clicked();

private:
  Ui::GetCredentials *ui;

  bool m_allowEmptyPassword = false;
};
#endif // GETCREDENTIALS_HPP
