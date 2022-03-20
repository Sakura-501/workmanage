#include "dialog.h"
#include "ui_dialog.h"
#include<QMessageBox>
#include<QDialog>
dialog::dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog)
{
    ui->setupUi(this);
    this->setWindowTitle("登录窗口");//设置窗口的名称
    //点击登录窗口的退出按钮结束进程
    connect(ui->quit,&QPushButton::clicked,[=](){
        //点击退出按钮会弹出警告！
        if(QMessageBox::Yes == QMessageBox::question(this,"警告","确定退出系统？",
                                                     QMessageBox::Yes|QMessageBox::No))
        {
                this->close();
        }
    });
    //点击登录窗口的登录按钮进行登录
    connect(ui->login,&QPushButton::clicked,[=](){
        //设置里两个账号密码
        if((ui->usernameinput->text()=="root"&&ui->passwordinput->text()=="root")||
                (ui->usernameinput->text()=="admin"&&ui->passwordinput->text()=="123456"))
       {
            this->close();//登录窗口关闭
            w.show();//主窗口打开
        }
        else
        {
            QMessageBox::critical(this,"警告","账号或密码输入有误，请重新输入！");
        }
    });
}

dialog::~dialog()
{
    delete ui;
}
