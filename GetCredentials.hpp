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
  // buttons
  void on_pbOk_clicked();
  void on_pbCancel_clicked();

  // closing dialog by clicking X or hitting ESC
  void on_GetCredentials_rejected();

private:
  Ui::GetCredentials *ui;

  bool m_allowEmptyPassword = false;
  bool m_verifyPassword = false;
  int m_passwordLength = 0;
  bool m_allowQuotes = true;
};
#endif // GETCREDENTIALS_HPP
