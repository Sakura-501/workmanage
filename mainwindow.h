#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<Worker.h>
#include<Employee.h>
#include<Manager.h>
#include<Boss.h>
#include<fstream>
#include<QFile>
//关联文本文件的绝对路径
const string FILENAME="C:\\Users\\Lenovo\\Desktop\\qtproject\\workermanage\\out.txt";
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    //通用的成员函数：
    void init_worker();//从文本文件中读取数据初始化职工数组
    int Get_WorkerNumber();//统计当前文本中的职工人数
    void save();//将职工数组的信息更新到文本文件中
    int find_worker(string);//根据职工姓名查询对应的数组下标
    int find_workerbyid(string);//根据职工工号查询对应的数组下标
    //操作功能的成员函数：
    void quit();//退出系统操作
    void Add_Worker();//添加职工操作
    void query();//查询职工操作
    void delete_worker();//删除职工操作
    void modify();//修改职工操作
    void sortworker();//排序职工操作
    ~MainWindow();

    int WorkerNumber=0;//职工数组的人数
    Worker **WorkerArray;//职工数组存储指针
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
