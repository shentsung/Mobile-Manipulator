#include "authentication.h"
#include <QFont>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

#include <databaseconnection.h>
#include <QMessageBox>

// 编译时未报错，但生成的程序中文乱码
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

Authentication::Authentication(QWidget *parent) :
    QDialog(parent)
{
    this->setFixedSize(500,300);
    this->setWindowTitle(tr("身份验证"));
    
    title = new QLabel();
    title->setText(tr("移动机械臂控制系统"));
    QFont titleFont("华文新魏", 25);
    title->setFont(titleFont);
    title->setStyleSheet("QLabel{color: #3a96d9;}");
    title->setAlignment(Qt::AlignHCenter);

    QFont labelFont("幼圆", 12);
    userNameLabel = new QLabel();
    userNameLabel->setText(tr("用户名:"));
    userNameLabel->setFont(labelFont);

    passwordLabel = new QLabel();
    passwordLabel->setText(tr("密  码:"));
    passwordLabel->setFont(labelFont);

    userNameLineEdit = new QLineEdit();
    passwordLineEdit = new QLineEdit();

    QFont buttonFont("幼圆", 11);
    confirmBtn = new QPushButton();
    confirmBtn->setText(tr("确定"));
    confirmBtn->setFont(buttonFont);
    confirmBtn->setFixedSize(110, 35);

    cancleBtn = new QPushButton();
    cancleBtn->setText(tr("取消"));
    cancleBtn->setFont(buttonFont);
    cancleBtn->setFixedSize(110, 35);

    QHBoxLayout *titleLayout = new QHBoxLayout();
    QHBoxLayout *userNameLayout = new QHBoxLayout();
    QHBoxLayout *passWordLayout = new QHBoxLayout();
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QVBoxLayout *globalLayout = new QVBoxLayout();


    titleLayout->addWidget(title);

    userNameLayout->addSpacing(100);
    userNameLayout->addWidget(userNameLabel);
    userNameLayout->addSpacing(10);
    userNameLayout->addWidget(userNameLineEdit);
    userNameLayout->addSpacing(100);

    passWordLayout->addSpacing(100);
    passWordLayout->addWidget(passwordLabel);
    passWordLayout->addSpacing(10);
    passWordLayout->addWidget(passwordLineEdit);
    passWordLayout->addSpacing(100);

    buttonLayout->addSpacing(50);
    buttonLayout->addWidget(confirmBtn);
    buttonLayout->addWidget(cancleBtn);
    buttonLayout->addSpacing(50);

    globalLayout->addSpacing(50);
    globalLayout->addLayout(titleLayout);
    globalLayout->addLayout(userNameLayout);
    globalLayout->addSpacing(10);
    globalLayout->addLayout(passWordLayout);
    globalLayout->addSpacing(20);
    globalLayout->addLayout(buttonLayout);
    globalLayout->addSpacing(45);

    this->setLayout(globalLayout);


    QObject::connect(confirmBtn, SIGNAL(clicked()),this, SLOT(confirmBtn_Clicked()));
    QObject::connect(cancleBtn, SIGNAL(clicked()),this, SLOT(cancleBtn_Clicked()));

}

void Authentication::confirmBtn_Clicked()
{
    if(!createConnection())
        return;
    QSqlQuery query;
    query.exec("select * from user");
    while(query.next())
    {
        qDebug()<<query.value(0).toString()<<query.value(1).toString();
        if(query.value(0).toString() == userNameLineEdit->text())
        {
           if((query.value(1).toString() == passwordLineEdit->text()))
           {
              accept();
              qDebug()<<userNameLineEdit->text();
              qDebug()<<passwordLineEdit->text();
              qDebug()<<(query.value(0).toString()==userNameLineEdit->text());
              qDebug()<<(query.value(1).toString() == passwordLineEdit->text());
            }
           else
           {
               QMessageBox::warning(this, tr("提示"), tr("密码输入错误, 请重新输入"), QMessageBox::Ok);
           }
        }
        else
        {
             QMessageBox::warning(this, tr("提示"), tr("用户输入错误, 请重新输入"), QMessageBox::Ok);
        }

    }
}

void Authentication::cancleBtn_Clicked()
{
    this->close();
}

Authentication::~Authentication()
{
   ;
}
