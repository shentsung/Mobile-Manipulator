#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>


namespace Ui {
class Authentication;
}

class Authentication : public QDialog
{
    Q_OBJECT

public:
    explicit Authentication(QWidget *parent = 0);
    ~Authentication();

private:
    Ui::Authentication *ui;
    QLabel *title;

    QLabel *userNameLabel;
    QLabel *passwordLabel;

    QLineEdit *userNameLineEdit;
    QLineEdit *passwordLineEdit;

    QPushButton *confirmBtn;
    QPushButton *cancleBtn;

private slots:
    void confirmBtn_Clicked();
    void cancleBtn_Clicked();

};

#endif // AUTHENTICATION_H
