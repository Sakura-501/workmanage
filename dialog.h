#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<mainwindow.h>
namespace Ui {
class dialog;
}

class dialog : public QDialog
{
    Q_OBJECT

public:
    explicit dialog(QWidget *parent = nullptr);
    ~dialog();

private:
    Ui::dialog *ui;
    //创建了一个主窗口类的对象作为登录窗口类的数据成员，包含关系
    MainWindow w;
};

#endif // DIALOG_H
